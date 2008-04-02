//---------------------------------------------------------------------------
#include <vcl.h>
#include <IniFiles.hpp>
#include <stdlib.h>
#pragma hdrstop

#include "MainUnit.h"
#include "StartGameUnit.h"
#include "Dice.h"
#include "Pegs.h"
#include "Moves.h"
#include "Interface.h"
#include "SetBRUnit.h"
#include "AboutUnit.h"
#include "FileIO.h"
#include "GameOptions.h"
#include "AutoDiceUnit.h"
#include "TipsUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLudoForm *LudoForm;
AnsiString AppDir;			// Home directory (or home "folder")
extern Peg *Pegs[4][4];
MovesList *MovesInHand=NULL;// Moves buffer
extern TShape *HilightShape;// Defined in unit Pegs
TLabel *WinnerLabels[4];
TImage *WinnerImages[4];
extern short PlayerToMove;			// For MovesList handler
extern enum PlayerType Players[4];	// For MovesList handler
extern bool GameStarted;			// For Enable/Disable certain menu options
TIniFile *IniFile=NULL;				// The .INI file
extern short BlunderRange[4];		// For storing in IniFile
extern char ExtraThrows;	 	// Defined in Dice.cpp
extern bool Discard3Sixes;	 	// Defined in Dice.cpp
extern short TimeOutHuman;	 	// Defined in Interface.cpp
extern short TimeOutComputer;	// Defined in Interface.cpp
extern short PegsOnStart;	 	// Defined in Interface.cpp
extern short ProbOfSix;		 	// Defined in Dice.cpp
extern short TotalPegs;		 	// Defined in Interface.cpp
extern char GameType;		 	// Defined in Interface.cpp
extern bool JumpDouble;		 	// Defined in Moves.cpp
extern char IgnoreStars;		// Defined in Pegs.cpp
extern bool AutoComputer;	  	// Defined in Interface.cpp
extern bool Alert3Sixes;		// Defined in Dice.cpp
extern bool AutoHuman;			// Defined in Interface.cpp
extern bool AutoShowAutoDice;	// Defined in AutoDiceUnit.cpp
extern bool AutoCheckComputer;	// Defined in AutoDiceUnit.cpp
extern bool AutoCheckHuman;		// Defined in AutoDiceUnit.cpp
extern bool TipsAtStartup;		// Defined in TipsUnit.cpp
extern int CurrentTipPos;		// Defined in TipsUnit.cpp
extern bool SmartWinnersInfo;	// Defined in Interface.cpp
extern bool KillForHome;		// Defined in Moves.cpp
bool Playing;					// Automation is On or Off?
AnsiString CurOpenFile;			// Current open file
AnsiString DefaultBmps="Bitmaps\\";
#if (SECURE)
extern bool RestrictedMode;		// Defined in StartGameUnit.cpp
#endif
//---------------------------------------------------------------------------
#if (SECURE)
bool CheckAccess()
{
	// Change SECURE in options of project to enable/disable security.
	char Value[11];
	//	  		   Compatibility32,ZAPODUL,0x21110000
	char *Section="Dpnobsjajkjsz41+YBOPCVK+/w1222////",*Entry,*ChkValue;
	bool Result;
	for (char *i=Section; *i!=0; i++) *i=char((*i+1^1)-1);
	Section[15]=0; Entry=Section+16;
	Section[23]=0; ChkValue=Section+24;
	GetProfileString(Section,Entry,"",Value,sizeof(Value));
	Result = !lstrcmp(Value,ChkValue);
	if (!Result)
	{
		AnsiString pwd;
		if (InputQuery("Software Security",
			"Please enter key to enable software:",
			pwd))
		{
			if (StrToIntDef(pwd,0)==2005738503)
			{
				WriteProfileString(Section,Entry,ChkValue);
				Result=true;
			}
			else Application->MessageBox(
				"Invalid key entered. Software not unlocked.",
				"Error",MB_ICONERROR | MB_OK);
		}
	}
	return Result;
}
#endif
//---------------------------------------------------------------------------
void __fastcall TLudoForm::EnableTimer(bool Enable)
{
	if (!Playing) Enable=false;
	Timer->Enabled=Enable;
	Automation->Down=Enable;
}
//---------------------------------------------------------------------------
__fastcall TLudoForm::TLudoForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::RedrawBoard(bool ShowNumbers)
{
	RECT Rect;
	AnsiString Num;
	TCanvas *Canvas;
	static bool BoardDrawn=false;
	if (BoardDrawn==false || ShowNumbers==false)
		try {LudoBoard->Picture->LoadFromFile(AppDir+DefaultBmps+"Board.bmp");}
		catch (EFOpenError *Error)
		{
			if (BoardDrawn==false) throw;
			else Application->MessageBox(Error->Message.c_str(),
				"Error",MB_ICONERROR | MB_OK);
			return;
		}
	BoardDrawn=true;
	if (ShowNumbers)
	{
		Canvas=LudoBoard->Canvas;
		Canvas->Font->Name="Small Fonts";
		Canvas->Font->Size=5;
		Canvas->Brush->Style=bsClear;
		for (short i=0; i<=76; i++)
		{
			Num=IntToStr(i)+'.';
			GetRectFromPos(&Rect,i);
			OffsetRect(&Rect,-LudoBoard->Left,-LudoBoard->Top);
			Canvas->TextOut
				(Rect.right - Canvas->TextWidth(Num),
				Rect.bottom - Canvas->TextHeight(Num), Num);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::FormCreate(TObject *Sender)
{
#if (SECURE)
	if (!CheckAccess()) RestrictedMode=true;
#endif
	Application->HintHidePause=5000;
	AppDir=Application->ExeName;
	AppDir.SetLength(AppDir.LastDelimiter("\\"));
	try {
		HomeImage->Picture->LoadFromFile(AppDir+DefaultBmps+"Home.bmp");
		SetBoardForPegs(LudoBoard);
		RedrawBoard(false);
		HilightShape=Hilight;
		LoadPegs(this,this);
		DiceSetImage(DiceImage);
	}
	catch (EFOpenError *Error)
	{
		Application->MessageBox(Error->Message.c_str(),
			"Error",MB_ICONERROR | MB_OK);
		Application->Terminate();
		return;
	}
	MovesInHand=new MovesList(MovesListImage->Canvas);
	WinnerLabels[0]=First;	WinnerImages[0]=Winner1;
	WinnerLabels[1]=Second;	WinnerImages[1]=Winner2;
	WinnerLabels[2]=Third;	WinnerImages[2]=Winner3;
	WinnerLabels[3]=Fourth;	WinnerImages[3]=Winner4;
	PlayerType->Caption="";
	IniFile=new TIniFile(AppDir+"Ludo.ini");
	BlunderRange[0]=(short)IniFile->ReadInteger("Skills","Red",1000);
	BlunderRange[1]=(short)IniFile->ReadInteger("Skills","Green",1000);
	BlunderRange[2]=(short)IniFile->ReadInteger("Skills","Yellow",1000);
	BlunderRange[3]=(short)IniFile->ReadInteger("Skills","Blue",1000);
	if (IniFile->ReadBool("Options","Enumerate",false)) ShowNumbersClick(this);
	ExtraThrows=(char)IniFile->ReadInteger("Options","ExtraThrows",7);
	Discard3Sixes=IniFile->ReadBool("Options","Discard3Sixes",true);
	TimeOutHuman=(short)IniFile->ReadInteger("Options","TimeOutHuman",10000);
	TimeOutComputer=(short)IniFile->
		ReadInteger("Options","TimeOutComputer",1500);
	PegsOnStart=(short)IniFile->ReadInteger("Options","PegsOnStart",0);
	ProbOfSix=(short)IniFile->ReadInteger("Options","ProbOfSix",20);
	TotalPegs=(short)IniFile->ReadInteger("Options","TotalPegs",4);
	GameType=(char)IniFile->ReadInteger("Options","GameType",1);
	JumpDouble=IniFile->ReadBool("Options","JumpDouble",false);
	IgnoreStars=char(IniFile->ReadInteger("Options","IgnoreStars",0));
	Timer->Interval=IniFile->ReadInteger("Options","AutoInterval",500);
	AutoComputer=IniFile->ReadBool("Options","AutoComputer",true);
	Playing=IniFile->ReadBool("Options","Playing",true);
	Alert3Sixes=IniFile->ReadBool("Options","Alert3Sixes",true);
	AutoHuman=IniFile->ReadBool("Options","AutoHuman",false);
	AutoShowAutoDice=IniFile->ReadBool("Options","AutoShowAutoDice",true);
	AutoCheckComputer=IniFile->ReadBool("Options","AutoCheckComputer",true);
	AutoCheckHuman=IniFile->ReadBool("Options","AutoCheckHuman",false);
	TipsAtStartup=IniFile->ReadBool("Options","TipsAtStartup",true);
	CurrentTipPos=IniFile->ReadInteger("Options","CurrentTipPos",0);
	if (IniFile->ReadBool("Options","SmartWinnersInfo",true)) DumbWListClick(this);
	KillForHome=IniFile->ReadBool("Options","KillForHome",true);
	EnableDice(false);
	srand(GetTickCount());
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::FormDestroy(TObject *Sender)
{
	// IniFile will not be created if returned before due to exception
	if (IniFile)
	{
		IniFile->WriteInteger("Skills","Red",BlunderRange[0]);
		IniFile->WriteInteger("Skills","Green",BlunderRange[1]);
		IniFile->WriteInteger("Skills","Yellow",BlunderRange[2]);
		IniFile->WriteInteger("Skills","Blue",BlunderRange[3]);
		IniFile->WriteBool("Options","Enumerate",ShowNumbers->Checked);
		IniFile->WriteInteger("Options","ExtraThrows",ExtraThrows);
		IniFile->WriteBool("Options","Discard3Sixes",Discard3Sixes);
		IniFile->WriteInteger("Options","TimeOutHuman",TimeOutHuman);
		IniFile->WriteInteger("Options","TimeOutComputer",TimeOutComputer);
		IniFile->WriteInteger("Options","PegsOnStart",PegsOnStart);
		IniFile->WriteInteger("Options","ProbOfSix",ProbOfSix);
		IniFile->WriteInteger("Options","TotalPegs",TotalPegs);
		IniFile->WriteInteger("Options","GameType",GameType);
		IniFile->WriteBool("Options","JumpDouble",JumpDouble);
		IniFile->WriteInteger("Options","IgnoreStars",IgnoreStars);
		IniFile->WriteInteger("Options","AutoInterval",Timer->Interval);
		IniFile->WriteBool("Options","AutoComputer",AutoComputer);
		IniFile->WriteBool("Options","Playing",Playing);
		IniFile->WriteBool("Options","Alert3Sixes",Alert3Sixes);
		IniFile->WriteBool("Options","AutoHuman",AutoHuman);
		IniFile->WriteBool("Options","AutoShowAutoDice",AutoShowAutoDice);
		IniFile->WriteBool("Options","AutoCheckComputer",AutoCheckComputer);
		IniFile->WriteBool("Options","AutoCheckHuman",AutoCheckHuman);
		IniFile->WriteBool("Options","TipsAtStartup",TipsAtStartup);
		IniFile->WriteInteger("Options","CurrentTipPos",CurrentTipPos);
		IniFile->WriteBool("Options","SmartWinnersInfo",SmartWinnersInfo);
		IniFile->WriteBool("Options","KillForHome",KillForHome);
		delete IniFile;
	}
	// MovesInHand will not be created if returned before due to exception
	if (MovesInHand) delete MovesInHand;
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::NewGameClick(TObject *Sender)
{
	StartGameForm->SetChangeTypeMode(false);
	if (StartGameForm->ShowModal()==mrCancel) return;
	StartUpGame();
	if (AutoShowAutoDice) AutoDice->ShowModal();
	CurOpenFile=*NullStr;
	Caption="Ludo - [New Game]";
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::ChangeTypeClick(TObject *Sender)
{
	StartGameForm->SetChangeTypeMode(true);
	if (StartGameForm->ShowModal())
	{
		HilightPos(posOutsideBoard);
		UpdateDescriptionText();
	}
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::MovesListImageMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Players[PlayerToMove]==ptHuman)
		if (Button==mbLeft) MovesInHand->Selected=short((X/25)*3+Y/25);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::DiceImageMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button==mbLeft) ManageClick(X+DiceImage->Left,Y+DiceImage->Top);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::HilightMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ManageClick(X+Hilight->Left,Y+Hilight->Top);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::FormKeyPress(TObject *Sender, char &Key)
{
	if (Key==' ')
	{
		static bool AlreadyIn=false;
		if (!AlreadyIn)
		{
			AlreadyIn=true;
			TimerTimer(Timer);
			AlreadyIn=false;
		}
	}
	else if (Key==27) EnableTimer(false);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::GameMenuClick(TObject *Sender)
{
	Save->Enabled=GameStarted;
	EnableTimer(false);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::ExitProgClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::ComputersLevelClick(TObject *Sender)
{
	SetBR->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::AboutClick(TObject *Sender)
{
	bool Enable=Timer->Enabled;
	Timer->Enabled=false;
	AboutForm->ShowModal();
	Timer->Enabled=Enable;
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::SaveAsClick(TObject *Sender)
{
	SaveDialog->FileName=*NullStr;
	SaveDialog->FilterIndex=GameStarted?1:2;
	if (!SaveDialog->Execute()) return;
	if (SaveDialog->FilterIndex==1)
	{
		if (!GameStarted)
		{
			Application->MessageBox("Can't save game because game has not "
				"started.","Error",MB_OK | MB_ICONSTOP);
			return;
		}
		CurOpenFile=SaveDialog->FileName;
		Caption="Ludo - "+ExtractFileName(CurOpenFile);
	}
	SaveToFile(SaveDialog->FileName,SaveDialog->FilterIndex==2);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::LoadClick(TObject *Sender)
{
	MSG msg;
#if (SECURE)
	if (RestrictedMode)
	{
		Application->MessageBox("This feature is disabled because the game is "
			"in restricted mode.","Shareware",MB_OK | MB_ICONSTOP);
		return;
	}
#endif
	OpenDialog->FileName=*NullStr;
	if (!OpenDialog->Execute()) return;
	if (LoadFromFile(OpenDialog->FileName,OpenDialog->FilterIndex==2))
	{
		// The following lines are to make sure that the dice doesn't spin when
		// the OpenDialog is closed by a double click.
		Screen->Cursor=crHourGlass;
		Sleep(500);
		while (PeekMessage(&msg,NULL,WM_LBUTTONUP,WM_LBUTTONUP,PM_REMOVE));
		Screen->Cursor=crDefault;
		if (OpenDialog->FilterIndex==1)
		{
			CurOpenFile=OpenDialog->FileName;
			Caption="Ludo - "+ExtractFileName(CurOpenFile);
			// AutoDice->ShowModal();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::LudoBoardMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button==mbLeft) ManageClick(X+LudoBoard->Left,Y+LudoBoard->Top);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::ShowNumbersClick(TObject *Sender)
{
	ShowNumbers->Checked=!ShowNumbers->Checked;
	RedrawBoard(ShowNumbers->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::SuggestClick(TObject *Sender)
{
	HintMove(!ShowNumbers->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::GameOptionsClick(TObject *Sender)
{
	OptionsForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::ResetGameClick(TObject *Sender)
{
	if (Application->MessageBox("Are you sure you want to reset?",
		"Reset Game",MB_YESNO | MB_ICONQUESTION)==mrNo)
		return;
	Caption="Ludo";
	HilightPos(posOutsideBoard);
	GameStarted=false;
	for (short i=0; i<4; i++)
		for (short j=0; j<4; j++)
			Pegs[i][j]->Position=posOutsideBoard;
	MovesInHand->ClearAll();
	DiceSetValue(6);
	EnableDice(false);
	GroupWinners->Visible=false;
	CurPlayerImage->Canvas->Brush->Color=clBlack;	// Transparent Color
	CurPlayerImage->Canvas->FillRect(Rect(0,0,
		CurPlayerImage->Width,CurPlayerImage->Height));
	PlayerType->Caption=*NullStr;
	CurOpenFile=*NullStr;
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::RestrictSize(TMessage &Message)
{
	LPMINMAXINFO lpmmi=(LPMINMAXINFO)Message.LParam;
	lpmmi->ptMaxTrackSize.x=640;
	lpmmi->ptMaxTrackSize.y=480;
	Message.Result=0;
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::SaveClick(TObject *Sender)
{
	if (CurOpenFile.IsEmpty()) SaveAsClick(Sender);
	else SaveToFile(CurOpenFile,false);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::TimerTimer(TObject *Sender)
{
	ManageClick(LudoBoard->Left,LudoBoard->Top);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::HelpMenuClick(TObject *Sender)
{
	EnableTimer(false);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::AutoDiceOptionsClick(TObject *Sender)
{
	AutoDice->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::AutomationClick(TObject *Sender)
{
	Playing=Automation->Down;
	if (GameStarted)
	{
		Timer->Enabled=Automation->Down;
		if (Timer->Enabled)
			ManageClick(LudoBoard->Left,LudoBoard->Top);
	}
	else if (Automation->Down)
	{
		NewGameClick(Automation);
		if (!GameStarted) Automation->Down=false;
		else Timer->Enabled=true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::AutomationMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button==mbRight && GameStarted)
	{
		bool Enable=Timer->Enabled;
		Timer->Enabled=false;
		AutoDice->ShowModal();
		Timer->Enabled=Enable;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::LogoMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button==mbRight)
	{
		bool Enable=Timer->Enabled;
		Timer->Enabled=false;
		OptionsForm->ShowModal();
		Timer->Enabled=Enable;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::MenuAutoClick(TObject *Sender)
{
	Playing=!Playing;
	if (Playing) TimerTimer(Timer);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::OptionsMenuClick(TObject *Sender)
{
	ChangeType->Enabled=GameStarted;
	AutoDiceOptions->Enabled=GameStarted;
	MenuAuto->Checked=Playing;
	EnableTimer(false);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::TodaysTipsClick(TObject *Sender)
{
	TipsForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::FormActivate(TObject *Sender)
{
	if (TipsAtStartup) TodaysTipsClick(TodaysTips);
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::DumbWListClick(TObject *Sender)
{
	DumbWList->Checked=SmartWinnersInfo;
	SmartWinnersInfo=!SmartWinnersInfo;
	ShowAllWinners();
}
//---------------------------------------------------------------------------
void __fastcall TLudoForm::PlayerTypeMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button==mbRight && GameStarted)
	{
		bool Enable=Timer->Enabled;
		Timer->Enabled=false;
		ChangeTypeClick(PlayerType);
		Timer->Enabled=Enable;
	}
}
//---------------------------------------------------------------------------

