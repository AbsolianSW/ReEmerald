EverGrandeCity_HallOfFame_EventScript_SetGameClearFlags::
	special SetChampionSaveWarp
	setflag FLAG_IS_CHAMPION
	call EverGrandeCity_HallOfFame_EventScript_ResetDefeatedEventLegendaries
	call_if_eq VAR_FOSSIL_MANIAC_STATE, 0, EverGrandeCity_HallOfFame_EventScript_SetDesertUnderpassCommentReady
	clearflag FLAG_HIDE_LILYCOVE_MOTEL_GAME_DESIGNERS
	call EverGrandeCity_HallOfFame_EventScript_ResetEliteFour
	setflag FLAG_HIDE_SLATEPORT_CITY_STERNS_SHIPYARD_MR_BRINEY
	clearflag FLAG_HIDE_SS_TIDAL_CORRIDOR_MR_BRINEY
	clearflag FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_INVISIBLE_NINJA_BOY
	setvar VAR_STEVENS_HOUSE_STATE, 2
	setflag FLAG_HIDE_VICTORY_ROAD_ENTRANCE_WALLY
	clearflag FLAG_HIDE_VICTORY_ROAD_EXIT_WALLY
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_SS_TIDAL
	clearflag FLAG_HIDE_LILYCOVE_HARBOR_SSTIDAL
	setflag FLAG_HIDE_SAFARI_ZONE_SOUTH_CONSTRUCTION_WORKERS
	clearflag FLAG_HIDE_SAFARI_ZONE_SOUTH_EAST_EXPANSION
	setflag FLAG_HIDE_LILYCOVE_CITY_RIVAL
	special UpdateTrainerFanClubGameClear
	call_if_unset FLAG_RECEIVED_SS_TICKET, EverGrandeCity_HallOfFame_EventScript_ReadyReceiveSSTicketEvent
	call_if_unset FLAG_RECEIVED_JIRACHI, EverGrandeCity_HallOfFame_EventScript_ShowStevensHouseJirachi
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_BEDROOM
	call_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 0, EverGrandeCity_HallOfFame_EventScript_ReadyDexUpgradeEvent
	return

EverGrandeCity_HallOfFame_EventScript_ResetDefeatedEventLegendaries::
	clearflag FLAG_DEFEATED_MEW
	clearflag FLAG_DEFEATED_LATIAS_OR_LATIOS
	clearflag FLAG_DEFEATED_DEOXYS
	clearflag FLAG_DEFEATED_LUGIA
	clearflag FLAG_DEFEATED_HO_OH
	return

EverGrandeCity_HallOfFame_EventScript_SetDesertUnderpassCommentReady::
	setvar VAR_FOSSIL_MANIAC_STATE, 1
	return

EverGrandeCity_HallOfFame_EventScript_ShowStevensHouseJirachi::
	clearflag FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_JIRACHI_POKEBALL
	return

EverGrandeCity_HallOfFame_EventScript_ReadyReceiveSSTicketEvent::
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 3
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 3
	clearflag FLAG_HIDE_PLAYERS_HOUSE_DAD
	return

EverGrandeCity_HallOfFame_EventScript_ReadyDexUpgradeEvent::
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1
	return