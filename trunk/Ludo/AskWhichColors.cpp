//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AskWhichColors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWhichColors *WhichColors;
//---------------------------------------------------------------------------
__fastcall TWhichColors::TWhichColors(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TWhichColors::UpDownClick(TObject *Sender,
	  TUDBtnType Button)
{
	TUpDown *ToChange;
	if (Sender==UpDown1) ToChange=UpDown2;
	else ToChange=UpDown1;
	ToChange->Position=short(Total-((TUpDown *)(Sender))->Position);
}
//---------------------------------------------------------------------------
void __fastcall TWhichColors::FormActivate(TObject *Sender)
{
	UpDown1->Min=0; UpDown2->Min=0;
	if (Total==1)
	{
		if (UpDown1->Max > 1) UpDown1->Max=1;
		if (UpDown2->Max > 1) UpDown2->Max=1;
	}
	else
	{
		if (UpDown1->Max > 2) UpDown1->Max=2;
		if (UpDown2->Max > 2) UpDown2->Max=2;
		if (UpDown1->Max==2 && UpDown2->Max==1) UpDown1->Min=1;
		if (UpDown2->Max==2 && UpDown1->Max==1) UpDown2->Min=1;
	}
	UpDown1->Position=UpDown1->Max;
	UpDown2->Position=short(Total-UpDown1->Max);
}
//---------------------------------------------------------------------------

