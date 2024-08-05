Island1_KindleRoad_MapScripts::
	.byte 0

Island1_KindleRoad_EventScript_Maria::
	trainerbattle_single TRAINER_MARIA_SEVII, Island1_KindleRoad_Text_MariaIntro, Island1_KindleRoad_Text_MariaDefeat
	msgbox Island1_KindleRoad_Text_MariaPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Abigail::
	trainerbattle_single TRAINER_ABIGAIL_SEVII, Island1_KindleRoad_Text_AbigailIntro, Island1_KindleRoad_Text_AbigailDefeat
	msgbox Island1_KindleRoad_Text_AbigailPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Finn::
	trainerbattle_single TRAINER_FINN, Island1_KindleRoad_Text_FinnIntro, Island1_KindleRoad_Text_FinnDefeat
	msgbox Island1_KindleRoad_Text_FinnPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Garrett::
	trainerbattle_single TRAINER_GARRETT, Island1_KindleRoad_Text_GarrettIntro, Island1_KindleRoad_Text_GarrettDefeat
	msgbox Island1_KindleRoad_Text_GarrettPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Tommy::
	trainerbattle_single TRAINER_TOMMY_SEVII, Island1_KindleRoad_Text_TommyIntro, Island1_KindleRoad_Text_TommyDefeat
	msgbox Island1_KindleRoad_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Sharon::
	trainerbattle_single TRAINER_SHARON_SEVII, Island1_KindleRoad_Text_SharonIntro, Island1_KindleRoad_Text_SharonDefeat
	msgbox Island1_KindleRoad_Text_SharonPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Tanya::
	trainerbattle_single TRAINER_TANYA_SEVII, Island1_KindleRoad_Text_TanyaIntro, Island1_KindleRoad_Text_TanyaDefeat
	msgbox Island1_KindleRoad_Text_TanyaPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Shea::
	trainerbattle_single TRAINER_SHEA, Island1_KindleRoad_Text_SheaIntro, Island1_KindleRoad_Text_SheaDefeat
	msgbox Island1_KindleRoad_Text_SheaPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Hugh::
	trainerbattle_single TRAINER_HUGH_SEVII, Island1_KindleRoad_Text_HughIntro, Island1_KindleRoad_Text_HughDefeat
	msgbox Island1_KindleRoad_Text_HughPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Bryce::
	trainerbattle_single TRAINER_BRYCE, Island1_KindleRoad_Text_BryceIntro, Island1_KindleRoad_Text_BryceDefeat
	msgbox Island1_KindleRoad_Text_BrycePostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Claire::
	trainerbattle_single TRAINER_CLAIRE, Island1_KindleRoad_Text_ClaireIntro, Island1_KindleRoad_Text_ClaireDefeat
	msgbox Island1_KindleRoad_Text_ClairePostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Kia::
	trainerbattle_double TRAINER_MIK_AND_KIA, Island1_KindleRoad_Text_KiaIntro, Island1_KindleRoad_Text_KiaDefeat, Island1_KindleRoad_Text_KiaNotEnoughMons
	msgbox Island1_KindleRoad_Text_KiaPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_Mik::
	trainerbattle_double TRAINER_MIK_AND_KIA, Island1_KindleRoad_Text_MikIntro, Island1_KindleRoad_Text_MikDefeat, Island1_KindleRoad_Text_MikNotEnoughMons
	msgbox Island1_KindleRoad_Text_MikPostBattle, MSGBOX_AUTOCLOSE
	end

Island1_KindleRoad_EventScript_RouteSign::
	msgbox Island1_KindleRoad_Text_RouteSign, MSGBOX_SIGN
	end

Island1_KindleRoad_EventScript_EmberSpaSign::
	msgbox Island1_KindleRoad_Text_EmberSpaSign, MSGBOX_SIGN
	end

Island1_KindleRoad_Text_MariaIntro::
    .string "The weather's gorgeous!\n"
    .string "Perfect for a battle!$"

Island1_KindleRoad_Text_MariaDefeat::
    .string "Aiyeeeeh!$"

Island1_KindleRoad_Text_MariaPostBattle::
    .string "Please don't splash my face!\n"
    .string "You'll ruin the makeup.$"

Island1_KindleRoad_Text_AbigailIntro::
    .string "I'm beginning to get a sunburn…$"

Island1_KindleRoad_Text_AbigailDefeat::
    .string "Oh, you're so awful.$"

Island1_KindleRoad_Text_AbigailPostBattle::
    .string "I like that cap you have.\n"
    .string "Maybe I should wear one, too…$"

Island1_KindleRoad_Text_FinnIntro::
    .string "Seen from here, Mt. Ember towers\n"
    .string "into the sky.$"

Island1_KindleRoad_Text_FinnDefeat::
    .string "Well, aren't you tough?$"

Island1_KindleRoad_Text_FinnPostBattle::
    .string "The sky is immense around these\n"
    .string "parts.$"

Island1_KindleRoad_Text_GarrettIntro::
    .string "I took a break from swimming,\n"
    .string "only to have you challenge me?$"

Island1_KindleRoad_Text_GarrettDefeat::
    .string "Hey, what the…\n"
    .string "You're good, you!$"

Island1_KindleRoad_Text_GarrettPostBattle::
    .string "Instead of using Surf all the time,\n"
    .string "you should swim some.$"

Island1_KindleRoad_Text_TommyIntro::
    .string "Wait! Wait a second!\n"
    .string "I think I've hooked a big one!$"

Island1_KindleRoad_Text_TommyDefeat::
    .string "I've lost it all again…$"

Island1_KindleRoad_Text_TommyPostBattle::
    .string "Not only did I lose, the big one\n"
    .string "got away from me, too!$"

Island1_KindleRoad_Text_SharonIntro::
    .string "Could you help me out with my\n"
    .string "training?$"

Island1_KindleRoad_Text_SharonDefeat::
    .string "You were a cut above me.$"

Island1_KindleRoad_Text_SharonPostBattle::
    .string "It's clear that you're skilled.\n"
    .string "I like you!$"

Island1_KindleRoad_Text_TanyaIntro::
    .string "There isn't a day in which we don't\n"
    .string "train!$"

Island1_KindleRoad_Text_TanyaDefeat::
    .string "I'm such a fool!$"

Island1_KindleRoad_Text_TanyaPostBattle::
    .string "I'll train even harder from now on!$"

Island1_KindleRoad_Text_SheaIntro::
    .string "Every morning, before breakfast,\n"
    .string "I swim around this island.$"

Island1_KindleRoad_Text_SheaDefeat::
    .string "Gasp…\n"
    .string "Gasp…$"

Island1_KindleRoad_Text_SheaPostBattle::
    .string "I lost because I'm pooped from all\n"
    .string "the running I did…$"

Island1_KindleRoad_Text_HughIntro::
    .string "Dress appropriately for battle!\n"
    .string "Lose that frivolous outfit!$"

Island1_KindleRoad_Text_HughDefeat::
    .string "Why me?!$"

Island1_KindleRoad_Text_HughPostBattle::
    .string "Even martial artists are becoming\n"
    .string "fashion conscious these days…$"

Island1_KindleRoad_Text_BryceIntro::
    .string "You know, everything tastes great\n"
    .string "when you're out in the wild.$"

Island1_KindleRoad_Text_BryceDefeat::
    .string "Awww, blown!$"

Island1_KindleRoad_Text_BrycePostBattle::
    .string "The water in volcanic regions is\n"
    .string "delicious.$"

Island1_KindleRoad_Text_ClaireIntro::
    .string "My lunch filled us up, so will you\n"
    .string "battle us for some exercise?$"

Island1_KindleRoad_Text_ClaireDefeat::
    .string "That made me all sweaty.$"

Island1_KindleRoad_Text_ClairePostBattle::
    .string "What should I do?\n"
    .string "I feel famished again.$"

Island1_KindleRoad_Text_KiaIntro::
    .string "Kia: My big brother and I make\n"
    .string "an awesome combination!$"

Island1_KindleRoad_Text_KiaDefeat::
    .string "Kia: Huh?!\n"
    .string "I can't believe it!$"

Island1_KindleRoad_Text_KiaPostBattle::
    .string "Kia: How could there be anyone\n"
    .string "better than my big brother?$"

Island1_KindleRoad_Text_KiaNotEnoughMons::
    .string "Kia: If you want to battle with\n"
    .string "us, you have to bring at least two\l"
    .string "Pokémon with you.$"

Island1_KindleRoad_Text_MikIntro::
    .string "Mik: Together with Kia, I'm afraid\n"
    .string "of nothing!$"

Island1_KindleRoad_Text_MikDefeat::
    .string "Mik: Whoa!\n"
    .string "That's too much!$"

Island1_KindleRoad_Text_MikPostBattle::
    .string "Mik: How could my combination\n"
    .string "with Kia fail?$"

Island1_KindleRoad_Text_MikNotEnoughMons::
    .string "Mik: Hey, do you want to battle\n"
    .string "with us two?\p"
    .string "Bring two or more Pokémon with\n"
    .string "you, then.$"

Island1_KindleRoad_Text_RouteSign::
    .string "This is Kindle Road.\p"
    .string "Go straight for Mt. Ember.$"

Island1_KindleRoad_Text_EmberSpaSign::
    .string "Light the Fire in Your Heart!\n"
    .string "Ember Spa$"
