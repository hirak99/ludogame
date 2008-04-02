//---------------------------------------------------------------------------
#ifndef MovesH
#define MovesH

extern bool CanGetAdvantage;		// To control if advantage can be computed
enum ColorFlag {ThisColor,OtherColor,MixedColor};
struct AMove
{
	short Position1;
	short Position2;
	short PegCount;
	enum ColorFlag Colors;
	int Advantage;
};
enum Modes {ModeSingle,ModeDouble};
enum PlayerType {ptNone,ptHuman,ptComputer};
//---- Team Functions -------------------------------------------------------
short __fastcall GetTeamCount(short Position, short Color);
short __fastcall GetDoubleCount(short Position, short Color);
short __fastcall CountDoublesPegs(short Color);
short __fastcall TeamPegsOutside(short Color);
short __fastcall DoublesPegsOutside(short Color); // Returns pegs movable by
		//  current color which are outside
short __fastcall TeamPassed(short Color);
void __fastcall SetNoTeam();
void __fastcall SetTeam(short Color1,short Color2);
bool __fastcall IsTeamedGame();
bool __fastcall IsSameTeam(short Color1,short Color2);
short __fastcall GetTeamMate(short Color);
short __fastcall GetOpponent(short Color);
//---- Other Functions ------------------------------------------------------
// bool __fastcall IsDoubleBetween(short Pos1,short Pos2,short Color);
/* void __fastcall PutInMove(AMove Moves[],
	short Pos1,
	short Pos2,
	short &Count,
	short PegCount,
	short Color); */
short __fastcall GetAllMoves(
	short Pos,
	short Color,
	short DiceVal,
	AMove Moves[]);
bool __fastcall IsMovePossible(
	short Pos1,
	short Pos2,
	short DiceVal,
	short Color,
	short PegCount);
int __fastcall GetAdvantage(AMove *Move,short Color,bool EvenIfAlreadyIn=false);
#endif
