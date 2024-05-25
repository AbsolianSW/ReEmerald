.set LOCALID_NURSE, 1

Island2_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Island2_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0
	
Island2_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_ISLAND2_MAIN
	end

Island2_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

Island2_PokemonCenter_1F_EventScript_GBAKid::
	lock
	faceplayer
	msgbox Island2_PokemonCenter_1F_Text_HaveYouVisitedGameCorner
	release
	end

Island2_PokemonCenter_1F_EventScript_BugCatcher::
	lock
	faceplayer
	msgbox Island2_PokemonCenter_1F_Text_OldLadyLivesOutOnCape
	release
	end


Island2_PokemonCenter_1F_Text_HaveYouVisitedGameCorner::
    .string "Have you visited the GAME CORNER\n"
    .string "already?\p"
    .string "It's no good to go alone.\n"
    .string "You have to take a friend along.$"

Island2_PokemonCenter_1F_Text_OldLadyLivesOutOnCape::
    .string "Not many people live on\n"
    .string "TWO ISLAND.\p"
    .string "There is this old lady who lives\n"
    .string "out on the cape.\p"
    .string "She's been there since I don't know\n"
    .string "when.$"
