Island2_House_MapScripts::
	.byte 0


Island2_Main_House_EventScript_MoveRelearner::
	lockall
	applymovement LOCALID_MOVE_RELEARNER, Common_Movement_FacePlayer
	waitmovement 0
	goto_if_set FLAG_TEMP_1, Island2_Main_House_EventScript_CheckItems
	msgbox Island2_Main_House_Text_ImTheMoveTutor, MSGBOX_DEFAULT
	setflag FLAG_TEMP_1
	goto Island2_Main_House_EventScript_CheckItems
	end

Island2_Main_House_EventScript_CheckItems::
	checkitem ITEM_BIG_MUSHROOM
	goto_if_eq VAR_RESULT, TRUE, Island2_Main_House_EventScript_Teach_Mushroom
	checkitem ITEM_TINY_MUSHROOM, 2
	goto_if_eq VAR_RESULT, TRUE, Island2_Main_House_EventScript_Teach_TinyMushroom
	goto Island2_Main_House_EventScript_ComeBackWithMushroom
	end
	
	
Island2_Main_House_EventScript_Teach_Mushroom::	
	msgbox Island2_Main_House_Text_ThoseAreMushroomsWantMeToTeachMove, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Island2_Main_House_EventScript_ComeBackWithMushroom
	goto Island2_Main_House_EventScript_ChooseMonMushroom
	end
	
Island2_Main_House_EventScript_Teach_TinyMushroom::	
	msgbox Island2_Main_House_Text_ThoseAreMushroomsWantMeToTeachMove, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, Island2_Main_House_EventScript_ComeBackWithMushroom
	goto Island2_Main_House_EventScript_ChooseMonTinyMushroom
	end

Island2_Main_House_EventScript_ChooseMonMushroom::
	msgbox Island2_Main_House_Text_TutorWhichMon, MSGBOX_DEFAULT
	special ChooseMonForMoveRelearner
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Island2_Main_House_EventScript_ComeBackWithMushroom
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, Island2_Main_House_EventScript_CantTeachEggMushroom
	goto_if_eq VAR_0x8005, 0, Island2_Main_House_EventScript_NoMoveToTeachMonMushroom
	goto Island2_Main_House_EventScript_ChooseMoveMushroom
	end
	
Island2_Main_House_EventScript_ChooseMonTinyMushroom::
	msgbox Island2_Main_House_Text_TutorWhichMon, MSGBOX_DEFAULT
	special ChooseMonForMoveRelearner
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Island2_Main_House_EventScript_ComeBackWithMushroom
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, Island2_Main_House_EventScript_CantTeachEggTinyMushroom
	goto_if_eq VAR_0x8005, 0, Island2_Main_House_EventScript_NoMoveToTeachMonTinyMushroom
	goto Island2_Main_House_EventScript_ChooseMoveTinyMushroom
	end

Island2_Main_House_EventScript_ChooseMoveMushroom::
	msgbox Island2_Main_House_Text_TeachWhichMove, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto_if_eq VAR_0x8004, 0, Island2_Main_House_EventScript_ChooseMonMushroom
	msgbox Island2_Main_House_Text_HandedOverMushroom, MSGBOX_DEFAULT
	removeitem ITEM_BIG_MUSHROOM
	goto Island2_Main_House_EventScript_ComeBackWithMushroom
	end
	
Island2_Main_House_EventScript_ChooseMoveTinyMushroom::
	msgbox Island2_Main_House_Text_TeachWhichMove, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto_if_eq VAR_0x8004, 0, Island2_Main_House_EventScript_ChooseMonTinyMushroom
	msgbox Island2_Main_House_Text_HandedOverMushrooms, MSGBOX_DEFAULT
	removeitem ITEM_TINY_MUSHROOM, 2
	goto Island2_Main_House_EventScript_ComeBackWithMushroom
	end

Island2_Main_House_EventScript_NoMoveToTeachMonMushroom::
	msgbox Island2_Main_House_Text_DontHaveMoveToTeachPokemon, MSGBOX_DEFAULT
	goto Island2_Main_House_EventScript_ChooseMonMushroom
	end

Island2_Main_House_EventScript_CantTeachEggMushroom::
	msgbox Island2_Main_House_Text_CantTeachEgg, MSGBOX_DEFAULT
	goto Island2_Main_House_EventScript_ChooseMonMushroom
	end

Island2_Main_House_EventScript_NoMoveToTeachMonTinyMushroom::
	msgbox Island2_Main_House_Text_DontHaveMoveToTeachPokemon, MSGBOX_DEFAULT
	goto Island2_Main_House_EventScript_ChooseMonTinyMushroom
	end

Island2_Main_House_EventScript_CantTeachEggTinyMushroom::
	msgbox Island2_Main_House_Text_CantTeachEgg, MSGBOX_DEFAULT
	goto Island2_Main_House_EventScript_ChooseMonTinyMushroom
	end

Island2_Main_House_EventScript_ComeBackWithMushroom::
	msgbox Island2_Main_House_Text_ComeBackWithMushroom, MSGBOX_DEFAULT
	releaseall
	end

Island2_Main_House_Text_ImTheMoveTutor:
	.string "I'm the POKéMON MOVE MANIAC.\p"
    .string "I know every single move that\n"
    .string "POKéMON learn growing up.\p"
    .string "I'm also a mushroom maniac.\p"
    .string "Bring me two TINYMUSHROOMS,\n"
    .string "or one BIG MUSHROOM.\p"
    .string "If you do that for me, I'll teach\n"
    .string "a move to one POKéMON.$"

Island2_Main_House_Text_ThoseAreMushroomsWantMeToTeachMove:
	.string "Sniff, sniff…\n"
    .string "Hm! You smell of MUSHROOMS!\p"
    .string "Do you want me to teach a move\n"
    .string "to a POKéMON of yours?$"

Island2_Main_House_Text_TutorWhichMon:
	.string "Which POKéMON needs tutoring?$"

Island2_Main_House_Text_TeachWhichMove:
	.string "Which move should I teach?$"

Island2_Main_House_Text_DontHaveMoveToTeachPokemon:
	.string "Sorry…\p"
	.string "It doesn't appear as if I have any move\n"
	.string "I can teach that POKéMON.$"

Island2_Main_House_Text_HandedOverMushroom:
	.string "{PLAYER} handed over one\n"
    .string "BIG MUSHROOM in exchange.$"
	
Island2_Main_House_Text_HandedOverMushrooms:
	.string "{PLAYER} handed over two\n"
    .string "TINYMUSHROOMS in exchange.$"

Island2_Main_House_Text_ComeBackWithMushroom:
	.string "If your POKéMON need to learn a move,\n"
	.string "come back with some MUSHROOMS.$"

Island2_Main_House_Text_CantTeachEgg:
	.string "Hunh? There isn't a single move that\n"
	.string "I can teach an EGG.$"
