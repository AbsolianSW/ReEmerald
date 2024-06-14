Common_EventScript_TalkToFollower::
	pokemonfaceplayer
	lock
	bufferfollowerspecies
	msgbox EventScript_Follower_Text, MSGBOX_DEFAULT
	release
	end
	
EventScript_Follower_Text:
	.string "{STR_VAR_1}$"
