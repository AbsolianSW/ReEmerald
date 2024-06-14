Common_EventScript_SetupRivalGfxId::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EventScript_SetupRivalGfxIdFemale
	goto_if_eq VAR_RESULT, FEMALE, EventScript_SetupRivalGfxIdMale
	end

EventScript_SetupRivalGfxIdFemale::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL
	return

EventScript_SetupRivalGfxIdMale::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL
	return
	
Common_EventScript_SetupRivalFollowerId::
	switch VAR_STARTER_MON
	case 0, EventScript_RivalFire
	case 1, EventScript_RivalWater
	case 2, EventScript_RivalGrass
	end

EventScript_RivalGrass::
	switch VAR_0x8004
	case 0, EventScript_RivalTreecko
	case 1, EventScript_RivalGrovyle
	case 2, EventScript_RivalSceptile
	return 

EventScript_RivalTreecko::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_TREECKO
	return
	
EventScript_RivalGrovyle::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_GROVYLE
	return

EventScript_RivalSceptile::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_SCEPTILE
	return
	
EventScript_RivalFire::
	switch VAR_0x8004
	case 0, EventScript_RivalTorchic
	case 1, EventScript_RivalCombusken
	case 2, EventScript_RivalBlaziken
	return 

EventScript_RivalTorchic::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_TORCHIC
	return
	
EventScript_RivalCombusken::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_COMBUSKEN
	return

EventScript_RivalBlaziken::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_BLAZIKEN
	return
	
EventScript_RivalWater::
	switch VAR_0x8004
	case 0, EventScript_RivalMudkip
	case 1, EventScript_RivalMarshtomp
	case 2, EventScript_RivalSwampert
	return 

EventScript_RivalMudkip::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_MUDKIP
	return
	
EventScript_RivalMarshtomp::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_MARSHTOMP
	return

EventScript_RivalSwampert::
	setvar VAR_OBJ_GFX_ID_1,OBJ_EVENT_GFX_SWAMPERT
	return


Common_EventScript_SetupRivalOnBikeGfxId::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EventScript_SetupRivalOnBikeGfxIdFemale
	goto_if_eq VAR_RESULT, FEMALE, EventScript_SetupRivalOnBikeGfxIdMale
	end

EventScript_SetupRivalOnBikeGfxIdFemale::
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_RIVAL_MAY_MACH_BIKE
	return

EventScript_SetupRivalOnBikeGfxIdMale::
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_RIVAL_BRENDAN_MACH_BIKE
	return

@ Unused
Common_EventScript_SetupRivalGfxIdSameGender::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EventScript_SetupRivalGfxIdMale2
	goto_if_eq VAR_RESULT, FEMALE, EventScript_SetupRivalGfxIdFemale2
	end

EventScript_SetupRivalGfxIdMale2::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL
	return

EventScript_SetupRivalGfxIdFemale2::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL
	return
