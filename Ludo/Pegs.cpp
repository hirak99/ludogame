//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Pegs.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
extern AnsiString AppDir;
short Hilighted=posOutsideBoard;		// Position which is hilighted
Peg *Pegs[4][4];		// First [index] color
TImage *Board;			// Original board
TShape *HilightShape;	// Used for hilighlighting
bool UpdatePegImages=true;
char IgnoreStars;		// 0 for Ignore none,
						// 1 for Ignore whites,
						// 2 for Ignore all

extern AnsiString DefaultBmps;
/*** Functions Defined Here ***----------------------------------------------
void __fastcall RedrawPos(short Position)
------------------------------------------------------------------------ ***/

//---------------------------------------------------------------------------
void __fastcall RedrawPos(short Position)
{
	short i,j,Count;
	RECT Rect;
	if (!UpdatePegImages || Position>71) return;
	Count=0;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			if (Pegs[i][j]->Position==Position) Count++;
	if (Count)
	{
		short NewCount=0;
		int x,y,*Crit,Actual;
		GetRectFromPos(&Rect,Position);
		x=Rect.left; y=Rect.top;
		if (Rect.right-Rect.left > Rect.bottom-Rect.top) {Crit=&x; Actual=x;}
		else {Crit=&y; Actual=y;}
		for (i=0; i<4 && Count!=NewCount; i++)
			for (j=0; j<4 && Count!=NewCount; j++)
			if (Pegs[i][j]->Position==Position)
			{
				// Divide *Crit between Actual and Actual+36-22
				//  NewCount=0       *Crit=Actual
				//     ...				...
				//  NewCount=Count-1 *Crit=Actual+14
				if (Count==1) *Crit=Actual+7;
				else *Crit=Actual+NewCount*13/(Count-1);
				Pegs[i][j]->Left=x;
				Pegs[i][j]->Top=y;
				NewCount++;
			}
	}
}
//---------------------------------------------------------------------------
void __fastcall Peg::SetPosition(short Value)
{
	short OldPos=FPosition;
	if (OldPos==Value) return;	// It was already here!
	if (Value==FColor+72) SendToHome();
	else if (Value>=72 && Value<=75) return;
	FPosition=Value;
	if (Value==posOutsideBoard) Visible=false;
	else Visible=true;
	RedrawPos(OldPos);
	if (UpdatePegImages && Value==76)
	{
		RECT Rect;
		int x,y,tmp;
		GetRectFromPos(&Rect,76);
		OffsetRect(&Rect,-Board->Left,-Board->Top);
		x=Rect.left+5;
		y=(Rect.top+Rect.bottom-10*(2*FIndex-3)-Width)/2;
		for (int i=0; i<FColor; i++)
			{tmp=x; x=408-y; y=tmp;}
		tmp=FColor;
		if (tmp>1) tmp=5-tmp;
		x-=(Width-1)*(tmp & 1);
		y-=(Width-1)*(tmp >> 1);
		Left=x+Board->Left; Top=y+Board->Top;
	}
	else RedrawPos(Value);
}
//---------------------------------------------------------------------------
void __fastcall Peg::PegMouseUp(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ManageClick(X+Left,Y+Top);
}
//---------------------------------------------------------------------------
__fastcall Peg::Peg(Classes::TComponent *AOwner,short ThisColor,short ThisIndex)
	: TImage(AOwner)
{
	FColor=ThisColor;
	FIndex=ThisIndex;
	FPosition=posOutsideBoard;		// Outside board
	OnMouseUp=PegMouseUp;
}
//---------------------------------------------------------------------------
void Peg::SendToHome()
{
	short OldPos=FPosition,DrawValue;
	RECT Rect;
	if (AtHome) return;
	FPosition=short(FColor+72);		// At home
	if (UpdatePegImages)
	{
		RedrawPos(OldPos);
		Visible=false;
		GetRectFromPos(&Rect,FPosition);
		DrawValue=short(3-FIndex);
		Left=Rect.left+10+37*(DrawValue & 1);
		Top=Rect.top+10+37*(DrawValue >> 1);
		Visible=true;
	}
}
//---------------------------------------------------------------------------
// Other functions ----------------------------------------------------------
void __fastcall SetBoardForPegs(TImage *ToSet)
{
	Board=ToSet;
}
//---------------------------------------------------------------------------
void __fastcall LoadPegs(Classes::TComponent *Owner, TWinControl *Parent)
{
	AnsiString FileName;
	short int i,j;
	HilightShape->Visible=false;
	HilightShape->Parent=Parent;
	HilightShape->Brush->Style=bsClear;
	HilightShape->Pen->Width=2;
	for (i=0; i<4; i++)
	{
		switch (i)
		{
			case 0: FileName=DefaultBmps+"Red.bmp"; break;
			case 1: FileName=DefaultBmps+"Green.bmp"; break;
			case 2: FileName=DefaultBmps+"Yellow.bmp"; break;
			case 3: FileName=DefaultBmps+"Blue.bmp"; break;
		}
		for (j=0; j<4; j++)
		{
			Pegs[i][j]=new Peg(Owner,i,j);
			Pegs[i][j]->Parent=Parent;
			Pegs[i][j]->Center=true;
			Pegs[i][j]->Height=23;
			Pegs[i][j]->Width=23;
			Pegs[i][j]->Transparent=true;
			Pegs[i][j]->Visible=false;
			Pegs[i][j]->Picture->LoadFromFile(AppDir+FileName);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall GetRectFromPos(RECT *Rect, short Position)
{
	int i;		// Position on first quadrant (reflected - y axis)
	int Rotates;
	int x1,y1,x2,y2;
	if (Position<52)
	{
		Rotates=Position / 13;
		i=Position % 13;
		if (i==6) {x1=4; y1=186;}
		else
		{
			if (i<6) {i=5-i; y1=224;}
			else {i-=7; y1=148;}
			x1=4+24*i;
		}
		x2=x1+22; y2=y1+36;
	}
	else if (Position<72)
	{
		Rotates=(Position-52) / 5;
		i=(Position-52) % 5;
		x1=28+24*i;
		y1=186;
		x2=x1+22; y2=y1+36;
	}
	else if (Position<76)
	{
		Rotates=Position - 72;
		x1=y1=33; x2=y2=114;
	}
	else
	{
		x1=y1=148;
		x2=y2=260;
		Rotates=0;
	}
	for (i=0; i<Rotates; i++)
	{
		int tmp;
		tmp=x1; x1=408-y1; y1=tmp;
		tmp=x2; x2=408-y2; y2=tmp;
	}
	if (x1>x2) {i=x1; x1=x2; x2=i;}
	if (y1>y2) {i=y1; y1=y2; y2=i;}
	SetRect(Rect,x1,y1,x2,y2);
	OffsetRect(Rect,Board->Left,Board->Top);
}
//---------------------------------------------------------------------------
short __fastcall GetPosFromCoords(int x,int y)
{
	RECT Rect;
	POINT Point;
	Point.x=x; Point.y=y;
	short i;
	for (i=0; i<77; i++)
	{
		GetRectFromPos(&Rect,i);
		if (PtInRect(&Rect,Point)) break;
	}
	if (i<77) return i;
	else return posOutsideBoard;
}
//---------------------------------------------------------------------------
void __fastcall HilightPos(short Position)
{
	if (Hilighted!=Position)
	{
		RECT Rect;
		GetRectFromPos(&Rect,Position);
		HilightShape->Visible=false;
		if (Position!=posOutsideBoard)
		{
			HilightShape->Left=Rect.left;
			HilightShape->Top=Rect.top;
			HilightShape->Width=Rect.right-Rect.left+1;
			HilightShape->Height=Rect.bottom-Rect.top+1;
			HilightShape->Visible=true;
		}
	}
	Hilighted=Position;
}
//---------------------------------------------------------------------------
short __fastcall GetHilightedPos() {return Hilighted;}
//---------------------------------------------------------------------------
bool __fastcall IsPositionStar(short Position)
{
	switch (IgnoreStars)
	{
		case 1: return ((Position % 13)==8);
		case 2: return ((Position % 13)==3);
		case 3: return false;
		default: return (((Position % 13) % 5)==3);
	}
}
//---------------------------------------------------------------------------
short __fastcall GetPegsCount(short Position,short Color)
{
	short Count=0;
	for (short j=0; j<4; j++)
		if (Pegs[Color][j]->Position==Position)
			Count++;
	return Count;
}
//---------------------------------------------------------------------------
short __fastcall GetPegsCount(short Position)
{
	short Count=0;
	for (short i=0; i<4; i++)
		for (short j=0; j<4; j++)
			if (Pegs[i][j]->Position==Position)
				Count++;
	return Count;
}
//---------------------------------------------------------------------------
Peg* __fastcall GetPegOnPos(short Position,short Color,short Index)
{
	short Count=0;
	Peg *MyPeg=NULL;
	for (short i=0; i<4 && MyPeg==NULL; i++)
		for (short j=0; j<4 && MyPeg==NULL; j++)
			if (Pegs[i][j]->Position==Position && Pegs[i][j]->Color==Color)
				if (Count++==Index) MyPeg=Pegs[i][j];
	return MyPeg;
}
//---------------------------------------------------------------------------
Peg* __fastcall GetPegOnPos(short Position,short Index)
{
	short Count=0;
	Peg *MyPeg=NULL;
	for (short i=0; i<4 && MyPeg==NULL; i++)
		for (short j=0; j<4 && MyPeg==NULL; j++)
			if (Pegs[i][j]->Position==Position)
				if (Count++==Index) MyPeg=Pegs[i][j];
	return MyPeg;
}
//---------------------------------------------------------------------------
short int __fastcall DistFromStart(short Position,short Color)
{
	if (Position<52) return short((Position+52-8-13*Color) % 52);
	else if (Position<72) return short(52 + (Position-52)%5);
	else if (Position==76) return 57;
	else return 0;
}
//---------------------------------------------------------------------------
short __fastcall CountPegs(short Color)
{
	short Count=0;
	for (short i=0; i<4; i++)
		if (Pegs[Color][i]->Position != posOutsideBoard) Count++;
	return Count;
}
//---------------------------------------------------------------------------
short __fastcall CountPegsOutside(short Color)
{
	short Count=0;
	for (short i=0; i<4; i++)
		if (Pegs[Color][i]->Position < 52) Count++;
	return Count;
}
//---------------------------------------------------------------------------
short __fastcall CountPassed(short Color)
{
	short Count=0;
	for (short i=0; i<4; i++)
	{
		short Position=Pegs[Color][i]->Position;
		if (Position==76 || (Position>=52 && Position<72)) Count++;
		// ??? else if (Position==posOutsideBoard) Count--;
	}
	return Count;
}
//---------------------------------------------------------------------------

