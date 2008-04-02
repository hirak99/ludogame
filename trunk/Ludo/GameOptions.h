//---------------------------------------------------------------------------
#ifndef OtherOptionsH
#define OtherOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TOptionsForm : public TForm
{
__published:	// IDE-managed Components
	TButton *OkButton;
	TButton *Cancel;
	TButton *Defaults;
	TButton *Restore;
	TGroupBox *GroupAim;
	TRadioButton *RadioNormal;
	TRadioButton *RadioElem;
	TGroupBox *GroupExtra;
	TCheckBox *GetExtra1;
	TCheckBox *GetExtra2;
	TCheckBox *GetExtra3;
	TCheckBox *GetExtra4;
	TGroupBox *GroupDie;
	TTrackBar *TrackBias;
	TLabel *BiasLabel;
	TBevel *Bevel1;
	TCheckBox *ChkDiscard;
	TGroupBox *GroupPegs;
	TLabel *Label3;
	TEdit *Counters;
	TUpDown *UDPegs;
	TLabel *SayCounter;
	TEdit *Ready;
	TUpDown *UDStart;
	TLabel *Label5;
	TGroupBox *GroupAutoDice;
	TEdit *HumanStop;
	TUpDown *UpDown1;
	TLabel *Label1;
	TEdit *ComputerStop;
	TUpDown *UpDown2;
	TLabel *Label2;
	TGroupBox *GroupAuto;
	TCheckBox *CheckAutoComp;
	TEdit *EditAutoInterval;
	TUpDown *UpDown3;
	TLabel *Label4;
	TGroupBox *GroupOther;
	TCheckBox *CheckWStars;
	TCheckBox *CheckJump;
	TCheckBox *CheckBStars;
	TCheckBox *SixAlert;
	TCheckBox *AutoOnlyMove;
	TCheckBox *cbKillForHome;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TrackBiasChange(TObject *Sender);
	void __fastcall RestoreClick(TObject *Sender);
	void __fastcall DefaultsClick(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall GetExtra1Click(TObject *Sender);
	void __fastcall CountersChange(TObject *Sender);
	
	
	void __fastcall AutoStopChange(TObject *Sender);
	void __fastcall EditAutoIntervalExit(TObject *Sender);
	
	void __fastcall ChkDiscardClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TOptionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionsForm *OptionsForm;
//---------------------------------------------------------------------------
#endif
       