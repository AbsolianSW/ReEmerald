PacifidlogTown_House2_MapScripts::
	.byte 0

PacifidlogTown_House2_EventScript_FanClubYoungerBrother::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_RECEIVED_TM_FRUSTRATION, PacifidlogTown_House2_EventScript_AlreadyVicious
	call_if_set FLAG_MET_FANCLUB_YOUNGER_BROTHER, PacifidlogTown_House2_EventScript_MonAssessment
	call_if_unset FLAG_MET_FANCLUB_YOUNGER_BROTHER, PacifidlogTown_House2_EventScript_FirstMonAssessment
	setflag FLAG_MET_FANCLUB_YOUNGER_BROTHER
	specialvar VAR_RESULT, GetLeadMonFriendshipScore
	goto_if_ge VAR_RESULT, FRIENDSHIP_50_TO_99, PacifidlogTown_House2_EventScript_PutInEffort
	goto PacifidlogTown_House2_EventScript_GiveFrustration
	end

PacifidlogTown_House2_EventScript_MonAssessment::
	msgbox PacifidlogTown_House2_Text_AhYourPokemon, MSGBOX_DEFAULT
	return

PacifidlogTown_House2_EventScript_FirstMonAssessment::
	msgbox PacifidlogTown_House2_Text_ChairmansYoungerBrotherOnVacation, MSGBOX_DEFAULT
	msgbox PacifidlogTown_House2_Text_AhYourPokemon, MSGBOX_DEFAULT
	return

PacifidlogTown_House2_EventScript_PutInEffort::
	msgbox PacifidlogTown_House2_Text_PutInSomeMoreEffort, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House2_EventScript_GiveFrustration::
	msgbox PacifidlogTown_House2_Text_ViciousPokemonTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_FRUSTRATION
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_FRUSTRATION
	msgbox PacifidlogTown_House2_Text_ExplainFrustration, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House2_EventScript_AlreadyVicious::
	msgbox PacifidlogTown_House2_Text_AlreadyVicious, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House2_EventScript_HappyAzurill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZURILL, CRY_MODE_NORMAL
	msgbox PacifidlogTown_House2_Text_Rurii, MSGBOX_DEFAULT
	waitmoncry
	msgbox PacifidlogTown_House2_Text_VeryFriendlyWithTrainer, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House2_EventScript_UnhappyAzurill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZURILL, CRY_MODE_ENCOUNTER
	msgbox PacifidlogTown_House2_Text_Rururi, MSGBOX_DEFAULT
	waitmoncry
	msgbox PacifidlogTown_House2_Text_DoesntLikeTrainerVeryMuch, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House2_Text_ChairmansYoungerBrotherOnVacation:
	.string "Er-hem!\p"
	.string "I am the Pokémon Fan Club's most\n"
	.string "important person, the Chairman!\p"
	.string "That's all my brother ever says. He's \p"
	.string "the golden child, it's so frustrating.$"

PacifidlogTown_House2_Text_AhYourPokemon:
	.string "Ah!\n"
	.string "Your Pokémon…$"

PacifidlogTown_House2_Text_PutInSomeMoreEffort:
	.string "Hmm…\n"
	.string "It's not bad, but it's also not good.\p"
	.string "I just don't see enough frustration$"

PacifidlogTown_House2_Text_ViciousPokemonTakeThis:
	.string "It has a vicious look to it.\p"
	.string "A frightening Pokémon like that\n"
	.string "deserves a TM like this.$"

PacifidlogTown_House2_Text_ExplainFrustration:
	.string "If it doesn't like you, Frustration's\n"
	.string "power goes up.$"

PacifidlogTown_House2_Text_AlreadyVicious:
	.string "Your Pokemon are so frustrated, it's \n"
	.string "great to see.$"

PacifidlogTown_House2_Text_Rurii:
	.string "Azurill: Rurii.$"

PacifidlogTown_House2_Text_VeryFriendlyWithTrainer:
	.string "It appears to be very friendly with the\n"
	.string "trainer.$"

PacifidlogTown_House2_Text_Rururi:
	.string "Azurill: Rururi!$"

PacifidlogTown_House2_Text_DoesntLikeTrainerVeryMuch:
	.string "It doesn't appear to like the trainer\n"
	.string "very much.$"
