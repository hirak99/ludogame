//---------------------------------------------------------------------------
#ifndef AboutUnitH
#define AboutUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TAboutForm : public TForm
{
__published:	// IDE-managed Components
	TImage *AboutImage;
	TButton *OkButton;
	TLabel *Label1;
	TBevel *Bevel1;
	TLabel *EMail;
	TLabel *WebSite;
	TLabel *Label2;
	TLabel *Label3;
	void __fastcall IneernetClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAboutForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutForm *AboutForm;
//---------------------------------------------------------------------------
#endif
