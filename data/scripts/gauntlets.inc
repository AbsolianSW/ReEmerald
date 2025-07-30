EventScript_GauntletTriggerRight::
    special BufferMidGauntletInfo
    goto_if_eq VAR_RESULT, 1, EventScript_GauntletEnd
    msgbox Gauntlets_Text_GauntletNotFinished, MSGBOX_DEFAULT
    closemessage
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveRight
    waitmovement 0
    release
    end

EventScript_GauntletTriggerLeft::
    special BufferMidGauntletInfo
    goto_if_eq VAR_RESULT, 1, EventScript_GauntletEnd
    msgbox Gauntlets_Text_GauntletNotFinished, MSGBOX_DEFAULT
    closemessage
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveLeft
    waitmovement 0
    release
    end

EventScript_GauntletTriggerUp::
    special BufferMidGauntletInfo
    goto_if_eq VAR_RESULT, 1, EventScript_GauntletEnd
    msgbox Gauntlets_Text_GauntletNotFinished, MSGBOX_DEFAULT
    closemessage
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveUp
    waitmovement 0
    release
    end

EventScript_GauntletTriggerDown::
    special BufferMidGauntletInfo
    goto_if_eq VAR_RESULT, 1, EventScript_GauntletEnd
    msgbox Gauntlets_Text_GauntletNotFinished, MSGBOX_DEFAULT
    closemessage
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveDown
    waitmovement 0
    release
    end

EventScript_GauntletEnd::
    msgbox Gauntlets_Text_GauntletFinished, MSGBOX_DEFAULT
    setvar VAR_ACTIVE_GAUNTLET, 0
    release
    end

EventScript_GauntletBegins::
    goto_if_eq VAR_0x8005, 0, EventScript_GauntletNotReady
    goto_if_ne VAR_0x8005, 255, EventScript_GauntletBeginsWarp
    special BufferGauntletInfo
	msgbox Gauntlets_Text_GauntletBegins, MSGBOX_YESNO
    closemessage
	goto_if_eq VAR_RESULT, NO, EventScript_GauntletNo
    setvar VAR_ACTIVE_GAUNTLET, 2
	special SaveActiveGauntlet
    goto_if_eq VAR_0x8004, DIR_EAST, EventScript_GauntletMoveLeft
    goto_if_eq VAR_0x8004, DIR_WEST, EventScript_GauntletMoveRight
    goto_if_eq VAR_0x8004, DIR_NORTH, EventScript_GauntletMoveDown
    goto_if_eq VAR_0x8004, DIR_SOUTH, EventScript_GauntletMoveUp
    end

EventScript_GauntletBeginsWarp::
    special BufferGauntletInfo
	msgbox Gauntlets_Text_GauntletBegins, MSGBOX_YESNO
    closemessage
	goto_if_eq VAR_RESULT, NO, EventScript_GauntletWarpNo
    setvar VAR_ACTIVE_GAUNTLET, 2
	special SaveActiveGauntlet
    goto_if_eq VAR_0x8004, DIR_EAST, EventScript_GauntletMoveLeft
    goto_if_eq VAR_0x8004, DIR_WEST, EventScript_GauntletMoveRight
    goto_if_eq VAR_0x8004, DIR_NORTH, EventScript_GauntletMoveDown
    goto_if_eq VAR_0x8004, DIR_SOUTH, EventScript_GauntletMoveUp
    msgbox Gauntlets_Text_GauntletFinished, MSGBOX_DEFAULT
    end

EventScript_GauntletWarpNo::
    setvar VAR_ACTIVE_GAUNTLET, 0
    goto_if_eq VAR_0x8005, 1, EventScript_GauntletWarpRustboro
    goto_if_eq VAR_0x8005, 2, EventScript_GauntletWarpDewford
    goto_if_eq VAR_0x8005, 3, EventScript_GauntletWarpBeach
    end

EventScript_GauntletWarpRustboro:
    warp MAP_RUSTBORO_CITY, 0

EventScript_GauntletWarpDewford:
    warp MAP_DEWFORD_TOWN, 2

EventScript_GauntletWarpBeach:
    setvar VAR_FACING, DIR_SOUTH
    call Route109_EventScript_StartDepartForDewford

EventScript_GauntletNotReady::
    msgbox Gauntlets_Text_GauntletNotReady, MSGBOX_DEFAULT
    closemessage
EventScript_GauntletNo::
    setvar VAR_ACTIVE_GAUNTLET, 0
    special BufferGauntletInfo
    goto_if_eq VAR_0x8004, DIR_EAST, EventScript_GauntletMoveRight
    goto_if_eq VAR_0x8004, DIR_WEST, EventScript_GauntletMoveLeft
    goto_if_eq VAR_0x8004, DIR_NORTH, EventScript_GauntletMoveUp
    goto_if_eq VAR_0x8004, DIR_SOUTH, EventScript_GauntletMoveDown
    end

test:
    .string "test$"

EventScript_GauntletMoveRight::
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveRight
    waitmovement 0
    end

EventScript_GauntletMoveLeft::
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveLeft
    waitmovement 0
    end

EventScript_GauntletMoveUp::
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveUp
    waitmovement 0
    end

EventScript_GauntletMoveDown::
    applymovement OBJ_EVENT_ID_PLAYER, Gauntlets_Movement_MoveDown
    waitmovement 0
    end

EventScript_CheckGauntletBossfight::
    lock
    special CheckGauntletReadyForBoss
    goto_if_eq VAR_RESULT, 1, EventScript_CheckGauntletBossfightReturn
    msgbox Gauntlets_Text_GauntletBossNotReady, MSGBOX_DEFAULT
    release
    end

EventScript_CheckGauntletBossfightReturn::
    special HealPlayerParty
    return

Gauntlets_Movement_MoveRight::
    walk_right
    step_end

Gauntlets_Movement_MoveLeft::
    walk_left
    step_end

Gauntlets_Movement_MoveUp::
    walk_up
    step_end

Gauntlets_Movement_MoveDown::
    walk_down
    step_end

Gauntlets_Text_GauntletBegins::
    .string "A {Str_Var_1}gauntlet with {Str_Var_2} trainer(s) and {Str_Var_3}\n"
    .string "heal(s) is about to begin. Are you ready?$"

Gauntlets_Text_GauntletNotFinished::
    .string "You have not yet completed the {Str_Var_1}\n"
    .string "gauntlet, there are {Str_Var_2} trainer(s) left!$"

Gauntlets_Text_GauntletFinished::
    .string "You have completed the gauntlet. Well\n"
    .string "done, trainer!$"

Gauntlets_Text_GauntletBossNotReady::
    .string "You can't fight the boss yet, there are\n"
    .string "{Str_Var_1} trainers left to fight!$"

Gauntlets_Text_GauntletNotReady::
    .string "You can't fight this gauntlet yet, you\n"
    .string "need to progress elsewhere!$"
