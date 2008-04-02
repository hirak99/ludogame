//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SetBRUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSetBR *SetBR;
extern short BlunderRange[4];	// Defined in Interface.cpp
short WorkSpace[4];
//---------------------------------------------------------------------------
__fastcall TSetBR::TSetBR(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSetBR::EnableSetAll()
{
	bool Enable=false;
	for (short i=1; i<4 && !Enable; i++)
		if (WorkSpace[0]!=WorkSpace[i]) Enable=true;
	SetAll->Enabled=Enable;
}
//---------------------------------------------------------------------------
void __fastcall TSetBR::OkButtonClick(TObject *Sender)
{
	for (short i=0; i<4; i++)
		BlunderRange[i]=WorkSpace[i];
	ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TSetBR::FormActivate(TObject *Sender)
{
	for (short i=0; i<4; i++)
		WorkSpace[i]=BlunderRange[i];
	TabColors->TabIndex=0;
	TabColorsChange(TabColors);	// To reflect the current BRTrack->Position
	ActiveControl=TabColors;
}
//---------------------------------------------------------------------------
void __fastcall TSetBR::TabColorsChange(TObject *Sender)
{
	BRTrack->Position=BRTrack->Max-WorkSpace[TabColors->TabIndex];
	BRTrackChange(BRTrack);
}
//---------------------------------------------------------------------------
void __fastcall TSetBR::SetAllClick(TObject *Sender)
{
	for (short i=0; i<4; i++)
		WorkSpace[i]=WorkSpace[TabColors->TabIndex];
	SetAll->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TSetBR::BRTrackChange(TObject *Sender)
{
	BRTrack->SelEnd=BRTrack->Position;
	WorkSpace[TabColors->TabIndex]=short(BRTrack->Max-BRTrack->Position);
	ValueLabel->Caption=IntToStr(BRTrack->Position);
	EnableSetAll();
}
//---------------------------------------------------------------------------

