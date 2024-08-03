Island2_CapeBrink_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Island2_CapeBrink_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

Island2_CapeBrink_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
	map_script_2 VAR_TWO_ISLAND_STATE, 0, Island2_CapeBrink_SuicuneCatchingScene
	.2byte 0


Island2_CapeBrink_SuicuneCatchingScene::
	lockall
	getplayerxy VAR_TEMP_2, VAR_TEMP_3
	special SpawnCameraObject
	switch VAR_TEMP_2
	case 12, Island2_CapeBrink_SuicuneCatchingScene_3
	case 13, Island2_CapeBrink_SuicuneCatchingScene_4
	case 14, Island2_CapeBrink_SuicuneCatchingScene_5
	case 15, Island2_CapeBrink_SuicuneCatchingScene_6
	case 16, Island2_CapeBrink_SuicuneCatchingScene_7
Island2_CapeBrink_SuicuneCatchingScene_1:
	waitmovement 0
	special RemoveCameraObject
	applymovement 3, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox Island2_CapeBrink_SuicuneCatchingScene_Text_0
	closemessage
	applymovement 1, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox Island2_CapeBrink_SuicuneCatchingScene_Text_1
	closemessage
	playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
	delay 30
	removeobject 2
	msgbox Island2_CapeBrink_SuicuneCatchingScene_Text_2
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 3
	removeobject 1
	setflag FLAG_CAPE_BRINK_HIDE_AQUA
	setvar VAR_TWO_ISLAND_STATE, 2
	fadescreen FADE_FROM_BLACK
	special SpawnCameraObject
	switch VAR_TEMP_2
	case 12, Island2_CapeBrink_SuicuneCatchingScene_10
	case 13, Island2_CapeBrink_SuicuneCatchingScene_11
	case 14, Island2_CapeBrink_SuicuneCatchingScene_12
	case 15, Island2_CapeBrink_SuicuneCatchingScene_13
	case 16, Island2_CapeBrink_SuicuneCatchingScene_14
Island2_CapeBrink_SuicuneCatchingScene_8:
	waitmovement 0
	special RemoveCameraObject
	releaseall
	end

Island2_CapeBrink_SuicuneCatchingScene_3:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_Camera12
	goto Island2_CapeBrink_SuicuneCatchingScene_1

Island2_CapeBrink_SuicuneCatchingScene_4:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_Camera13
	goto Island2_CapeBrink_SuicuneCatchingScene_1

Island2_CapeBrink_SuicuneCatchingScene_5:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_Camera14
	goto Island2_CapeBrink_SuicuneCatchingScene_1

Island2_CapeBrink_SuicuneCatchingScene_6:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_Camera15
	goto Island2_CapeBrink_SuicuneCatchingScene_1

Island2_CapeBrink_SuicuneCatchingScene_7:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_Camera16
	goto Island2_CapeBrink_SuicuneCatchingScene_1

Island2_CapeBrink_SuicuneCatchingScene_10:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_CameraBack12
	goto Island2_CapeBrink_SuicuneCatchingScene_8

Island2_CapeBrink_SuicuneCatchingScene_11:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_CameraBack13
	goto Island2_CapeBrink_SuicuneCatchingScene_8

Island2_CapeBrink_SuicuneCatchingScene_12:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_CameraBack14
	goto Island2_CapeBrink_SuicuneCatchingScene_8

Island2_CapeBrink_SuicuneCatchingScene_13:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_CameraBack15
	goto Island2_CapeBrink_SuicuneCatchingScene_8

Island2_CapeBrink_SuicuneCatchingScene_14:
	applymovement OBJ_EVENT_ID_CAMERA, Island2_CapeBrink_CameraBack16
	goto Island2_CapeBrink_SuicuneCatchingScene_8


Island2_CapeBrink_GruntWalkAwayRight:
	delay_16
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Island2_CapeBrink_ShellyWalkAwayRight:
	walk_down
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Island2_CapeBrink_GruntWalkAwayLeft:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Island2_CapeBrink_ShellyWalkAwayLeft:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Island2_CapeBrink_Camera12:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Island2_CapeBrink_Camera13:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Island2_CapeBrink_Camera14:
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Island2_CapeBrink_Camera15:
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Island2_CapeBrink_Camera16:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Island2_CapeBrink_CameraBack12:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Island2_CapeBrink_CameraBack13:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	step_end

Island2_CapeBrink_CameraBack14:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	step_end

Island2_CapeBrink_CameraBack15:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

Island2_CapeBrink_CameraBack16:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Island2_CapeBrink_SuicuneCatchingScene_Text_0:
	.string "Holy moly boss, you've actually found\n"
	.string "it! It looks so majestic!$"

Island2_CapeBrink_SuicuneCatchingScene_Text_1:
	.string "It sure does...and with it by our side,\n"
	.string "surely we're unstoppable! Let's just\l"
	.string "hope these Master Balls are as good as\l"
	.string "advertised.$"

Island2_CapeBrink_SuicuneCatchingScene_Text_2:
	.string "HAHAHA! Yes! It worked! Quick, let's get\n"
	.string "out of here and meet up with Matt and\l"
	.string "the others on Three Island!$"
