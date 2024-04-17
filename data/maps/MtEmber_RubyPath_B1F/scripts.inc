MtEmber_RubyPath_B1F_MapScripts::
	.byte 0


MtEmber_RubyPath_B1F_Grunt::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_4, MtEmber_RubyPath_B1F_Text_GruntIntro, MtEmber_RubyPath_B1F_Text_GruntDefeat
	msgbox MtEmber_RubyPath_B1F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_RubyPath_B1F_Text_GruntIntro::
	.string "Really? First day on the job and a child\n"
	.string "gets past him? What an idiot...$"

MtEmber_RubyPath_B1F_Text_GruntDefeat::
	.string "...$"

MtEmber_RubyPath_B1F_Text_GruntPostBattle::
	.string "Wow, are you that kid from Hoenn? You're\n"
	.string "so strong!$"
