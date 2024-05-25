Island2_Main_MapScripts::
	.byte 0


Island2_Main_Merchant::
	lockall
	faceplayer
	msgbox Island2_Main_Merchant_Text_0
	pokemart Island2_Main_Mart
	releaseall
	end


	.align 2
Island2_Main_Mart:
	.2byte ITEM_SODA_POP
	.2byte ITEM_FRESH_WATER
	.2byte ITEM_LEMONADE
	.2byte ITEM_BERRY_JUICE
	.2byte ITEM_LAVA_COOKIE
	.2byte ITEM_LIGHT_BALL
	.2byte ITEM_SOUL_DEW
	.2byte ITEM_STICK
	.2byte ITEM_THICK_CLUB
	.2byte ITEM_LUCKY_PUNCH
	.2byte ITEM_LUCKY_EGG
	.2byte ITEM_MOOMOO_MILK
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_NONE

Island2_Main_Sailor::
	lockall
	faceplayer
	msgbox Island2_Main_Sailor_Text_0
	releaseall
	end


Island2_Main_Woman::
	lockall
	faceplayer
	msgbox Island2_Main_Woman_Text_0
	releaseall
	end


Island2_Main_Beauty::
	lockall
	faceplayer
	msgbox Island2_Main_Beauty_Text_0
	releaseall
	end


Island2_Main_SuperNerd::
	lockall
	faceplayer
	msgbox Island2_Main_SuperNerd_Text_0
	releaseall
	end


Island2_Main_Boy::
	lockall
	faceplayer
	msgbox Island2_Main_Boy_Text_0
	releaseall
	end


Island2_Main_LittleBoy::
	lockall
	faceplayer
	msgbox Island2_Main_LittleBoy_Text_0
	releaseall
	end


Island2_Main_Sign::
	msgbox Island2_Main_Sign_Text_0, MSGBOX_SIGN
	end


Island2_Main_FastCurrentSign::
	msgbox Island2_Main_FastCurrentSign_Text_0, MSGBOX_SIGN
	end


Island2_Main_GameCornerSign::
	msgbox Island2_Main_GameCornerSign_Text_0, MSGBOX_SIGN
	end


Island2_Main_Merchant_Text_0:
	.string "Salutations traveler! I sell only the\n"
	.string "most exclusive of items, come have a\l"
	.string "look!$"

Island2_Main_Sailor_Text_0:
	.string "That fellow there, he moved here from\n"
	.string "CELADON CITY. He started up a shop\l"
	.string "with his younger brother. You have to\l"
	.string "wonder if he knew what he was getting\l"
	.string "into...$"

Island2_Main_Woman_Text_0:
	.string "The shopkeepers brother took a job at\n"
	.string "the GAME CORNER. He was originally just\l"
	.string "trying to get some money so they could\l"
	.string "expand, but he liked it so much that he\l"
	.string "stayed there! The shop is doing well\l"
	.string "these days too!$"

Island2_Main_Beauty_Text_0:
	.string "Listen, listen. Did you know? A famous\n"
	.string "TRAINER came to the ISLAND a while\l"
	.string "back. He's one of the Kanto region's\l"
	.string "ELITE FOUR. His name is BRUNO. He went\l"
	.string "away disappointed when he found out\l"
	.string "that they were all sold out of Rage\l"
	.string "Candybars.$"

Island2_Main_SuperNerd_Text_0:
	.string "The discerning trainer knows to buy\n"
	.string "rare items here!$"

Island2_Main_Boy_Text_0:
	.string "There's an old woman who lives in a\n"
	.string "cottage out on the cape. She gives the\l"
	.string "eye to POKéMON. It's a bit unnerving.$"

Island2_Main_LittleBoy_Text_0:
	.string "Boing! Boing!\n"
	.string "Have you tried the jumping game?\p"
	.string "The POKéMON you can enter…\n"
	.string "Boing!\p"
	.string "Some go boing!\p"
	.string "And some go boooooooing!\p"
	.string "Did you know that?\n"
	.string "Boing!$"

Island2_Main_Sign_Text_0:
	.string "TWO ISLAND\n"
	.string "Boon island for two$"

Island2_Main_FastCurrentSign_Text_0:
	.string "DANGER! FAST CURRENT!\n"
	.string "Fishing and surfing prohibited$"

Island2_Main_GameCornerSign_Text_0:
	.string "Skip&Chomp!\n"
	.string "Joyful GAME CORNER$"
