Island_Harbors_EventScript_GoToLilycove::
	call Island_Harbors_EventScript_BoardFerry
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end
	
	
	
	
	
	
	
	
	
Island_Harbors_EventScript_BoardFerry::
	msgbox Island_Harbors_Text_PleaseBoard, MSGBOX_DEFAULT
	closemessage
	hideobjectat 2, MAP_ISLAND1_HARBOR
	hideobjectat OBJ_EVENT_ID_PLAYER, 0
	switch VAR_0x8000
	case 1, Island_Harbors_EventScript_HideSailorPort1
	@note: 1 is always the LOCAL_ID of the ship in every Island Harbor
	setvar VAR_0x8004, 1
	call Common_EventScript_FerryDepart
	return
	
Island_Harbors_EventScript_HideSailorPort1:
	@note: 2 is always the LOCAL_ID of the sailor in every Island Harbor
	hideobjectat 2, MAP_ISLAND1_HARBOR
	return
	
Island_Harbors_Text_PleaseBoard:
	.string "Alright, off we go!$"
