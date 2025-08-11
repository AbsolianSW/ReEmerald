#include "global.h"
#include "new_game.h"
#include "random.h"
#include "pokemon.h"
#include "roamer.h"
#include "pokemon_size_record.h"
#include "script.h"
#include "lottery_corner.h"
#include "play_time.h"
#include "mauville_old_man.h"
#include "match_call.h"
#include "lilycove_lady.h"
#include "load_save.h"
#include "pokeblock.h"
#include "dewford_trend.h"
#include "berry.h"
#include "rtc.h"
#include "easy_chat.h"
#include "event_data.h"
#include "money.h"
#include "trainer_hill.h"
#include "tv.h"
#include "coins.h"
#include "text.h"
#include "overworld.h"
#include "mail.h"
#include "battle_records.h"
#include "item.h"
#include "pokedex.h"
#include "apprentice.h"
#include "frontier_util.h"
#include "pokedex.h"
#include "save.h"
#include "link_rfu.h"
#include "main.h"
#include "contest.h"
#include "item_menu.h"
#include "pokemon_storage_system.h"
#include "pokemon_jump.h"
#include "decoration_inventory.h"
#include "secret_base.h"
#include "player_pc.h"
#include "field_specials.h"
#include "berry_powder.h"
#include "union_room_chat.h"
#include "constants/items.h"

extern const u8 EventScript_ResetAllMapFlags[];

static void ClearFrontierRecord(void);
static void WarpToTruck(void);
static void ResetMiniGamesRecords(void);
static void HandleChallenges(void);

EWRAM_DATA bool8 gDifferentSaveFile = 0;
EWRAM_DATA bool8 gEnableContestDebugging = FALSE;

static const struct ContestWinner sContestWinnerPicDummy =
{
    .monName = _(""),
    .trainerName = _("")
};

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

u32 GetTrainerId(u8 *trainerId)
{
    return (trainerId[3] << 24) | (trainerId[2] << 16) | (trainerId[1] << 8) | (trainerId[0]);
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = 0;
    while(!trainerId)
    {
        trainerId = (Random() << 16) | GetGeneratedTrainerIdLower();//make sure the dummy ID of 0 isn't generated here
    }
    SetTrainerId(trainerId, gSaveBlock1Ptr->playerTrainerId);
}

// L=A isnt set here for some reason.
static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_FAST;
    gSaveBlock2Ptr->optionsWindowFrameType = 0;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_MONO;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SHIFT;
    gSaveBlock2Ptr->optionsBattleSceneOff = FALSE;
}

void SetDefaultChallenges(void)
{
    gSaveBlock1Ptr->challenges.xpMultiplier=10;
    gSaveBlock1Ptr->challenges.grassStarter=SpeciesToNationalPokedexNum(SPECIES_TREECKO);
    gSaveBlock1Ptr->challenges.waterStarter=SpeciesToNationalPokedexNum(SPECIES_MUDKIP);
    gSaveBlock1Ptr->challenges.fireStarter=SpeciesToNationalPokedexNum(SPECIES_TORCHIC);
    gSaveBlock1Ptr->challenges.startingMoney=29;
    gSaveBlock1Ptr->challenges.shinyOdds=13;
}

static void ClearPokedexFlags(void)
{
    gUnusedPokedexU8 = 0;
    memset(&gSaveBlock1Ptr->pokedex.owned, 0, sizeof(gSaveBlock1Ptr->pokedex.owned));
    memset(&gSaveBlock1Ptr->pokedex.seen, 0, sizeof(gSaveBlock1Ptr->pokedex.seen));
}

void ClearAllContestWinnerPics(void)
{
    s32 i;

    ClearContestWinnerPicsInContestHall();

    // Clear Museum paintings
    for (i = MUSEUM_CONTEST_WINNERS_START; i < NUM_CONTEST_WINNERS; i++)
        gSaveBlock1Ptr->contestWinners[i] = sContestWinnerPicDummy;
}

static void ClearFrontierRecord(void)
{
    CpuFill32(0, &gSaveBlock1Ptr->frontier, sizeof(gSaveBlock1Ptr->frontier));

    gSaveBlock1Ptr->frontier.opponentNames[0][0] = EOS;
    gSaveBlock1Ptr->frontier.opponentNames[1][0] = EOS;
}

static void WarpToTruck(void)
{
    SetWarpDestination(MAP_GROUP(INSIDE_OF_TRUCK), MAP_NUM(INSIDE_OF_TRUCK), WARP_ID_NONE, -1, -1);
    WarpIntoMap();
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ResetPokedexScrollPositions();
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagScrollPositions();
    ResetPokeblockScrollPositions();
}

static const s32 sPowersOfTen[] =
{
             1,
            10,
           100,
          1000,
         10000,
        100000,
       1000000,
      10000000,
     100000000,
    1000000000,
};
static u32 GetStartingMoney()
{
    u32 number,i,j;
    i=0;
    j=0;
    while (TRUE)
    {
        if (gSaveBlock1Ptr->challenges.startingMoney < 9 * (i + 1))
        {
            number = (gSaveBlock1Ptr->challenges.startingMoney - (9 * i)+1) * sPowersOfTen[j];
            break;
        }
        i++;
        j++;
    }
    if (number == 100000000)
        number = 99999999;
    return number;
}

void NewGameInitData(void)
{
    if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_CORRUPT)
        RtcReset();

    gDifferentSaveFile = TRUE;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetPokedex();
    ClearFrontierRecord();
    //ClearSav1();
    ClearAllMail();
    gSaveBlock1Ptr->specialSaveWarpFlags = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ClearTVShowData();
    ResetGabbyAndTy();
    ClearSecretBases();
    ClearBerryTrees();
    SetMoney(&gSaveBlock1Ptr->money, GetStartingMoney());
    SetCoins(0);
    ResetLinkContestBoolean();
    ResetGameStats();
    ClearAllContestWinnerPics();
    ClearPlayerLinkBattleRecords();
    InitSeedotSizeRecord();
    InitLotadSizeRecord();
    gPlayerPartyCount = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    ClearRoamerData();
    ClearRoamerLocationData();
    memset(gSaveBlock1Ptr->registeredItems, 0, sizeof(gSaveBlock1Ptr->registeredItems));
    ClearBag();
    NewGameInitPCItems();
    ClearPokeblocks();
    ClearDecorationInventories();
    InitEasyChatPhrases();
    SetMauvilleOldMan();
    InitDewfordTrend();
    ResetFanClub();
    ResetLotteryCorner();
    WarpToTruck();
    RunScriptImmediately(EventScript_ResetAllMapFlags);
    ResetMiniGamesRecords();
    InitUnionRoomChatRegisteredTexts();
    InitLilycoveLady();
    ResetAllApprenticeData();
    ClearRankingHallRecords();
    InitMatchCallCounters();
    WipeTrainerNameRecords();
    ResetTrainerHillResults();
    ResetContestLinkResults();
    HandleChallenges();
}

static void ResetMiniGamesRecords(void)
{
    CpuFill16(0, &gSaveBlock1Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock1Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokemonJumpRecords();
    CpuFill16(0, &gSaveBlock1Ptr->berryPick, sizeof(struct BerryPickingResults));
}

static void HandleChallenges(void)
{
    if (gSaveBlock1Ptr->challenges.levelCap)
    {
        FlagSet(FLAG_CHALLENGES_LEVEL_CAP);
        if (gSaveBlock1Ptr->challenges.levelCap == 2)
            FlagSet(FLAG_CHALLENGES_LEVEL_CAP_EXTREME);
    }
    if (gSaveBlock1Ptr->challenges.permaDeath)
        FlagSet(FLAG_CHALLENGES_PERMA_DEATH);
    if (gSaveBlock1Ptr->challenges.limitedEncounters)
        FlagSet(FLAG_CHALLENGES_LIMITED_ENCOUNTERS);
    if (gSaveBlock1Ptr->challenges.speciesClause)
        FlagSet(FLAG_CHALLENGES_SPECIES_CLAUSE);
    if(gSaveBlock1Ptr -> challenges.noBattleItems)
        FlagSet(FLAG_CHALLENGES_NO_BATTLE_ITEMS);
    if(gSaveBlock1Ptr -> challenges.forceSetMode)
        FlagSet(FLAG_CHALLENGES_FORCE_SET_MODE);
    if(gSaveBlock1Ptr -> challenges.infiniteCandy)
        FlagSet(FLAG_CHALLENGES_INFINITE_CANDY);
    if(gSaveBlock1Ptr -> challenges.repellant)
        FlagSet(FLAG_CHALLENGES_REPELLANT);
    if(gSaveBlock1Ptr -> challenges.starterAffectsRival)
        FlagSet(FLAG_CHALLENGES_STARTERAFFECTSRIVAL);
    if(!gSaveBlock1Ptr -> challenges.gauntletMode)
        VarSet(VAR_ACTIVE_GAUNTLET, 3);

    
    VarSet(VAR_XP_MULTIPLIER, gSaveBlock1Ptr->challenges.xpMultiplier);
}
