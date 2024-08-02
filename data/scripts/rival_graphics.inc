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
	setvar VAR_0x8003, 0
	special BufferRivalFollowerSpecies
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_OW_MON
	return 

EventScript_RivalWater::
	setvar VAR_0x8003, 1
	special BufferRivalFollowerSpecies
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_OW_MON
	return 
	
EventScript_RivalFire::
	setvar VAR_0x8003, 2
	special BufferRivalFollowerSpecies
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_OW_MON
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
