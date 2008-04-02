//---------------------------------------------------------------------------
#ifndef AutoDiceUnitH
#define AutoDiceUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TAutoDice : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TCheckBox *AutoDiceR;
	TCheckBox *AutoDiceG;
	TCheckBox *AutoDiceY;
	TCheckBox *AutoDiceB;
	TButton *Ok;
	TBevel *Bevel1;
	TButton *Cancel;
	TGroupBox *Options;
	TCheckBox *Always;
	TCheckBox *Computer;
	TCheckBox *Human;
	TBevel *Bevel2;
	TSpeedButton *Extend;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CheckBoxClick(TObject *Sender);
	void __fastcall ExtendClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAutoDice(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAutoDice *AutoDice;
//---------------------------------------------------------------------------
#endif
      