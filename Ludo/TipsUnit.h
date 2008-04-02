//---------------------------------------------------------------------------
#ifndef TipsUnitH
#define TipsUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TTipsForm : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TButton *CloseTips;
	TCheckBox *ShowTips;
	TButton *NextTip;
	TLabel *Label1;
	TButton *ResetTips;
	TLabel *LabelTip;
	TBevel *Bevel1;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall NextTipClick(TObject *Sender);
	void __fastcall ResetTipsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TTipsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTipsForm *TipsForm;
//---------------------------------------------------------------------------
#endif
