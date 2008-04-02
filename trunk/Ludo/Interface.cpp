//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "Interface.h"
#include "MainUnit.h"
#include "Pegs.h"
#include "Dice.h"
#include "Moves.h"
#include "AskWhichColors.h"
#include "MovesRec.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
extern char ExtraThrows;	// Defined in Dice.cpp
enum Modes CurrentMode;		// Current mode, enum defined in Moves.h
enum PlayerType Players[4];	// Players,		 enum defined in Moves.h
extern Peg *Pegs[4][4];
extern MovesList *MovesInHand;	// Moves buffer
short PlayerToMove;			// Current player
bool GameStarted;			// Whether game is going on
int Count1,Count2;			// Used by Step... functions
AMove Moves[40];			// For use in subs
unsigned char DiceIndices[40];	// DiceIndices for each of the Moves[]
short Winners;				// Number of winners till now
short MaxWinners;			// Maximum no. of winners (total no. of players)
short WinnersList[4];		// List of winners
short BlunderRange[4];		// Control computer's blunder
extern TLabel *WinnerLabels[4];
extern TImage *WinnerImages[4];
short TimeOutHuman;			// Dice timeout for humans
short TimeOutComputer;		// Dice timeout for computers
short PegsOnStart;			// Number of counters at start when game starts
short TotalPegs;			// Total no. of pegs for each color
char GameType;				// Type of game
bool FirstMove;				// Whether player has already moved in this turn
extern bool AutoThrowDice[4];	// Defined in AutoDiceUnit.cpp
bool AutoComputer;	  			// Is computer player automatic?
bool AutoHuman;					// Automatically moves if only 1 possible move
extern bool AutoCheckComputer;	// Defined in AutoDiceUnit.cpp
extern bool AutoCheckHuman;		// Defined in AutoDiceUnit.cpp
bool SmartWinnersInfo;		// Whether to show the last winner before game ends
extern short HasKilled[4];		// Defined in Moves.cpp
//---------------------------------------------------------------------------
// void __fastcall DrawPlayer(TCanvas *Canvas,short Player=PlayerToMove)
// void UpdateDescriptionText()
// void __fastcall ShowAllWinners()
// void __fastcall RegisterWinner(short Player)
// int __fastcall GetAllMoves()
// int __fastcall CountMoves()
// void __fastcall MakeMove(short Position,Peg *Peg1,Peg *Peg2=NULL)
// void __fastcall StepHumanMove(short OldPos,short NewPos)
// bool __fastcall StepComputerMove(short OldPos,short NewPos)
// void __fastcall MouseDownHandler(int X,int Y)
//---------------------------------------------------------------------------
void __fastcall DrawPlayer(TCanvas *Canvas,short Player=PlayerToMove)
{
	Canvas->Brush->Color=clBtnFace;
	Canvas->FillRect(Rect(0,0,46,23));
	if (CurrentMode==ModeDouble)
	{
		short P1,P2;
		P1=Player; P2=GetTeamMate(P1);
		Canvas->Draw(0,0,Pegs[P1][0]->Picture->Bitmap);
		Canvas->Draw(23,0,Pegs[P2][0]->Picture->Bitmap);
	}
	else
	{
		Canvas->Draw(0,0,Pegs[Player][0]->Picture->Bitmap);
		if (IsTeamedGame())
			Canvas->StretchDraw(Rect(10,10,23,23),
				Pegs[GetTeamMate(Player)][0]->Picture->Bitmap);
	}
	return;
}
//---------------------------------------------------------------------------
void __fastcall UpdateDescriptionText()
{
	if (Players[PlayerToMove]==ptHuman)
		LudoForm->PlayerType->Caption="(Human to play)";
	else
	{
		LudoForm->PlayerType->Caption="(Computer to play)";
	}
	LudoForm->PlayerType->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall ShowAllWinners()
{
	int LastWinner,LastWinnerPos=0xFF;
	if (SmartWinnersInfo && Winners==MaxWinners-1)
	{
		for (LastWinner=0; LastWinner<4; LastWinner++)
			if (Players[LastWinner]!=ptNone) break;
		if (GameType==1)
			LastWinnerPos=MaxWinners-1;
		else
		{
			short MyCount;
			MyCount=CountDoublesPegs((short)LastWinner);
			if (MyCount)
				for (LastWinnerPos=Winners; LastWinnerPos>0 &&
					MyCount>GetDoubleCount(76,WinnersList[LastWinnerPos-1]);
					LastWinnerPos--);
			else
				for (LastWinnerPos=Winners; LastWinnerPos>0 &&
					GetDoubleCount(76,WinnersList[LastWinnerPos-1])==0;
					LastWinnerPos--);
		}
	}
	for (short i=0; i<MaxWinners-(LastWinnerPos!=0xFF); i++)
		if (i<Winners) DrawPlayer(WinnerImages[i+(i>=LastWinnerPos)]->Canvas,
			WinnersList[i]);
		else WinnerImages[i+(i>=LastWinnerPos)]->Canvas->
			FillRect(Rect(0,0,46,23));
	if (LastWinnerPos!=0xFF)
		DrawPlayer(WinnerImages[LastWinnerPos]->Canvas,(short)LastWinner);
}
//---------------------------------------------------------------------------
void __fastcall RegisterWinner(short Player)
{
	if (CurrentMode==ModeDouble && GetTeamMate(Player)<Player)
		Player=GetTeamMate(Player);
	Players[Player]=ptNone;
	if (CurrentMode==ModeDouble)
		Players[GetTeamMate(Player)]=ptNone;
	if (GameType==1)
		WinnersList[Winners]=Player;
	else
	{
		short j,MyCount=GetDoubleCount(76,Player);
		if (MyCount)
			for (j=Winners; j>0 && MyCount>GetDoubleCount(76,WinnersList[j-1]); j--)
				WinnersList[j]=WinnersList[j-1];
		else
			for (j=Winners; j>0 && GetDoubleCount(76,WinnersList[j-1])==0; j--)
				WinnersList[j]=WinnersList[j-1];
		WinnersList[j]=Player;
	}
	if (++Winners==MaxWinners)
	{
		GameStarted=false;
		// Game Ended. Draw the Peg in Black & White.
		TCanvas *Canvas=LudoForm->CurPlayerImage->Canvas;
		for (int i=0; i<LudoForm->CurPlayerImage->Width; i++)
		for (int j=0; j<LudoForm->CurPlayerImage->Height; j++)
		{
			TColor Color=Canvas->Pixels[i][j];
			if (Color!=Canvas->Pixels[0][0])
			{
				int Value=((Color & 0xFF)+(Color/0x100 & 0xFF)+
					Color/0x10000)/3;
				Value=(Value+0xFF)/2;
				Canvas->Pixels[i][j]=TColor(Value*0x10101);
			}
		}
		EnableDice(false);
		MovesInHand->ClearAll();
		LudoForm->PlayerType->Caption="GAME OVER";
	}
	ShowAllWinners();
}
//---------------------------------------------------------------------------
void __fastcall StartUpGame(bool LoadingFile)
{
	if (!LoadingFile)
	{
		Winners=0; MaxWinners=0;
		for (int i=0; i<4; i++)
		{
			if (Players[i]!=ptNone) MaxWinners++;
			for (int j=0; j<4; j++)
			{
				if (Players[i]==ptNone || j<4-TotalPegs)
					Pegs[i][j]->Position=posOutsideBoard;
				else
				{
					if (j<PegsOnStart+4-TotalPegs)
						Pegs[i][j]->Position=short(8+13*i);
					else Pegs[i][j]->SendToHome();
				}
			}
			HasKilled[i]=0;
		}
		if (CurrentMode==ModeDouble) MaxWinners=2;
	}
	for (int i=0; i<4; i++)
	{
		if (Winners>i)
		{
			short Temp=PlayerToMove;
			PlayerToMove=WinnersList[i];
			PlayerToMove=Temp;
		}
		else
		{
			WinnerLabels[i]->Visible = i<MaxWinners;
			WinnerImages[i]->Visible = i<MaxWinners;
		}
	}
	ShowAllWinners();
	LudoForm->GroupWinners->Height=17+32*MaxWinners;
	LudoForm->GroupWinners->Visible=true;
	HilightPos(posOutsideBoard);
	MovesInHand->ClearAll();
	// Carefull! The line below needs atleast one player to be not of the
	//   type ptNone. Otherwise it will go into infinite loop and hang.
	if (!LoadingFile)
	{
		for (PlayerToMove=0; Players[PlayerToMove]==ptNone; PlayerToMove++);
		for (int i=0; i<4; i++)
		{
			AutoThrowDice[i]=(Players[i]==ptHuman && AutoCheckHuman)
				|| (Players[i]==ptComputer && AutoCheckComputer);
		}
	}
	DrawPlayer(LudoForm->CurPlayerImage->Canvas);
	DiceSetValue(6);
	UpdateDescriptionText();
	GameStarted=true;
	FirstMove=true;
}
//---------------------------------------------------------------------------
int __fastcall GetAllMoves()
{
	int Count=0,Increment;
	short i,j,Dice,test,Position;
	bool IsNewValue;
	if (CanGetAdvantage) Screen->Cursor=crHourGlass;
	for (i=0; i<4; i++)
		if (i==PlayerToMove
			|| (CurrentMode==ModeDouble && IsSameTeam(i,PlayerToMove)))
			for (j=0;j<4;j++)
			{
				IsNewValue=true;
				Position=Pegs[i][j]->Position;
				for (test=0; test<Count && IsNewValue; test++)
					if (Moves[test].Position1==Position) IsNewValue=false;
				if (!IsNewValue) continue;
				for (Dice=0; Dice<MovesInHand->Count; Dice++)
				{
					IsNewValue=true;
					for (test=0; test<Dice; test++)
						if (MovesInHand->Values[Dice]
							==MovesInHand->Values[test])
							IsNewValue=false;
					if (!IsNewValue) continue;
					Increment=GetAllMoves(Position,i,
						MovesInHand->Values[Dice],(Moves+Count));
					for (test=(short)Count; test<Count+Increment; test++)
						DiceIndices[test]=Dice;
					Count+=Increment;
				}
			}
	if (Count>sizeof(Moves)/sizeof(AMove))
	{
		Application->MessageBox("Moves out of bounds in GetAllMoves, "
			"unit Interface.","Error",MB_ICONERROR | MB_OK);
		Application->Terminate();
	}
	if (CanGetAdvantage) Screen->Cursor=crDefault;
	return Count;
}
//---------------------------------------------------------------------------
int __fastcall CountMoves()
{
	int Result;
	bool Temp=CanGetAdvantage;
	CanGetAdvantage=false;
	Result=GetAllMoves();
	CanGetAdvantage=Temp;
	return Result;
}
//---------------------------------------------------------------------------
void __fastcall HintMove(bool BasicHint)
{
	AnsiString HintStr;
	if (!GameStarted)
		HintStr="Press F2 to start a new game.";
	else if (GetThrowsLeft())
		HintStr="First you must complete all the throws.\n"
			"(Hit SPACE to roll or stop the die.)";
	else if (GetHilightedPos()!=posOutsideBoard)
	{
		if (Players[PlayerToMove]==ptHuman)
			HintStr="Select a dice value and click on a possible "
				"destination to complete the move.";
		else HintStr="Hit SPACE to complete the computer's move.";
	}
	else if (Players[PlayerToMove]==ptComputer)
	{
		if (CountMoves()==0)
			HintStr="Hit SPACE to make the computer move.";
		else
			HintStr="Hit SPACE to make the computer pass "
				"since no move exists.";
	}
	else if (BasicHint)
	{
		if (CountMoves()>0)
			HintStr="Hilight a position to select counter(s) for moving.\n"
				"(Options->Show Numbers for suggestions on moves.)";
		else HintStr="Hit SPACE to pass since no possible move exists.";
	}
	else
	{
		int Count;
		AMove *BestMove;
		CanGetAdvantage=true;
		Count=GetAllMoves();
		if (Count==0)
			HintStr="Press SPACE to pass since no possible move exists.";
		else
		{
			int Best=0,Choices=1;
			for (short i=1; i<Count; i++)
				if (Moves[i].Advantage > Moves[Best].Advantage)
					Best=i;
				else if (Moves[i].Advantage == Moves[Best].Advantage)
				{
					if (random(++Choices)==0) Best=i;
				}
			BestMove=&Moves[Best];
			HintStr="Try moving from position "+IntToStr(BestMove->Position1)+
				" to position "+IntToStr(BestMove->Position2)+" using dice"
				" value "+IntToStr(MovesInHand->Values[DiceIndices[Best]])+".";
		}
	}
	Application->MessageBox(HintStr.c_str(),"Suggestion",
		MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall MakeMove(short Position,Peg *Peg1,Peg *Peg2=NULL)
{
	short PegCount;
	if (Peg2==NULL) PegCount=1;
	else PegCount=2;
	if (!IsPositionStar(Position) && Position!=76
		&& !GetTeamCount(Position,PlayerToMove)
		&& GetPegsCount(Position)==PegCount)
	{
		if (GameType==1)
		{
			GetPegOnPos(Position,0)->SendToHome();
			if (Peg2!=NULL) GetPegOnPos(Position,0)->SendToHome();
		}
		else
			while (GetPegsCount(Position))
			{
				Peg *Opponent=GetPegOnPos(Position,0);
				short Color=Opponent->Color;
				bool AllRemoved=true;
				Opponent->Position=posOutsideBoard;
				for (short i=0;i<4 && AllRemoved;i++)
				{
					if (Pegs[Color][i]->Position<76)
						AllRemoved=false;
					if (CurrentMode==ModeDouble &&
						Pegs[GetTeamMate(Color)][i]->Position<76)
						AllRemoved=false;
				}
				if (AllRemoved) RegisterWinner(Color);
			}
		if (ExtraThrows & ET_REMOVE) AddAThrow();
		// To update kill records
		HasKilled[Peg1->Color]++;
		if (IsTeamedGame() || CurrentMode==ModeDouble)
			HasKilled[GetTeamMate(Peg1->Color)]++;
	}
	Peg1->Position=Position;
	if (Peg2!=NULL) Peg2->Position=Position;
	if (Position==76 && (ExtraThrows & ET_PROMOTE)) AddAThrow();
	FirstMove=false;
}
//---------------------------------------------------------------------------
bool __fastcall StepHumanMove(short OldPos,short NewPos)
{
	// Returns true if the move is done, false if it is still to be finished.
	CanGetAdvantage=false;
	if (OldPos==posOutsideBoard)
	{
		if (CountMoves()==0)
		{
			if (FirstMove && (ExtraThrows & ET_NOMOVE))
				{MovesInHand->ClearAll(); AddAThrow();}
			return true;
		}
		if (NewPos<76)
		{
			Count1=GetPegsCount(NewPos,PlayerToMove);
			if (CurrentMode==ModeDouble)
				Count2=GetPegsCount(NewPos,GetTeamMate(PlayerToMove));
			else Count2=0;
			if (Count1+Count2>0) HilightPos(NewPos);
		}
		return false;
	}
	else
	{
		bool CanMoveOne,CanMoveTwo;
		Peg *Peg1,*Peg2;
		short TeamMate;
		if (OldPos==NewPos)
		{
			HilightPos(posOutsideBoard);
			return false;
		}
		TeamMate=GetTeamMate(PlayerToMove);
		CanMoveOne=IsMovePossible(OldPos,NewPos,
			MovesInHand->SelectedValue,PlayerToMove,1);
		CanMoveTwo=(Count1+Count2>1) && IsMovePossible(OldPos,NewPos,
				MovesInHand->SelectedValue,PlayerToMove,2);
		if (CurrentMode==ModeDouble)
		{
			CanMoveOne|=IsMovePossible(OldPos,NewPos,
				MovesInHand->SelectedValue,TeamMate,1);
			CanMoveTwo|=(Count1+Count2>1) && IsMovePossible(OldPos,NewPos,
					MovesInHand->SelectedValue,TeamMate,2);
		}
		if (!CanMoveOne && !CanMoveTwo) return false;	// Move not possible
		if (CurrentMode==ModeDouble && OldPos<52 && NewPos>=52)
		{
			// Only one color can go to the a single passing line!
			if (6+PlayerToMove*13>=OldPos && OldPos>PlayerToMove*13) Count2=0;
			else Count1=0;
		}
		if (CurrentMode==ModeDouble && Count1 && Count2
			&& !(Count1==1 && Count2==1 && CanMoveTwo))
		{
			if (CanMoveOne) WhichColors->Total=1;
			else WhichColors->Total=2;
			WhichColors->UpDown1->Max=(short)Count1;
			WhichColors->UpDown2->Max=(short)Count2;
			WhichColors->Image1->Canvas->
				Draw(0,0,Pegs[PlayerToMove][0]->Picture->Bitmap);
			WhichColors->Image2->Canvas->
				Draw(0,0,Pegs[TeamMate][0]->Picture->Bitmap);
			if (WhichColors->ShowModal()==mrCancel)
			{
				HilightPos(posOutsideBoard);
				return false;
			}
			Count1=WhichColors->UpDown1->Position;
			Count2=WhichColors->UpDown2->Position;
		}
		if (Count1) Peg1=GetPegOnPos(OldPos,PlayerToMove,0);
		else if (Count2) Peg1=GetPegOnPos(OldPos,TeamMate,0);
		if (CanMoveTwo)
		{
			if (Count1>=2) Peg2=GetPegOnPos(OldPos,PlayerToMove,1);
			else if (Count2>=2) Peg2=GetPegOnPos(OldPos,TeamMate,1);
			else if (Count1 && Count2) Peg2=GetPegOnPos(OldPos,TeamMate,0);
			MakeMove(NewPos,Peg1,Peg2);
		}
		else MakeMove(NewPos,Peg1);
		HilightPos(posOutsideBoard);
		MovesInHand->Remove(MovesInHand->Selected);
		if (MovesInHand->Count) return false;
		else return true;
	}
}
//---------------------------------------------------------------------------
bool __fastcall StepComputerMove(short OldPos,short NewPos)
{
	static AMove *CompusMove;
	CanGetAdvantage=true;
	if (OldPos==posOutsideBoard)
	{
		int Count;
		short ChoseFrom;
		Count=GetAllMoves();
		if (Count==0)
		{
			if (FirstMove && (ExtraThrows & ET_NOMOVE))
				{MovesInHand->ClearAll(); AddAThrow();}
			return true;
		}
		// Simply sort the array of moves in descending order of Advantage.
		for (short i=0; i<Count; i++)
			for (short j=0; j<i; j++)
				if (Moves[i].Advantage > Moves[j].Advantage)
				{
					AMove tempmove=Moves[i];
					Moves[i]=Moves[j];
					Moves[j]=tempmove;
					short tmpdice=DiceIndices[i];
					DiceIndices[i]=DiceIndices[j];
					DiceIndices[j]=tmpdice;
				}
		for (ChoseFrom=0;
			ChoseFrom<Count
			&& Moves[ChoseFrom].Advantage
			>= Moves[0].Advantage-BlunderRange[PlayerToMove];
			ChoseFrom++);
		ChoseFrom=(short)random(ChoseFrom);
		CompusMove=&Moves[ChoseFrom];
		HilightPos(CompusMove->Position1);
		MovesInHand->Selected=DiceIndices[ChoseFrom];
		return false;
	}
	else
	{
		Peg *Peg1,*Peg2;
		short TeamMate=GetTeamMate(PlayerToMove);
		if (CompusMove->Colors==ThisColor
			|| CompusMove->Colors==MixedColor)
			Peg1=GetPegOnPos(OldPos,PlayerToMove,0);
		else
			Peg1=GetPegOnPos(OldPos,TeamMate,0);
		if (CompusMove->PegCount==1)
			MakeMove(CompusMove->Position2,Peg1);
		else
		{
			if (CompusMove->Colors==ThisColor)
				Peg2=GetPegOnPos(OldPos,PlayerToMove,1);
			else if (CompusMove->Colors==OtherColor)
				Peg2=GetPegOnPos(OldPos,TeamMate,1);
			else
				Peg2=GetPegOnPos(OldPos,TeamMate,0);
			MakeMove(CompusMove->Position2,Peg1,Peg2);
		}
		MovesInHand->Remove(MovesInHand->Selected);
		HilightPos(posOutsideBoard);
		if (MovesInHand->Count) return false;
		else return true;
	}
}
//---------------------------------------------------------------------------
void __fastcall MouseUpHandler(int X,int Y)
{
	bool PlayerDone,HasWon;
	short i;
	if (!GameStarted) return;
	if (GetThrowsLeft())
	{
		if (Players[PlayerToMove] == ptHuman)
			DiceRandomRoll(AutoThrowDice[PlayerToMove] ?
				LudoForm->Timer->Interval : TimeOutHuman);
		else DiceRandomRoll(AutoThrowDice[PlayerToMove] ?
				LudoForm->Timer->Interval : TimeOutComputer);
		return;
	}
	if (MovesInHand->Count)
	{
		if (Players[PlayerToMove] == ptHuman)
		{
			short Position;
			if (AutoHuman && CountMoves()==1)
			{
				if (GetHilightedPos()==posOutsideBoard)
				{
					Position=Moves[0].Position1;
					MovesInHand->Selected=DiceIndices[0];
				}
				else Position=Moves[0].Position2;
			}
			else Position=GetPosFromCoords(X,Y);
			PlayerDone=StepHumanMove(GetHilightedPos(),Position);
		}
		else
			PlayerDone=StepComputerMove(GetHilightedPos(),
				GetPosFromCoords(X,Y));
	}
	else PlayerDone=true;
	// Playerdone now means that the player has completed the move
	HasWon=true;
	if (CurrentMode==ModeSingle)
	{
		for (i=0;i<4 && HasWon;i++)
			if (Pegs[PlayerToMove][i]->Position<76) HasWon=false;
	}
	else
	{
		for (i=0;i<4 && HasWon;i++)
			if (Pegs[PlayerToMove][i]->Position<76
				|| Pegs[GetTeamMate(PlayerToMove)][i]->Position<76)
				HasWon=false;
	}
	if (HasWon)
	{
		RegisterWinner(PlayerToMove);
		if (!GameStarted) return;
		PlayerDone=true;
	}
	if ((PlayerDone && !GetThrowsLeft()) || Players[PlayerToMove]==ptNone)
	{
		MovesInHand->ClearAll();
		if (CurrentMode==ModeDouble)
		{
			if (Players[GetOpponent(PlayerToMove)]!=ptNone)
			{
				PlayerToMove=GetOpponent(PlayerToMove);
				if (GetTeamMate(PlayerToMove)<PlayerToMove)
					PlayerToMove=GetTeamMate(PlayerToMove);
			}
		}
		else
		{
			for (i=0; i<4; i++)
				if (Players[(PlayerToMove+i+1) % 4]!=ptNone) break;
			if (i<4) PlayerToMove+=short(i+1); PlayerToMove%=(short)4;
		}
		DrawPlayer(LudoForm->CurPlayerImage->Canvas);
		UpdateDescriptionText();
		FirstMove=true;
	}
}
//---------------------------------------------------------------------------
void __fastcall ManageClick(int X,int Y)
{
	bool Enable=false;
	if (!GameStarted) {LudoForm->EnableTimer(false); return;}
	LudoForm->Timer->Enabled=false;
	MouseUpHandler(X,Y);
	if (GetThrowsLeft())	// Next one is a dice throw
	{
		if (AutoThrowDice[PlayerToMove]) Enable=true;
	}
	else					// Next is a move
	{
		if (AutoComputer && Players[PlayerToMove]==ptComputer)
			Enable=true;
		else if (Players[PlayerToMove]==ptHuman && AutoThrowDice[PlayerToMove])
		{
			int Count=CountMoves();
			Enable=(Count==0 || (AutoHuman && Count==1));
		}
	}
	LudoForm->EnableTimer(Enable);
}
//---------------------------------------------------------------------------

