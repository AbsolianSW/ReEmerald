Island1_House1_MapScripts::
	.byte 0


Island1_House1_EventScript_OldMan::
	lock
	faceplayer
	msgbox Island1_House1_EventScript_OldMan_Text_0
	release
	return


Island1_House1_EventScript_OldWoman::
	lock
	faceplayer
	msgbox Island1_House1_EventScript_OldWoman_Text_0
	release
	return


Island1_House1_EventScript_OldMan_Text_0:
	.string "On sunny days, I step out and gaze upon\n"
	.string "the volcano. That's my daily routine.$"

Island1_House1_EventScript_OldWoman_Text_0:
	.string "Let me think… When was the last time Mt.\n"
	.string "Ember erupted?$"
