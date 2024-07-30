#include "global.h"
#include "challenge_menu.h"
#include "bg.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"
#include "string_util.h"
#include "event_data.h"
#include "constants/species.h"
#include "pokemon.h"

#define tLimitedEncounters currentSettings[0]
#define tSpeciesClause currentSettings[1]
#define tPermaDeath currentSettings[2]
#define tForceSetMode currentSettings[3]
#define tInfiniteCandy currentSettings[4]
#define tRepellant currentSettings[5]
#define tEscapeRope currentSettings[6]
#define tFieldMoves currentSettings[7]
#define tGrassStarter currentSettings[8]
#define tWaterStarter currentSettings[9]
#define tFireStarter currentSettings[10]
#define tStarterAffectsRival currentSettings[11]
#define tLevelCap currentSettings[12]
#define tGauntletMode currentSettings[13]
#define tNoBattleItems currentSettings[13]
#define tXPMultiplier currentSettings[15]
#define tMenuSelection currentSettings[16]

// Menu items Pg1
enum
{
    MENUITEM_LIMITEDENCOUNTERS,
    MENUITEM_SPECIESCLAUSE,
    MENUITEM_PERMADEATH,
    MENUITEM_FORCESETMODE,
    MENUITEM_CONFIRM,
    MENUITEM_COUNT,
};

// Menu items Pg2
enum
{
    MENUITEM_INFINITECANDY,
    MENUITEM_REPELLANT,
    MENUITEM_ESCAPEROPE,
    MENUITEM_FIELDMOVES,
    MENUITEM_CONFIRM_PG2,
    MENUITEM_COUNT_PG2,
};

// Menu items Pg3
enum
{
    MENUITEM_GRASSSTARTER,
    MENUITEM_WATERSTARTER,
    MENUITEM_FIRESTARTER,
    MENUITEM_STARTERAFFECTSRIVAL,
    MENUITEM_CONFIRM_PG3,
    MENUITEM_COUNT_PG3,
};

// Menu items Pg4
enum
{
    MENUITEM_LEVELCAP,
    MENUITEM_GAUNTLETMODE,
    MENUITEM_XPMULTIPLIER,
    MENUITEM_NOBATTLEITEMS,
    MENUITEM_CONFIRM_PG4,
    MENUITEM_COUNT_PG4,
};

enum
{
    WIN_HEADER,
    WIN_CHALLENGES,
};
//Pg 1
#define YPOS_LIMITEDENCOUNTERS  (MENUITEM_LIMITEDENCOUNTERS * 16)
#define YPOS_SPECIESCLAUSE        (MENUITEM_SPECIESCLAUSE * 16)
#define YPOS_PERMADEATH  (MENUITEM_PERMADEATH * 16)
#define YPOS_FORCESETMODE    (MENUITEM_FORCESETMODE * 16)

//Pg2
#define YPOS_INFINITECANDY      (MENUITEM_INFINITECANDY * 16)
#define YPOS_REPELLANT      (MENUITEM_REPELLANT * 16)
#define YPOS_ESCAPE_ROPE      (MENUITEM_ESCAPEROPE * 16)
#define YPOS_FIELD_MOVES      (MENUITEM_FIELDMOVES * 16)

//Pg3
#define YPOS_GRASSSTARTER        (MENUITEM_GRASSSTARTER* 16)
#define YPOS_FIRESTARTER      (MENUITEM_FIRESTARTER * 16)
#define YPOS_WATERSTARTER      (MENUITEM_WATERSTARTER * 16)
#define YPOS_STARTERAFFECTSRIVAL      (MENUITEM_STARTERAFFECTSRIVAL * 16)

//Pg4
#define YPOS_LEVELCAP    (MENUITEM_LEVELCAP * 16)
#define YPOS_GAUNTLETMODE   (MENUITEM_GAUNTLETMODE * 16)
#define YPOS_NOBATTLEITEMS        (MENUITEM_NOBATTLEITEMS * 16)
#define YPOS_XPMULTIPLIER      (MENUITEM_XPMULTIPLIER * 16)

#define MAX_XP_MULTIPLIER 25
#define PAGE_COUNT  4

static void Task_ChallengeMenuFadeIn(u8 taskId);
static void Task_ChallengeMenuProcessInput(u8 taskId);
static void Task_ChallengeMenuSave(u8 taskId);
static void Task_ChallengeMenuFadeOut(u8 taskId);
static void HighlightChallengeMenuItem(u8 index);
static void LimitedEncounters_DrawChoices(u16 selection);
static void SpeciesClause_DrawChoices(u16 selection);
static void PermaDeath_DrawChoices(u16 selection);
static void ForceSetMode_DrawChoices(u16 selection);
static void InfiniteCandy_DrawChoices(u16 selection);
static void Repellant_DrawChoices(u16 selection);
static void EscapeRope_DrawChoices(u16 selection);
static void FieldMoves_DrawChoices(u16 selection);
static void GrassStarter_DrawChoices(u16 selection);
static void FireStarter_DrawChoices(u16 selection);
static void WaterStarter_DrawChoices(u16 selection);
static void StarterAffectsRival_DrawChoices(u16 selection);
static void LevelCap_DrawChoices(u16 selection);
static void GauntletMode_DrawChoices(u16 selection);
static void NoBattleItems_DrawChoices(u16 selection);
static void XPMultiplier_DrawChoices(u16 selection);
static void DrawHeaderText(u8 taskId);
static void DrawChallengeMenuTexts(void);
static void DrawBgWindowFrames(void);

EWRAM_DATA static bool8 sArrowPressed = FALSE;
EWRAM_DATA static u8 sCurrPage = 0;
EWRAM_DATA static u8 isInDetails = 0;
EWRAM_DATA static u16 currentSettings[4*PAGE_COUNT];

static const u16 sChallengeMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text.gbapal");

static const u8 *const sChallengeMenuItemsNames_Pg1[MENUITEM_COUNT] =
{
    [MENUITEM_PERMADEATH]         = gText_PermaDeath,
    [MENUITEM_SPECIESCLAUSE]      = gText_SpeciesClause,
    [MENUITEM_LIMITEDENCOUNTERS]  = gText_LimitedEncounters,
    [MENUITEM_FORCESETMODE]       = gText_ForceSetMode,
    [MENUITEM_CONFIRM]            = gText_Confirm2,
};

static const u8 *const sChallengeMenuItemsNames_Pg2[MENUITEM_COUNT_PG2] =
{
    [MENUITEM_INFINITECANDY]      = gText_InfiniteCandy,
    [MENUITEM_REPELLANT]          = gText_Repellant,
    [MENUITEM_ESCAPEROPE]         = gText_EscapeRope,
    [MENUITEM_FIELDMOVES]         = gText_FieldMoves,
    [MENUITEM_CONFIRM_PG2]        = gText_Confirm2,
};

static const u8 *const sChallengeMenuItemsNames_Pg3[MENUITEM_COUNT_PG3] =
{
    [MENUITEM_GRASSSTARTER]       = gText_GrassStarter,
    [MENUITEM_WATERSTARTER]       = gText_WaterStarter,
    [MENUITEM_FIRESTARTER]        = gText_FireStarter,
    [MENUITEM_STARTERAFFECTSRIVAL]= gText_StarterAffectsRival,
    [MENUITEM_CONFIRM_PG3]        = gText_Confirm2,
};

static const u8 *const sChallengeMenuItemsNames_Pg4[MENUITEM_COUNT_PG4] =
{
    [MENUITEM_LEVELCAP]           = gText_LevelCap,
    [MENUITEM_GAUNTLETMODE]       = gText_GauntletMode,
    [MENUITEM_NOBATTLEITEMS]      = gText_NoBattleItems,
    [MENUITEM_XPMULTIPLIER]       = gText_XPMultiplier,
    [MENUITEM_CONFIRM_PG4]        = gText_Confirm2,
};

static const struct WindowTemplate sChallengeMenuWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 6,
        .paletteNum = 1,
        .baseBlock = 2
    },
    [WIN_CHALLENGES] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 9,
        .width = 26,
        .height = 10,
        .paletteNum = 1,
        .baseBlock = 158
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sChallengeMenuBgTemplates[] =
{
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const u16 sChallengeMenuBg_Pal[] = {RGB(17, 18, 31)};

//number of selections for each challenge.
static const u16 maxValue[PAGE_COUNT*(MENUITEM_COUNT-1)] =
{
    [MENUITEM_LIMITEDENCOUNTERS] = 2,
    [MENUITEM_SPECIESCLAUSE] = 2,
    [MENUITEM_PERMADEATH] = 2,
    [MENUITEM_FORCESETMODE] = 2,
    [MENUITEM_INFINITECANDY+4] = 2,
    [MENUITEM_REPELLANT+4] = 2,
    [MENUITEM_ESCAPEROPE+4] = 2,
    [MENUITEM_FIELDMOVES+4] = 2,
    [MENUITEM_GRASSSTARTER+8] = 386,
    [MENUITEM_WATERSTARTER+8] = 386,
    [MENUITEM_FIRESTARTER+8] = 386,
    [MENUITEM_STARTERAFFECTSRIVAL+8] = 2,
    [MENUITEM_LEVELCAP+12] = 3,
    [MENUITEM_GAUNTLETMODE+12] = 2,
    [MENUITEM_NOBATTLEITEMS+12] = 2,
    [MENUITEM_XPMULTIPLIER+12] = 25,
};

static const u8 sChallengeDetailTexts[PAGE_COUNT*(MENUITEM_COUNT-1)][120] =
{
    [MENUITEM_LIMITEDENCOUNTERS] = _("You can only catch the first\nencounter per Location. Underwater\nRoutes not counted separately"),
    [MENUITEM_SPECIESCLAUSE] = _("Key item with Repel functionality"),
    [MENUITEM_PERMADEATH] = _("Key item with Repel functionality"),
    [MENUITEM_FORCESETMODE] = _("Key item with Repel functionality"),
    [MENUITEM_INFINITECANDY+4] = _("Key item with Repel functionality"),
    [MENUITEM_REPELLANT+4] = _("Key item with Repel functionality"),
    [MENUITEM_ESCAPEROPE+4] = _("Key item with Repel functionality"),
    [MENUITEM_FIELDMOVES+4] = _("Key item with Repel functionality"),
    [MENUITEM_GRASSSTARTER+8] = _("Key item with Repel functionality"),
    [MENUITEM_WATERSTARTER+8] = _("Key item with Repel functionality"),
    [MENUITEM_FIRESTARTER+8] = _("Key item with Repel functionality"),
    [MENUITEM_STARTERAFFECTSRIVAL+8] = _("Key item with Repel functionality"),
    [MENUITEM_LEVELCAP+12] = _("Key item with Repel functionality"),
    [MENUITEM_GAUNTLETMODE+12] = _("Key item with Repel functionality"),
    [MENUITEM_NOBATTLEITEMS+12] = _("Key item with Repel functionality"),
    [MENUITEM_XPMULTIPLIER+12] = _("Key item with Repel functionality")
};

static void ReadAllCurrentSettings(u8 taskId)
{
    tMenuSelection = 0;
    tLimitedEncounters = gSaveBlock2Ptr->challenges.limitedEncounters;
    tSpeciesClause = gSaveBlock2Ptr->challenges.speciesClause;
    tPermaDeath = gSaveBlock2Ptr->challenges.permaDeath;
    tForceSetMode = gSaveBlock2Ptr->challenges.forceSetMode;
    tInfiniteCandy = gSaveBlock2Ptr->challenges.infiniteCandy;
    tEscapeRope = gSaveBlock2Ptr->challenges.escapeRope;
    tRepellant = gSaveBlock2Ptr->challenges.repellant;
    tFieldMoves = gSaveBlock2Ptr->challenges.fieldMoves;
    tGrassStarter = gSaveBlock2Ptr->challenges.grassStarter;
    tWaterStarter = gSaveBlock2Ptr->challenges.waterStarter;
    tFireStarter = gSaveBlock2Ptr->challenges.fireStarter;
    tStarterAffectsRival = gSaveBlock2Ptr->challenges.starterAffectsRival;
    tLevelCap = gSaveBlock2Ptr->challenges.levelCap;
    tGauntletMode = gSaveBlock2Ptr->challenges.gauntletMode;
    tNoBattleItems = gSaveBlock2Ptr->challenges.noBattleItems;
    tXPMultiplier = gSaveBlock2Ptr-> challenges.xpMultiplier;
}

static void DrawChallengesPg1(u8 taskId)
{  
    ReadAllCurrentSettings(taskId);
    LimitedEncounters_DrawChoices(tLimitedEncounters);
    SpeciesClause_DrawChoices(tSpeciesClause);
    PermaDeath_DrawChoices(tPermaDeath);
    ForceSetMode_DrawChoices(tForceSetMode);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void DrawChallengesPg2(u8 taskId)
{
    ReadAllCurrentSettings(taskId);
    InfiniteCandy_DrawChoices(tInfiniteCandy);
    Repellant_DrawChoices(tRepellant);
    EscapeRope_DrawChoices(tEscapeRope);
    FieldMoves_DrawChoices(tFieldMoves);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void DrawChallengesPg3(u8 taskId)
{  
    ReadAllCurrentSettings(taskId);
    GrassStarter_DrawChoices(tGrassStarter);
    WaterStarter_DrawChoices(tWaterStarter);
    FireStarter_DrawChoices(tFireStarter);
    StarterAffectsRival_DrawChoices(tStarterAffectsRival);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void DrawChallengesPg4(u8 taskId)
{
    ReadAllCurrentSettings(taskId);
    LevelCap_DrawChoices(tLevelCap);
    GauntletMode_DrawChoices(tGauntletMode);
    NoBattleItems_DrawChoices(tNoBattleItems);
    XPMultiplier_DrawChoices(tXPMultiplier);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void (*const sDrawChallenges[PAGE_COUNT])(u8 taskId) = {DrawChallengesPg1,DrawChallengesPg2,DrawChallengesPg3,DrawChallengesPg4};
static void (*const sDrawChoices[PAGE_COUNT][MENUITEM_COUNT-1])(u16 selection) = 
{
    {LimitedEncounters_DrawChoices,SpeciesClause_DrawChoices,PermaDeath_DrawChoices,ForceSetMode_DrawChoices},
    {InfiniteCandy_DrawChoices,Repellant_DrawChoices,EscapeRope_DrawChoices,FieldMoves_DrawChoices},
    {GrassStarter_DrawChoices,WaterStarter_DrawChoices,FireStarter_DrawChoices,StarterAffectsRival_DrawChoices},
    {LevelCap_DrawChoices,GauntletMode_DrawChoices,NoBattleItems_DrawChoices,XPMultiplier_DrawChoices},
};

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitChallengeMenu(void)
{
    u8 taskId;
    DebugPrintf("Initializing challenge menu, state is %d", gMain.state);
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sChallengeMenuBgTemplates, ARRAY_COUNT(sChallengeMenuBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);
        InitWindows(sChallengeMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sChallengeMenuBg_Pal, BG_PLTT_ID(0), sizeof(sChallengeMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sChallengeMenuText_Pal, BG_PLTT_ID(1), sizeof(sChallengeMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_HEADER);
        DrawHeaderText(taskId);
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_CHALLENGES);
        DrawChallengeMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
        taskId = CreateTask(Task_ChallengeMenuFadeIn, 0);
        sDrawChallenges[sCurrPage](taskId);
        gMain.state++;
        break;
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static u8 Process_ChangePage(u8 CurrentPage)
{
    if (JOY_NEW(R_BUTTON))
    {
        if (CurrentPage < PAGE_COUNT - 1)
            CurrentPage++;
        else
            CurrentPage = 0;
    }
    if (JOY_NEW(L_BUTTON))
    {
        if (CurrentPage != 0)
            CurrentPage--;
        else
            CurrentPage = PAGE_COUNT - 1;
    }
    return CurrentPage;
}

static void save(u8 taskId)
{
    gSaveBlock2Ptr->challenges.limitedEncounters = tLimitedEncounters;
    gSaveBlock2Ptr->challenges.speciesClause = tSpeciesClause;
    gSaveBlock2Ptr->challenges.permaDeath = tPermaDeath;
    gSaveBlock2Ptr->challenges.forceSetMode = tForceSetMode;
    gSaveBlock2Ptr->challenges.infiniteCandy = tInfiniteCandy;
    gSaveBlock2Ptr->challenges.repellant = tRepellant;
    gSaveBlock2Ptr->challenges.escapeRope = tEscapeRope;
    gSaveBlock2Ptr->challenges.fieldMoves = tFieldMoves;
    //in case of page 2 never being called, we have to set starters to default here
    if(gSaveBlock2Ptr->challenges.grassStarter == SPECIES_NONE)
    {
        gSaveBlock2Ptr->challenges.grassStarter = SpeciesToNationalPokedexNum(SPECIES_TREECKO);
    }
    if(gSaveBlock2Ptr->challenges.waterStarter == SPECIES_NONE)
    {
        gSaveBlock2Ptr->challenges.waterStarter = SpeciesToNationalPokedexNum(SPECIES_MUDKIP);
    }
    if(gSaveBlock2Ptr->challenges.fireStarter == SPECIES_NONE)
    {
        gSaveBlock2Ptr->challenges.fireStarter = SpeciesToNationalPokedexNum(SPECIES_TORCHIC);
    }
    gSaveBlock2Ptr->challenges.starterAffectsRival = tStarterAffectsRival;
    gSaveBlock2Ptr->challenges.levelCap = tLevelCap;
    gSaveBlock2Ptr->challenges.gauntletMode = tGauntletMode;
    gSaveBlock2Ptr->challenges.noBattleItems = tNoBattleItems;
    gSaveBlock2Ptr->challenges.xpMultiplier = tXPMultiplier;
    return;
}

static void Task_ChangePage(u8 taskId)
{
    save(taskId);
    tMenuSelection = 0;
    DrawHeaderText(taskId);
    PutWindowTilemap(1);
    DrawChallengeMenuTexts();
    sDrawChallenges[sCurrPage](taskId);
    gTasks[taskId].func = Task_ChallengeMenuFadeIn;
}

static void Task_ChallengeMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_ChallengeMenuProcessInput;
}

static void Task_ChallengeMenuProcessInput(u8 taskId)
{
    if ((JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON)))
    {
        FillWindowPixelBuffer(WIN_CHALLENGES, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_CHALLENGES, FALSE);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        if (tMenuSelection == MENUITEM_CONFIRM)
        {
            gTasks[taskId].func = Task_ChallengeMenuSave;
        } else
        {
        if (!isInDetails)
            isInDetails = 1;
        else 
            isInDetails = 0;
        }
        sArrowPressed = TRUE;
        DrawHeaderText(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        if(isInDetails)
        {
            isInDetails = 0;
            sArrowPressed = TRUE;
            DrawHeaderText(taskId);
        } else
        {
            gTasks[taskId].func = Task_ChallengeMenuSave;
        }
    }
    else if (JOY_REPEAT(DPAD_UP))
    {
        if (tMenuSelection > 0)
            tMenuSelection--;
        else
            tMenuSelection = MENUITEM_CONFIRM;
        HighlightChallengeMenuItem(tMenuSelection);
        if(isInDetails) 
        {
            sArrowPressed = TRUE;
            DrawHeaderText(taskId);
        }
    }
    else if (JOY_REPEAT(DPAD_DOWN))
    {
        if (tMenuSelection < MENUITEM_CONFIRM)
            tMenuSelection++;
        else
            tMenuSelection = 0;
        HighlightChallengeMenuItem(tMenuSelection);
        if (isInDetails)
        {
            sArrowPressed = TRUE;
            DrawHeaderText(taskId);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        u8 previousChallenge = currentSettings[(sCurrPage * 4) + tMenuSelection];
        if (currentSettings[(sCurrPage * 4) + tMenuSelection] < maxValue[(sCurrPage * 4) + tMenuSelection])
            currentSettings[(sCurrPage * 4) + tMenuSelection]++;
        else
            currentSettings[(sCurrPage * 4) + tMenuSelection] = 0;

        if (previousChallenge != currentSettings[(sCurrPage * 4) + tMenuSelection])
            sDrawChoices[sCurrPage][tMenuSelection](currentSettings[(sCurrPage * 4) + tMenuSelection]);
        sArrowPressed = TRUE;
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        u8 previousChallenge = currentSettings[(sCurrPage * 4) + tMenuSelection];
        if (currentSettings[(sCurrPage * 4) + tMenuSelection] != 0)
            currentSettings[(sCurrPage * 4) + tMenuSelection]--;
        else
            currentSettings[(sCurrPage * 4) + tMenuSelection] = maxValue[(sCurrPage * 4) + tMenuSelection];

        if (previousChallenge != currentSettings[(sCurrPage * 4) + tMenuSelection])
            sDrawChoices[sCurrPage][tMenuSelection](currentSettings[(sCurrPage * 4) + tMenuSelection]);
        sArrowPressed = TRUE;
    }
    if (sArrowPressed)
    {
        sArrowPressed = FALSE;
        CopyWindowToVram(WIN_CHALLENGES, COPYWIN_GFX);
    }
}

static void Task_ChallengeMenuSave(u8 taskId)
{
    save(taskId);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_ChallengeMenuFadeOut;
}

static void Task_ChallengeMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void HighlightChallengeMenuItem(u8 index)
{
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(16, DISPLAY_WIDTH - 16));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(index * 16 + 72, index * 16 + 88));
}

static void DrawChallengeMenuChoice(const u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[16];
    u16 i;
    for (i = 0; *text != EOS && i < ARRAY_COUNT(dst) - 1; i++)
        dst[i] = *(text++);

    if (style != 0)
    {
        dst[2] = TEXT_COLOR_RED;
        dst[5] = TEXT_COLOR_LIGHT_RED;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_CHALLENGES, FONT_NORMAL, dst, x, y + 1, TEXT_SKIP_DRAW, NULL);
}

static void ChallengesDraw2Choices(u16 selection, const u8* leftText, const u8* rightText, u8 ypos)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawChallengeMenuChoice(leftText, 104, ypos, styles[0]);
    DrawChallengeMenuChoice(rightText, GetStringRightAlignXOffset(FONT_NORMAL, rightText, 198), ypos, styles[1]);
}

static void ChallengesDraw3Choices(u16 selection,const u8* leftText, const u8* middleText, const u8* rightText, u8 ypos)
{
    u8 styles[3];
    s32 widthLeft, widthMid, widthRight, xMid;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;


    widthLeft = GetStringWidth(FONT_NORMAL, gText_TextSpeedSlow, 0);
    widthMid = GetStringWidth(FONT_NORMAL, gText_TextSpeedMid, 0);
    widthRight = GetStringWidth(FONT_NORMAL, gText_TextSpeedFast, 0);

    widthMid -= 94;
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    DrawChallengeMenuChoice(leftText, 104, ypos, styles[0]);
    DrawChallengeMenuChoice(middleText, xMid, ypos, styles[1]);
    DrawChallengeMenuChoice(rightText, GetStringRightAlignXOffset(FONT_NORMAL, gText_TextSpeedFast, 198), ypos, styles[2]);
}

static void LimitedEncounters_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_LIMITEDENCOUNTERS);
}

static void SpeciesClause_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_SPECIESCLAUSE);
}

static void PermaDeath_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_PERMADEATH);
}

static void ForceSetMode_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_FORCESETMODE);
}

static void InfiniteCandy_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_INFINITECANDY);
}

static void Repellant_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_REPELLANT);
}

static void EscapeRope_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_ESCAPE_ROPE);
}

static void FieldMoves_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_FIELD_MOVES);
}

static void GrassStarter_DrawChoices(u16 selection)
{
    u8 text[19];
    u8 speciesName[POKEMON_NAME_LENGTH + 1];
    u8 i = 0;
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];
    if(selection / 100)
    {
        text[i++] = selection / 100 + CHAR_0;
    } else
    {
       text[i++] = CHAR_0; 
    }
    if(selection / 10)
    {
        text[i++] = (selection / 10)%10 + CHAR_0;
    }else
    {
       text[i++] = CHAR_0; 
    }
    text[i++] = selection % 10 + CHAR_0;
    text[i++] = CHAR_COLON;
    text[i] = EOS;
    GetSpeciesName(speciesName,NationalPokedexNumToSpecies(selection));
    StringAppend(text,speciesName);
    DrawChallengeMenuChoice(text, 128, YPOS_GRASSSTARTER, 1);
}

static void WaterStarter_DrawChoices(u16 selection)
{
    u8 text[19];
    u8 speciesName[POKEMON_NAME_LENGTH + 1];
    u8 i = 0;
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];
    if(selection / 100)
    {
        text[i++] = selection / 100 + CHAR_0;
    } else
    {
       text[i++] = CHAR_0; 
    }
    if(selection / 10)
    {
        text[i++] = (selection / 10)%10 + CHAR_0;
    }else
    {
       text[i++] = CHAR_0; 
    }
    text[i++] = selection % 10 + CHAR_0;
    text[i++] = CHAR_COLON;
    text[i] = EOS;
    GetSpeciesName(speciesName,NationalPokedexNumToSpecies(selection));
    StringAppend(text,speciesName);
    DrawChallengeMenuChoice(text, 128, YPOS_WATERSTARTER, 1);
}

static void FireStarter_DrawChoices(u16 selection)
{
    u8 text[19];
    u8 speciesName[POKEMON_NAME_LENGTH + 1];
    u8 i = 0;
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];
    if(selection / 100)
    {
        text[i++] = selection / 100 + CHAR_0;
    } else
    {
       text[i++] = CHAR_0; 
    }
    if(selection / 10)
    {
        text[i++] = (selection / 10)%10 + CHAR_0;
    }else
    {
       text[i++] = CHAR_0; 
    }
    text[i++] = selection % 10 + CHAR_0;
    text[i++] = CHAR_COLON;
    text[i] = EOS;
    GetSpeciesName(speciesName,NationalPokedexNumToSpecies(selection));
    StringAppend(text,speciesName);
    DrawChallengeMenuChoice(text, 128, YPOS_FIRESTARTER, 1);
}

static void StarterAffectsRival_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_STARTERAFFECTSRIVAL);
}

static void LevelCap_DrawChoices(u16 selection)
{
    ChallengesDraw3Choices(selection, gText_ChallengesNo,gText_ChallengesYes,gText_Extreme,YPOS_FIELD_MOVES);
}

static void GauntletMode_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_GAUNTLETMODE);
}

static void NoBattleItems_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS_NOBATTLEITEMS);
}

static void XPMultiplier_DrawChoices(u16 selection)
{
    u8 text[18];
    u8 n = selection;
    u16 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];
    text[i++] = CHAR_x;
    if (n / 10 != 0)
    {
        text[i++] = n / 10 + CHAR_0;
        text[i++] = CHAR_PERIOD;
        text[i++] = n % 10 + CHAR_0;
    }
    else
    {
        text[i++] = CHAR_0;
        text[i++] = CHAR_PERIOD;
        text[i++] = n % 10 + CHAR_0;
    }

    text[i] = EOS;
    DrawChallengeMenuChoice(text, 128, YPOS_XPMULTIPLIER, 1);
}

static void DrawHeaderText(u8 taskId)
{
    u32 i, widthChallenges, xMid;
    u8 pageDots[9] = _(""); // Array size should be at least (2 * PAGE_COUNT) -1
    widthChallenges = GetStringWidth(FONT_NORMAL, gText_Challenge, 0);
    FillWindowPixelBuffer(WIN_HEADER, PIXEL_FILL(1));
    if (!isInDetails)
    {

        for (i = 0; i < PAGE_COUNT; i++)
        {
            if (i == sCurrPage)
                StringAppend(pageDots, gText_LargeDot);
            else
                StringAppend(pageDots, gText_SmallDot);
            if (i < PAGE_COUNT - 1)
                StringAppend(pageDots, gText_Space);
        }
        xMid = (8 + widthChallenges + 5);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_Challenge, 8, 1, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, pageDots, xMid, 1, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav, GetStringRightAlignXOffset(FONT_NORMAL, gText_PageNav, 198), 1, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_ChallengeDetails, 8, 17, TEXT_SKIP_DRAW, NULL);
        CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
    }
    else
    {
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, sChallengeDetailTexts[(sCurrPage * 4) + tMenuSelection], 8, 1, TEXT_SKIP_DRAW, NULL);
        CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
    }
}

static void DrawChallengeMenuTexts(void)
{
    u8 i, items;
    const u8* const* menu = NULL;

    switch (sCurrPage){
    case 0:
        items = MENUITEM_COUNT;
        menu = sChallengeMenuItemsNames_Pg1;
        break;
    case 1:
        items = MENUITEM_COUNT_PG2;
        menu = sChallengeMenuItemsNames_Pg2;
        break;    
    case 2:
        items = MENUITEM_COUNT;
        menu = sChallengeMenuItemsNames_Pg3;
        break;
    case 3:
        items = MENUITEM_COUNT_PG2;
        menu = sChallengeMenuItemsNames_Pg2;
        break;    
    }

    FillWindowPixelBuffer(WIN_CHALLENGES, PIXEL_FILL(1));
    for (i = 0; i < items; i++)
        AddTextPrinterParameterized(WIN_CHALLENGES, FONT_NORMAL, menu[i], 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  6,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  6,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  7,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  7, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  7,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  8,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  8, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  8,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  9,  1, 14,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  9,  1, 14,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
