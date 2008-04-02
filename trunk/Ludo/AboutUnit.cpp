//---------------------------------------------------------------------------
#include <vcl.h>
#include <ShellApi.h>
#pragma hdrstop

#include "AboutUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;
//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::IneernetClick(TObject *Sender)
{
	char *Exec;
	if (Sender==WebSite) Exec="http://www.geocities.com/hirak_99/index.html";
	else Exec="mailto:hirak_99@yahoo.com";
	ShellExecute (Application->Handle,"open",Exec,NULL,NULL,SW_SHOW);
}
//---------------------------------------------------------------------------

