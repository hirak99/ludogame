//---------------------------------------------------------------------------
#ifndef SetBRUnitH
#define SetBRUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TSetBR : public TForm
{
__published:	// IDE-managed Components
	TButton *OkButton;
	TButton *CancelButton;
	TTabControl *TabColors;
	TLabel *Label1;
	TLabel *Label2;
	TTrackBar *BRTrack;
	TLabel *ValueLabel;
	TButton *SetAll;
	TLabel *Label3;
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TabColorsChange(TObject *Sender);
	void __fastcall SetAllClick(TObject *Sender);
	void __fastcall BRTrackChange(TObject *Sender);

private:	// User declarations
	void __fastcall EnableSetAll();
public:		// User declarations
	__fastcall TSetBR(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetBR *SetBR;
//---------------------------------------------------------------------------
#endif
