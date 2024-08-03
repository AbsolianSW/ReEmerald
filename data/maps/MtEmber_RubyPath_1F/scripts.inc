MtEmber_RubyPath_1F_MapScripts::
	.byte 0


MtEmber_RubyPath_1F_Grunt::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_3, MtEmber_RubyPath_1F_Text_GruntIntro, MtEmber_RubyPath_1F_Text_GruntDefeat
	msgbox MtEmber_RubyPath_1F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_RubyPath_1F_Text_GruntIntro::
	.string "What? How did a child get in here? It's\n"
	.string "my first day on the job, I can't let this\l"
	.string "happen!$"

MtEmber_RubyPath_1F_Text_GruntDefeat::
	.string "I am so fired...$"

MtEmber_RubyPath_1F_Text_GruntPostBattle::
	.string "Beaten by a child...maybe I'm just not\n"
	.string "cut out for Team Magma?$"
