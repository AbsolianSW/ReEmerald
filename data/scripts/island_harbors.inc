.set LOCALID_SHIP, 1
.set LOCALID_SAILOR, 2

Island_Harbors_EventScript_GoToLilycove::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland1::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND1_HARBOR, 8, 4
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland2::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND2_HARBOR, 8, 4
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland3::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND3_HARBOR, 8, 4
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland4::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND4_HARBOR, 8, 4
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland5::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND5_HARBOR, 8, 4
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland6::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND6_HARBOR, 8, 4
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland7::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND7_HARBOR, 8, 4
	waitstate
	release
	end
	

Island_Harbors_EventScript_BoardFerry1::
	msgbox Island_Harbors_Text_PleaseBoard, MSGBOX_DEFAULT
	closemessage
	hideobjectat OBJ_EVENT_ID_PLAYER, 0
	removeobject LOCALID_SAILOR
	applymovement OBJ_EVENT_ID_FOLLOWER, Island_Harbors_Movement_1Down
	waitmovement 0
	hideobjectat OBJ_EVENT_ID_FOLLOWER, 0
	setvar VAR_0x8004, LOCALID_SHIP
	call Common_EventScript_FerryDepart
	return

Island_Harbors_Movement_1Down::
	walk_down
	step_end
	
Island_Harbors_Text_PleaseBoard:
	.string "Alright, off we go!$"

Island_Harbors_Text_Oi_Chap::
	.string "Oi, chap! Were're we going today?$"
