
EventScript_UseSurf::
	checkpartymove MOVE_SURF
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_EndUseSurf
	copyvar VAR_0x8007, VAR_RESULT
	bufferpartymonnick Str_Var_1, VAR_0x8007
	setfieldeffectargument 0, VAR_0x8007
	lockall
	msgbox gText_WantToUseSurf, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ReleaseUseSurf
	msgbox gText_PlayerUsedSurf, MSGBOX_DEFAULT
	IsFollowerFieldMoveUser VAR_0x8004
	setfieldeffectargument 3, VAR_0x8004 @ different animation if so
	call_if_eq VAR_0x8004, TRUE, EventScript_FollowerSurfMove
	callnative RemoveFollowingPokemon
	call_if_eq VAR_0x8004, TRUE, EventScript_FollowerSwapSurf2
	call_if_eq VAR_0x8004, TRUE, EventScript_FollowerSwapSurf3
	setfieldeffectargument 0, VAR_0x8007@this gets modified somewhere for some reason, so we have to set it again
	dofieldeffect FLDEFF_USE_SURF
	callnative UpdateFollowingPokemon
EventScript_ReleaseUseSurf::
	releaseall
EventScript_EndUseSurf::
	end

EventScript_FollowerSurfMove:
	callfunc ScrFunc_GetDirectionToFace
	.2byte VAR_0x8005
	.byte OBJ_EVENT_ID_FOLLOWER
	.byte OBJ_EVENT_ID_PLAYER
	specialvar VAR_0x8006, GetPlayerFacingDirection
	goto_if_eq VAR_0x8005, DIR_NONE, EventScript_FollowerSurfMoveEnd
	@ Swap follower and player
	call EventScript_FollowerSwapSurf
	@ Face follower in direction and jump
	switch VAR_0x8006
	case DIR_NORTH, EventScript_FollowerJumpInWaterNorth
	case DIR_EAST, EventScript_FollowerJumpInWaterEast
	case DIR_SOUTH, EventScript_FollowerJumpInWaterSouth
	case DIR_WEST, EventScript_FollowerJumpInWaterWest
EventScript_FollowerSurfMoveEnd:
	return

EventScript_FollowerSwapSurf:
	switch VAR_0x8005
	case DIR_NORTH, EventScript_FollowerMoveNorthSurf
	case DIR_EAST, EventScript_FollowerMoveEastSurf
	case DIR_SOUTH, EventScript_FollowerMoveSouthSurf
	case DIR_WEST, EventScript_FollowerMoveWestSurf
	return

EventScript_FollowerSwapSurf2:
	switch VAR_0x8005
	case DIR_NORTH, EventScript_FollowerMoveNorthSurf2
	case DIR_EAST, EventScript_FollowerMoveEastSurf2
	case DIR_SOUTH, EventScript_FollowerMoveSouthSurf2
	case DIR_WEST, EventScript_FollowerMoveWestSurf2

EventScript_FollowerSwapSurf3:
	switch VAR_0x8006
	case DIR_NORTH, EventScript_Player_Face_North
	case DIR_EAST, EventScript_Player_Face_East
	case DIR_SOUTH, EventScript_Player_Face_South
	case DIR_WEST, EventScript_Player_Face_West
	return

EventScript_FollowerMoveNorthSurf:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkDown
	waitmovement 0
	return

EventScript_FollowerMoveNorthSurf2:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkUp
	waitmovement 0
	return

EventScript_FollowerMoveEastSurf:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkLeft
	waitmovement 0
	return

EventScript_FollowerMoveEastSurf2:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkRight
	waitmovement 0
	return

EventScript_FollowerMoveSouthSurf:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkUp
	waitmovement 0
	return

EventScript_FollowerMoveSouthSurf2:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkDown
	waitmovement 0
	return

EventScript_FollowerMoveWestSurf:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkRight
	waitmovement 0
	return

EventScript_FollowerMoveWestSurf2:
	applymovement OBJ_EVENT_ID_PLAYER, Movement_WalkLeft
	waitmovement 0
	return

EventScript_Player_Face_North:
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	return

EventScript_Player_Face_East:
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	return

EventScript_Player_Face_South:
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement 0
	return

EventScript_Player_Face_West:
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	return

EventScript_FollowerJumpInWaterNorth:
	applymovement OBJ_EVENT_ID_FOLLOWER, Movement_JumpInWaterUp
	waitmovement 0
	return

EventScript_FollowerJumpInWaterEast:
	applymovement OBJ_EVENT_ID_FOLLOWER, Movement_JumpInWaterRight
	waitmovement 0
	return

EventScript_FollowerJumpInWaterSouth:
	applymovement OBJ_EVENT_ID_FOLLOWER, Movement_JumpInWaterDown
	waitmovement 0
	return

EventScript_FollowerJumpInWaterWest:
	applymovement OBJ_EVENT_ID_FOLLOWER, Movement_JumpInWaterLeft
	waitmovement 0
	return

Movement_JumpInWaterUp:
	jump_up
	step_end

Movement_JumpInWaterRight:
	jump_right
	step_end

Movement_JumpInWaterDown:
	jump_down
	step_end

Movement_JumpInWaterLeft:
	jump_left
	step_end
