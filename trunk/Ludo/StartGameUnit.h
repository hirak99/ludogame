//---------------------------------------------------------------------------
#ifndef StartGameUnitH
#define StartGameUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TStartGameForm : public TForm
{
__published:	// IDE-managed Components
	TPageControl *GameModes;
	TTabSheet *Mode1;
	TTabSheet *Mode2;
	TCheckBox *RedPlaying;
	TCheckBox *GreenPlaying;
	TCheckBox *BluePlaying;
	TCheckBox *YellowPlaying;
	TComboBox *RedPlayer;
	TComboBox *GreenPlayer;
	TComboBox *YellowPlayer;
	TComboBox *BluePlayer;
	TComboBox *DoubleGroup1;
	TComboBox *DoubleGroup2;
	TComboBox *DoublePlayer1;
	TComboBox *DoublePlayer2;
	TLabel *Label2;
	TLabel *Label1;
	TComboBox *ComboTeam;
	TButton *OkButton;
	TButton *CancelButton;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ManageClick();
	void __fastcall PlayingClick(TObject *Sender);
	void __fastcall DoubleGroupChange(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TStartGameForm(TComponent* Owner);
	void __fastcall SetChangeTypeMode(bool Value);
};
//---------------------------------------------------------------------------
extern PACKAGE TStartGameForm *StartGameForm;
//---------------------------------------------------------------------------
#endif
