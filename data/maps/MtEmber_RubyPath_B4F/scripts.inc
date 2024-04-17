MtEmber_RubyPath_B4F_MapScripts::
	.byte 0


MtEmber_RubyPath_B4F_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_6, MtEmber_RubyPath_B4F_Text_Grunt1Intro, MtEmber_RubyPath_B4F_Text_Grunt1Defeat
	msgbox MtEmber_RubyPath_B4F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_RubyPath_B4F_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_7, MtEmber_RubyPath_B4F_Text_Grunt2Intro, MtEmber_RubyPath_B4F_Text_Grunt2Defeat
	msgbox MtEmber_RubyPath_B4F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_RubyPath_B4F_Text_Grunt1Intro::
	.string "You can't be here! Official TEAM MAGMA\n"
	.string "business!$"

MtEmber_RubyPath_B4F_Text_Grunt1Defeat::
	.string "Don't you dare go down that ladder$"

MtEmber_RubyPath_B4F_Text_Grunt1PostBattle::
	.string "Get lost already!$"

MtEmber_RubyPath_B4F_Text_Grunt2Intro::
	.string "Official TEAM MAGMA business! Please\n"
	.string "leave the area immediately!$"

MtEmber_RubyPath_B4F_Text_Grunt2Defeat::
	.string "Access strictly forbidden!$"

MtEmber_RubyPath_B4F_Text_Grunt2PostBattle::
	.string "Please leave immediately$"
