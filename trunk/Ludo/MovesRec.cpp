//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MovesRec.h"
#include "Pegs.h"
#include "Moves.h"
#include "Dice.h"
#include "MainUnit.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
MovesRec AllMoves;
extern Peg *Pegs[4][4];
extern enum PlayerType Players[4];	// Players,		 enum defined in Moves.h
extern MovesList *MovesInHand;	// Moves buffer
extern short PlayerToMove;		// Defined in Interface.cpp
extern short WinnersList[4];	// Defined in Interface.cpp
extern TImage *WinnerImages[4];
// void __fastcall TakeSnapShot(MovesLink *Link)
//---------------------------------------------------------------------------
void __fastcall TakeSnapShot(MovesLink *Link)
{
	for (int i=0; i<4; i++)
	for (int j=0; j<4; j++)
		Link->PegPos[i][j]=Pegs[i][j]->Position;
	Link->PlayerToMove=char(PlayerToMove);
	for (int i=0; i<4; i++)
		Link->WinnersList=char(Link->WinnersList*4+WinnersList[i]);
}
//---------------------------------------------------------------------------
void __fastcall RestoreStatus(MovesLink *Link)
{
	MovesInHand->ClearAll();
	for (int i=0; i<4; i++)
	for (int j=0; j<4; j++)
		Pegs[i][j]->Position=Link->PegPos[i][j];
	PlayerToMove=Link->PlayerToMove;
	DrawPlayer(LudoForm->CurPlayerImage->Canvas,PlayerToMove);
	for (int i=3; i>=0; i--)
	{
		WinnersList[i]=short(Link->WinnersList & 4);
		Link->WinnersList<<=2;
	}
}
//---------------------------------------------------------------------------
__fastcall MovesRec::MovesRec()
{
	FirstLink=new MovesLink;
	FirstLink->Prev=FirstLink->Next=FirstLink;
	CurLink=FirstLink;
}
//---------------------------------------------------------------------------
void __fastcall MovesRec::SnapShot()
{
	if (MovesInHand->Count==0 && GetThrowsLeft()==1)
	{
		MovesLink *NewLink=new MovesLink;
		TakeSnapShot(NewLink);
		NewLink->Next=CurLink->Next;
		NewLink->Prev=CurLink;
		CurLink->Next->Prev=NewLink;
		CurLink->Next=NewLink;
		CurLink=NewLink;
	}
}
//---------------------------------------------------------------------------
void __fastcall MovesRec::DeleteCurrent()
{
	if (CurLink!=FirstLink)
	{
		MovesLink *Temp=CurLink;
		CurLink=CurLink->Prev;
		CurLink->Next=Temp->Next;
		CurLink->Next->Prev=CurLink;
		delete Temp;
	}
}
//---------------------------------------------------------------------------
void __fastcall MovesRec::Clear()
{
	while (FirstLink->Next!=FirstLink)
	{
		CurLink=FirstLink->Next;
		FirstLink->Next=CurLink->Next;
		delete CurLink;
	}
	CurLink=FirstLink;
}
//---------------------------------------------------------------------------
void __fastcall MovesRec::ClearNext()
{
	while (CurLink->Next!=FirstLink)
	{
		MovesLink *Temp=CurLink->Next;
		CurLink->Next=CurLink->Next->Next;
		delete Temp;
	}
}
//---------------------------------------------------------------------------
void __fastcall MovesRec::TakeBack()
{
	if (CurLink->Prev!=FirstLink)
	{
		CurLink=CurLink->Prev;
		RestoreStatus(CurLink);
	}
}
//---------------------------------------------------------------------------
void __fastcall MovesRec::Forward()
{
	if (CurLink->Next!=FirstLink)
	{
		CurLink=CurLink->Next;
		RestoreStatus(CurLink);
	}
}
//---------------------------------------------------------------------------
__fastcall MovesRec::~MovesRec()
{
//	for (CurLink=FirstLink->Next; CurLink!=FirstLink; CurLink=CurLink->Next);
	Clear();
	delete FirstLink;
}
//---------------------------------------------------------------------------

