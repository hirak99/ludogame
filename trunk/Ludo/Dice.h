//---------------------------------------------------------------------------
#ifndef DiceH
#define DiceH
//---------------------------------------------------------------------------
#define ET_SIX 1		// Define bits for ExtraThrows
#define ET_REMOVE 2
#define ET_PROMOTE 4
#define ET_NOMOVE 8
//---------------------------------------------------------------------------
#define MAXMOVESLISTCOUNT 18
class MovesList
{
	private:
		TCanvas *Canvas;
		short FCount,FSelected,CountSixes;
		unsigned char Numbers[MAXMOVESLISTCOUNT];	// Can display only 18
		void __fastcall Redraw();
		void __fastcall SetSelected(short NewValue);
		unsigned char __fastcall GetValues(int Index) {return Numbers[Index];}
		unsigned char __fastcall GetSelectedValue() {return Numbers[FSelected];}
	public:
		__fastcall MovesList(TCanvas *ToSet);
		void __fastcall Add(unsigned char NewValue);
		void __fastcall Remove(short Index);
		void __fastcall ClearAll();
		__property short Count = {read=FCount};
		__property short Selected = {read=FSelected, write=SetSelected};
		__property unsigned char Values[int Index] = {read=GetValues};
		__property unsigned char SelectedValue = {read=GetSelectedValue};
};
//---------------------------------------------------------------------------
void __fastcall EnableDice(bool Enable);
bool __fastcall IsDiceEnabled();
void __fastcall AddAThrow();
void __fastcall NoMoreThrows();
short __fastcall GetThrowsLeft();
void __fastcall DiceSetImage(TImage *ToSet);
void __fastcall DiceSetValue(unsigned char Value);
unsigned char __fastcall DiceRandomRoll(int Wait);
//---------------------------------------------------------------------------
#endif

