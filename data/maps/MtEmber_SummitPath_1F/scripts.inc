MtEmber_SummitPath_1F_MapScripts::
	.byte 0


MtEmber_SummitPath_1F_Grunt::
	trainerbattle_single TRAINER_GRUNT_MT_EMBER_8, MtEmber_SummitPath_1F_Text_GruntIntro, MtEmber_SummitPath_1F_Text_GruntDefeat
	msgbox MtEmber_SummitPath_1F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	waitmessage
	end


MtEmber_SummitPath_1F_Text_GruntIntro::
	.string "Hey you! What are you doing here?$"

MtEmber_SummitPath_1F_Text_GruntDefeat::
	.string "Nosey kids always sniffing around...$"

MtEmber_SummitPath_1F_Text_GruntPostBattle::
	.string "I guess I can't stop you now.$"
