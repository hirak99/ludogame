//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "GameOptions.h"
#include "Dice.h"
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionsForm *OptionsForm;
extern bool GameStarted;		// Whether game has started
extern char ExtraThrows;		// Defined in Dice.cpp
extern bool Discard3Sixes;		// Defined in Dice.cpp
extern short TimeOutHuman;		// Defined in Interface.cpp
extern short TimeOutComputer;	// Defined in Interface.cpp
extern short PegsOnStart;		// Defined in Interface.cpp
extern short ProbOfSix;			// Defined in Dice.cpp
extern short TotalPegs;			// Defined in Interface.cpp
extern char GameType;			// Defined in Interface.cpp
extern bool JumpDouble;			// Defined in Moves.cpp
extern char IgnoreStars;		// Defined in Pegs.cpp
extern bool AutoComputer;		// Defined in Interface.cpp
extern bool Alert3Sixes;		// Defined in Dice.cpp
extern bool AutoHuman;			// Defined in Interface.cpp
extern bool KillForHome;		// Defined in Moves.cpp
//---------------------------------------------------------------------------
__fastcall TOptionsForm::TOptionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::FormActivate(TObject *Sender)
{
	RestoreClick(Restore);
	ActiveControl=NULL;
	RadioNormal->Enabled=!GameStarted;
	RadioElem->Enabled=!GameStarted;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::RestoreClick(TObject *Sender)
{
	GetExtra1->Checked=ExtraThrows & ET_SIX;
	GetExtra2->Checked=ExtraThrows & ET_REMOVE;
	GetExtra3->Checked=ExtraThrows & ET_PROMOTE;
	GetExtra4->Checked=ExtraThrows & ET_NOMOVE;
	ChkDiscard->Checked=Discard3Sixes;
	SixAlert->Checked=Alert3Sixes;
	UpDown1->Position=short(TimeOutHuman/100);
	UpDown2->Position=short(TimeOutComputer/100);
	UDPegs->Position=TotalPegs;
	UDStart->Position=PegsOnStart;
	TrackBias->Position=ProbOfSix;
	TrackBiasChange(this);
	if (GameType==1) RadioNormal->Checked=true;
	else RadioElem->Checked=true;
	CheckJump->Checked=JumpDouble;
	CheckWStars->Checked=IgnoreStars & 1;
	CheckBStars->Checked=IgnoreStars / 2;
	UpDown3->Position=short(LudoForm->Timer->Interval/10);
	CheckAutoComp->Checked=AutoComputer;
	AutoOnlyMove->Checked=AutoHuman;
	cbKillForHome->Checked=KillForHome;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::TrackBiasChange(TObject *Sender)
{
	short Num1=short(TrackBias->Position),Num2=120;
	if (Num1<20)	{TrackBias->SelStart=Num1;	TrackBias->SelEnd=20;}
	else 			{TrackBias->SelStart=20;	TrackBias->SelEnd=Num1;}
	if (Num1==0) BiasLabel->Caption="No probability of six.";
	else if (Num1==20) BiasLabel->Caption="No biasing of die (Normal).";
	else
	{
		AnsiString Message;
		for (short i=2; i<=5; i++)
			while (Num1%i==0 && Num2%i==0) {Num1/=i; Num2/=i;}
		Message=IntToStr(Num1)+" six";
		if (Num1>1) Message+="es";
		Message+=" out of "+IntToStr(Num2)+" throws.";
		BiasLabel->Caption=Message;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::DefaultsClick(TObject *Sender)
{
	GetExtra1->Checked=true;
	GetExtra2->Checked=true;
	GetExtra3->Checked=true;
	GetExtra4->Checked=false;
	ChkDiscard->Checked=true;
	UDPegs->Position=4;
	UDStart->Position=0;
	TrackBias->Position=20;
	TrackBiasChange(this);
	if (RadioNormal->Enabled) RadioNormal->Checked=true;
	else if (!RadioNormal->Checked)
		Application->MessageBox("Can't change 'Aim of the Game' because "
		"game has already started.\n(Game->Reset and then change Aim)",
		"Warning",MB_OK | MB_ICONWARNING);
	CheckJump->Checked=false;
	CheckWStars->Checked=false;
	CheckBStars->Checked=false;
	cbKillForHome->Checked=false;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::OkButtonClick(TObject *Sender)
{
	ExtraThrows=char (GetExtra1->Checked * ET_SIX |
		GetExtra2->Checked * ET_REMOVE |
		GetExtra3->Checked * ET_PROMOTE |
		GetExtra4->Checked * ET_NOMOVE);
	Discard3Sixes=ChkDiscard->Checked;
	Alert3Sixes=SixAlert->Checked;
	TimeOutHuman=short(UpDown1->Position * 100);
	TimeOutComputer=short(UpDown2->Position * 100);
	TotalPegs=UDPegs->Position;
	PegsOnStart=UDStart->Position;
	ProbOfSix=short(TrackBias->Position);
	if (RadioNormal->Checked) GameType=1;
	else GameType=2;
	JumpDouble=CheckJump->Checked;
	IgnoreStars=char(CheckWStars->Checked | (CheckBStars->Checked<<1));
	LudoForm->Timer->Interval=UpDown3->Position*10;
	AutoComputer=CheckAutoComp->Checked;
	AutoHuman=AutoOnlyMove->Checked;
	KillForHome=cbKillForHome->Checked;
	ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::GetExtra1Click(TObject *Sender)
{
	ChkDiscard->Enabled=GetExtra1->Checked;
	ChkDiscard->Checked=ChkDiscard->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::CountersChange(TObject *Sender)
{
	if (UDStart->Position > UDPegs->Position) Ready->Text=Counters->Text;
	UDStart->Max=UDPegs->Position;
	if (UDPegs->Position > 1) SayCounter->Caption="counters,";
	else SayCounter->Caption="counter,";
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::AutoStopChange(TObject *Sender)
{
	int Value;
	if (Sender==HumanStop) Value=UpDown1->Position;
	else Value=UpDown2->Position;
	if (Value) ((TEdit *)(Sender))->Text=IntToStr(Value);
	else ((TEdit *)(Sender))->Text="Infinite";
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::EditAutoIntervalExit(TObject *Sender)
{
	EditAutoInterval->Text=UpDown3->Position;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::ChkDiscardClick(TObject *Sender)
{
	SixAlert->Enabled=ChkDiscard->Checked;
	SixAlert->Checked=SixAlert->Enabled;
}
//---------------------------------------------------------------------------

