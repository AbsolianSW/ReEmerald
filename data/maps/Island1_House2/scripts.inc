Island1_House2_MapScripts::
	.byte 0


Island1_House2_EventScript_Lass::
	lock
	faceplayer
	msgbox Island1_House2_EventScript_Lass_Text_0
	release
	return


Island1_House2_EventScript_Lass_Text_0:
	.string "Young people all want to go off to big\n"
	.string "cities. But I want to stay here\l"
	.string "forever!$"
