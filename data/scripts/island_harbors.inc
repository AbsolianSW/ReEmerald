Island_Harbors_EventScript_GoToLilycove::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland1::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND1_HARBOR, 8, 6
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland2::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND2_HARBOR, 8, 5
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland3::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND3_HARBOR, 8, 5
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland4::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND4_HARBOR, 8, 5
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland5::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND5_HARBOR, 8, 5
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland6::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND6_HARBOR, 8, 5
	waitstate
	release
	end
	
Island_Harbors_EventScript_GoToIsland7::
	call Island_Harbors_EventScript_BoardFerry1
	warp MAP_ISLAND7_HARBOR, 8, 5
	waitstate
	release
	end
	
Island_Harbors_EventScript_BoardFerry1::
	msgbox Island_Harbors_Text_PleaseBoard, MSGBOX_DEFAULT
	closemessage
	hideobjectat OBJ_EVENT_ID_PLAYER, 0
	switch VAR_0x8001
	case 1, Island_Harbors_EventScript_HideSailorPort1
	case 2, Island_Harbors_EventScript_HideSailorPort2
	case 3, Island_Harbors_EventScript_HideSailorPort3
	case 4, Island_Harbors_EventScript_HideSailorPort4
	case 5, Island_Harbors_EventScript_HideSailorPort5
	case 6, Island_Harbors_EventScript_HideSailorPort6
	case 7, Island_Harbors_EventScript_HideSailorPort7
	end
	
@note: 1 is always the LOCAL_ID of the ship in every Island Harbor
Island_Harbors_EventScript_BoardFerry2::
	setvar VAR_0x8004, 1
	call Common_EventScript_FerryDepart
	return
	
@note: 2 is always the LOCAL_ID of the sailor in every Island Harbor	
Island_Harbors_EventScript_HideSailorPort1:
	closemessage
	hideobjectat 2, MAP_ISLAND1_HARBOR
	goto Island_Harbors_EventScript_BoardFerry2
	return

Island_Harbors_EventScript_HideSailorPort2:
	closemessage
	hideobjectat 2, MAP_ISLAND2_HARBOR
	goto Island_Harbors_EventScript_BoardFerry2
	return
	
Island_Harbors_EventScript_HideSailorPort3:
	closemessage
	hideobjectat 2, MAP_ISLAND3_HARBOR
	goto Island_Harbors_EventScript_BoardFerry2
	return
	
Island_Harbors_EventScript_HideSailorPort4:
	closemessage
	hideobjectat 2, MAP_ISLAND4_HARBOR
	goto Island_Harbors_EventScript_BoardFerry2
	return
	
Island_Harbors_EventScript_HideSailorPort5:
	closemessage
	hideobjectat 2, MAP_ISLAND5_HARBOR
	goto Island_Harbors_EventScript_BoardFerry2
	return
	
Island_Harbors_EventScript_HideSailorPort6:
	closemessage
	hideobjectat 2, MAP_ISLAND6_HARBOR
	goto Island_Harbors_EventScript_BoardFerry2
	return
	
Island_Harbors_EventScript_HideSailorPort7:
	closemessage
	hideobjectat 2, MAP_ISLAND7_HARBOR
	goto Island_Harbors_EventScript_BoardFerry2
	return
	
Island_Harbors_Text_PleaseBoard:
	.string "Alright, off we go!$"


