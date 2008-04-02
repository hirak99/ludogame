//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TipsUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTipsForm *TipsForm;
bool TipsAtStartup;			// Show tips at StartUp?
int CurrentTipPos;			// Current tips number
extern AnsiString AppDir;	// Defined in MainUnit
TStringList *AllTips;
//---------------------------------------------------------------------------
__fastcall TTipsForm::TTipsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTipsForm::FormActivate(TObject *Sender)
{
	if (NextTip->Enabled) NextTipClick(this);
	ActiveControl=CloseTips;
}
//---------------------------------------------------------------------------
void __fastcall TTipsForm::FormDestroy(TObject *Sender)
{
	delete AllTips;
	TipsAtStartup=ShowTips->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TTipsForm::FormCreate(TObject *Sender)
{
	AllTips=new TStringList;
	try {AllTips->LoadFromFile("LudoTips.tip");}
	catch (EFOpenError *x)
	{
		Application->MessageBox(x->Message.c_str(),
			"Ludo",MB_OK | MB_ICONWARNING);
	}
	if (AllTips->Count==0) NextTip->Enabled=false;
	ShowTips->Checked=TipsAtStartup;
}
//---------------------------------------------------------------------------
void __fastcall TTipsForm::NextTipClick(TObject *Sender)
{
	if (CurrentTipPos >= AllTips->Count) CurrentTipPos=0;
	LabelTip->Caption=AllTips->Strings[CurrentTipPos++];
}
//---------------------------------------------------------------------------
void __fastcall TTipsForm::ResetTipsClick(TObject *Sender)
{
	CurrentTipPos=0;
	NextTipClick(this);
}
//---------------------------------------------------------------------------

