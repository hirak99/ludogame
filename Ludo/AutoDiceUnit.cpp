//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AutoDiceUnit.h"
#include "Moves.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAutoDice *AutoDice;
bool AutoThrowDice[4];			// Whose dice are to be auto-thrown
extern enum PlayerType Players[4];	// Defined in Inteerface.cpp
extern enum Modes CurrentMode;
extern bool GameStarted;
bool AutoShowAutoDice;
bool AutoCheckComputer;
bool AutoCheckHuman;
TCheckBox *CheckBoxes[4];		// The checkboxes
//---------------------------------------------------------------------------
__fastcall TAutoDice::TAutoDice(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAutoDice::FormActivate(TObject *Sender)
{
	for (int i=3; i>=0; i--)
	{
		if (Players[i]==ptNone)
		{
			CheckBoxes[i]->Enabled=false;
			CheckBoxes[i]->Checked=false;
		}
		else
		{
			CheckBoxes[i]->Enabled=true;
			CheckBoxes[i]->Checked=AutoThrowDice[i];
			CheckBoxClick(CheckBoxes[i]);
		}
	}
	Always->Checked=AutoShowAutoDice;
	Computer->Checked=AutoCheckComputer;
	Human->Checked=AutoCheckHuman;
	ActiveControl=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TAutoDice::OkClick(TObject *Sender)
{
	for (int i=0; i<4; i++)
		AutoThrowDice[i]=CheckBoxes[i]->Checked;
	AutoShowAutoDice=Always->Checked;
	AutoCheckComputer=Computer->Checked;
	AutoCheckHuman=Human->Checked;
	ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAutoDice::FormCreate(TObject *Sender)
{
	CheckBoxes[0]=AutoDiceR;
	CheckBoxes[1]=AutoDiceG;
	CheckBoxes[2]=AutoDiceY;
	CheckBoxes[3]=AutoDiceB;
	ExtendClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TAutoDice::CheckBoxClick(TObject *Sender)
{
	if (GameStarted && CurrentMode==ModeDouble)
	{
		TCheckBox *CurBox=(TCheckBox *)Sender;
		CheckBoxes[GetTeamMate(short(CurBox->Tag))]->Checked=CurBox->Checked;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAutoDice::ExtendClick(TObject *Sender)
{
	if (Extend->Down)
	{
		Options->Visible=true;
		Bevel1->Height=149*PixelsPerInch/96;
		Ok->Top=153*PixelsPerInch/96;
		Cancel->Top=153*PixelsPerInch/96;
		Height=205*PixelsPerInch/96;
	}
	else
	{
		Options->Visible=false;
		Bevel1->Height=73*PixelsPerInch/96;
		Ok->Top=77*PixelsPerInch/96;
		Cancel->Top=77*PixelsPerInch/96;
		Height=129*PixelsPerInch/96;
	}
}
//---------------------------------------------------------------------------
      