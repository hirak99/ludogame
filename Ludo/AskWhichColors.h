//---------------------------------------------------------------------------
#ifndef AskWhichColorsH
#define AskWhichColorsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TWhichColors : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TImage *Image1;
	TEdit *Edit1;
	TUpDown *UpDown1;
	TLabel *Label3;
	TLabel *Label4;
	TImage *Image2;
	TEdit *Edit2;
	TUpDown *UpDown2;
	TButton *OkButton;
	TButton *CancelButton;
	void __fastcall UpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	int Total;
	__fastcall TWhichColors(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWhichColors *WhichColors;
//---------------------------------------------------------------------------
#endif
