MtEmber_RubyPath_B5F_MapScripts::
	.byte 0


MtEmber_RubyPath_B5F_Event_Script_Message::
	msgbox MtEmber_RubyPath_B5F_Event_Script_Message_Text_0
	return


MtEmber_RubyPath_B5F_Event_Script_Tabitha::
	msgbox MtEmber_RubyPath_B5F_Event_Script_Tabitha_Text_0
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement 1, Common_Movement_FacePlayer
	trainerbattle_single TRAINER_TABITHA_ISLAND_1, Ember_RubyPath_B5F_Text_Tabitha_Intro, MtEmber_RubyPath_B5F_TabithaDefeat
	end


MtEmber_RubyPath_B5F_TabithaDefeat::
	msgbox MtEmber_RubyPath_B5F_TabithaDefeat_Text_0
	fadescreen FADE_TO_BLACK
	removeobject 1
	fadescreen FADE_FROM_BLACK
	SetFlag FLAG_ISLAND1_TABITHA_DEFEATED
	SetVar VAR_ONE_ISLAND_STATE, 3
	end


MtEmber_RubyPath_B5F_Event_Script_Message_Text_0:
	.string "The Ruby used to link us up to the\n"
	.string "Hoenn Region was found here.\p"
	.string "Celio$"

MtEmber_RubyPath_B5F_Event_Script_Tabitha_Text_0:
	.string "TABITHA: Hmmm...surely if they found\n"
	.string "one Ruby in this cave there must be\l"
	.string "another one somewhere...$"

MtEmber_RubyPath_B5F_TabithaDefeat_Text_0:
	.string "Ahaha! Wow, you are really strong. Oh\n"
	.string "well, this was just our side mission\l"
	.string "here anyways...the real deal is going on\l"
	.string "at the summit anyways! I'm out of here!$"

Ember_RubyPath_B5F_Text_Tabitha_Intro::
	.string "What? How did you get in here? You still\n"
	.string "didn't learn your lesson, huh? Well let\l"
	.string "me show you the true power of TEAM\l"
	.string "MAGMA!$"
