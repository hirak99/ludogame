//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FileIO.h"
#include "Pegs.h"
#include "Moves.h"
#include "Dice.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
const char FileHeader[]="Ludo311";
// Shared from Interface.cpp
extern enum Modes CurrentMode;		// Current mode, enum defined in Moves.h
extern enum PlayerType Players[4];	// Players,		 enum defined in Moves.h
extern short PlayerToMove;			// Current player
extern short Winners;			// Number of winners till now
extern short MaxWinners;  		// Maximum no. of winners (total no. of players)
extern short WinnersList[4];	// List of winners
extern Peg *Pegs[4][4];			// First [index] color
extern MovesList *MovesInHand;	// Moves buffer
extern char ExtraThrows;		// Defined in Dice.cpp
extern bool Discard3Sixes;		// Defined in Dice.cpp
extern char GameType;			// Defined in Interface.cpp
extern bool JumpDouble;			// Defined in Moves.cpp
extern char IgnoreStars;		// Defined in Pegs.cpp
extern short PegsOnStart;		// Defined in Interface.cpp
extern short ProbOfSix;			// Defined in Dice.cpp
extern short TotalPegs;			// Defined in Interface.cpp
extern bool GameStarted;		// Defined in Interface.cpp
extern bool AutoThrowDice[4];	// Defined in AutoDiceUnit.cpp
extern bool KillForHome;		// Defined in Moves.cpp
extern short HasKilled[4];		// Defined in Moves.cpp
//---------------------------------------------------------------------------
// void __fastcall SaveNumber(short Number,TFileStream *File)
// inline void __fastcall LoadNumber(short &Number,TFileStream *File)
// bool AskToRestore(AnsiString &Question)
//---------------------------------------------------------------------------
void __fastcall SaveNumber(short Number,TFileStream *File)
{
	if (Number>255 || Number<0)
	{
		Application->MessageBox("Invalid number in procedure SaveNumber, "
			"might cause error while loading this file.",
			"Error",MB_OK | MB_ICONSTOP);
	}
	unsigned char Ch=(unsigned char)Number;
	File->Write(&Ch,sizeof(unsigned char));
}
inline short __fastcall LoadNumber(TFileStream *File)
{
	unsigned char Number;
	File->Read(&Number,sizeof(unsigned char));
	return Number;
}
//---------------------------------------------------------------------------
bool AskToRestore(char *Question)
{
	if (Application->MessageBox(Question,"Rule Mismatch",
		MB_YESNO | MB_ICONQUESTION)==mrYes)
		return true;
	else return false;
}
//---------------------------------------------------------------------------
void __fastcall SaveToFile(AnsiString &TheFile,bool RulesOnly)
{
	TFileStream *SaveFile;
	try {
		SaveFile=new TFileStream(TheFile,fmCreate);
	}
	catch (...) {
		Application->MessageBox("Write access to specified file denied.",
			"Error",MB_OK | MB_ICONSTOP);
		return;
	}
	// Save FileHeader without including the terminating null char.
	SaveFile->Write(FileHeader,sizeof(FileHeader)-1);
	// Saving the game rules
	SaveNumber(GameType,SaveFile);
	SaveNumber(ExtraThrows,SaveFile);
	SaveNumber(Discard3Sixes,SaveFile);
	SaveNumber(JumpDouble,SaveFile);
	SaveNumber(IgnoreStars,SaveFile);
	SaveNumber(KillForHome,SaveFile);
	SaveNumber(ProbOfSix,SaveFile);
	if (RulesOnly)
	{
		SaveNumber(PegsOnStart,SaveFile);
		SaveNumber(TotalPegs,SaveFile);
	}
	else
	{
		// Saving game status
		char AutoDice=0;
		SaveFile->Write(&CurrentMode,sizeof(CurrentMode));
		SaveNumber(GetTeamMate(ColorRed),SaveFile);
		SaveFile->Write(Players,sizeof(Players));
		SaveNumber(PlayerToMove,SaveFile);
		SaveNumber(MaxWinners,SaveFile);
		SaveNumber(Winners,SaveFile);
		SaveFile->Write(WinnersList,sizeof(WinnersList));
		for (short i=0; i<4; i++)
		{
			for (short j=0; j<4; j++)
				SaveNumber(Pegs[i][j]->Position,SaveFile);
			SaveNumber(HasKilled[i],SaveFile);
			AutoDice<<=1;
			if (AutoThrowDice[i]) AutoDice++;
		}
		SaveNumber(AutoDice,SaveFile);
		SaveNumber(MovesInHand->Count,SaveFile);
		for (short i=0; i<MovesInHand->Count; i++)
		{
			short Value=MovesInHand->Values[i];
			SaveNumber(Value,SaveFile);
		}
		SaveNumber(GetThrowsLeft(),SaveFile);
	}
	delete SaveFile;
}
//---------------------------------------------------------------------------
bool __fastcall LoadFromFile(AnsiString &TheFile,bool RulesOnly)
{
	TFileStream *LoadFile;
	char Test[sizeof(FileHeader)-1];
	short Count;
	try {
		LoadFile=new TFileStream(TheFile,fmOpenRead);
	}
	catch (...) {
		Application->MessageBox("Read access to specified file denied.",
			"Error",MB_OK | MB_ICONSTOP);
		return false;
	}
	LoadFile->Read(Test,sizeof(FileHeader)-1);
	for (short i=0; i<sizeof(FileHeader)-1; i++)
		if (FileHeader[i]!=Test[i])
		{
			Application->MessageBox("Specified file is not a Ludo file "
				"or its format is not compatible with current version.",
				"Error",MB_OK | MB_ICONSTOP);
			delete LoadFile;
			return false;
		}
	// Loading game rules
	Test[0]=LoadNumber(LoadFile);
	if (Test[0]!=GameType)
	{
		if (RulesOnly)
		{
			if (GameStarted)
				Application->MessageBox("Game Type will not be restored since "
					"a game has already started.","Warning",
					MB_OK | MB_ICONWARNING);
			else
				GameType=Test[0];
		}
		else
		{
			if (AskToRestore("Game Type doesn't match. "
				"Can't proceed without restoring game type.\n"
				"Restore game type?"))
				GameType=Test[0];
			else
			{
				delete LoadFile;
				return false;
			}
		}
	}
	Test[0]=LoadNumber(LoadFile);
	if (Test[0]!=ExtraThrows)
	{
		if (RulesOnly || AskToRestore("Extra Throw rules don't match.\n"
			"Restore extra throw rules?"))
		{
			if ((~ExtraThrows & ET_SIX) && (Test[0] & ET_SIX))
				Discard3Sixes=true;
			ExtraThrows=Test[0];
		}
	}
	Test[0]=LoadNumber(LoadFile);
	if ((ExtraThrows & ET_SIX) && Test[0]!=Discard3Sixes)
	{
		if (RulesOnly || AskToRestore("Discard three sixes rule doesn't "
			"match.\nRestore discard three sixes rule?"))
			Discard3Sixes=Test[0];
	}
	Test[0]=LoadNumber(LoadFile);
	if (Test[0]!=JumpDouble)
	{
		if (RulesOnly || AskToRestore("Jump Doubles rule doesn't match.\n"
			"Restore Jump Doubles rule?"))
			JumpDouble=Test[0];
	}
	Test[0]=LoadNumber(LoadFile);
	if (Test[0]!=IgnoreStars)
	{
		if (RulesOnly || AskToRestore("Ignore Stars rule doesn't match.\n"
			"Restore Ignore Stars rule?"))
			IgnoreStars=Test[0];
	}
	Test[0]=LoadNumber(LoadFile);
	if (Test[0]!=KillForHome)
	{
		if (RulesOnly || AskToRestore("Kill for Home rule doesn't match.\n"
			"Restore Kill for Home rule?"))
			KillForHome=Test[0];
	}
	Count=LoadNumber(LoadFile);
	if (Count!=ProbOfSix)
	{
		if (RulesOnly || AskToRestore("Probability of six doesn't match.\n"
			"Restore probability of six?"))
			ProbOfSix=Count;
	}
	if (RulesOnly)
	{
		PegsOnStart=LoadNumber(LoadFile);
		TotalPegs=LoadNumber(LoadFile);
	}
	else
	{
		// Loading game
		char AutoDice;
		LoadFile->Read(&CurrentMode,sizeof(CurrentMode));
		SetTeam(ColorRed,LoadNumber(LoadFile));
		LoadFile->Read(Players,sizeof(Players));
		PlayerToMove=LoadNumber(LoadFile);
		MaxWinners=LoadNumber(LoadFile);
		Winners=LoadNumber(LoadFile);
		LoadFile->Read(WinnersList,sizeof(WinnersList));
		for (short i=0; i<4; i++)
		{
			for (short j=0; j<4; j++)
				Pegs[i][j]->Position=LoadNumber(LoadFile);
			HasKilled[i]=LoadNumber(LoadFile);
		}
		StartUpGame(true);	// true indicates loading from file
		AutoDice=LoadNumber(LoadFile);
		for (short i=3; i>=0; i--)
		{
			AutoThrowDice[i]=(AutoDice & 1);
			AutoDice>>=1;
		}
		Count=LoadNumber(LoadFile);
		for (short i=0; i<Count; i++)
			MovesInHand->Add(LoadNumber(LoadFile));
		if (LoadNumber(LoadFile)) AddAThrow();	// Since can't have >1 throws
		else NoMoreThrows();
	}
	delete LoadFile;
	return true;
}
//---------------------------------------------------------------------------

