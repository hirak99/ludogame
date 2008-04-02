//---------------------------------------------------------------------------
#ifndef MovesRecH
#define MovesRecH
//---------------------------------------------------------------------------
#include "Dice.h"
//---------------------------------------------------------------------------
struct MovesLink {
	unsigned char PegPos[4][4];
	char PlayerToMove;
	unsigned char WinnersList;
	MovesLink *Prev,*Next;
};
//---------------------------------------------------------------------------
class MovesRec {
	private:
		MovesLink *FirstLink,*CurLink;
	public:
		__fastcall MovesRec();
		void __fastcall SnapShot();
		void __fastcall DeleteCurrent();
		void __fastcall Clear();
		void __fastcall ClearNext();
		void __fastcall TakeBack();
		void __fastcall Forward();
		__fastcall ~MovesRec();
};
//---------------------------------------------------------------------------
#endif
