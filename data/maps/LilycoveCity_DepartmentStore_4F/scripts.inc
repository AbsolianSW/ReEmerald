LilycoveCity_DepartmentStore_4F_MapScripts::
	.byte 0

LilycoveCity_DepartmentStore_4F_EventScript_Gentleman::
	msgbox LilycoveCity_DepartmentStore_4F_Text_AttackOrDefenseTM, MSGBOX_NPC
	end

LilycoveCity_DepartmentStore_4F_EventScript_Woman::
	msgbox LilycoveCity_DepartmentStore_4F_Text_FiftyDifferentTMs, MSGBOX_NPC
	end

LilycoveCity_DepartmentStore_4F_EventScript_Youngster::
	msgbox LilycoveCity_DepartmentStore_4F_Text_PokemonOnlyHaveFourMoves, MSGBOX_NPC
	end

LilycoveCity_DepartmentStore_4F_EventScript_ClerkLeft::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LilycoveCity_DepartmentStore_4F_Pokemart_EvoItems
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStore_4F_Pokemart_EvoItems:
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_MOON_STONE
	.2byte ITEM_METAL_COAT
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_DEEP_SEA_TOOTH
	.2byte ITEM_DEEP_SEA_SCALE
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_UP_GRADE
	.2byte ITEM_NONE
	release
	end

LilycoveCity_DepartmentStore_4F_EventScript_ClerkRight::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LilycoveCity_DepartmentStore_4F_Pokemart_HeldItems
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStore_4F_Pokemart_HeldItems:
    .2byte ITEM_MACHO_BRACE
    .2byte ITEM_EXP_SHARE 
    .2byte ITEM_QUICK_CLAW 
    .2byte ITEM_SOOTHE_BELL
    .2byte ITEM_CHOICE_BAND 
    .2byte ITEM_AMULET_COIN 
    .2byte ITEM_CLEANSE_TAG 
    .2byte ITEM_SMOKE_BALL 
    .2byte ITEM_EVERSTONE 
    .2byte ITEM_FOCUS_BAND 
    .2byte ITEM_SCOPE_LENS  
    .2byte ITEM_LEFTOVERS 
    .2byte ITEM_SILVER_POWDER  
    .2byte ITEM_SOFT_SAND 
    .2byte ITEM_HARD_STONE 
    .2byte ITEM_MIRACLE_SEED 
    .2byte ITEM_BLACK_GLASSES 
    .2byte ITEM_BLACK_BELT 
    .2byte ITEM_MAGNET 
    .2byte ITEM_MYSTIC_WATER 
    .2byte ITEM_SHARP_BEAK 
    .2byte ITEM_POISON_BARB 
    .2byte ITEM_NEVER_MELT_ICE 
    .2byte ITEM_SPELL_TAG 
    .2byte ITEM_TWISTED_SPOON 
    .2byte ITEM_CHARCOAL 
    .2byte ITEM_DRAGON_FANG 
    .2byte ITEM_SILK_SCARF
    .2byte ITEM_SEA_INCENSE 
    .2byte ITEM_LAX_INCENSE 
	.2byte ITEM_NONE
	release
	end

LilycoveCity_DepartmentStore_4F_Text_AttackOrDefenseTM:
	.string "Hmm…\p"
	.string "An attacking move…\n"
	.string "Or a defensive move…\p"
	.string "It's no easy matter to decide which TM\n"
	.string "moves should be taught to POKéMON…$"

LilycoveCity_DepartmentStore_4F_Text_FiftyDifferentTMs:
	.string "There are so many different kinds of\n"
	.string "TM moves.\p"
	.string "A catalog I read said there are fifty\n"
	.string "different kinds.$"

LilycoveCity_DepartmentStore_4F_Text_PokemonOnlyHaveFourMoves:
	.string "I'd like to get all the different TMs,\n"
	.string "but a POKéMON learns only four moves.$"
