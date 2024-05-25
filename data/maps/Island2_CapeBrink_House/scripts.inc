Island2_CapeBrink_House_MapScripts::
	.byte 0


Island2_CapeBrink_House_EventScript_Tutor::
	lock
	faceplayer
	msgbox Island2_CapeBrink_House_EventScript_Tutor_Text_0
	waitmessage
	multichoicedefault 21, 6, MULTI_CAPE_BRINK_TUTOR, 0, FALSE
	switch VAR_RESULT
	case 0, Island2_CapeBrink_House_EventScript_Tutor_3
	case 1, Island2_CapeBrink_House_EventScript_Tutor_4
	case 2, Island2_CapeBrink_House_EventScript_Tutor_5
	case 3, Island2_CapeBrink_House_EventScript_Tutor_6
Island2_CapeBrink_House_EventScript_Tutor_1:
	call MoveTutor_EventScript_OpenPartyMenu
	compare VAR_RESULT, FALSE
	goto_if_eq Island2_CapeBrink_House_EventScript_Tutor_8
	msgbox Island2_CapeBrink_House_EventScript_Tutor_Text_2
	waitmessage
	releaseall
	end

Island2_CapeBrink_House_EventScript_Tutor_3:
	setvar VAR_0x8005, TUTOR_MOVE_FRENZY_PLANT
	goto Island2_CapeBrink_House_EventScript_Tutor_1

Island2_CapeBrink_House_EventScript_Tutor_4:
	setvar VAR_0x8005, TUTOR_MOVE_BLAST_BURN
	goto Island2_CapeBrink_House_EventScript_Tutor_1

Island2_CapeBrink_House_EventScript_Tutor_5:
	setvar VAR_0x8005, TUTOR_MOVE_HYDRO_CANNON
	goto Island2_CapeBrink_House_EventScript_Tutor_1

Island2_CapeBrink_House_EventScript_Tutor_6:
	msgbox Island2_CapeBrink_House_EventScript_Tutor_Text_1
	waitmessage
	releaseall
	end

Island2_CapeBrink_House_EventScript_Tutor_8:
	msgbox Island2_CapeBrink_House_EventScript_Tutor_Text_1
	waitmessage
	releaseall
	end


Island2_CapeBrink_House_EventScript_Tutor_Text_0:
	.string "I perfected the ultimate move of every\n"
	.string "starter type...But no one wants to use\l"
	.string "them...do you?$"

Island2_CapeBrink_House_EventScript_Tutor_Text_1:
	.string "Gaah! You reject it? No, I won't be\n"
	.string "dissuaded!$"

Island2_CapeBrink_House_EventScript_Tutor_Text_2:
	.string "Gasp, gasp, gasp...I didn't think I could\n"
	.string "teach that move while I still lived...$"
