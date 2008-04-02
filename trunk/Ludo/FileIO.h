//---------------------------------------------------------------------------
#ifndef FileIOH
#define FileIOH
//---------------------------------------------------------------------------
void __fastcall SaveToFile(AnsiString &TheFile,bool RulesOnly);
bool __fastcall LoadFromFile(AnsiString &TheFile,bool RulesOnly);
#endif
