ÿ
 TSTARTGAMEFORM 0@  TPF0TStartGameFormStartGameFormLeftÈ Top«BorderStylebsDialogCaptionSelect Game TypeClientHeightâ ClientWidth[Font.CharsetANSI_CHARSET
Font.ColorclWindowTextFont.Heightõ	Font.NameComic Sans MS
Font.Style PositionpoScreenCenterShowHint	
OnActivateFormActivateOnCreate
FormCreate	OnDestroyFormDestroyPixelsPerInch`
TextHeight TPageControl	GameModesLeft TopWidthHeightÙ 
ActivePageMode1TabOrder  	TTabSheetMode1CaptionSingle TLabelLabel1LeftTopWidthç Height-CaptionThis page allows you to play a game, with one person playing for one color. You can team any two players up in a four player game.WordWrap	  	TCheckBox
RedPlayingTagLeft0Top8Width9HeightCaption&RedTabOrder OnClickPlayingClick  	TCheckBoxGreenPlayingTagLeft0TopPWidth9HeightCaption&GreenTabOrderOnClickPlayingClick  	TCheckBoxBluePlayingTagLeft0Top Width9HeightCaption&BlueTabOrderOnClickPlayingClick  	TCheckBoxYellowPlayingTagLeft0TophWidth9HeightCaption&YellowTabOrderOnClickPlayingClick  	TComboBox	RedPlayerLeft Top8WidthYHeightStylecsDropDownList
ItemHeightItems.StringsHumanComputer TabOrderVisible  	TComboBoxGreenPlayerLeft TopPWidthYHeightStylecsDropDownList
ItemHeightItems.StringsHumanComputer TabOrderVisible  	TComboBoxYellowPlayerLeft TophWidthYHeightStylecsDropDownList
ItemHeightItems.StringsHumanComputer TabOrderVisible  	TComboBox
BluePlayerLeft Top WidthYHeightStylecsDropDownList
ItemHeightItems.StringsHumanComputer TabOrderVisible  	TComboBox	ComboTeamLeft0Top Width© HeightHintSet team typeStylecsDropDownList
ItemHeightItems.StringsNo TeamRed+Yellow vs Blue+GreenRed+Green vs Blue+YellowRed+Blue vs Green+Yellow TabOrderVisible   	TTabSheetMode2CaptionDouble TLabelLabel2LeftTopWidthç HeightCaptionLThis page allows you to play a game, with one person playing for two colors.WordWrap	  TLabelLabel3LeftxTopHWidthHeightCaptionvs  TLabelLabel4LeftTop8Width.HeightCaption
Player &A:  TLabelLabel5Left Top8Width-HeightCaption
Player &B:  	TComboBoxDoubleGroup1LeftTopHWidthiHeightStylecsDropDownList
ItemHeightItems.Strings
Red+Yellow	Red+GreenRed+Blue TabOrder OnChangeDoubleGroupChange  	TComboBoxDoubleGroup2Left TopHWidthiHeightStylecsDropDownList
ItemHeightItems.Strings
Blue+GreenBlue+YellowGreen+Yellow TabOrderOnChangeDoubleGroupChange  	TComboBoxDoublePlayer1LeftTop`WidthYHeightStylecsDropDownList
ItemHeightItems.StringsHumanComputer TabOrder  	TComboBoxDoublePlayer2Left Top`WidthYHeightStylecsDropDownList
ItemHeightItems.StringsHumanComputer TabOrder    TButtonOkButtonLeftTop`WidthAHeightCaption&OkDefault	TabOrderOnClickOkButtonClick  TButtonCancelButtonLeftTop WidthAHeightCancel	Caption&CancelModalResultTabOrder   