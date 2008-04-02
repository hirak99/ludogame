//---------------------------------------------------------------------------
#ifndef InterfaceH
#define InterfaceH
//---------------------------------------------------------------------------
void __fastcall UpdateDescriptionText();	// Updates 'Whose Move' display
void __fastcall ShowAllWinners();
void __fastcall StartUpGame(bool LoadingFile=false);
void __fastcall HintMove(bool BasicHint);
void __fastcall ManageClick(int X,int Y);
#endif
