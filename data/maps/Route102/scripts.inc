Route102_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route102_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE Route102_OnFrame
	.byte 0

Route102_OnTransition::
	goto_if_eq VAR_ACTIVE_GAUNTLET, 3, Route102_OnTransitionEnd
	goto_if_defeated TRAINER_CALVIN, Route102_OnTransitionEnd
	setvar VAR_ACTIVE_GAUNTLET, 1
Route102_OnTransitionEnd::
	end

Route102_OnFrame::
	map_script_2 VAR_ACTIVE_GAUNTLET, 1 Route102_EventScript_BeginGauntlet
	.2byte 0

Route102_EventScript_BeginGauntlet::
	setvar VAR_GAUNTLET_ID, GAUNTLET_ROUTE102
	setvar VAR_0x8004, DIR_EAST
	goto EventScript_GauntletBegins
	end

Route102_EventScript_LittleBoy::
	msgbox Route102_Text_ImNotVeryTall, MSGBOX_NPC
	end

Route102_EventScript_RouteSignOldale::
	msgbox Route102_Text_RouteSignOldale, MSGBOX_SIGN
	end

Route102_EventScript_RouteSignPetalburg::
	msgbox Route102_Text_RouteSignPetalburg, MSGBOX_SIGN
	end

Route102_EventScript_Boy::
	msgbox Route102_Text_CatchWholeBunchOfPokemon, MSGBOX_NPC
	end

Route102_EventScript_Calvin::
	trainerbattle_single TRAINER_CALVIN, Route102_Text_CalvinIntro, Route102_Text_CalvinDefeated
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_CalvinRematch
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRematch::
	trainerbattle_rematch TRAINER_CALVIN, Route102_Text_CalvinRematchIntro, Route102_Text_CalvinRematchDefeated
	msgbox Route102_Text_CalvinRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Rick::
	trainerbattle_single TRAINER_RICK, Route102_Text_RickIntro, Route102_Text_RickDefeated
	msgbox Route102_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Tiana::
	trainerbattle_single TRAINER_TIANA, Route102_Text_TianaIntro, Route102_Text_TianaDefeated
	msgbox Route102_Text_TianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Allen::
	trainerbattle_single TRAINER_ALLEN, Route102_Text_AllenIntro, Route102_Text_AllenDefeated
	msgbox Route102_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_Text_WatchMeCatchPokemon:
	.string "Wally: {Player}…\n"
	.string "Pokémon hide in tall grass like this,\l"
	.string "don't they?\p"
	.string "Please watch me and see if I can\n"
	.string "catch one properly.\p"
	.string "…Whoa!$"

Route102_Text_WallyIDidIt:
	.string "Wally: I did it… It's my…\n"
	.string "My Pokémon!$"

Route102_Text_LetsGoBack:
	.string "{Player}, thank you!\n"
	.string "Let's go back to the Gym!$"

Route102_Text_ImNotVeryTall:
	.string "I'm…not very tall, so I sink right\n"
	.string "into tall grass.\p"
	.string "The grass goes up my nose and…\n"
	.string "Fwafwafwafwafwa…\p"
	.string "Fwatchoo!$"

Route102_Text_CatchWholeBunchOfPokemon:
	.string "I'm going to catch a whole bunch of\n"
	.string "Pokémon!$"

Route102_Text_RouteSignOldale:
	.string "Route 102\n"
	.string "{Right_Arrow} Oldale Town$"

Route102_Text_RouteSignPetalburg:
	.string "Route 102\n"
	.string "{Left_Arrow} Petalburg City$"
