MtEmber_RubyPath_B3F_MapScripts::
	.byte 0


MtEmber_RubyPath_B3F_Grunt::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_5, MtEmber_RubyPath_B3F_Text_GruntIntro, MtEmber_RubyPath_B3F_Text_GruntDefeat
	msgbox MtEmber_RubyPath_B3F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_RubyPath_B3F_Text_GruntIntro::
	.string "I was supposed to follow the other\n"
	.string "guys, but I can't remember which way to\l"
	.string "go...$"

MtEmber_RubyPath_B3F_Text_GruntDefeat::
	.string "Do you know which way Tabitha went?$"

MtEmber_RubyPath_B3F_Text_GruntPostBattle::
	.string "Man i hate caves, I always get lost!$"
