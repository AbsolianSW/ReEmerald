MtEmber_SummitPath_2F_MapScripts::
	.byte 0


MtEmber_SummitPath_2F_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_9, MtEmber_SummitPath_2F_Text_Grunt1Intro, MtEmber_SummitPath_2F_Text_Grunt1Defeat
	msgbox MtEmber_SummitPath_2F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_SummitPath_2F_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_10, MtEmber_SummitPath_2F_Text_Grunt2Intro, MtEmber_SummitPath_2F_Text_Grunt2Defeat
	msgbox MtEmber_SummitPath_2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_SummitPath_2F_Grunt3::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_11, MtEmber_SummitPath_2F_Text_Grunt3Intro, MtEmber_SummitPath_2F_Text_Grunt3Defeat
	msgbox MtEmber_SummitPath_2F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_SummitPath_2F_Text_Grunt1Intro::
	.string "Hey, be careful! You don't want to fall\n"
	.string "in, do you?$"

MtEmber_SummitPath_2F_Text_Grunt1Defeat::
	.string "Holy moly, you are really strong!$"

MtEmber_SummitPath_2F_Text_Grunt1PostBattle::
	.string "Again, please be careful not to fall\n"
	.string "into the hole!$"

MtEmber_SummitPath_2F_Text_Grunt2Intro::
	.string "I think it's getting hot in here. Do you\n"
	.string "feel that too?$"

MtEmber_SummitPath_2F_Text_Grunt2Defeat::
	.string "Man I got sweat in my eye...$"

MtEmber_SummitPath_2F_Text_Grunt2PostBattle::
	.string "Under normal conditions I would've\n"
	.string "beaten you easily!$"

MtEmber_SummitPath_2F_Text_Grunt3Intro::
	.string "Courtney is conducting important\n"
	.string "business! You can't go up!$"

MtEmber_SummitPath_2F_Text_Grunt3Defeat::
	.string "Courtney is going to be so mad...$"

MtEmber_SummitPath_2F_Text_Grunt3PostBattle::
	.string "Go on at your own risk...$"
