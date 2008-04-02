//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Ludo.res");
USEFORM("MainUnit.cpp", LudoForm);
USEUNIT("Pegs.cpp");
USEFORM("StartGameUnit.cpp", StartGameForm);
USEUNIT("Dice.cpp");
USEUNIT("Moves.cpp");
USEUNIT("Interface.cpp");
USEFORM("AskWhichColors.cpp", WhichColors);
USEFORM("SetBRUnit.cpp", SetBR);
USEFORM("AboutUnit.cpp", AboutForm);
USEUNIT("FileIO.cpp");
USEFORM("GameOptions.cpp", OptionsForm);
USEFORM("AutoDiceUnit.cpp", AutoDice);
USEFORM("TipsUnit.cpp", TipsForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Read the help to know that the next line can be safely deleted.
//		Application->Initialize();
		Application->CreateForm(__classid(TLudoForm), &LudoForm);
		Application->CreateForm(__classid(TStartGameForm), &StartGameForm);
		Application->CreateForm(__classid(TWhichColors), &WhichColors);
		Application->CreateForm(__classid(TSetBR), &SetBR);
		Application->CreateForm(__classid(TAboutForm), &AboutForm);
		Application->CreateForm(__classid(TOptionsForm), &OptionsForm);
		Application->CreateForm(__classid(TAutoDice), &AutoDice);
		Application->CreateForm(__classid(TTipsForm), &TipsForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
