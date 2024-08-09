MtEmber_Summit_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MtEmber_Summit_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

MtEmber_Summit_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
	map_script_2 VAR_MT_EMBER_SUMMIT_STATE, 0, MtEmber_Summit_Cutscene
	.2byte 0


MtEmber_Summit_Cutscene::
	goto_if_set FLAG_MT_EMBER_HIDE_MOLTRES, MtEmber_Summit_Cutscene_2
	lockall
	delay 60
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, MtEmber_Summit_CameraMovementUp
	waitmovement 0
	special RemoveCameraObject
	msgbox MtEmber_Summit_Cutscene_Text_0
	playmoncry SPECIES_MOLTRES, CRY_MODE_ENCOUNTER
	delay 30
	removeobject 5
	SetFlag FLAG_MT_EMBER_HIDE_MOLTRES
	msgbox MtEmber_Summit_Cutscene_Text_1
	applymovement 6, MtEmber_Summit_TurnCourtney
	waitmovement 0
	playse SE_PIN
	applymovement 6, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox MtEmber_Summit_Cutscene_Text_2
	delay 30
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, MtEmber_Summit_CameraMovementDown
	waitmovement 0
	special RemoveCameraObject
	SetVar VAR_MT_EMBER_SUMMIT_STATE, 1
	releaseall
	end

MtEmber_Summit_Cutscene_2:
	end


MtEmber_Summit_CameraMovementUp:
	walk_slow_up
	walk_slow_up
	walk_slow_up
	walk_slow_up
	walk_slow_up
	walk_slow_up
	walk_slow_up
	step_end

MtEmber_Summit_CameraMovementDown:
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	step_end

MtEmber_Summit_TurnCourtney:
	face_down
	step_end

MtEmber_Summit_Courtney::
	trainerbattle_single TRAINER_COURTNEY_ISLAND_1, MtEmber_Summit_Text_CourtneyIntro, MtEmber_Summit_Text_CourtneyDefeat, MtEmber_Summit_CourtneyDefeat
	msgbox MtEmber_Summit_Text_CourtneyPostBattle
	return


MtEmber_Summit_CourtneyDefeat::
	SetFlag FLAG_ISLAND1_COURTNEY_DEFEATED
	lockall
	playse SE_PIN
	applymovement 6, Common_Movement_ExclamationMark
	addobject 5
	playmoncry SPECIES_MOLTRES, CRY_MODE_WEAK
	applymovement 5, MtEmber_Summit_MoltresFlyAway
	waitmovement 0
	removeobject 5
	msgbox MtEmber_Summit_CourtneyDefeat_Text_0
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 6
	fadescreen FADE_FROM_BLACK
	releaseall
	end


MtEmber_Summit_MoltresFlyAway:
	face_up
	delay_16
	delay_16
	fly_up
	step_end

MtEmber_Summit_Moltres::
	bufferspeciesname 0, SPECIES_MOLTRES
	end


MtEmber_Summit_Cutscene_Text_0:
	.string "At last...I've found you, Moltres.\n"
	.string "Together, we shall finally conquer this\l"
	.string "world and expand the land masses, just\l"
	.string "as Maxie foresaw!$"

MtEmber_Summit_Cutscene_Text_1:
	.string "I've got it! Let's move!$"

MtEmber_Summit_Cutscene_Text_2:
	.string "You again? I won't let you stop me this\n"
	.string "time...$"

MtEmber_Summit_CourtneyDefeat_Text_0:
	.string "Wow! These legendaries are really hard\n"
	.string "to control...well, it was ineffective\l"
	.string "against you anyways, but I won't give\l"
	.string "up!$"

MtEmber_Summit_Text_CourtneyIntro::
	.string "...$"

MtEmber_Summit_Text_CourtneyDefeat::
	.string "Even Moltres couldn't stop you...I guess\n"
	.string "I need to find something stronger...$"

MtEmber_Summit_Text_CourtneyPostBattle::
	.string "You're starting to really annoy me...but\n"
	.string "we will prevail!$"
