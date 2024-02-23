OldaleTown_Mart_MapScripts::
	.byte 0

OldaleTown_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart OldaleTown_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
OldaleTown_Mart_Pokemart:
	.2byte ITEM_PLACEHOLDER_DYNAMIC_SHOP_OLDALE
	release
	end

OldaleTown_Mart_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_ADVENTURE_STARTED, OldaleTown_Mart_EventScript_PokeBallsInStock
	msgbox OldaleTown_Mart_Text_PokeBallsAreSoldOut, MSGBOX_DEFAULT
	release
	end

OldaleTown_Mart_EventScript_PokeBallsInStock::
	msgbox OldaleTown_Mart_Text_ImGoingToBuyPokeBalls, MSGBOX_DEFAULT
	release
	end

OldaleTown_Mart_EventScript_Boy::
	msgbox OldaleTown_Mart_Text_RestoreHPWithPotion, MSGBOX_NPC
	end

OldaleTown_Mart_Text_PokeBallsAreSoldOut:
	.string "The clerk says they're all sold out.\n"
	.string "I can't buy any POKé BALLS.$"

OldaleTown_Mart_Text_ImGoingToBuyPokeBalls:
	.string "I'm going to buy a bunch of POKé BALLS\n"
	.string "and catch a bunch of POKéMON!$"

OldaleTown_Mart_Text_RestoreHPWithPotion:
	.string "If a POKéMON gets hurt and loses its HP\n"
	.string "and faints, it won't be able to battle.\p"
	.string "To prevent your POKéMON from fainting,\n"
	.string "restore its HP with a POTION.$"