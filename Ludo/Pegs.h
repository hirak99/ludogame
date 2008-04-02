//---------------------------------------------------------------------------
#ifndef PegsH
#define PegsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
// This file doesn't only handle pegs, it handles other things also.
const int ColorRed=0,ColorGreen=1,ColorYellow=2,ColorBlue=3;
extern bool UpdatePegImages;
//---------------------------------------------------------------------------
void __fastcall GetRectFromPos(RECT *Rect, short Position);
short __fastcall GetPosFromCoords(int x,int y);
void __fastcall HilightPos(short Position);
short __fastcall GetHilightedPos();
bool __fastcall IsPositionStar(short Position);
short int __fastcall DistFromStart(short Position,short Color);
//---------------------------------------------------------------------------
#define posOutsideBoard 255
class Peg : public TImage
{
	private:
		short FColor;
		short FIndex;
		short FPosition;
		void __fastcall SetPosition(short Value);
		bool __fastcall GetAtHome() {return FPosition==short(FColor+72);}
		bool __fastcall GetAtStar() {return IsPositionStar(FPosition);}
		short int __fastcall GetPathCovered()
			{return DistFromStart(FPosition,FColor);}
		void __fastcall PegMouseUp(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y);
	public:
		__fastcall Peg(Classes::TComponent *AOwner,
			short ThisColor,short ThisIndex);
		__property short int Color = {read=FColor};
		__property short int Index = {read=FIndex};
		__property short int Position = {read=FPosition, write=SetPosition};
		__property short int PathCovered = {read=GetPathCovered};
		__property bool AtHome = {read=GetAtHome};
		__property bool AtStar = {read=GetAtStar};
		void SendToHome();
};
//---------------------------------------------------------------------------
void __fastcall LoadPegs(Classes::TComponent *Owner, TWinControl *Parent);
void __fastcall SetBoardForPegs(TImage *Board);
short __fastcall GetPegsCount(short Position,short Color);
short __fastcall GetPegsCount(short Position);
Peg* __fastcall GetPegOnPos(short Position,short Color,short Index);
Peg* __fastcall GetPegOnPos(short Position,short Index);
short __fastcall CountPegs(short Color);
short __fastcall CountPegsOutside(short Color);
short __fastcall CountPassed(short Color);
//---------------------------------------------------------------------------
#endif

