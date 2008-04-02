//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TLudoForm : public TForm
{
__published:	// IDE-managed Components
	TImage *LudoBoard;
	TBevel *BoardBevel;
	TMainMenu *MainMenu;
	TMenuItem *GameMenu;
	TMenuItem *NewGame;
	TBevel *DiceBevel;
	TImage *DiceImage;
	TBevel *MovesListBevel;
	TImage *MovesListImage;
	TLabel *Label2;
	TShape *Hilight;
	TImage *CurPlayerImage;
	TLabel *Label1;
	TGroupBox *GroupWinners;
	TLabel *Second;
	TLabel *Third;
	TLabel *Fourth;
	TImage *Winner1;
	TImage *Winner2;
	TImage *Winner3;
	TImage *Winner4;
	TLabel *PlayerType;
	TImage *HomeImage;
	TMenuItem *ChangeType;
	TMenuItem *ExitProg;
	TBevel *LogoBevel;
	TMenuItem *N2;
	TMenuItem *ComputersLevel;
	TMenuItem *HelpMenu;
	TMenuItem *About;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TMenuItem *SaveAs;
	TMenuItem *Load;
	TMenuItem *OptionsMenu;
	TMenuItem *ShowNumbers;
	TMenuItem *N1;
	TMenuItem *Suggest;
	TMenuItem *N3;
	TMenuItem *GameOptions;
	TMenuItem *N4;
	TMenuItem *ResetGame;
	TLabel *Note;
	TMenuItem *Save;
	TTimer *Timer;
	TMenuItem *AutoDiceOptions;
	TLabel *First;
	TSpeedButton *Automation;
	TImage *Logo;
	TMenuItem *MenuAuto;
	TMenuItem *TodaysTips;
	TMenuItem *DumbWList;
	TMenuItem *N5;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall NewGameClick(TObject *Sender);
	void __fastcall ChangeTypeClick(TObject *Sender);
	void __fastcall MovesListImageMouseUp(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall DiceImageMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall HilightMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall GameMenuClick(TObject *Sender);
	void __fastcall ExitProgClick(TObject *Sender);
	void __fastcall ComputersLevelClick(TObject *Sender);
	void __fastcall AboutClick(TObject *Sender);
	void __fastcall SaveAsClick(TObject *Sender);
	void __fastcall LoadClick(TObject *Sender);
	void __fastcall LudoBoardMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall ShowNumbersClick(TObject *Sender);
	void __fastcall SuggestClick(TObject *Sender);
	void __fastcall GameOptionsClick(TObject *Sender);
	void __fastcall ResetGameClick(TObject *Sender);
	void __fastcall RestrictSize(TMessage &Message);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall HelpMenuClick(TObject *Sender);
	void __fastcall AutoDiceOptionsClick(TObject *Sender);
	void __fastcall AutomationClick(TObject *Sender);
	void __fastcall AutomationMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall LogoMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall MenuAutoClick(TObject *Sender);
	void __fastcall OptionsMenuClick(TObject *Sender);
	void __fastcall TodaysTipsClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall DumbWListClick(TObject *Sender);
	void __fastcall PlayerTypeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
	void __fastcall RedrawBoard(bool ShowNumbers);
public:		// User declarations
	__fastcall TLudoForm(TComponent* Owner);
	void __fastcall EnableTimer(bool Enable);
BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TLudoForm *LudoForm;
//---------------------------------------------------------------------------
#endif
 