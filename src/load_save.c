#include "global.h"
#include "malloc.h"
#include "berry_powder.h"
#include "event_data.h"
#include "item.h"
#include "load_save.h"
#include "main.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "save_location.h"
#include "string_util.h"
#include "trainer_hill.h"
#include "gba/flash_internal.h"
#include "decoration_inventory.h"
#include "agb_flash.h"
#include "constants/event_objects.h"

#define SAVEBLOCK_MOVE_RANGE    128

struct LoadedSaveData
{
 /*0x0000*/ struct ItemSlot medicine[BAG_MEDICINE_COUNT];
 /*0x0000*/ struct ItemSlot powerup[BAG_POWERUP_COUNT];
 /*0x0000*/ struct ItemSlot battle[BAG_BATTLE_COUNT];
 /*0x0000*/ struct ItemSlot misc[BAG_MISC_COUNT];
 /*0x0078*/ struct ItemSlot keyItems[BAG_KEYITEMS_COUNT];
 /*0x00F0*/ struct ItemSlot pokeBalls[BAG_POKEBALLS_COUNT];
 /*0x0130*/ struct ItemSlot TMsHMs[BAG_TMHM_COUNT];
 /*0x0230*/ struct ItemSlot berries[BAG_BERRIES_COUNT];
 /*0x02E8*/ struct Mail mail[MAIL_COUNT];
};

// EWRAM DATA
EWRAM_DATA struct SaveBlock2ASLR gSaveblock2 = {0};
EWRAM_DATA struct SaveBlock1ASLR gSaveblock1 = {0};
EWRAM_DATA struct PokemonStorageASLR gPokemonStorage = {0};

EWRAM_DATA struct LoadedSaveData gLoadedSaveData = {0};

// IWRAM common
bool32 gFlashMemoryPresent;
struct SaveBlockProfile *gSaveBlock1Ptr;
struct SaveBlockGeneral *gSaveBlock2Ptr;
struct PokemonStorage *gPokemonStoragePtr;

// code
void CheckForFlashMemory(void)
{
    if (!IdentifyFlash())
    {
        gFlashMemoryPresent = TRUE;
        InitFlashTimer();
    }
    else
    {
        gFlashMemoryPresent = FALSE;
    }
}

void ClearSav2(void)
{
    CpuFill16(0, &gSaveblock2, sizeof(struct SaveBlock2ASLR));
}

void ClearSav1(void)
{
    CpuFill16(0, &gSaveblock1, sizeof(struct SaveBlock1ASLR));
}

// Offset is the sum of the trainer id bytes
void SetSaveBlocksPointers(u16 offset)
{
    struct SaveBlockProfile** sav1_LocalVar = &gSaveBlock1Ptr;

    offset = (offset + Random()) & (SAVEBLOCK_MOVE_RANGE - 4);

    gSaveBlock2Ptr = (void *)(&gSaveblock2) + offset;
    *sav1_LocalVar = (void *)(&gSaveblock1) + offset;
    gPokemonStoragePtr = (void *)(&gPokemonStorage) + offset;

    SetBagItemsPointers();
    SetDecorationInventoriesPointers();
}

void MoveSaveBlocks_ResetHeap(void)
{
    void *vblankCB, *hblankCB;
    struct SaveBlockGeneral *saveBlock2Copy;
    struct SaveBlockProfile *saveBlock1Copy;
    struct PokemonStorage *pokemonStorageCopy;

    // save interrupt functions and turn them off
    vblankCB = gMain.vblankCallback;
    hblankCB = gMain.hblankCallback;
    gMain.vblankCallback = NULL;
    gMain.hblankCallback = NULL;
    gTrainerHillVBlankCounter = NULL;

    saveBlock2Copy = (struct SaveBlockGeneral *)(gHeap);
    saveBlock1Copy = (struct SaveBlockProfile *)(gHeap + sizeof(struct SaveBlockGeneral));
    pokemonStorageCopy = (struct PokemonStorage *)(gHeap + sizeof(struct SaveBlockGeneral) + sizeof(struct SaveBlockProfile));

    // backup the saves.
    *saveBlock2Copy = *gSaveBlock2Ptr;
    *saveBlock1Copy = *gSaveBlock1Ptr;
    *pokemonStorageCopy = *gPokemonStoragePtr;

    // change saveblocks' pointers
    // argument is a sum of the individual trainerId bytes
    SetSaveBlocksPointers(
      saveBlock1Copy->playerTrainerId[0] +
      saveBlock1Copy->playerTrainerId[1] +
      saveBlock1Copy->playerTrainerId[2] +
      saveBlock1Copy->playerTrainerId[3]);

    // restore saveblock data since the pointers changed
    *gSaveBlock2Ptr = *saveBlock2Copy;
    *gSaveBlock1Ptr = *saveBlock1Copy;
    *gPokemonStoragePtr = *pokemonStorageCopy;

    // heap was destroyed in the copying process, so reset it
    InitHeap(gHeap, HEAP_SIZE);

    // restore interrupt functions
    gMain.hblankCallback = hblankCB;
    gMain.vblankCallback = vblankCB;
}

u32 UseContinueGameWarp(void)
{
    return gSaveBlock1Ptr->specialSaveWarpFlags & CONTINUE_GAME_WARP;
}

void ClearContinueGameWarpStatus(void)
{
    gSaveBlock1Ptr->specialSaveWarpFlags &= ~CONTINUE_GAME_WARP;
}

void SetContinueGameWarpStatus(void)
{
    gSaveBlock1Ptr->specialSaveWarpFlags |= CONTINUE_GAME_WARP;
}

void SetContinueGameWarpStatusToDynamicWarp(void)
{
    SetContinueGameWarpToDynamicWarp(0);
    gSaveBlock1Ptr->specialSaveWarpFlags |= CONTINUE_GAME_WARP;
}

void ClearContinueGameWarpStatus2(void)
{
    gSaveBlock1Ptr->specialSaveWarpFlags &= ~CONTINUE_GAME_WARP;
}

void SavePlayerParty(void)
{
    int i;
    u16 species;
    gSaveBlock1Ptr->playerPartyCount = gPlayerPartyCount;
    gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].partyCount = gPlayerPartyCount;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        gSaveBlock1Ptr->playerParty[i] = gPlayerParty[i];
        gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].partySpecies[i] = GetMonData(&gPlayerParty[i],MON_DATA_SPECIES);
        gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].partyPersonalities[i] = GetMonData(&gPlayerParty[i],MON_DATA_PERSONALITY);
    }
}

void LoadPlayerParty(void)
{
    int i;

    gPlayerPartyCount = gSaveBlock1Ptr->playerPartyCount;

    for (i = 0; i < PARTY_SIZE; i++)
        gPlayerParty[i] = gSaveBlock1Ptr->playerParty[i];
}

void SaveRelevantData(void)
{
    int i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        gSaveBlock1Ptr->objectEvents[i] = gObjectEvents[i];
        // To avoid crash on vanilla, save follower as inactive
        if (gObjectEvents[i].localId == OBJ_EVENT_ID_FOLLOWER)
            gSaveBlock1Ptr->objectEvents[i].active = FALSE;
    }
    gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].profileSaved = TRUE;
    StringCopy_PlayerName(gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].playerName, gSaveBlock1Ptr->playerName);
    gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].playerGender = gSaveBlock1Ptr->playerGender;
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
    {
        if (IsNationalPokedexEnabled())
            gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].dexCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
        else
            gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].dexCount = GetHoennPokedexCount(FLAG_GET_CAUGHT);
    }
    gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].badgeCount = 0;
    for (i = FLAG_BADGE01_GET; i < FLAG_BADGE01_GET + NUM_BADGES; i++)
    {
        if (FlagGet(i))
            gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].badgeCount++;
    }
    gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].playTimeHours = gSaveBlock1Ptr->playTimeHours;
    gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].playTimeMinutes = gSaveBlock1Ptr->playTimeMinutes;
    gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].regionMapSectionId = GetCurrentRegionMapSectionId();
}

void LoadObjectEvents(void)
{
    int i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        gObjectEvents[i] = gSaveBlock1Ptr->objectEvents[i];
    // Try to restore saved inactive follower
    if (gObjectEvents[i].localId == OBJ_EVENT_ID_FOLLOWER &&
        !gObjectEvents[i].active &&
        gObjectEvents[i].extra.asU16)
        gObjectEvents[i].active = TRUE;
}

void CopyPartyAndObjectsToSave(void)
{
    SavePlayerParty();
    SaveRelevantData();
}

void CopyPartyAndObjectsFromSave(void)
{
    LoadPlayerParty();
    LoadObjectEvents();
}

void LoadPlayerBag(void)
{
    int i;

    // load player medicine.
    for (i = 0; i < BAG_MEDICINE_COUNT; i++)
        gLoadedSaveData.medicine[i] = gSaveBlock1Ptr->bagPocket_Medicine[i];

    // load player powerup items.
    for (i = 0; i < BAG_POWERUP_COUNT; i++)
        gLoadedSaveData.powerup[i] = gSaveBlock1Ptr->bagPocket_Powerup[i];

    // load player battle items.
    for (i = 0; i < BAG_BATTLE_COUNT; i++)
        gLoadedSaveData.battle[i] = gSaveBlock1Ptr->bagPocket_Battle[i];

    // load player items.
    for (i = 0; i < BAG_MISC_COUNT; i++)
        gLoadedSaveData.misc[i] = gSaveBlock1Ptr->bagPocket_Misc[i];

    // load player key items.
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
        gLoadedSaveData.keyItems[i] = gSaveBlock1Ptr->bagPocket_KeyItems[i];

    // load player pokeballs.
    for (i = 0; i < BAG_POKEBALLS_COUNT; i++)
        gLoadedSaveData.pokeBalls[i] = gSaveBlock1Ptr->bagPocket_PokeBalls[i];

    // load player TMs and HMs.
    for (i = 0; i < BAG_TMHM_COUNT; i++)
        gLoadedSaveData.TMsHMs[i] = gSaveBlock1Ptr->bagPocket_TMHM[i];

    // load player berries.
    for (i = 0; i < BAG_BERRIES_COUNT; i++)
        gLoadedSaveData.berries[i] = gSaveBlock1Ptr->bagPocket_Berries[i];

    // load mail.
    for (i = 0; i < MAIL_COUNT; i++)
        gLoadedSaveData.mail[i] = gSaveBlock1Ptr->mail[i];
}

void SavePlayerBag(void)
{
    int i;

    // save player medicine.
    for (i = 0; i < BAG_MEDICINE_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Medicine[i] = gLoadedSaveData.medicine[i];

    // save player items.
    for (i = 0; i < BAG_POWERUP_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Powerup[i] = gLoadedSaveData.powerup[i];

    // save player items.
    for (i = 0; i < BAG_BATTLE_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Battle[i] = gLoadedSaveData.battle[i];

    // save player items.
    for (i = 0; i < BAG_MISC_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Misc[i] = gLoadedSaveData.misc[i];

    // save player key items.
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_KeyItems[i] = gLoadedSaveData.keyItems[i];

    // save player pokeballs.
    for (i = 0; i < BAG_POKEBALLS_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_PokeBalls[i] = gLoadedSaveData.pokeBalls[i];

    // save player TMs and HMs.
    for (i = 0; i < BAG_TMHM_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_TMHM[i] = gLoadedSaveData.TMsHMs[i];

    // save player berries.
    for (i = 0; i < BAG_BERRIES_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Berries[i] = gLoadedSaveData.berries[i];

    // save mail.
    for (i = 0; i < MAIL_COUNT; i++)
        gSaveBlock1Ptr->mail[i] = gLoadedSaveData.mail[i];
}