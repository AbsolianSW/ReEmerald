Island3_Harbor_MapScripts::
	.byte 0

Island3_Harbor_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, 3
	special ScriptMenu_CreateSeviiFerryMultichoice
	waitstate
	setvar VAR_0x8001, 3
	switch VAR_RESULT
	case 0, Island_Harbors_EventScript_GoToLilycove
	case 1, Island_Harbors_EventScript_GoToIsland1
	case 2, Island_Harbors_EventScript_GoToIsland2
	case 3, Island_Harbors_EventScript_GoToIsland4
	case 4, Island_Harbors_EventScript_GoToIsland5
	case 5, Island_Harbors_EventScript_GoToIsland6
	case 6, Island_Harbors_EventScript_GoToIsland7
	end
