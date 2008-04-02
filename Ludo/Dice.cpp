//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "Dice.h"
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
extern AnsiString AppDir;	// Application directory
TImage *DiceImage;			// TImage for dice
TImageList *DiceImages;		// TImageList of dice
unsigned char CurDiceValue;	// Current dice value
short ThrowsToGo;			// Throws left (can be only 1 at max)
extern MovesList *MovesInHand;	// Defined in MainForm
bool DiceEnabled;			// Whether dice is enabled
char ExtraThrows;			// Conditions for getting extra throws
bool Discard3Sixes;			// Whether to discard three sixes
short ProbOfSix;			// Gives prob. of six when divided by 120
bool Alert3Sixes;			// Alert when we have 3 sixes
extern AnsiString DefaultBmps;
//---------------------------------------------------------------------------
__fastcall MovesList::MovesList(TCanvas *ToSet)
{
	Canvas=ToSet;
	ClearAll();
}
//---------------------------------------------------------------------------
void __fastcall MovesList::Add(unsigned char NewValue)
{
	if (FCount==MAXMOVESLISTCOUNT)
	{
		Application->MessageBox("MovesList out of bounds in MovesList::Add().",
			"Error",MB_ICONERROR | MB_OK);
		Application->Terminate();
	}
	Numbers[FCount++]=NewValue;
	if (FSelected==-1) FSelected=0;
	Redraw();
	if (NewValue==6 && (ExtraThrows & ET_SIX))
	{
		if (++CountSixes==3 && Discard3Sixes)
		{
			if (Alert3Sixes)
				Application->MessageBox ("Three sixes! Will be discarded.",
					"Message",MB_OK | MB_ICONINFORMATION);
			FCount-=(short)3;
			if (!FCount) FSelected=-1;
			CountSixes=0;
			Redraw();
		}
	}
	else
	{
		CountSixes=0;
		if (ThrowsToGo) ThrowsToGo--;
		if (ThrowsToGo==0) EnableDice(false);
	}
}
//---------------------------------------------------------------------------
void __fastcall MovesList::Remove(short Index)
{
	if (Index<0 || Index>=FCount) return;
	FCount--;
	if (!FCount) FSelected=-1;
	else if (FSelected==Index) FSelected=0;
	for (short i=Index; i<FCount; i++)
		Numbers[i]=Numbers[i+1];
	Redraw();
}
//---------------------------------------------------------------------------
void __fastcall MovesList::ClearAll()
{
	FCount=0; FSelected=-1; CountSixes=0;
	ThrowsToGo=1;
	EnableDice(true);
	Redraw();
}
//---------------------------------------------------------------------------
void __fastcall MovesList::Redraw()
{
	int x,y;
	Canvas->Brush->Color=clBtnFace;
	for (short i=0; i<18; i++)
	{
		x=(i/3)*25; y=(i % 3)*25;
		if (i<Count)
		{
			DiceImages->Draw(Canvas,x,y,Numbers[i]);
			if (i==FSelected) DiceImages->Draw(Canvas,x,y,0);
		}
		else Canvas->FillRect(Rect(x,y,x+25,y+25));
	}
}
//---------------------------------------------------------------------------
void __fastcall MovesList::SetSelected(short NewValue)
{
	if (NewValue<Count && NewValue!=FSelected)
	{
		FSelected=NewValue;
		Redraw();
	}
}
//---------------------------------------------------------------------------
void __fastcall EnableDice(bool Enable)
{
	TCanvas *Canvas=DiceImage->Canvas;
	if (DiceEnabled==Enable) return;
	DiceEnabled=Enable;
	if (Enable)
	{
		Canvas->Brush->Color=clBtnFace;
		Canvas->FillRect(Rect(0,0,DiceImage->Height,DiceImage->Width));
		DiceImages->Draw(DiceImage->Canvas,0,0,CurDiceValue);
	}
	else
	{
		Canvas->Pen->Color=clGray;
		for (int i=-25;i<25;i+=3)
		{
			Canvas->MoveTo(i,0);
			Canvas->LineTo(i+25,25);
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall IsDiceEnabled() {return DiceEnabled;}
//---------------------------------------------------------------------------
void __fastcall AddAThrow()
{
	ThrowsToGo=1;	// Not ThrowsToGo++ since can have maximum one throw
	EnableDice(true);
}
//---------------------------------------------------------------------------
void __fastcall NoMoreThrows()
{
	ThrowsToGo=0;
	EnableDice(false);
}
//---------------------------------------------------------------------------
short __fastcall GetThrowsLeft() {return ThrowsToGo;}
//---------------------------------------------------------------------------
void __fastcall DiceSetImage(TImage *ToSet)
{
	DiceImage=ToSet;
	DiceImages=new TImageList(ToSet->Owner);
	DiceImages->AllocBy=6;
	DiceImages->Height=25;
	DiceImages->Width=25;
	DiceImages->FileLoad(rtBitmap,AppDir+DefaultBmps+"Dice.bmp",clFuchsia);
	DiceSetValue(6);
}
//---------------------------------------------------------------------------
void __fastcall DiceSetValue(unsigned char Value)
{
	if (CurDiceValue==Value) return;
	CurDiceValue=Value;
	DiceImages->Draw(DiceImage->Canvas,0,0,Value);
	DiceImage->Repaint();
}
//---------------------------------------------------------------------------
unsigned char __fastcall DiceRandomRoll(int Wait)
{
	MSG msg;
	DWORD TimeEntered;
	static bool AlreadyInside=false;
	short Num;
	bool Hault=false;
	if (AlreadyInside || !DiceEnabled) return 0;
	AlreadyInside=true;
	TimeEntered=GetTickCount()+Wait;
	do
	{
		Sleep(65);
		Num=short(random(120));
		if (Num<ProbOfSix) DiceSetValue(6);
		else DiceSetValue((unsigned char)(random(5)+1));
		PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE);
		if (msg.message==WM_LBUTTONDOWN)
		{
			// To check if not inside dice area and LudoBoard area
			TImage *LudoBoard=LudoForm->LudoBoard;
			int X=LOWORD(msg.lParam),Y=HIWORD(msg.lParam);
			if (!((X>LudoBoard->Left && X<LudoBoard->Left+LudoBoard->Width
				&& Y>LudoBoard->Top && Y<LudoBoard->Top+LudoBoard->Height) ||
				(X>DiceImage->Left && X<DiceImage->Left+DiceImage->Width
				&& Y>DiceImage->Top && Y<DiceImage->Top+DiceImage->Height)))
				Hault=true;
		}
		else if (msg.message==WM_LBUTTONUP || msg.message==WM_KEYDOWN
			|| msg.message==WM_RBUTTONDOWN)
			Hault=true;
		if (!Hault || msg.message==WM_LBUTTONUP)
			PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE);
	} while (!Hault && (!Wait || GetTickCount()<TimeEntered));
	Application->ProcessMessages();
	AlreadyInside=false;
	MovesInHand->Add(CurDiceValue);
	return CurDiceValue;
}
//---------------------------------------------------------------------------

