Island3_Port_MapScripts::
	.byte 0


Island3_Port_Woman::
	lock
	faceplayer
	compare VAR_THREE_ISLAND_STATE, 2
	goto_if_ge Island3_Port_Woman_2
	msgbox Island3_Port_Woman_Text_1
	release
	end

Island3_Port_Woman_2:
	msgbox Island3_Port_Woman_Text_0
	release
	end


Island3_Port_Aqua1::
	msgbox Island3_Port_Aqua1_Text_0
	end


Island3_Port_Aqua2::
	lock
	msgbox Island3_Port_Aqua2_Text_0
	release
	end


Island3_Port_AquaBlockade::
	lock
	faceplayer
	msgbox Island3_Port_AquaBlockade_Text_0
	applymovement 4, Common_Movement_FaceUp
	applymovement 5, Common_Movement_FaceUp
	applymovement 6, Common_Movement_FaceUp
	waitmovement 0
	release
	end


Island3_Port_Woman_Text_0:
	.string "Those Goons ran off, and LOSTELLE was\n"
	.string "found safe. Everything turned out for\l"
	.string "the best!$"

Island3_Port_Woman_Text_1:
	.string "Aiyeeh!\p"
	.string "Wh-who are you?!\n"
	.string "I… I'll call the police!$"

Island3_Port_Aqua1_Text_0:
	.string "We're TEAM AQUA! I'm just a lowly grunt,\n"
	.string "so I don't get to go on cool missions\l"
	.string "yet...$"

Island3_Port_Aqua2_Text_0:
	.string "Hey, hey, lady! Fork over some spending\n"
	.string "money!$"

Island3_Port_AquaBlockade_Text_0:
	.string "Stop right there! Official TEAM AQUA\n"
	.string "Business$"
