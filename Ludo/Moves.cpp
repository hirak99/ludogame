//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Moves.h"
#include "Pegs.h"
#include "Dice.h"
extern Peg *Pegs[4][4];			// The Pegs
extern enum Modes CurrentMode;	// Current mode, enum defined in Pegs.h
extern MovesList *MovesInHand;	// Moves buffer
short RedsTeamMate;				// This variable keeps track of all team info.
short AdvDiceValue;				// Required in GetAdvantage
bool CanGetAdvantage=true;		// To control if advantage can be computed
unsigned short DeepLevel=0;		// Recurssion level for GetAdvantage(...)
extern short TotalPegs;			// Defined in Interface.cpp
extern short ProbOfSix;			// Defined in Dice.cpp
bool JumpDouble;				// Can single jump (opponent's) double?
short Adv6Factor;				// Used in GetAdvantage, GetPosAdvantage
	// This is the %factor by which the thinking procedure will divide its
	// probability of catching up with a peg more than 6 units away.
extern char GameType;		// Defined in Interface.cpp
extern short Winners;		// Defined in Interface.cpp
extern short MaxWinners;	// Defined in Interface.cpp
extern char ExtraThrows;	// Defined in Dice.cpp
bool KillForHome;			// Bombay style (by Milind Gavakar)
short HasKilled[4];			// Keeps track of number of kills
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// Team Functions
short __fastcall GetTeamCount(short Position, short Color)
{
	short Count=GetPegsCount(Position,(short)Color);
	if (IsTeamedGame())
		Count+=GetPegsCount(Position,GetTeamMate((short)Color));
	return Count;
}
short __fastcall GetDoubleCount(short Position, short Color)
{
	short Count=GetPegsCount(Position,(short)Color);
	if (CurrentMode==ModeDouble)
		Count+=GetPegsCount(Position,GetTeamMate((short)Color));
	return Count;
}
short __fastcall CountDoublesPegs(short Color)
{
	short Count=CountPegs(Color);
	if (CurrentMode==ModeDouble) Count+=CountPegs(GetTeamMate(Color));
	return Count;
}
short __fastcall TeamPegsOutside(short Color)
{
	short Count=CountPegsOutside(Color);
	if (IsTeamedGame()) Count+=CountPegsOutside(GetTeamMate(Color));
	return Count;
}
short __fastcall DoublesPegsOutside(short Color)
{
	short Count=CountPegsOutside(Color);
	if (CurrentMode==ModeDouble) Count+=CountPegsOutside(GetTeamMate(Color));
	return Count;
}
short __fastcall TeamPassed(short Color)
{
	short Result=CountPassed(Color);
	if (IsTeamedGame()) Result+=CountPassed(GetTeamMate(Color));
	return Result;
}
void __fastcall SetNoTeam() {RedsTeamMate=ColorRed;}
void __fastcall SetTeam(short Color1,short Color2)
{
	if (Color1==ColorRed) RedsTeamMate=Color2;
	else if (Color2==ColorRed) RedsTeamMate=Color1;
	else RedsTeamMate=short(6-Color1-Color2);	// This line works!
}
bool __fastcall IsTeamedGame() {return RedsTeamMate!=ColorRed;}
bool __fastcall IsSameTeam(short Color1,short Color2)
{
	if (Color1==Color2) return true;
	if (Color1==ColorRed) return RedsTeamMate==Color2;
	else if (Color2==ColorRed) return RedsTeamMate==Color1;
	else return RedsTeamMate==6-Color1-Color2;	// This line works!
}
short __fastcall GetTeamMate(short Color)
{
	if (!IsTeamedGame()) return Color;
	if (Color==ColorRed || Color==RedsTeamMate)
		return short(RedsTeamMate-Color);
	else return short(6-RedsTeamMate-Color);
}
short __fastcall GetOpponent(short Color)
{
	if (IsTeamedGame())
	{
		if (Color!=ColorRed && Color!=RedsTeamMate) return ColorRed;
		else return short((RedsTeamMate % 3)+1);
	}
	else return short(3-Color);
}
//---------------------------------------------------------------------------
//---- Other Functions ------------------------------------------------------
bool __fastcall IsDoubleBetween(short Pos1,short Pos2,short Color)
{
	bool Result=false;
	short Pos;
	if (!JumpDouble)
	{
		Pos1=short((Pos1+52)%52);
		Pos2=short((Pos2+52)%52);
		if ((Pos2-Pos1+52)%52==51) return Result;
		if (Pos2<Pos1) Pos2+=short(52);
		for (short i=(short)Pos1; i<=Pos2 && !Result; i++)
		{
			Pos=short((i+52)%52);
			if (IsPositionStar(Pos)) continue;
			for (short j=0; j<4 && !Result; j++)
			{
				if (IsSameTeam(j,Color)) continue;
				if (GetTeamCount(Pos,j)>1) Result=true;;
			}
		}
	}
	return Result;
}
//---------------------------------------------------------------------------
inline void __fastcall SetMove(AMove *Move,short Pos1,short Pos2,short PegCount)
{
	Move->Position1=Pos1;
	Move->Position2=Pos2;
	Move->PegCount=PegCount;
}
void __fastcall PutInMove(AMove Moves[],
	short Pos1,
	short Pos2,
	short &Count,
	short PegCount,
	short Color)
{
	if (CurrentMode==ModeDouble)
	{
		int Count1,Count2;
		bool CanMoveThis,CanMoveOther;
		if (GetTeamMate(Color)<Color) Color=GetTeamMate(Color);
		Count1=GetPegsCount(Pos1,Color);
		Count2=GetPegsCount(Pos1,GetTeamMate(Color));
		CanMoveThis=Pos2<52 || Pos2==76 || (Pos2-52)/5==Color;
		CanMoveOther=Pos2<52 || Pos2==76 || (Pos2-52)/5==GetTeamMate(Color);
		if (PegCount==1)
		{
			if (Count1>=1 && CanMoveThis)
			{
				SetMove(&Moves[Count],Pos1,Pos2,PegCount);
				Moves[Count].Colors=ThisColor;
				Moves[Count].Advantage=GetAdvantage(&Moves[Count],Color);
				Count++;
			}
			if (Count2>=1 && CanMoveOther)
			{
				SetMove(&Moves[Count],Pos1,Pos2,PegCount);
				Moves[Count].Colors=OtherColor;
				Moves[Count].Advantage=GetAdvantage(&Moves[Count],Color);
				Count++;
			}
		}
		else if (PegCount==2)
		{
			if (Count1>=2 && CanMoveThis)
			{
				SetMove(&Moves[Count],Pos1,Pos2,PegCount);
				Moves[Count].Colors=ThisColor;
				Moves[Count].Advantage=GetAdvantage(&Moves[Count],Color);
				Count++;
			}
			if (Count2>=2 && CanMoveOther)
			{
				SetMove(&Moves[Count],Pos1,Pos2,PegCount);
				Moves[Count].Colors=OtherColor;
				Moves[Count].Advantage=GetAdvantage(&Moves[Count],Color);
				Count++;
			}
			if (Count1 && Count2 && Pos2<52)
			{
				SetMove(&Moves[Count],Pos1,Pos2,PegCount);
				Moves[Count].Colors=MixedColor;
				Moves[Count].Advantage=GetAdvantage(&Moves[Count],Color);
				Count++;
			}
		}
	}
	else
	{
		SetMove(&Moves[Count],Pos1,Pos2,PegCount);
		Moves[Count].Colors=ThisColor;
		Moves[Count].Advantage=GetAdvantage(&Moves[Count],Color);
		Count++;
	}
}
//---------------------------------------------------------------------------
short __fastcall GetAllMoves(
	short Pos,
	short Color,
	short DiceVal,
	AMove Moves[])
{
	short Count=0,NewPos,Max;
	if (DiceVal & 1) Max=1;
	else Max=2;
	if (DeepLevel==0) AdvDiceValue=DiceVal;	// Required for getting advantage
	for (short PegCount=1; PegCount<=Max; PegCount++)
	{
		if (CurrentMode==ModeDouble)
		{
			if (GetTeamCount(Pos,Color) < PegCount) break;
		}
		else
		{
			if (GetPegsCount(Pos,Color) < PegCount) break;
		}
		if (PegCount==2) DiceVal/=(short)2;
		if (Pos<52)
		{
			NewPos=short((Pos+DiceVal) % 52);
			if (PegCount!=1 ||
				!IsDoubleBetween(short(Pos+1),short(NewPos-1),Color))
				PutInMove(Moves,Pos,NewPos,Count,PegCount,Color);
			if (Pos<=6+Color*13 && 6+Color*13<NewPos)
			{
				NewPos-=short(6+Color*13);
				if (PegCount!=1 ||
					!IsDoubleBetween(short(Pos+1),short(6+Color*13),Color))
				{
					if (!KillForHome || DoublesPegsOutside(Color)!=1 || HasKilled[Color])
					{
						NewPos+=short(51+Color*5);
						if (NewPos==57+Color*5) NewPos=76;
						PutInMove(Moves,Pos,NewPos,Count,PegCount,Color);
					}
				}
			}
		}
		else if (Pos<72)
		{
			bool Result=true;
			NewPos=short(Pos+DiceVal);
			if (NewPos==57+Color*5) NewPos=76;
			else if (NewPos>57+Color*5) Result=false;
			if (CurrentMode==ModeDouble && PegCount==2
				&& GetPegsCount(Pos,Color)<2
				&& GetPegsCount(Pos,GetTeamMate(Color))<2)
				Result=false;
			if (Result) PutInMove(Moves,Pos,NewPos,Count,PegCount,Color);
		}
		if (PegCount==2) DiceVal*=(short)2;
	}
	if (71<Pos && Pos<76 && DiceVal==6)
	{
		NewPos=short(8+13*(Pos-72));
		PutInMove(Moves,Pos,NewPos,Count,1,short(Pos-72));
	}
	return Count;
}
//---------------------------------------------------------------------------
bool __fastcall IsMovePossible(
	short Pos1,
	short Pos2,
	short DiceVal,
	short Color,
	short PegCount)
{
	bool Result=false,AdvEnabled=CanGetAdvantage;
	short Count;
	AMove Moves[8];
	CanGetAdvantage=false;
	Count=GetAllMoves(Pos1,Color,DiceVal,Moves);
	CanGetAdvantage=AdvEnabled;
	if (Count>8)
	{
		Application->MessageBox ("Out of bounds in IsMovePossible, unit Moves.",
			"Error",MB_ICONERROR | MB_OK);
		Application->Terminate();
	}
	for (short i=0; i<Count && !Result; i++)
		if (Moves[i].Position2==Pos2 && Moves[i].PegCount==PegCount)
			Result=true;
	return Result;
}
//---------------------------------------------------------------------------
int __fastcall GetPosAdvantage(short Position,short Color,
	short PegsToMove,bool WillMove)
{
	// WillMove means will move TO this position,
	// !WillMove means FROM this position
	const static int Range=25;
	int Advantage,Increment;
	short i,NewPos,MyPegs/* My peg count */,PegCount/* Opponents PegCount */;
	if (Position>=52) return 0;
	Advantage=0;
	for (i=-Range; i<=Range; i++)
	{
		NewPos=short((Position + 52 + i) % 52);
		PegCount=short(GetPegsCount(NewPos)-GetTeamCount(NewPos,Color));
		if (i<0 && !WillMove && NewPos%13==8
			&& !IsSameTeam(short(NewPos/13),Color)
			&& GetPegsCount(short(NewPos/13 + 72))>0)
		{
			Increment=3+i/2;
			if (Increment>0) Advantage-=Increment;
		}
		if (PegCount)
		{
			//MyPegs=short((WillMove ? PegsToMove:0)+GetTeamCount(NewPos,Color));
			Increment=0;
			if (i<0)
			{
				int Mult;
				// DivFactor is -ve!
				int DivFactor=-1;
				for (int j=0; j>(i+1)/6; j--) DivFactor*=Adv6Factor;
				for (int j=0; j>(i+1)/6; j--)
					DivFactor=(DivFactor+25)/100-1;
				if (IsPositionStar(Position)) Mult=PegCount;  // Opponent's Pegs
				else Mult=GetPegsCount(Position,Color)+		  // My pegs
					(WillMove ? PegsToMove : 0);
				Increment=15 * Mult *
					(DistFromStart(Position,Color)+15
					+ Mult*(Range+i))/DivFactor
					+ TeamPassed(Color)*30/
						(CurrentMode==ModeDouble ? 2 : 1)/DivFactor;
				if (IsPositionStar(Position))
				{
					Increment=-Increment*2/5;
					for (short j=0; GetPegOnPos(NewPos,j)!=NULL; j++)
					{
						short PegColor=GetPegOnPos(NewPos,j)->Color;
						if (IsSameTeam(PegColor,Color)) continue;
						Increment+=TeamPassed(PegColor)*30/
							(CurrentMode==ModeDouble ? 2 : 1)/DivFactor;
					}
				}
				else if (Mult>=2)
				{
					bool Chased;
					if (CurrentMode==ModeDouble)
						Chased=(GetTeamCount(NewPos,GetOpponent(Color))>=2);
					else
					{
						Chased=false;
						for (short i=0; i<4; i++)
						{
							if (IsSameTeam(Color,i)) continue;
							if (GetPegsCount(NewPos,i)>=2) Chased=true;
						}
					}
					if (!Chased)
					{
						// Remember that Increment is negative
						// Mult now means how many of my peg in Position
						// Opponent has 1, I have >=2
						if (PegsToMove==1)
						{
							// Being chased, and forming doubles
							if (WillMove)
							{
								if (DoublesPegsOutside(Color)>2)
									Increment=-Increment*2/5;
							}
							// It's better now than it would be if separated
							else if (Mult==2) Increment=-Increment*3;
						}
						else if (!WillMove && !JumpDouble)	// Trap pegs behind
							Increment-=Increment*(DoublesPegsOutside(Color)-2)
								*3/5;
					}
					// Better fly with 2 pegs than 1 peg if being chased
					else if (PegsToMove==2 && WillMove) Increment/=2;
				}
				if (PegCount==1 && i>-3)
				{
					short Opponent=GetPegOnPos(NewPos,0)->Color;
					if (DoublesPegsOutside(Opponent)==1)
						Increment+=100+DistFromStart(NewPos,Opponent)/GameType;
				}
			}
			// Also to be added 4000 for i==0 in GetAdvantage()
			else
			{
				for (short iPeg=0; GetPegOnPos(NewPos,iPeg)!=NULL; iPeg++)
				{
					short PegColor=GetPegOnPos(NewPos,iPeg)->Color;
					if (IsSameTeam(PegColor,Color)) continue;
					int DistPriority=(DistFromStart(NewPos,PegColor));
					if (i>0 && DistPriority==50) DistPriority=32;//Almost passed
					if (MaxWinners-Winners==2 &&
						TeamPassed(PegColor)>TeamPassed(Color))
						DistPriority=DistPriority*
							(TeamPassed(PegColor)+5)/(TeamPassed(Color)+5);
					int DivFactor=1;
					// (i+3) : In next move the peg can advance by 3 in avg.
					for (int j=0; j<(i+3)/6; j++) DivFactor*=Adv6Factor;
					for (int j=0; j<(i+3)/6; j++)
						DivFactor=(DivFactor+25)/100+1;
					Increment+=15 *
						(DistPriority/GameType+15
						+ PegCount*(Range-i))/DivFactor
						+ TeamPassed(PegColor)*30/
							(CurrentMode==ModeDouble ? 2 : 1)/DivFactor;
				}
				if (PegCount==2 && PegsToMove==2) Increment*=2;
				else if (WillMove && PegsToMove>=2 && PegCount==1)
					Increment/=2; // Can't move 2 pegs for 1
				if ((IsPositionStar(NewPos) && !(PegCount==1
					&& DoublesPegsOutside(GetPegOnPos(NewPos,0)->Color)==1))
					|| (i==0 && !WillMove))
					Increment/=3*PegCount;
				// if (MyPegs>=2) Increment/=2; // ???
				if (i>0)
				{
					int CurDist=DistFromStart(Position,Color);
					int NewDist=DistFromStart(NewPos,Color);
					if (NewDist==51 || NewDist<CurDist) Increment/=3;
				}
			}
			Advantage+=Increment;
		}
	}
	// Need to be extra causious for GameMode 2
	if (Advantage<0 && GameType==2 && MaxWinners-Winners>2)
		Advantage=Advantage*6/5;
	// The 'Range' was originally 12
	Advantage=Advantage*12/Range;
	MyPegs=short(GetTeamCount(Position,Color)+(WillMove ? PegsToMove : 0));
	if (IsPositionStar(Position))
	{
		Advantage += (DistFromStart(Position,Color)*52/(Range+40)+102-Range);
		Increment =  GetTeamCount(Position,Color);		// My pegs
		Increment -= GetPegsCount(Position)-Increment;	// Opponent's
		Advantage -= Increment*4/PegsToMove;
	}
	else
	{
		// Breaking double
		if (!WillMove && GetTeamCount(Position,Color)-PegsToMove==1)
			Advantage-=25;
		// Making double
		if (GetTeamCount(Position,Color)==1 && MyPegs>1) Advantage+=25;
	}
	return Advantage;
}
//---------------------------------------------------------------------------
int __fastcall RealGetAdvantage(AMove *Move,short Color)
{
	int Advantage;
	if (Move->Position1>=52 && Move->Position1<72)	// In passing line
	{
		if (Move->Position2==76) return Move->PegCount;
		else return -Move->PegCount;
	}
	Advantage=0;
	if (Move->Colors==OtherColor) Color=GetTeamMate(Color);
	for (short i=0; i<Move->PegCount; i++)
	{
		Advantage+=(GetPosAdvantage(Move->Position2,Color,Move->PegCount,true)
			- GetPosAdvantage(Move->Position1,Color,Move->PegCount,false));
		if (Move->Position2 >= 52)
		{
			Advantage+=1000+TeamPassed(Color)*20
				/(CurrentMode==ModeDouble ? 2 : 1);	// Going to passing line
			if (TeamPassed(Color)==(IsTeamedGame()?TotalPegs*2:TotalPegs)-1)
			{
				Advantage+=10000;
				if (Move->Position2==76) Advantage+=5000;
			}
		}
		else if (Move->Position1 <= 6+Color*13 &&
			Move->Position2 > 6+Color*13)			// About to tour again
			Advantage-=4100-TeamPassed(Color)*20
				/(CurrentMode==ModeDouble ? 2 : 1);
		if (!IsPositionStar(Move->Position2)
			&& GetPegsCount(Move->Position2)==Move->PegCount
			&& !GetTeamCount(Move->Position2,Color))
		{
			Advantage+=3500;						// Remove Opponent
			if (GameType==2) Advantage+=500;
			if (~ExtraThrows & ET_REMOVE) Advantage-=500;
			if (KillForHome && HasKilled[Color]==0) Advantage+=100;	// First blood!
		}
		if (Move->Position1>=72)	// Move to startpos
		{
			Advantage+=3510*50/(ProbOfSix+30);
			if (GameType==2) Advantage-=1000;	// Elimination Game
		}
		if (Move->Colors==MixedColor) Color=GetTeamMate(Color);
		else {Advantage*=Move->PegCount; i=0xFF;}
	}
	return Advantage;
}
//---------------------------------------------------------------------------
int __fastcall GetAdvantage(AMove *Move,short Color,bool EvenIfAlreadyIn)
{
	static bool *DiceDone;
	static short TeamMate;
	Peg *MyPeg1,*MyPeg2;
	int Advantage;
	int Increment=-0xFFFFFF;
	AMove NewMoves[8];
	if (!CanGetAdvantage || (DeepLevel && !EvenIfAlreadyIn)) return 0;
	if (DeepLevel++==0)
	{
		short CurIndex;
		DiceDone=new bool[MovesInHand->Count];
		for (short i=0; i<MovesInHand->Count; i++)
		{
			DiceDone[i]=false;
			if (AdvDiceValue==MovesInHand->Values[i]) CurIndex=i;
		}
		DiceDone[CurIndex]=true;
		UpdatePegImages=false;
		TeamMate=GetTeamMate(Color);
		// Adv6Factor should be (120/ProbOfSix*(4.5/6)-1)*100
		if (ProbOfSix>0) Adv6Factor=short(9000/ProbOfSix-100);
		else Adv6Factor=10000;
	}
	switch (Move->Colors)
	{
		case ThisColor:
			MyPeg1=GetPegOnPos(Move->Position1,Color,0);
			MyPeg2=GetPegOnPos(Move->Position1,Color,1);
			break;
		case OtherColor:
			MyPeg1=GetPegOnPos(Move->Position1,TeamMate,0);
			MyPeg2=GetPegOnPos(Move->Position1,TeamMate,1);
			break;
		case MixedColor:
			MyPeg1=GetPegOnPos(Move->Position1,Color,0);
			MyPeg2=GetPegOnPos(Move->Position1,TeamMate,0);
			break;
	}
	Advantage=RealGetAdvantage(Move,Color);
	for (short i=0; i<MovesInHand->Count; i++)
		if (DiceDone[i]==false)
		{
			short Count;
			for (short j=0; j<MovesInHand->Count; j++)
				if (MovesInHand->Values[j]==MovesInHand->Values[i])
					DiceDone[j]=true;
			DiceDone[i]=true;
			MyPeg1->Position=Move->Position2;
			if (Move->PegCount==2) MyPeg2->Position=Move->Position2;
			for (short iPegs=0; iPegs<8; iPegs++)
			{
				if (iPegs>=4)
				{
					if (CurrentMode==ModeSingle) break;
					Count=GetAllMoves(Pegs[TeamMate][iPegs-4]->Position,Color,
						MovesInHand->Values[i],NewMoves);
				}
				else
					Count=GetAllMoves(Pegs[Color][iPegs]->Position,Color,
						MovesInHand->Values[i],NewMoves);
				if (Count>sizeof(NewMoves))
				{
					Application->MessageBox("NewMoves is out of bounds in "
						"GetAdvantage, unit Moves.",
						"Error",MB_ICONERROR | MB_OK);
				}
				for (short j=0; j<Count; j++)
					if (NewMoves[j].PegCount==Move->PegCount)
					{
						int CurAdvantage=GetAdvantage(&NewMoves[j],Color,true);
						CurAdvantage-=DeepLevel;	// "As fast as possible"
						if (CurAdvantage>Increment) Increment=CurAdvantage;
					}
			}
			for (short j=0; j<MovesInHand->Count; j++)
				if (MovesInHand->Values[j]==MovesInHand->Values[i])
					DiceDone[j]=false;
			DiceDone[i]=false;
			MyPeg1->Position=Move->Position1;
			if (Move->PegCount==2) MyPeg2->Position=Move->Position1;
		}
	if (Increment!=-0xFFFFFF) Advantage+=Increment;
	if (--DeepLevel==0)
	{
		delete[] DiceDone;
		UpdatePegImages=true;
	}
	return Advantage;
}
//---------------------------------------------------------------------------

