.set LOCALID_RIVAL, 1

LittlerootTown_BrendansHouse_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_BrendansHouse_2F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_BrendansHouse_2F_OnWarp
	.byte 0

LittlerootTown_BrendansHouse_2F_OnTransition:
	call_if_lt VAR_LITTLEROOT_RIVAL_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_CheckSetReadyToMeetBrendan
	call_if_ge VAR_LITTLEROOT_RIVAL_STATE, 3, LittlerootTown_BrendansHouse_2F_EventScript_CheckShouldUpdateBrendanPos
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 4, PlayersHouse_2F_EventScript_BlockStairsUntilClockIsSet
	call SecretBase_EventScript_SetDecorationFlags
	setvar VAR_SECRET_BASE_INITIALIZED, 0
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckShouldUpdateBrendanPos::
	goto_if_set FLAG_MET_RIVAL_LILYCOVE, LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos
	goto_if_ge VAR_BIRCH_LAB_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_Ret
	goto LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos

LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_2F_EventScript_Ret
	@ Odd that the MaysHouse equivalent was used below instead
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_MaysHouse_2F_EventScript_Ret
	setobjectxyperm LOCALID_RIVAL, 0, 2
	setobjectmovementtype LOCALID_RIVAL, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_2F_EventScript_Ret::
	return

LittlerootTown_BrendansHouse_2F_EventScript_CheckSetReadyToMeetBrendan::
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, LittlerootTown_BrendansHouse_2F_EventScript_SetReadyToMeetBrendan
	return

LittlerootTown_BrendansHouse_2F_EventScript_SetReadyToMeetBrendan::
	setvar VAR_LITTLEROOT_RIVAL_STATE, 2
	return

LittlerootTown_BrendansHouse_2F_OnWarp:
	map_script_2 VAR_SECRET_BASE_INITIALIZED, 0, LittlerootTown_BrendansHouse_2F_EventScript_CheckInitDecor
	.2byte 0

LittlerootTown_BrendansHouse_2F_EventScript_CheckInitDecor::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, SecretBase_EventScript_InitDecorations
	end

LittlerootTown_BrendansHouse_2F_EventScript_RivalsPokeBall::
	lockall
	goto_if_eq VAR_LITTLEROOT_RIVAL_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendan
	msgbox RivalsHouse_2F_Text_ItsRivalsPokeBall, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendan::
	delay 10
	addobject LOCALID_RIVAL
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanEnters
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVAL, Common_Movement_Delay48
	waitmovement 0
	delay 10
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_eq VAR_FACING, DIR_NORTH, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanNorth
	call_if_eq VAR_FACING, DIR_SOUTH, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanSouth
	call_if_eq VAR_FACING, DIR_WEST, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanWest
	call_if_eq VAR_FACING, DIR_EAST, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanEast
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanNorth::
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerNorth
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanNorth
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCNorth
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanSouth::
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerSouth
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanSouth
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCSouth
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanWest::
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerWest
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanWest
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCWest
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanEast::
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerEast
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCEast
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_Movement_BrendanEnters:
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerNorth:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCNorth:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanNorth:
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerSouth:
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCSouth:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanSouth:
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerWest:
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCWest:
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanWest:
	delay_8
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerEast:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCEast:
	walk_up
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

@ Unused, the player is already facing this direction so its unneeded
LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_EventScript_PC::
	lockall
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_2F_EventScript_CheckPlayersPC
	goto_if_eq VAR_RESULT, FEMALE, LittlerootTown_BrendansHouse_2F_EventScript_CheckRivalsPC
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckPlayersPC::
	setvar VAR_0x8004, PC_LOCATION_BRENDANS_HOUSE
	special DoPCTurnOnEffect
	playse SE_PC_ON
	msgbox gText_PlayerHouseBootPC, MSGBOX_DEFAULT
	special BedroomPC
	waitstate
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_TurnOffPlayerPC::
	setvar VAR_0x8004, PC_LOCATION_BRENDANS_HOUSE
	playse SE_PC_OFF
	special DoPCTurnOffEffect
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckRivalsPC::
	msgbox gText_PokemonTrainerSchoolEmail, MSGBOX_DEFAULT
	releaseall
	end

PlayersHouse_2F_EventScript_Notebook::
	msgbox PlayersHouse_2F_Text_Notebook, MSGBOX_SIGN
	call_if_set FLAG_CHALLENGES_LIMITED_ENCOUNTERS, PlayersHouse_2F_EventScript_NotebookLimitedEncounters
	call_if_set FLAG_CHALLENGES_SPECIES_CLAUSE, PlayersHouse_2F_EventScript_NotebookSpeciesClause
	call_if_set FLAG_CHALLENGES_PERMA_DEATH, PlayersHouse_2F_EventScript_NotebookPermaDeath
	call_if_set FLAG_CHALLENGES_FORCE_SET_MODE, PlayersHouse_2F_EventScript_NotebookForceSetMode
	call_if_set FLAG_CHALLENGES_INFINITE_CANDY, PlayersHouse_2F_EventScript_NotebookInfiniteCandy
	call_if_set FLAG_CHALLENGES_REPELLANT, PlayersHouse_2F_EventScript_NotebookRepellant
	special bufferChallengeInfoForNotebook
	call PlayersHouse_2F_EventScript_NotebookShinyOdds
	call PlayersHouse_2F_EventScript_NotebookStartingMoney
	call_if_set FLAG_CHALLENGES_LEVEL_CAP, PlayersHouse_2F_EventScript_NotebookLevelCap
	call_if_eq VAR_ACTIVE_GAUNTLET, 0, PlayersHouse_2F_EventScript_NotebookGauntletMode
	call_if_set FLAG_CHALLENGES_NO_BATTLE_ITEMS, PlayersHouse_2F_EventScript_NotebookNoBattleItems
	call PlayersHouse_2F_EventScript_NotebookXPMultiplier
	special bufferStartersForNotebook;
	call PlayersHouse_2F_EventScript_NotebookCustomStarter
	call_if_set FLAG_CHALLENGES_STARTERAFFECTSRIVAL, PlayersHouse_2F_EventScript_NotebookStarterAffectsRival
	end

PlayersHouse_2F_EventScript_NotebookLevelCap::
	msgbox PlayersHouse_2F_Text_NotebookLevelCap, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookPermaDeath::
	msgbox PlayersHouse_2F_Text_NotebookPermaDeath, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookLimitedEncounters::
	msgbox PlayersHouse_2F_Text_NotebookLimitedEncounters, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookSpeciesClause::
	msgbox PlayersHouse_2F_Text_NotebookSpeciesClause, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookNoBattleItems::
	msgbox PlayersHouse_2F_Text_NotebookNoBattleItems, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookForceSetMode::
	msgbox PlayersHouse_2F_Text_NotebookForceSetMode, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookGauntletMode::
	msgbox PlayersHouse_2F_Text_NotebookGauntletMode, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookStarterAffectsRival::
	msgbox PlayersHouse_2F_Text_NotebookStarterAffectsRival, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookCustomStarter::
	msgbox PlayersHouse_2F_Text_NotebookCustomStarter, MSGBOX_SIGN
	return
	
PlayersHouse_2F_EventScript_NotebookXPMultiplier::
	msgbox PlayersHouse_2F_Text_NotebookXPMultiplier, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookShinyOdds::
	msgbox PlayersHouse_2F_Text_NotebookShinyOdds, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookStartingMoney::
	msgbox PlayersHouse_2F_Text_NotebookStartingMoney, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookInfiniteCandy::
	msgbox PlayersHouse_2F_Text_NotebookInfiniteCandy, MSGBOX_SIGN
	return

PlayersHouse_2F_EventScript_NotebookRepellant::
	msgbox PlayersHouse_2F_Text_NotebookRepellant, MSGBOX_SIGN
	return
	
PlayersHouse_2F_EventScript_GameCube::
	msgbox PlayersHouse_2F_Text_ItsAGameCube, MSGBOX_SIGN
	end

PlayersHouse_2F_Text_ClockIsStopped:
	.string "The clock is stopped…\p"
	.string "Better set it and start it!$"

PlayersHouse_2F_Text_HowDoYouLikeYourRoom:
	.string "Mom: {Player}, how do you like your\n"
	.string "new room?\p"
	.string "Good! Everything's put away neatly!\p"
	.string "They finished moving everything in\n"
	.string "downstairs, too.\p"
	.string "Pokémon movers are so convenient!\p"
	.string "Oh, you should make sure that\n"
	.string "everything's all there on your desk.$"

PlayersHouse_2F_Text_Notebook:
	.string "{Player} flipped open the notebook.\p"
	.string "Challenges active:$"
	
PlayersHouse_2F_Text_NotebookLevelCap:
	.string "Level Cap: Caps your level to\n"
	.string "the next Gym Leader/E4 member.$"
	
PlayersHouse_2F_Text_NotebookPermaDeath:
	.string "Permanent Death: A fainted Poke-\n"
	.string "mon can not be revived in any way.$"
	
PlayersHouse_2F_Text_NotebookLimitedEncounters:
	.string "Limited Encounters: You can only\n"
	.string "catch the first encounter in each\p"
	.string "Location.$"
	
PlayersHouse_2F_Text_NotebookSpeciesClause:
	.string "Species Clause: Duplicates do not\n"
	.string "count against the encounter limit.$"
	
PlayersHouse_2F_Text_NotebookNoBattleItems:
	.string "No Battle Items: You can not use\n"
	.string "items in battle. Held items are\p"
	.string "are allowed, and the enemies will\n"
	.string "still use items.$"
	
PlayersHouse_2F_Text_NotebookForceSetMode:
	.string "Force Set Mode: Every battle is\n"
	.string "in set mode.$"
	
PlayersHouse_2F_Text_NotebookCustomStarter:
	.string "Starter Choices: Your starter\n"
	.string "choices are:\p"
	.string "{Str_Var_1}, {Str_Var_2} and \n"
	.string "{Str_Var_3}.$"
	
PlayersHouse_2F_Text_NotebookXPMultiplier:
	.string "XP Multiplier set to {Str_Var_1}$"
	
PlayersHouse_2F_Text_NotebookInfiniteCandy:
	.string "Infinite Candy: You started the\n"
	.string "game with a Key Item that acts\p"
	.string "as an infinite Rare Candy.$"
	
PlayersHouse_2F_Text_NotebookRepellant:
	.string "Repellant: You started the game\n"
	.string "with a Key Item that acts as an\p"
	.string "infinite Repel.$"

PlayersHouse_2F_Text_NotebookShinyOdds::
	.string "The shiny odds are {Str_Var_2}.$"

PlayersHouse_2F_Text_NotebookStartingMoney::
	.string "You started with a small loan of\n"
	.string "{Str_Var_3}.$"

PlayersHouse_2F_Text_NotebookStarterAffectsRival::
	.string "Your custom starter choices affect\n"
	.string "the rival's team.$"

PlayersHouse_2F_Text_NotebookGauntletMode::
	.string "The game is in gauntlet mode.$"

Common_Text_LookCloserAtMap:
	.string "{Player} took a closer look at the\n"
	.string "Hoenn region map.$"

PlayersHouse_2F_Text_ItsAGameCube:
	.string "It's a Nintendo GameCube.\p"
	.string "A Game Boy Advance is connected to\n"
	.string "serve as the Controller.$"
