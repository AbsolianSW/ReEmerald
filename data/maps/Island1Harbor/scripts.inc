Island1Harbor_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Island1Harbor_OnFrameTable
	.byte 0
	
Island1Harbor_OnFrameTable::
	map_script_2 VAR_ONE_ISLAND_STATE, 0, Island1Harbor_EventScript_EnterIsland1HarborFirstTime
	.2byte 0
	
Island1Harbor_EventScript_EnterIsland1HarborFirstTime::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, Island1Harbor_Movement_PlayerExitHarbor
	waitmovement 0
	warp MAP_ISLAND1MAIN, 12, 18
	waitstate
	releaseall
	end
	
Island1Harbor_Movement_PlayerExitHarbor:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Island1Harbor_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, 1
	special ScriptMenu_CreateSeviiFerryMultichoice
	waitstate
	special GetSeviiFerrySelection
	setvar VAR_0x8000, 1
	switch VAR_RESULT
	case 0, Island_Harbors_EventScript_GoToLilycove
	end
