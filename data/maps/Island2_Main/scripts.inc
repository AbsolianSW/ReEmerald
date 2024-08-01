Island2_Main_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Island2_Main_MapScripts_MAP_SCRIPT_ON_TRANSITION
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Island2_Main_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

Island2_Main_MapScripts_MAP_SCRIPT_ON_TRANSITION:
	setflag FLAG_VISITED_TWO_ISLAND
	end

Island2_Main_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
	map_script_2 VAR_TWO_ISLAND_STATE, 1, Island2_Main_CutsceneFromGameCorner
	map_script_2 VAR_TWO_ISLAND_STATE, 2, Island2_Main_CutsceneFromCapeBrink
	.2byte 0


Island2_Main_Merchant::
	lockall
	faceplayer
	msgbox Island2_Main_Merchant_Text_0
	pokemart Island2_Main_Mart
	releaseall
	end


	.align 2
Island2_Main_Mart:
	.2byte ITEM_SODA_POP
	.2byte ITEM_FRESH_WATER
	.2byte ITEM_LEMONADE
	.2byte ITEM_BERRY_JUICE
	.2byte ITEM_LAVA_COOKIE
	.2byte ITEM_LIGHT_BALL
	.2byte ITEM_SOUL_DEW
	.2byte ITEM_STICK
	.2byte ITEM_THICK_CLUB
	.2byte ITEM_LUCKY_PUNCH
	.2byte ITEM_LUCKY_EGG
	.2byte ITEM_MOOMOO_MILK
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_NONE

Island2_Main_Sailor::
	lockall
	faceplayer
	msgbox Island2_Main_Sailor_Text_0
	releaseall
	end


Island2_Main_Woman::
	lockall
	faceplayer
	msgbox Island2_Main_Woman_Text_0
	releaseall
	end


Island2_Main_Beauty::
	lockall
	faceplayer
	msgbox Island2_Main_Beauty_Text_0
	releaseall
	end


Island2_Main_SuperNerd::
	lockall
	faceplayer
	msgbox Island2_Main_SuperNerd_Text_0
	releaseall
	end


Island2_Main_Boy::
	lockall
	faceplayer
	msgbox Island2_Main_Boy_Text_0
	releaseall
	end


Island2_Main_LittleBoy::
	lockall
	faceplayer
	msgbox Island2_Main_LittleBoy_Text_0
	releaseall
	end


Island2_Main_Sign::
	msgbox Island2_Main_Sign_Text_0, MSGBOX_SIGN
	setflag FLAG_TWO_ISLAND_HIDE_AQUA
	setflag FLAG_TWO_ISLAND_HIDE_SHELLY
	end


Island2_Main_FastCurrentSign::
	msgbox Island2_Main_FastCurrentSign_Text_0, MSGBOX_SIGN
	end


Island2_Main_GameCornerSign::
	msgbox Island2_Main_GameCornerSign_Text_0, MSGBOX_SIGN
	end


Island2_Main_CutsceneFromGameCorner::
	lockall
	special SpawnCameraObject
	addobject 10
	addobject 11
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraToCutsceneGC
	waitmovement 0
	special RemoveCameraObject
	applymovement 11, Common_Movement_WalkInPlaceFasterLeft
	msgbox Island2_Main_CutsceneFromGameCorner_Text_0
	closemessage
	applymovement 10, Common_Movement_WalkInPlaceFasterRight
	msgbox Island2_Main_CutsceneFromGameCorner_Text_1
	closemessage
	applymovement 10, Island2Main_Movement_GruntWalkAwayGCCutscene
	applymovement 11, Island2Main_Movement_ShellyWalkAwayGCCutscene
	waitmovement 0
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraBackFromCutsceneGC
	waitmovement 0
	special RemoveCameraObject
	setvar VAR_TWO_ISLAND_STATE, 3
	clearflag FLAG_TWO_ISLAND_HIDE_SHELLY
	addobject 12
	removeobject 10
	removeobject 11
	releaseall
	end


Island2_Main_CutsceneFromCapeBrink::
	lockall
	special StorePlayerCoordsInVars
	special SpawnCameraObject
	switch VAR_0x8005
	case 0, Island2_Main_CutsceneFromCapeBrink_3
	case 8, Island2_Main_CutsceneFromCapeBrink_4
	case 10, Island2_Main_CutsceneFromCapeBrink_5
Island2_Main_CutsceneFromCapeBrink_1:
	special RemoveCameraObject
	setvar VAR_TWO_ISLAND_STATE, 3
	clearflag FLAG_TWO_ISLAND_HIDE_SHELLY
	releaseall
	end

Island2_Main_CutsceneFromCapeBrink_3:
	switch VAR_0x8004
	case 36, Island2_Main_CutsceneFromCapeBrink_8
	case 37, Island2_Main_CutsceneFromCapeBrink_9
	case 38, Island2_Main_CutsceneFromCapeBrink_10
	case 39, Island2_Main_CutsceneFromCapeBrink_11
	case 40, Island2_Main_CutsceneFromCapeBrink_12
Island2_Main_CutsceneFromCapeBrink_6:
	waitmovement 0
	playse SE_DOOR
	addobject 15
	msgbox Island2_Main_CutsceneFromCapeBrink_Text_0
	closemessage
	applymovement 15, Island2Main_Movement_MattWalkOut
	addobject 14
	playse SE_DOOR
	msgbox Island2_Main_CutsceneFromCapeBrink_Text_1
	closemessage
	applymovement 15, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Island2_Main_CutsceneFromCapeBrink_Text_2
	closemessage
	applymovement 15, Island2Main_Movement_MattLostelleWalkAway
	applymovement 14, Island2Main_Movement_MattLostelleWalkAway
	waitmovement 0
	removeobject 14
	removeobject 15
	addobject 12
	switch VAR_0x8004
	case 36, Island2_Main_CutsceneFromCapeBrink_15
	case 37, Island2_Main_CutsceneFromCapeBrink_16
	case 38, Island2_Main_CutsceneFromCapeBrink_17
	case 39, Island2_Main_CutsceneFromCapeBrink_18
	case 40, Island2_Main_CutsceneFromCapeBrink_19
Island2_Main_CutsceneFromCapeBrink_13:
	waitmovement 0
	goto Island2_Main_CutsceneFromCapeBrink_1

Island2_Main_CutsceneFromCapeBrink_4:
	addobject 16
	addobject 17
	addobject 12
	applymovement 12, Common_Movement_WalkInPlaceFasterLeft
	applymovement 17, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Island2_Main_CutsceneFromCapeBrink_Text_3
	applymovement 12, Common_Movement_WalkInPlaceFasterUp
	applymovement 16, Common_Movement_WalkInPlaceFasterUp
	applymovement 17, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	applymovement 16, Common_Movement_ExclamationMark
	applymovement 17, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Island2_Main_CutsceneFromCapeBrink_Text_4
	closemessage
	applymovement 17, Island2Main_Movement_MattWalkAwayPC
	applymovement 16, Island2Main_Movement_LostelleWalkAwayPC
	waitmovement 0
	removeobject 16
	removeobject 17
	goto Island2_Main_CutsceneFromCapeBrink_1

Island2_Main_CutsceneFromCapeBrink_5:
	addobject 19
	applymovement 19, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement 19, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Island2_Main_CutsceneFromCapeBrink_Text_5
	trainerbattle_single TRAINER_SHELLY_ISLAND_2, Island2_Main_CutsceneFromCapeBrink_Text_6, Island2_Main_CutsceneFromCapeBrink_Text_7, Island2_Main_ShellyDefeat2
	goto Island2_Main_CutsceneFromCapeBrink_1

Island2_Main_CutsceneFromCapeBrink_8:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraToCutsceneCB36
	goto Island2_Main_CutsceneFromCapeBrink_6

Island2_Main_CutsceneFromCapeBrink_9:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraToCutsceneCB37
	goto Island2_Main_CutsceneFromCapeBrink_6

Island2_Main_CutsceneFromCapeBrink_10:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraToCutsceneCB38
	goto Island2_Main_CutsceneFromCapeBrink_6

Island2_Main_CutsceneFromCapeBrink_11:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraToCutsceneCB39
	goto Island2_Main_CutsceneFromCapeBrink_6

Island2_Main_CutsceneFromCapeBrink_12:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraToCutsceneCB40
	goto Island2_Main_CutsceneFromCapeBrink_6

Island2_Main_CutsceneFromCapeBrink_15:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraBackFromCutsceneCB36
	goto Island2_Main_CutsceneFromCapeBrink_13

Island2_Main_CutsceneFromCapeBrink_16:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraBackFromCutsceneCB37
	goto Island2_Main_CutsceneFromCapeBrink_13

Island2_Main_CutsceneFromCapeBrink_17:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraBackFromCutsceneCB38
	goto Island2_Main_CutsceneFromCapeBrink_13

Island2_Main_CutsceneFromCapeBrink_18:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraBackFromCutsceneCB39
	goto Island2_Main_CutsceneFromCapeBrink_13

Island2_Main_CutsceneFromCapeBrink_19:
	applymovement OBJ_EVENT_ID_CAMERA, Island2Main_Movement_CameraBackFromCutsceneCB40
	goto Island2_Main_CutsceneFromCapeBrink_13


Island2_Main_Shelly::
	lockall
	applymovement 12, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement 12, Common_Movement_FacePlayer
	trainerbattle_single TRAINER_SHELLY_ISLAND_2, Island2_Main_CutsceneFromCapeBrink_Text_6, Island2_Main_CutsceneFromCapeBrink_Text_7, Island2_Main_ShellyDefeat
	return


Island2_Main_ShellyDefeat::
	addobject 13
	playmoncry SPECIES_SUICUNE, CRY_MODE_WEAK
	applymovement 13, Island2Main_Movement_SuicuneRunAway
	waitmovement 0
	msgbox Island2_Main_ShellyDefeat_Text_0
	closemessage
	applymovement 12, Island2Main_Movement_ShellyRunAway
	setvar VAR_TWO_ISLAND_STATE, 4
	setflag FLAG_TWO_ISLAND_HIDE_SHELLY
	fadescreen FADE_TO_BLACK
	removeobject 12
	removeobject 13
	fadescreen FADE_FROM_BLACK
	releaseall
	end


Island2_Main_ShellyDefeat2::
	addobject 18
	playmoncry SPECIES_SUICUNE, CRY_MODE_WEAK
	applymovement 18, Island2Main_Movement_SuicuneRunAway2
	waitmovement 0
	msgbox Island2_Main_ShellyDefeat_Text_0
	closemessage
	applymovement 19, Island2Main_Movement_ShellyRunAway2
	setvar VAR_TWO_ISLAND_STATE, 4
	setflag FLAG_TWO_ISLAND_HIDE_SHELLY
	fadescreen FADE_TO_BLACK
	removeobject 19
	removeobject 18
	fadescreen FADE_FROM_BLACK
	releaseall
	return


Island2Main_Movement_CameraToCutsceneGC:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	step_end

Island2Main_Movement_CameraBackFromCutsceneGC:
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Island2Main_Movement_CameraToCutsceneCB36:
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
	walk_right
	walk_right
	walk_right
	step_end

Island2Main_Movement_CameraToCutsceneCB37:
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
	walk_right
	walk_right
	step_end

Island2Main_Movement_CameraToCutsceneCB38:
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
	walk_right
	step_end

Island2Main_Movement_CameraToCutsceneCB39:
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

Island2Main_Movement_CameraToCutsceneCB40:
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
	walk_left
	step_end

Island2Main_Movement_CameraBackFromCutsceneCB36:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	step_end

Island2Main_Movement_CameraBackFromCutsceneCB37:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

Island2Main_Movement_CameraBackFromCutsceneCB38:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	step_end

Island2Main_Movement_CameraBackFromCutsceneCB39:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Island2Main_Movement_CameraBackFromCutsceneCB40:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	step_end

Island2Main_Movement_GruntWalkAwayGCCutscene:
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Island2Main_Movement_ShellyWalkAwayGCCutscene:
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Island2Main_Movement_MattWalkOut:
	walk_left
	step_end

Island2Main_Movement_MattLostelleWalkAway:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Island2Main_Movement_MattWalkAwayPC:
	walk_down
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Island2Main_Movement_LostelleWalkAwayPC:
	delay_16
	delay_16
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Island2Main_Movement_SuicuneRunAway:
	walk_faster_left
	walk_faster_left
	walk_faster_left
	jump_2_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	step_end

Island2Main_Movement_SuicuneRunAway2:
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	step_end

Island2Main_Movement_ShellyRunAway:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	step_end

Island2Main_Movement_ShellyRunAway2:
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Island2_Main_Merchant_Text_0:
	.string "Salutations traveler! I sell only the\n"
	.string "most exclusive of items, come have a\l"
	.string "look!$"

Island2_Main_Sailor_Text_0:
	.string "That fellow there, he moved here from\n"
	.string "CELADON CITY. He started up a shop\l"
	.string "with his younger brother. You have to\l"
	.string "wonder if he knew what he was getting\l"
	.string "into...$"

Island2_Main_Woman_Text_0:
	.string "The shopkeepers brother took a job at\n"
	.string "the GAME CORNER. He was originally just\l"
	.string "trying to get some money so they could\l"
	.string "expand, but he liked it so much that he\l"
	.string "stayed there! The shop is doing well\l"
	.string "these days too!$"

Island2_Main_Beauty_Text_0:
	.string "Listen, listen. Did you know? A famous\n"
	.string "TRAINER came to the ISLAND a while\l"
	.string "back. He's one of the Kanto region's\l"
	.string "ELITE FOUR. His name is BRUNO. He went\l"
	.string "away disappointed when he found out\l"
	.string "that they were all sold out of Rage\l"
	.string "Candybars.$"

Island2_Main_SuperNerd_Text_0:
	.string "The discerning trainer knows to buy\n"
	.string "rare items here!$"

Island2_Main_Boy_Text_0:
	.string "There's an old woman who lives in a\n"
	.string "cottage out on the cape. She gives the\l"
	.string "eye to POKéMON. It's a bit unnerving.$"

Island2_Main_LittleBoy_Text_0:
	.string "Boing! Boing!\n"
	.string "Have you tried the jumping game?\p"
	.string "The POKéMON you can enter…\n"
	.string "Boing!\p"
	.string "Some go boing!\p"
	.string "And some go boooooooing!\p"
	.string "Did you know that?\n"
	.string "Boing!$"

Island2_Main_Sign_Text_0:
	.string "TWO ISLAND\n"
	.string "Boon island for two$"

Island2_Main_FastCurrentSign_Text_0:
	.string "DANGER! FAST CURRENT!\n"
	.string "Fishing and surfing prohibited$"

Island2_Main_GameCornerSign_Text_0:
	.string "Skip&Chomp!\n"
	.string "Joyful GAME CORNER$"

Island2_Main_CutsceneFromGameCorner_Text_0:
	.string "Ok! We've got SUICUNE, and Matt has\n"
	.string "radioded me that he's got the girl as\l"
	.string "well. You go and follow\l"
	.string "Matt...apparently the brat has followed\l"
	.string "us all the way here so I'm going to stay\l"
	.string "behind to hold them off!$"

Island2_Main_CutsceneFromGameCorner_Text_1:
	.string "You got it boss! We'll get to THREE\n"
	.string "ISLAND as fast as possible.$"

Island2_Main_CutsceneFromCapeBrink_Text_0:
	.string "Shelly come in!...I've got the girl, let's\n"
	.string "go!$"

Island2_Main_CutsceneFromCapeBrink_Text_1:
	.string "Waaaaaaaaaah! Somebody help meeeee!$"

Island2_Main_CutsceneFromCapeBrink_Text_2:
	.string "Shut up you little brat! You're going to\n"
	.string "show us where your friend lives in that\l"
	.string "Berry Forest.$"

Island2_Main_CutsceneFromCapeBrink_Text_3:
	.string "Alright, let's head to THREE ISLAND...$"

Island2_Main_CutsceneFromCapeBrink_Text_4:
	.string "This brat again...I'll take care of them,\n"
	.string "you take the girl and go ahead!$"

Island2_Main_CutsceneFromCapeBrink_Text_5:
	.string "Where did you come from? Anyways, this\n"
	.string "time I will finally be able to end you!$"

Island2_Main_CutsceneFromCapeBrink_Text_6:
	.string "You! Stop right there! You're not going\n"
	.string "anywhere, I'm taking you down!$"

Island2_Main_CutsceneFromCapeBrink_Text_7:
	.string "Arrgh! How does this keep happening...$"

Island2_Main_ShellyDefeat_Text_0:
	.string "And now this thing just ran away...why\n"
	.string "are they so hard to control? Anyhow, I\l"
	.string "think I've bought enough time for the\l"
	.string "others, hehe...good luck trying to stop\l"
	.string "all of us!$"
