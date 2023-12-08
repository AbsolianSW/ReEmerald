 @why in fucks name are these not in order btw
 .set ID_REPEL, 86
 .set ID_SUPER_REPEL, 83
 .set ID_MAX_REPEL, 84
 
 EventScript_RepelWoreOff::
	lockall
	checkitem VAR_REPEL_LAST_USED, 1
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RepelWoreOff_NoMoreRepels
	msgbox Text_RepelWoreOff_UseAnother, MSGBOX_YESNO
	compare VAR_RESULT, 0
	goto_if_eq EventScript_RepelWoreOff_ChooseNo
	call_if_eq VAR_REPEL_LAST_USED, ID_REPEL, EventScript_SetRepelStepsNormalRepel
	call_if_eq VAR_REPEL_LAST_USED, ID_SUPER_REPEL, EventScript_SetRepelStepsSuperRepel
	call_if_eq VAR_REPEL_LAST_USED, ID_MAX_REPEL, EventScript_SetRepelStepsMaxRepel
	bufferitemname 1, VAR_REPEL_LAST_USED
	removeitem VAR_REPEL_LAST_USED, 1
	playse SE_REPEL
	msgbox gText_PlayerUsedVar2, MSGBOX_SIGN
	goto EventScript_RepelWoreOff_End

EventScript_RepelWoreOff_ChooseNo:
	closemessage
	goto EventScript_RepelWoreOff_End

EventScript_RepelWoreOff_NoMoreRepels:
	msgbox Text_RepelWoreOff, MSGBOX_SIGN

EventScript_RepelWoreOff_End:
	releaseall
	end

EventScript_SetRepelStepsNormalRepel:
	setvar VAR_REPEL_STEP_COUNT, 100
	return

EventScript_SetRepelStepsSuperRepel:
	setvar VAR_REPEL_STEP_COUNT, 200
	return

EventScript_SetRepelStepsMaxRepel:
	setvar VAR_REPEL_STEP_COUNT, 250
	return

Text_RepelWoreOff:
	.string "REPEL's effect wore off…$"

Text_RepelWoreOff_UseAnother:
	.string "REPEL's effect wore off…\n"
	.string "Use another?$"
