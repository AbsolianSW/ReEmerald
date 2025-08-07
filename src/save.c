#include "global.h"
#include "agb_flash.h"
#include "gba/flash_internal.h"
#include "fieldmap.h"
#include "save.h"
#include "task.h"
#include "decompress.h"
#include "load_save.h"
#include "overworld.h"
#include "pokemon_storage_system.h"
#include "main.h"
#include "trainer_hill.h"
#include "link.h"
#include "constants/game_stat.h"

static u16 CalculateChecksum(void *, u16);
static bool8 ReadFlashSector(u8, struct SaveSector *);
static u8 GetSaveValidStatus(const struct SaveSectorLocation *);
static u8 CopySaveProfileData(u16, struct SaveSectorLocation *);
static u8 TryWriteSector(u8, u8 *);
static u8 HandleWriteSector(u16, const struct SaveSectorLocation);
static u8 HandleReplaceSector(u16, const struct SaveSectorLocation);

// Divide save blocks into individual chunks to be written to flash sectors

/*
 * Sector Layout:
 *
 * Sectors 0 - 13:      Save Slot 1
 * Sectors 14 - 27:     Save Slot 2
 * Sectors 28 - 29:     Hall of Fame
 * Sector 30:           Trainer Hill
 * Sector 31:           Recorded Battle
 *
 * There are two save slots for saving the player's game data. We alternate between
 * them each time the game is saved, so that if the current save slot is corrupt,
 * we can load the previous one. We also rotate the sectors in each save slot
 * so that the same data is not always being written to the same sector. This
 * might be done to reduce wear on the flash memory, but I'm not sure, since all
 * 14 sectors get written anyway.
 *
 * See SECTOR_ID_* constants in save.h
 */

#define SAVEBLOCK_CHUNK(structure, chunkNum)                                   \
{                                                                              \
    chunkNum * SECTOR_DATA_SIZE,                                               \
    sizeof(structure) >= chunkNum * SECTOR_DATA_SIZE ?                         \
    min(sizeof(structure) - chunkNum * SECTOR_DATA_SIZE, SECTOR_DATA_SIZE) : 0 \
}

struct
{
    u16 offset;
    u16 size;
} static const sSaveSlotLayout[NUM_RAM_SECTORS] =
{
    SAVEBLOCK_CHUNK(struct SaveBlockGeneral, 0), // SECTOR_ID_SAVEBLOCK2

    SAVEBLOCK_CHUNK(struct SaveBlockProfile, 0), // SECTOR_ID_SAVEBLOCK1_START
    SAVEBLOCK_CHUNK(struct SaveBlockProfile, 1),
    SAVEBLOCK_CHUNK(struct SaveBlockProfile, 2),
    SAVEBLOCK_CHUNK(struct SaveBlockProfile, 3), // SECTOR_ID_SAVEBLOCK1_END

    SAVEBLOCK_CHUNK(struct PokemonStorage, 0), // SECTOR_ID_PKMN_STORAGE_START
    SAVEBLOCK_CHUNK(struct PokemonStorage, 1),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 2),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 3),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 4),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 5),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 6),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 7),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 8), 
    SAVEBLOCK_CHUNK(struct PokemonStorage, 9), 
    SAVEBLOCK_CHUNK(struct PokemonStorage, 10), // SECTOR_ID_PKMN_STORAGE_END
};

// These will produce an error if a save struct is larger than the space
// alloted for it in the flash.
STATIC_ASSERT(sizeof(struct SaveBlockGeneral) <= SECTOR_DATA_SIZE, SaveBlock2FreeSpace);
STATIC_ASSERT(sizeof(struct SaveBlockProfile) <= SECTOR_DATA_SIZE * (SECTOR_ID_SAVEBLOCK1_END - SECTOR_ID_SAVEBLOCK1_START + 1), SaveBlock1FreeSpace);
STATIC_ASSERT(sizeof(struct PokemonStorage) <= SECTOR_DATA_SIZE * (SECTOR_ID_PKMN_STORAGE_END - SECTOR_ID_PKMN_STORAGE_START + 1), PokemonStorageFreeSpace);
STATIC_ASSERT(sizeof(struct BoxPokemon) <= 80, BoxPokemonSize);

u32 gLastSaveCounter;
u32 gDamagedSaveSectors;
u32 gSaveCounter;
struct SaveSector *gReadWriteSector; // Pointer to a buffer for reading/writing a sector
u16 gIncrementalSectorId;
u16 gSaveUnusedVar;
u16 gSaveFileStatus;
void (*gGameContinueCallback)(void);
struct SaveSectorLocation gRamSaveSectorLocations[NUM_RAM_SECTORS];
u16 gSaveUnusedVar2;
u16 gSaveAttemptStatus;

EWRAM_DATA struct SaveSector gSaveDataBuffer = {0}; // Buffer used for reading/writing sectors

void ClearSaveData(void)
{
    u16 i;

    // Clear the full save two sectors at a time
    for (i = 0; i < SECTORS_COUNT / 2; i++)
    {
        EraseFlashSector(i);
        EraseFlashSector(i + SECTORS_COUNT / 2);
    }
}

void Save_ResetSaveCounters(void)
{
    gSaveCounter = 0;
    gDamagedSaveSectors = 0;
}

static bool32 SetDamagedSectorBits(u8 op, u8 sectorId)
{
    bool32 retVal = FALSE;

    switch (op)
    {
    case ENABLE:
        gDamagedSaveSectors |= (1 << sectorId);
        break;
    case DISABLE:
        gDamagedSaveSectors &= ~(1 << sectorId);
        break;
    case CHECK: // unused
        if (gDamagedSaveSectors & (1 << sectorId))
            retVal = TRUE;
        break;
    }

    return retVal;
}

static u8 WriteSaveProfile(u16 profileNum)
{
    u32 status;
    u16 i;

    gReadWriteSector = &gSaveDataBuffer;


    if(profileNum >= NUM_PROFILES)
        return SAVE_STATUS_ERROR;
    else
    {
        // Write specified profile.
        gLastSaveCounter = gSaveCounter;
        gSaveCounter++;
        status = SAVE_STATUS_OK;
        profileNum *= NUM_SECTORS_PER_PROFILE;
        profileNum += 1; //start of that specific profile
        HandleWriteSector(SECTOR_ID_SAVEBLOCKGENERAL,gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCKGENERAL]);
        for (i = 0; i < NUM_SECTORS_PER_PROFILE; i++)
            HandleWriteSector(profileNum+i, gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCK1_START+i]);
        for(i=0;i<(SECTOR_ID_PKMN_STORAGE_END-SECTOR_ID_PKMN_STORAGE_START+1);i++)
        HandleWriteSector(SECTOR_ID_PKMN_STORAGE_START+i,gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCK1_END+i+1]);
        if (gDamagedSaveSectors)
        {
            // At least one sector save failed
            status = SAVE_STATUS_ERROR;
            gSaveCounter = gLastSaveCounter;
        }
    }

    return status;
}

static u8 HandleWriteSector(u16 sectorId, const struct SaveSectorLocation location)
{
    u16 i;
    u8 *data;
    u16 size;
    // Get current save data
    data = location.data;
    size = location.size;

    // Clear temp save sector
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    // Set footer data
    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = SECTOR_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    // Copy current data to temp buffer for writing
    for (i = 0; i < size; i++)
        gReadWriteSector->data[i] = data[i];

    gReadWriteSector->checksum = CalculateChecksum(data, size);

    return TryWriteSector(sectorId, gReadWriteSector->data);
}

static u8 HandleWriteSectorNBytes(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *sector = &gSaveDataBuffer;

    // Clear temp save sector
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)sector)[i] = 0;

    sector->signature = SECTOR_SIGNATURE;

    // Copy data to temp buffer for writing
    for (i = 0; i < size; i++)
        sector->data[i] = data[i];

    sector->id = CalculateChecksum(data, size); // though this appears to be incorrect, it might be some sector checksum instead of a whole save checksum and only appears to be relevent to HOF data, if used.
    return TryWriteSector(sectorId, sector->data);
}

static u8 TryWriteSector(u8 sector, u8 *data)
{
    DebugPrintf("Writing sector %d", sector);
    if (ProgramFlashSectorAndVerify(sector, data)) // is damaged?
    {
        // Failed
        SetDamagedSectorBits(ENABLE, sector);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u32 RestoreSaveBackupVarsAndIncrement(const struct SaveSectorLocation *locations)
{
    gReadWriteSector = &gSaveDataBuffer;
    gLastSaveCounter = gSaveCounter;
    gSaveCounter++;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;
    return 0;
}

static u32 RestoreSaveBackupVars(const struct SaveSectorLocation *locations)
{
    gReadWriteSector = &gSaveDataBuffer;
    gLastSaveCounter = gSaveCounter;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;
    return 0;
}

static u8 HandleReplaceSectorAndVerify(u16 sectorId, const struct SaveSectorLocation location)
{
    u8 status = SAVE_STATUS_OK;

    HandleReplaceSector(sectorId, location);

    if (gDamagedSaveSectors)
    {
        status = SAVE_STATUS_ERROR;
        gSaveCounter = gLastSaveCounter;
    }
    return status;
}

// Similar to HandleWriteSector, but fully erases the sector first, and skips writing the first signature byte
static u8 HandleReplaceSector(u16 sectorId, const struct SaveSectorLocation location)
{
    u16 i;
    u8 *data;
    u16 size;
    u8 status;

    // Adjust sector id for current save slot

    // Get current save data
    data = location.data;
    size = location.size;

    // Clear temp save sector.
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    // Set footer data
    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = SECTOR_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    // Copy current data to temp buffer for writing
    for (i = 0; i < size; i++)
        gReadWriteSector->data[i] = data[i];

    gReadWriteSector->checksum = CalculateChecksum(data, size);

    // Erase old save data
    EraseFlashSector(sectorId);

    status = SAVE_STATUS_OK;

    // Write new save data up to signature field
    for (i = 0; i < SECTOR_SIGNATURE_OFFSET; i++)
    {
        if (ProgramFlashByte(sectorId, i, ((u8 *)gReadWriteSector)[i]))
        {
            status = SAVE_STATUS_ERROR;
            break;
        }
    }

    if (status == SAVE_STATUS_ERROR)
    {
        // Writing save data failed
        SetDamagedSectorBits(ENABLE, sectorId);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Writing save data succeeded, write signature and counter
        status = SAVE_STATUS_OK;

        // Write signature (skipping the first byte) and counter fields.
        // The byte of signature that is skipped is instead written by WriteSectorSignatureByte or WriteSectorSignatureByte_NoOffset
        for (i = 0; i < SECTOR_SIZE - (SECTOR_SIGNATURE_OFFSET + 1); i++)
        {
            if (ProgramFlashByte(sectorId, SECTOR_SIGNATURE_OFFSET + 1 + i, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET + 1 + i]))
            {
                status = SAVE_STATUS_ERROR;
                break;
            }
        }

        if (status == SAVE_STATUS_ERROR)
        {
            // Writing signature/counter failed
            SetDamagedSectorBits(ENABLE, sectorId);
            return SAVE_STATUS_ERROR;
        }
        else
        {
            // Succeeded
            SetDamagedSectorBits(DISABLE, sectorId);
            return SAVE_STATUS_OK;
        }
    }
}

static u8 WriteSectorSignatureByte_NoOffset(u16 sectorId)
{
    // Write just the first byte of the signature field, which was skipped by HandleReplaceSector
    if (ProgramFlashByte(sectorId, SECTOR_SIGNATURE_OFFSET, SECTOR_SIGNATURE & 0xFF))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sectorId);
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sectorId);
        return SAVE_STATUS_OK;
    }
}

static u8 CopySectorSignatureByte(u16 sectorId)
{
    // Copy just the first byte of the signature field from the read/write buffer
    if (ProgramFlashByte(sectorId, SECTOR_SIGNATURE_OFFSET, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET]))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sectorId);
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sectorId);
        return SAVE_STATUS_OK;
    }
}

static u8 WriteSectorSignatureByte(u16 sectorId)
{
    // Adjust sector id for current save slot

    // Write just the first byte of the signature field, which was skipped by HandleReplaceSector
    if (ProgramFlashByte(sectorId, SECTOR_SIGNATURE_OFFSET, SECTOR_SIGNATURE & 0xFF))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sectorId);
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sectorId);
        return SAVE_STATUS_OK;
    }
}

static u8 TryLoadSaveProfile(u16 profileId, struct SaveSectorLocation *locations)
{
    u8 status;
    gReadWriteSector = &gSaveDataBuffer;
    if (profileId >= NUM_PROFILES)
    {
        // validate profileId
        status = SAVE_STATUS_ERROR;
    }
    else
    {
        status = GetSaveValidStatus(locations);
        CopySaveProfileData(profileId, locations);
    }

    return status;
}

static u8 CopySaveProfileData(u16 profileId, struct SaveSectorLocation *locations)
{
    u16 i;
    u16 checksum;

    for (i = 0; i < NUM_SECTORS_PER_PROFILE; i++)
    {
        ReadFlashSector(profileId*NUM_SECTORS_PER_PROFILE+SECTOR_ID_SAVEBLOCK1_START+i, gReadWriteSector);
        checksum = CalculateChecksum(gReadWriteSector->data, locations[SECTOR_ID_SAVEBLOCK1_START+i].size);

        // Only copy data for sectors whose signature and checksum fields are correct
        if (gReadWriteSector->signature == SECTOR_SIGNATURE && gReadWriteSector->checksum == checksum)
        {
            u16 j;
            for (j = 0; j < locations[SECTOR_ID_SAVEBLOCK1_START+i].size; j++)
                ((u8 *)locations[SECTOR_ID_SAVEBLOCK1_START+i].data)[j] = gReadWriteSector->data[j];
        }
    }
    for(i=0;i<(SECTOR_ID_PKMN_STORAGE_END-SECTOR_ID_PKMN_STORAGE_START+1);i++)
    {
        ReadFlashSector(SECTOR_ID_PKMN_STORAGE_START+i, gReadWriteSector);
        checksum = CalculateChecksum(gReadWriteSector->data, locations[SECTOR_ID_SAVEBLOCK1_END+i+1].size);

        // Only copy data for sectors whose signature and checksum fields are correct
        if (gReadWriteSector->signature == SECTOR_SIGNATURE && gReadWriteSector->checksum == checksum)
        {
            u16 j;
            for (j = 0; j < locations[SECTOR_ID_SAVEBLOCK1_END+i+1].size; j++)
                ((u8 *)locations[SECTOR_ID_SAVEBLOCK1_END+i+1].data)[j] = gReadWriteSector->data[j];
        }
    }

    return SAVE_STATUS_OK;
}

static u8 CopySaveSectorData(u16 sectorId, struct SaveSectorLocation location)
{
    u16 checksum;
    gReadWriteSector = &gSaveDataBuffer;
    ReadFlashSector(sectorId, gReadWriteSector);
    checksum = CalculateChecksum(gReadWriteSector->data, location.size);
    
    if (gReadWriteSector->signature == SECTOR_SIGNATURE && gReadWriteSector->checksum == checksum)
    {
        u16 j;
        for (j = 0; j < location.size; j++)
        ((u8 *)location.data)[j] = gReadWriteSector->data[j];
    }
    if(gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].profileSaved)
    return SAVE_STATUS_OK;
    return SAVE_STATUS_EMPTY;
}

static u8 GetSaveValidStatus(const struct SaveSectorLocation *locations)
{
    u16 i;
    u16 checksum;
    u32 saveSlotCounter = 0;
    u32 validSectorFlags = 0;
    bool8 signatureValid = FALSE;
    u8 saveSlotStatus;

    for (i = 0; i < SECTOR_ID_HOF_1; i++)
    {
        ReadFlashSector(i, gReadWriteSector);
        if (gReadWriteSector->signature == SECTOR_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gReadWriteSector->data, locations[gReadWriteSector->id].size);
            if (gReadWriteSector->checksum == checksum)
            {
                saveSlotCounter = gReadWriteSector->counter;
                validSectorFlags |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectorFlags == (1 << SECTOR_ID_HOF_1) - 1)
            return SAVE_STATUS_OK;
        else
            return SAVE_STATUS_ERROR;
    }
    else
    {
        // No sectors have the correct signature, treat it as empty
        return SAVE_STATUS_EMPTY;
    }
}

static u8 TryLoadSaveSector(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *sector = &gSaveDataBuffer;
    ReadFlashSector(sectorId, sector);
    if (sector->signature == SECTOR_SIGNATURE)
    {
        u16 checksum = CalculateChecksum(sector->data, size);
        if (sector->id == checksum)
        {
            // Signature and checksum are correct, copy data
            for (i = 0; i < size; i++)
                data[i] = sector->data[i];
            return SAVE_STATUS_OK;
        }
        else
        {
            // Incorrect checksum
            return SAVE_STATUS_CORRUPT;
        }
    }
    else
    {
        // Incorrect signature value
        return SAVE_STATUS_EMPTY;
    }
}

// Return value always ignored
static bool8 ReadFlashSector(u8 sectorId, struct SaveSector *sector)
{
    ReadFlash(sectorId, 0, sector->data, SECTOR_SIZE);
    return TRUE;
}

static u16 CalculateChecksum(void *data, u16 size)
{
    u16 i;
    u32 checksum = 0;

    for (i = 0; i < (size / 4); i++)
    {
        checksum += *((u32 *)data);
        data += sizeof(u32);
    }

    return ((checksum >> 16) + checksum);
}

static void UpdateSaveAddresses(void)
{
    int i = SECTOR_ID_SAVEBLOCKGENERAL;
    gRamSaveSectorLocations[i].data = (void *)(gSaveBlock2Ptr) + sSaveSlotLayout[i].offset;
    gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;

    for (i = SECTOR_ID_SAVEBLOCK1_START; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
    {
        gRamSaveSectorLocations[i].data = (void *)(gSaveBlock1Ptr) + sSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;
    }

    for (; i < NUM_RAM_SECTORS; i++) //setting i to SECTOR_ID_PKMN_STORAGE_START does not match
    {
        gRamSaveSectorLocations[i].data = (void *)(gPokemonStoragePtr) + sSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;
    }
}

u8 HandleSavingData(u8 saveType)
{
    u8 i;
    u32 *backupVar = gTrainerHillVBlankCounter;
    u8 *tempAddr;

    gTrainerHillVBlankCounter = NULL;
    UpdateSaveAddresses();
    switch (saveType)
    {
    case SAVE_HALL_OF_FAME_ERASE_BEFORE:
        // Unused. Erases the special save sectors (HOF, Trainer Hill, Recorded Battle)
        // before overwriting HOF.
        for (i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
            EraseFlashSector(i);
        // fallthrough
    case SAVE_HALL_OF_FAME:
        if (GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
            IncrementGameStat(GAME_STAT_ENTERED_HOF);

        // Write the full save slot first
        CopyPartyAndObjectsToSave();
        WriteSaveProfile(gSaveBlock2Ptr->currentProfile);

        // Save the Hall of Fame
        tempAddr = gDecompressionBuffer;
        HandleWriteSectorNBytes(SECTOR_ID_HOF_1, tempAddr, SECTOR_DATA_SIZE);
        HandleWriteSectorNBytes(SECTOR_ID_HOF_2, tempAddr + SECTOR_DATA_SIZE, SECTOR_DATA_SIZE);
        break;
    case SAVE_NORMAL:
    case SAVE_LINK:
    default:
        CopyPartyAndObjectsToSave();
        WriteSaveProfile(gSaveBlock2Ptr->currentProfile);
        break;
    case SAVE_OVERWRITE_DIFFERENT_FILE:
        // Erase Hall of Fame
        for (i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
            EraseFlashSector(i);

        // Overwrite save slot
        CopyPartyAndObjectsToSave();
        WriteSaveProfile(gSaveBlock2Ptr->currentProfile);
        break;
    }
    gTrainerHillVBlankCounter = backupVar;
    return 0;
}

u8 TrySavingData(u8 saveType)
{
    if (gFlashMemoryPresent != TRUE)
    {
        gSaveAttemptStatus = SAVE_STATUS_ERROR;
        return SAVE_STATUS_ERROR;
    }

    HandleSavingData(saveType);
    if (!gDamagedSaveSectors)
    {
        gSaveAttemptStatus = SAVE_STATUS_OK;
        return SAVE_STATUS_OK;
    }
    else
    {
        DoSaveFailedScreen(saveType);
        gSaveAttemptStatus = SAVE_STATUS_ERROR;
        return SAVE_STATUS_ERROR;
    }
}

bool8 LinkFullSave_Init(void)
{
    if (gFlashMemoryPresent != TRUE)
        return TRUE;
    UpdateSaveAddresses();
    CopyPartyAndObjectsToSave();
    RestoreSaveBackupVarsAndIncrement(gRamSaveSectorLocations);
    return FALSE;
}

bool8 LinkFullSave_WriteSector(void)
{
    u8 status = WriteSaveProfile(gSaveBlock2Ptr->currentProfile);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);

    // In this case "error" either means that an actual error was encountered
    // or that the given max sector has been reached (meaning it has finished successfully).
    // If there was an actual error the save failed screen above will also be shown.
    if (status == SAVE_STATUS_ERROR)
        return TRUE;
    else
        return FALSE;
}

bool8 LinkFullSave_SetLastSectorSignature(void)
{
    CopySectorSignatureByte(SECTOR_ID_PKMN_STORAGE_END);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
    return FALSE;
}

u8 WriteSaveBlock2(void)
{
    if (gFlashMemoryPresent != TRUE)
        return TRUE;

    UpdateSaveAddresses();
    CopyPartyAndObjectsToSave();
    RestoreSaveBackupVars(gRamSaveSectorLocations);
    HandleReplaceSectorAndVerify(SECTOR_ID_SAVEBLOCKGENERAL, gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCKGENERAL]);
    return FALSE;
}

// Used in conjunction with WriteSaveBlock2 to write both for certain link saves.
// This will be called repeatedly in a task, writing each sector of SaveBlockProfile incrementally.
// It returns TRUE when finished.
bool8 WriteSaveBlock1Sector(void)
{
    u8 finished = FALSE;
    if (gIncrementalSectorId < NUM_SECTORS_PER_PROFILE)
    {
        // Write a single sector of SaveBlockProfile
        HandleReplaceSectorAndVerify(SECTOR_ID_SAVEBLOCK1_START+(gSaveBlock2Ptr->currentProfile*NUM_SECTORS_PER_PROFILE)+gIncrementalSectorId, gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCK1_START+gIncrementalSectorId]);
        WriteSectorSignatureByte(SECTOR_ID_SAVEBLOCK1_START+(gSaveBlock2Ptr->currentProfile*NUM_SECTORS_PER_PROFILE)+gIncrementalSectorId);
    }
    else
        finished = TRUE;
    gIncrementalSectorId++;
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_LINK);

    return finished;
}

u8 LoadGameSave(u8 saveType)
{
    u8 status;
    if (gFlashMemoryPresent != TRUE)
    {
        gSaveFileStatus = SAVE_STATUS_NO_FLASH;
        return SAVE_STATUS_ERROR;
    }

    UpdateSaveAddresses();
    switch (saveType)
    {
    case SAVE_NORMAL:
    default:
        status = TryLoadSaveProfile(gSaveBlock2Ptr->currentProfile, gRamSaveSectorLocations);
        CopyPartyAndObjectsFromSave();
        if(gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].profileSaved)
        gSaveFileStatus = SAVE_STATUS_OK;
        else
        gSaveFileStatus = SAVE_STATUS_EMPTY;
        gGameContinueCallback = 0;
        break;
    case SAVE_GENERAL:
        status = CopySaveSectorData(SECTOR_ID_SAVEBLOCKGENERAL, gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCKGENERAL]);
        if(gSaveBlock2Ptr->profileData[gSaveBlock2Ptr->currentProfile].profileSaved)
        gSaveFileStatus = SAVE_STATUS_OK;
        else
        gSaveFileStatus = SAVE_STATUS_EMPTY;
        break;
    case SAVE_HALL_OF_FAME:
        status = TryLoadSaveSector(SECTOR_ID_HOF_1, gDecompressionBuffer, SECTOR_DATA_SIZE);
        if (status == SAVE_STATUS_OK)
            status = TryLoadSaveSector(SECTOR_ID_HOF_2, &gDecompressionBuffer[SECTOR_DATA_SIZE], SECTOR_DATA_SIZE);
        break;
    }

    return status;
}

u16 GetSaveBlocksPointersBaseOffset(void)
{
    u16 i;
    struct SaveSector* sector;

    sector = gReadWriteSector = &gSaveDataBuffer;
    if (gFlashMemoryPresent != TRUE)
        return 0;
    UpdateSaveAddresses();
    GetSaveValidStatus(gRamSaveSectorLocations);
    for (i = 0; i < SECTOR_ID_HOF_1; i++)
    {
        ReadFlashSector(i, gReadWriteSector);

        // Base offset for SaveBlockGeneral is calculated using the trainer id
        if (gReadWriteSector->id == SECTOR_ID_SAVEBLOCKGENERAL)
            return sector->data[offsetof(struct SaveBlockProfile, playerTrainerId[0])] +
                   sector->data[offsetof(struct SaveBlockProfile, playerTrainerId[1])] +
                   sector->data[offsetof(struct SaveBlockProfile, playerTrainerId[2])] +
                   sector->data[offsetof(struct SaveBlockProfile, playerTrainerId[3])];
    }
    return 0;
}

u32 TryReadSpecialSaveSector(u8 sector, u8 *dst)
{
    s32 i;
    s32 size;
    u8 *savData;

    if (sector != SECTOR_ID_RECORDED_BATTLE)
        return SAVE_STATUS_ERROR;

    ReadFlash(sector, 0, (u8 *)&gSaveDataBuffer, SECTOR_SIZE);
    if (*(u32 *)(&gSaveDataBuffer.data[0]) != SPECIAL_SECTOR_SENTINEL)
        return SAVE_STATUS_ERROR;

    // Copies whole save sector except u32 counter
    i = 0;
    size = SECTOR_COUNTER_OFFSET - 1;
    savData = &gSaveDataBuffer.data[4]; // data[4] to skip past SPECIAL_SECTOR_SENTINEL
    for (; i <= size; i++)
        dst[i] = savData[i];
    return SAVE_STATUS_OK;
}

u32 TryWriteSpecialSaveSector(u8 sector, u8 *src)
{
    s32 i;
    s32 size;
    u8 *savData;
    void *savDataBuffer;

    if (sector != SECTOR_ID_RECORDED_BATTLE)
        return SAVE_STATUS_ERROR;

    savDataBuffer = &gSaveDataBuffer;
    *(u32 *)(savDataBuffer) = SPECIAL_SECTOR_SENTINEL;

    // Copies whole save sector except u32 counter
    i = 0;
    size = SECTOR_COUNTER_OFFSET - 1;
    savData = &gSaveDataBuffer.data[4]; // data[4] to skip past SPECIAL_SECTOR_SENTINEL
    for (; i <= size; i++)
        savData[i] = src[i];
    if (ProgramFlashSectorAndVerify(sector, savDataBuffer) != 0)
        return SAVE_STATUS_ERROR;
    return SAVE_STATUS_OK;
}

#define tState         data[0]
#define tTimer         data[1]
#define tInBattleTower data[2]

// Note that this is very different from TrySavingData(SAVE_LINK).
// Most notably it does save the PC data.
void Task_LinkFullSave(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        gSoftResetDisabled = TRUE;
        tState = 1;
        break;
    case 1:
        SetLinkStandbyCallback();
        tState = 2;
        break;
    case 2:
        if (IsLinkTaskFinished())
        {
            if (!tInBattleTower)
                SaveMapView();
            tState = 3;
        }
        break;
    case 3:
        if (!tInBattleTower)
            SetContinueGameWarpStatusToDynamicWarp();
        LinkFullSave_Init();
        tState = 4;
        break;
    case 4:
        if (++tTimer == 5)
        {
            tTimer = 0;
            tState = 5;
        }
        break;
    case 5:
        if (LinkFullSave_WriteSector())
            tState = 6;
        else
            tState = 4; // Not finished, delay again
        break;
    case 6:
        tState = 7;
        break;
    case 7:
        if (!tInBattleTower)
            ClearContinueGameWarpStatus2();
        SetLinkStandbyCallback();
        tState = 8;
        break;
    case 8:
        if (IsLinkTaskFinished())
        {
            LinkFullSave_SetLastSectorSignature();
            tState = 9;
        }
        break;
    case 9:
        SetLinkStandbyCallback();
        tState = 10;
        break;
    case 10:
        if (IsLinkTaskFinished())
            tState++;
        break;
    case 11:
        if (++tTimer > 5)
        {
            gSoftResetDisabled = FALSE;
            DestroyTask(taskId);
        }
        break;
    }
}
