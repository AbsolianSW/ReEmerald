Island3_Main_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Island3_Main_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

Island3_Main_MapScripts_MAP_SCRIPT_ON_TRANSITION:
	setflag FLAG_VISITED_THREE_ISLAND
	end


Island3_Main_Doduo::
	bufferspeciesname 0, SPECIES_DODUO
	end


Island3_Main_AquaIntroScene::
	lockall
	applymovement 4, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Island3_Main_Text_AreYouBossGoBackToHoenn, MSGBOX_DEFAULT
	applymovement 12, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox Island3_Main_Text_JustGotHerePal, MSGBOX_DEFAULT
	applymovement 4, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Island3_Main_Text_FollowersRaisingHavoc, MSGBOX_DEFAULT
	applymovement 12, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox Island3_Main_Text_OughtToBeThankingUs, MSGBOX_DEFAULT
	applymovement 4, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Island3_Main_Text_YouCowardsToughInPack, MSGBOX_DEFAULT
	setvar VAR_THREE_ISLAND_STATE, 2
	releaseall
	end


Island3_Main_Text_IslandSign::
    .string "Three Island\n"
    .string "Kin Island of Family Bonding$"

Island3_Main_Text_IslandBelongsToUs::
    .string "Hyahoo!\p"
    .string "From this day on, this island\n"
    .string "belongs to us!$"

Island3_Main_Text_GoBackToHoenn::
    .string "We don't need you people bringing\n"
    .string "your noise and trouble here!\p"
    .string "We're asking you to go back to\n"
    .string "Hoenn!$"

Island3_Main_Text_BossIsOnHisWay::
    .string "Hey, go cry somewhere else.\n"
    .string "Our boss is conducting business.\p"
    .string "When he gets here, we'll give you\n"
    .string "a legendary show you won't\l"
    .string "soon forget!$"

Island3_Main_Text_GetOffIslandNow::
    .string "W-what!? Not on your life!\n"
    .string "Get off the island now!$"

Island3_Main_Text_WhosGonnaMakeMe::
    .string "Who's gonna make me?$"

Island3_Main_Text_AreYouBossGoBackToHoenn::
    .string "Are you the boss?\n"
    .string "Go back to Hoenn right now!$"

Island3_Main_Text_JustGotHerePal::
    .string "Hah?\p"
    .string "I just got here, pal.\p"
    .string "What's with the hostile attitude?\n"
    .string "It's mighty cold of you!$"

Island3_Main_Text_FollowersRaisingHavoc::
    .string "Your gang of followers have been\n"
    .string "raising havoc on the island.\p"
    .string "Do you have any idea how much\n"
    .string "trouble they've caused us on the\l"
    .string "here?$"

Island3_Main_Text_OughtToBeThankingUs::
    .string "No, man, I don't get it at all.\p"
    .string "Look at this place.\n"
    .string "What do you do for entertainment?\p"
    .string "You ought to be thanking us for\n"
    .string "livening up this sleepy village.\p"
    .string "But hey, if you insist, you can try\n"
    .string "making us leave.$"

Island3_Main_Text_YouCowardsToughInPack::
    .string "Grr… You cowards…\n"
    .string "So tough in a pack…$"

Island3_Main_Text_WannaMakeSomethingOfYourStaring::
    .string "You, what are you staring at?\n"
    .string "Don't you know it's not polite?\p"
    .string "You wanna make something of it\n"
    .string "or what?$"

Island3_Main_Text_ThatsSmart::
    .string "That's smart.\n"
    .string "Keep your nose out of this.$"

Island3_Main_Text_Aqua1Intro::
    .string "Heh, I like your guts.\n"
    .string "You'll be losing money to me, but…$"

Island3_Main_Text_Aqua1Defeat::
    .string "Wha…\n"
    .string "What is this kid?!$"

Island3_Main_Text_Aqua1PostBattle::
    .string "Aww, man…\n"
    .string "Don't you dare laugh!$"

Island3_Main_Text_Aqua2Intro::
    .string "Aren't you from Hoenn?\n"
    .string "You should be on our side!$"

Island3_Main_Text_Aqua2Defeat::
    .string "Stop fooling around!$"

Island3_Main_Text_Aqua2PostBattle::
    .string "What's the matter with you,\n"
    .string "getting all hot like that?\p"
    .string "Totally uncool, man!$"

Island3_Main_Text_Aqua3Intro::
    .string "We're setting up for the boss out \n"
    .string "here,but you had to mess it up!\p"
    .string "You embarrassed us, man!$"

Island3_Main_Text_Aqua3Defeat::
    .string "… … …   … … …$"

Island3_Main_Text_Aqua3PostBattle::
    .string "Boss, I'm telling you, you've gotta\n"
    .string "do something about this kid!$"

Island3_Main_Text_MainGruntIntro::
    .string "I've been watching you, and I'd say\n"
    .string "you've done enough.\p"
    .string "What are you, their friend or\n"
    .string "something?\p"
    .string "Then I guess you'll be battling me\n"
    .string "in their place.$"

Island3_Main_Text_MainGruntDefeat::
    .string "All right, enough!\n"
    .string "We'll go like you wanted!\p"
    .string "We'll be happy to see the last of\n"
    .string "this boring island!$"

Island3_Main_Text_MainGruntPostBattle::
    .string "Humph! Yeah, go right on hanging\n"
    .string "around with these hayseeds!$"

Island3_Main_Text_ThankYouOhYourMonGotHurt::
    .string "Thank you! Those goons were\n"
    .string "nothing but bad trouble.\p"
    .string "Oh, look, your Pokémon got hurt\n"
    .string "on my account.$"

Island3_Main_Text_GivenFullRestore::
    .string "{Player} was given\n"
    .string "a Full Restore.$"

Island3_Main_Text_OhYourBagIsFull::
    .string "Oh?\n"
    .string "Your bag is full.$"

Island3_Main_Text_YouveGotImpressiveMons::
    .string "You've got seriously impressive\n"
    .string "Pokémon with you.$"

Island3_Main_Text_LostelleWentOffTowardsBondBridge::
    .string "It'd be fantastic if someone as\n"
    .string "strong as you lived here.\p"
    .string "I hope you'll at least stay here\n"
    .string "a while.\p"
    .string "…I beg your pardon?\n"
    .string "You're looking for Lostelle?\p"
    .string "Lostelle was dragged towards Bond\n"
    .string "Bridge a while ago.$"

Island3_Main_Text_WouldntWantToSeeAquasHereAgain::
    .string "Three Island is actually the most\n"
    .string "populous of the islands here.\p"
    .string "Still, it could be less sleepy.\p"
    .string "But I wouldn't want to see goons\n"
    .string "like those Aquas here again.$"

Island3_Main_Text_WhenDodouEvolvesGoingToPlayGame::
    .string "I'm going to train my Doduo in the\n"
    .string "Berry Forest.\p"
    .string "When it evolves, I'm going to play\n"
    .string "a game on Two Island.$"

Island3_Main_Text_Doduo::
    .string "Doduo: Gigiih!$"
