//---------------------------------------------------------------------------
#include <vcl.h>
#include <IniFiles.hpp>
#pragma hdrstop

#include "StartGameUnit.h"
#include "Pegs.h"
#include "Moves.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStartGameForm *StartGameForm;
extern TIniFile *IniFile;
extern enum Modes CurrentMode;
extern enum PlayerType Players[4];
bool ChangeTypeMode=false;
#if (SECURE)
bool RestrictedMode=false;		// The game will run, but restricted
#endif
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::SetChangeTypeMode(bool Value)
{
	ChangeTypeMode=Value;
}
//---------------------------------------------------------------------------
__fastcall TStartGameForm::TStartGameForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::FormCreate(TObject *Sender)
{
	int Temp;
	if (IniFile->ReadBool("StartUp","Mode",false))
		GameModes->ActivePage=Mode2;
	Temp=IniFile->ReadInteger("StartUp","SingleState",185);
	if (Temp & 1) RedPlaying->Checked=true;
	RedPlayer->ItemIndex=(Temp & 2)/2;
	if (Temp & 4) GreenPlaying->Checked=true;
	GreenPlayer->ItemIndex=(Temp & 8)/8;
	if (Temp & 16) YellowPlaying->Checked=true;
	YellowPlayer->ItemIndex=(Temp & 32)/32;
	if (Temp & 64) BluePlaying->Checked=true;
	BluePlayer->ItemIndex=(Temp & 128)/128;
	ComboTeam->ItemIndex=Temp/256;
	Temp=IniFile->ReadInteger("StartUp","DoubleState",8);
	DoubleGroup1->ItemIndex=(Temp & 3);
	DoubleGroup2->ItemIndex=(Temp & 3);
	DoublePlayer1->ItemIndex=(Temp & 4)/4;
	DoublePlayer2->ItemIndex=(Temp & 8)/8;
}
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::FormActivate(TObject *Sender)
{
	Mode1->TabVisible=true;
	Mode2->TabVisible=true;
	DoubleGroup1->Enabled=true;
	DoubleGroup2->Enabled=true;
	DoublePlayer1->Enabled=true;
	DoublePlayer2->Enabled=true;
	ComboTeam->Enabled=true;
	if (ChangeTypeMode)
	{
		Caption="Select Player Types";
		if (CurrentMode==ModeSingle)
		{
			GameModes->ActivePage=Mode1;
			Mode2->TabVisible=false;
			// Following 4 lines turn ComboTeam->Visible on if necessary
			RedPlaying->Checked=(Players[ColorRed]!=ptNone);
			GreenPlaying->Checked=(Players[ColorGreen]!=ptNone);
			YellowPlaying->Checked=(Players[ColorYellow]!=ptNone);
			BluePlaying->Checked=(Players[ColorBlue]!=ptNone);
			RedPlaying->Enabled=false;
			GreenPlaying->Enabled=false;
			YellowPlaying->Enabled=false;
			BluePlaying->Enabled=false;
			if (Players[ColorRed]==ptHuman) RedPlayer->ItemIndex=0;
			else RedPlayer->ItemIndex=1;
			if (Players[ColorGreen]==ptHuman) GreenPlayer->ItemIndex=0;
			else GreenPlayer->ItemIndex=1;
			if (Players[ColorYellow]==ptHuman) YellowPlayer->ItemIndex=0;
			else YellowPlayer->ItemIndex=1;
			if (Players[ColorBlue]==ptHuman) BluePlayer->ItemIndex=0;
			else BluePlayer->ItemIndex=1;
			if (ComboTeam->Visible)
			{
				switch (GetTeamMate(ColorRed))
				{
					case ColorRed: ComboTeam->ItemIndex=0; break;
					case ColorYellow: ComboTeam->ItemIndex=1; break;
					case ColorGreen: ComboTeam->ItemIndex=2; break;
					case ColorBlue: ComboTeam->ItemIndex=3; break;
				}
				ComboTeam->Enabled=false;
			}
		}
		else
		{
			GameModes->ActivePage=Mode2;
			Mode1->TabVisible=false;
			switch (GetTeamMate(ColorRed))
			{
				case ColorYellow: DoubleGroup1->ItemIndex=0; break;
				case ColorGreen: DoubleGroup1->ItemIndex=1; break;
				case ColorBlue: DoubleGroup1->ItemIndex=2; break;
			}
			DoubleGroup1->Enabled=false;
			DoubleGroup2->Enabled=false;
			if (Players[ColorRed]==ptHuman) DoublePlayer1->ItemIndex=0;
			else DoublePlayer1->ItemIndex=1;
			if (Players[GetOpponent(ColorRed)]==ptHuman)
				DoublePlayer2->ItemIndex=0;
			else DoublePlayer2->ItemIndex=1;
			DoublePlayer1->Enabled=(Players[ColorRed]!=ptNone);
			DoublePlayer2->Enabled=(Players[GetOpponent(ColorRed)]!=ptNone);
		}
	}
	else
	{
		Caption="Select Game Type";
		ManageClick();
	}
	ActiveControl=GameModes;
}
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::ManageClick()
{
	TCheckBox *CheckBox,*SingleBox;
	short Count=0;
	for (short i=0; i<ComponentCount; i++)
		if (Components[i]->ClassType()==__classid(TCheckBox))
		{
			CheckBox=(TCheckBox *)Components[i];
			if (CheckBox->Tag!=1) continue;
			CheckBox->Enabled=true;
			if (CheckBox->Checked) {SingleBox=CheckBox; Count++;}
		}
	if (Count==1) SingleBox->Enabled=false;
	ComboTeam->Visible=(Count==4);
}
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::PlayingClick(TObject *Sender)
{
	TComboBox *Combo;
	ManageClick();
	if (Sender==RedPlaying) Combo=RedPlayer;
	else if (Sender==GreenPlaying) Combo=GreenPlayer;
	else if (Sender==YellowPlaying) Combo=YellowPlayer;
	else if (Sender==BluePlaying) Combo=BluePlayer;
	Combo->Visible=((TCheckBox *)Sender)->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::DoubleGroupChange(TObject *Sender)
{
	TComboBox *Other;
	if (Sender==DoubleGroup1) Other=DoubleGroup2;
	else Other=DoubleGroup1;
    Other->ItemIndex=((TComboBox *)Sender)->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::OkButtonClick(TObject *Sender)
{
	if (GameModes->ActivePage==Mode1)
	{
#if (SECURE)
		if (RestrictedMode && (GreenPlaying->Checked || BluePlaying->Checked))
		{
			Application->MessageBox("You can only play with Red and Yellow in "
				"restructed mode.","Shareware",MB_OK | MB_ICONSTOP);
			return;
		}
#endif
		for (int i=0; i<4; i++)
			Players[i]=ptNone;
		CurrentMode=ModeSingle;
		if (RedPlaying->Checked)
			Players[0]=PlayerType(RedPlayer->ItemIndex+1);	 // ItemIndex+1
		if (GreenPlaying->Checked)						   	 // because first
			Players[1]=PlayerType(GreenPlayer->ItemIndex+1); // value of
		if (YellowPlaying->Checked)							 // enum PlayerType
			Players[2]=PlayerType(YellowPlayer->ItemIndex+1);// is ptNone
		if (BluePlaying->Checked)
			Players[3]=PlayerType(BluePlayer->ItemIndex+1);
		if (!ChangeTypeMode)
		{
			if (ComboTeam->Visible)
				switch (ComboTeam->ItemIndex)
				{
					case 0: SetNoTeam(); break;
					case 1: SetTeam(ColorRed,ColorYellow); break;
					case 2: SetTeam(ColorRed,ColorGreen); break;
					case 3: SetTeam(ColorRed,ColorBlue); break;
				}
			else SetNoTeam();
		}
	}
	else
	{
		// I1,I2 are for Player1, I3,I4 are for Player2
		short I1=ColorRed,I2,I3,I4;
#if (SECURE)
		if (RestrictedMode &&
			(DoublePlayer1->ItemIndex==1 || DoublePlayer2->ItemIndex==1))
		{
			Application->MessageBox("You can only play Human vs. Human in "
				"restructed mode.","Shareware",MB_OK | MB_ICONSTOP);
			return;
		}
#endif
		CurrentMode=ModeDouble;
		switch (DoubleGroup1->ItemIndex)
		{
			case 0: SetTeam(ColorRed,ColorYellow); I2=ColorYellow; break;
			case 1: SetTeam(ColorRed,ColorGreen); I2=ColorGreen; break;
			case 2: SetTeam(ColorRed,ColorBlue); I2=ColorBlue; break;
		}
		if (I2==1) {I3=2; I4=3;}
		else {I3=1; I4=short(5-I1-I2);}
		Players[I1]=Players[I2]=PlayerType(DoublePlayer1->ItemIndex+1);
		Players[I3]=Players[I4]=PlayerType(DoublePlayer2->ItemIndex+1);
	}
	ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TStartGameForm::FormDestroy(TObject *Sender)
{
	int Temp;
	IniFile->WriteBool("StartUp","Mode",GameModes->ActivePage==Mode2);
	if (RedPlaying->Checked) Temp=1; else Temp=0;
	Temp|=RedPlayer->ItemIndex * 2;
	if (GreenPlaying->Checked) Temp|=4;
	Temp|=GreenPlayer->ItemIndex * 8;
	if (YellowPlaying->Checked) Temp|=16;
	Temp|=YellowPlayer->ItemIndex * 32;
	if (BluePlaying->Checked) Temp|=64;
	Temp|=BluePlayer->ItemIndex * 128;
	Temp|=ComboTeam->ItemIndex * 256;
	IniFile->WriteInteger("StartUp","SingleState",Temp);
	Temp=DoubleGroup1->ItemIndex;
	Temp|=DoublePlayer1->ItemIndex*4;
	Temp|=DoublePlayer2->ItemIndex*8;
	IniFile->WriteInteger("StartUp","DoubleState",Temp);
}
//---------------------------------------------------------------------------

