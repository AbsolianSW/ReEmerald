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

#define ROWS_OPTIONS  5
#define ROWS_HEADER  3
#define CONFIRM_INDEX  4
#define PAGE_COUNT  4
#define TILEMAP_WIDTH 30
#define TILEMAP_HEIGHT 20
#define DETAIL_TEXT_LENGTH 120
#define YPOS(index)  ((index%(ROWS_OPTIONS-1)) * 16)

#define TILE_TOP_CORNER_L 2
#define TILE_TOP_EDGE     (TILE_TOP_CORNER_L + 1)
#define TILE_TOP_CORNER_R (TILE_TOP_CORNER_L + 2)
#define TILE_LEFT_EDGE    (TILE_TOP_CORNER_L + 3)
#define TILE_RIGHT_EDGE   (TILE_TOP_CORNER_L + 5)
#define TILE_BOT_CORNER_L (TILE_TOP_CORNER_L + 6)
#define TILE_BOT_EDGE     (TILE_TOP_CORNER_L + 7)
#define TILE_BOT_CORNER_R (TILE_TOP_CORNER_L + 8)
#define TILE_NONE         (TILE_TOP_CORNER_L + 9)

#define tLimitedEncounters currentSettings[0]
#define tSpeciesClause currentSettings[1]
#define tPermaDeath currentSettings[2]
#define tForceSetMode currentSettings[3]
#define tInfiniteCandy currentSettings[4]
#define tRepellant currentSettings[5]
#define tStartingMoney currentSettings[6]
#define tShinyOdds currentSettings[7]
#define tGrassStarter currentSettings[8]
#define tWaterStarter currentSettings[9]
#define tFireStarter currentSettings[10]
#define tStarterAffectsRival currentSettings[11]
#define tLevelCap currentSettings[12]
#define tGauntletMode currentSettings[13]
#define tNoBattleItems currentSettings[13]
#define tXPMultiplier currentSettings[15]
#define tMenuSelection currentSettings[16]
enum
{
    MENUITEM_LIMITEDENCOUNTERS,
    MENUITEM_SPECIESCLAUSE,
    MENUITEM_PERMADEATH,
    MENUITEM_FORCESETMODE,
    MENUITEM_INFINITECANDY,
    MENUITEM_REPELLANT,
    MENUITEM_STARTINGMONEY,
    MENUITEM_SHINYODDS,
    MENUITEM_GRASSSTARTER,
    MENUITEM_WATERSTARTER,
    MENUITEM_FIRESTARTER,
    MENUITEM_STARTERAFFECTSRIVAL,
    MENUITEM_LEVELCAP,
    MENUITEM_GAUNTLETMODE,
    MENUITEM_NOBATTLEITEMS,
    MENUITEM_XPMULTIPLIER,
};

enum
{
    WIN_HEADER,
    WIN_CHALLENGES,
    WIN_CONFIRM_YESNO,
    WIN_CONFIRM,
};


static void Task_ChallengeMenuFadeIn(u8 taskId);
static void Task_ChallengeMenuProcessInput(u8 taskId);
static void Task_ChallengeMenuSave(u8 taskId);
static void Task_ChallengeMenuConfirm(u8 taskId);
static void Task_ChallengeMenuFadeOut(u8 taskId);
static void Task_ChallengeMenuConfirmProcessInput(u8 taskId);
static void HighlightChallengeMenuItem(u8 index);
static void LimitedEncounters_DrawChoices(u16 selection);
static void SpeciesClause_DrawChoices(u16 selection);
static void PermaDeath_DrawChoices(u16 selection);
static void ForceSetMode_DrawChoices(u16 selection);
static void InfiniteCandy_DrawChoices(u16 selection);
static void Repellant_DrawChoices(u16 selection);
static void StartingMoney_DrawChoices(u16 selection);
static void Shinyodds_DrawChoices(u16 selection);
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
static void DrawBgWindowFramesConfirm(void);

EWRAM_DATA static bool8 sArrowPressed = FALSE;
EWRAM_DATA static u8 sCurrPage = 0;
EWRAM_DATA static u8 isInDetails = 0;
EWRAM_DATA static u16 currentSettings[4*PAGE_COUNT+1];

static const u16 sChallengeMenuText_Pal[] = INCBIN_U16("graphics/interface/challenge_menu_text.gbapal");

static const u8 *const sChallengeMenuItemsNames[(ROWS_OPTIONS-1)*PAGE_COUNT] =
{
    [MENUITEM_LIMITEDENCOUNTERS]  = gText_LimitedEncounters,
    [MENUITEM_SPECIESCLAUSE]      = gText_SpeciesClause,
    [MENUITEM_PERMADEATH]         = gText_PermaDeath,
    [MENUITEM_FORCESETMODE]       = gText_ForceSetMode,
    [MENUITEM_INFINITECANDY]      = gText_InfiniteCandy,
    [MENUITEM_REPELLANT]          = gText_Repellant,
    [MENUITEM_STARTINGMONEY]      = gText_StartingMoney,
    [MENUITEM_SHINYODDS]          = gText_Shinyodds,
    [MENUITEM_GRASSSTARTER]       = gText_GrassStarter,
    [MENUITEM_WATERSTARTER]       = gText_WaterStarter,
    [MENUITEM_FIRESTARTER]        = gText_FireStarter,
    [MENUITEM_STARTERAFFECTSRIVAL]= gText_StarterAffectsRival,
    [MENUITEM_LEVELCAP]           = gText_LevelCap,
    [MENUITEM_GAUNTLETMODE]       = gText_GauntletMode,
    [MENUITEM_NOBATTLEITEMS]      = gText_NoBattleItems,
    [MENUITEM_XPMULTIPLIER]       = gText_XPMultiplier,
};

static const struct WindowTemplate sChallengeMenuWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = TILEMAP_WIDTH-4,
        .height = 2*ROWS_HEADER,
        .paletteNum = 1,
        .baseBlock = 14
    },
    [WIN_CHALLENGES] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 9,
        .width = TILEMAP_WIDTH-4,
        .height = 2*ROWS_OPTIONS,
        .paletteNum = 1,
        .baseBlock = 158
    },
    [WIN_CONFIRM_YESNO] = {
        .bg = 2,
        .tilemapLeft = 22,
        .tilemapTop = 9,
        .width = 6,
        .height = 4,
        .paletteNum = 1,
        .baseBlock = 180
    },
    [WIN_CONFIRM] = {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = TILEMAP_WIDTH-4,
        .height = 4,
        .paletteNum = 1,
        .baseBlock = 204
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sChallengeMenuBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 3,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
};

static const u16 sChallengeMenuBg_Pal[] = {RGB(17, 18, 31)};

//max selection indices.
static const u16 maxValue[PAGE_COUNT*(ROWS_OPTIONS-1)] =
{
    [MENUITEM_LIMITEDENCOUNTERS] = 1,
    [MENUITEM_SPECIESCLAUSE] = 1,
    [MENUITEM_PERMADEATH] = 1,
    [MENUITEM_FORCESETMODE] = 1,
    [MENUITEM_INFINITECANDY] = 1,
    [MENUITEM_REPELLANT] = 1,
    [MENUITEM_STARTINGMONEY] = 72,
    [MENUITEM_SHINYODDS] = 16,
    [MENUITEM_GRASSSTARTER] = 386,
    [MENUITEM_WATERSTARTER] = 386,
    [MENUITEM_FIRESTARTER] = 386,
    [MENUITEM_STARTERAFFECTSRIVAL] = 1,
    [MENUITEM_LEVELCAP] = 2,
    [MENUITEM_GAUNTLETMODE] = 1,
    [MENUITEM_NOBATTLEITEMS] = 1,
    [MENUITEM_XPMULTIPLIER] = 25,
};

static const u8 sChallengeDetailTexts[PAGE_COUNT*(ROWS_OPTIONS-1)][DETAIL_TEXT_LENGTH] =
{
    [MENUITEM_LIMITEDENCOUNTERS] = _("You can only catch the first\nencounter per Location. Underwater\nRoutes not counted separately."),
    [MENUITEM_SPECIESCLAUSE] = _("Encounters of families you have\nalready caught do not count against\nthe limit."),
    [MENUITEM_PERMADEATH] = _("If a Pokemon faints, it can not be\nrevived through any means."),
    [MENUITEM_FORCESETMODE] = _("Every battle will be in set mode,\nno matter what you select in the\noptions."),
    [MENUITEM_INFINITECANDY] = _("You start the game with a key item\nthat functions as an infinitely\nreusable Rare Candy."),
    [MENUITEM_REPELLANT] = _("You start the game with a key item\nthat functions as toggleable\nRepel."),
    [MENUITEM_STARTINGMONEY] = _("How much money you start the game\nwith."),
    [MENUITEM_SHINYODDS] = _("How likely it is for Pokémon to be\nshiny."),
    [MENUITEM_GRASSSTARTER] = _("What species will be in the grass\nstarter slot. Can optionally affect\nthe rival's team."),
    [MENUITEM_WATERSTARTER] = _("What species will be in the water\nstarter slot. Can optionally affect\nthe rival's team."),
    [MENUITEM_FIRESTARTER] = _("What species will be in the fire\nstarter slot. Can optionally affect\nthe rival's team."),
    [MENUITEM_STARTERAFFECTSRIVAL] = _("If set, the rival will use the\nspecies specified above, evolving\nat appropriate levels"),
    [MENUITEM_LEVELCAP] = _("Caps level to the next boss fights\nace's level. Extreme instead caps \nto 85%."),
    [MENUITEM_GAUNTLETMODE] = _("When you enter a route, dungeon or\ngym, you can't leave until you defeat\nevery trainer. Boss fights heal you."),
    [MENUITEM_NOBATTLEITEMS] = _("You won't be able to use items in\nbattle except for Pokéballs and held\nitems. Opponents unaffected."),
    [MENUITEM_XPMULTIPLIER] = _("This multiplier will be applied to all\nexperience earned.")
};

static void ReadAllCurrentSettings()
{
    tLimitedEncounters = gSaveBlock2Ptr->challenges.limitedEncounters;
    tSpeciesClause = gSaveBlock2Ptr->challenges.speciesClause;
    tPermaDeath = gSaveBlock2Ptr->challenges.permaDeath;
    tForceSetMode = gSaveBlock2Ptr->challenges.forceSetMode;
    tInfiniteCandy = gSaveBlock2Ptr->challenges.infiniteCandy;
    tStartingMoney = gSaveBlock2Ptr->challenges.startingMoney;
    tRepellant = gSaveBlock2Ptr->challenges.repellant;
    tShinyOdds = gSaveBlock2Ptr->challenges.shinyOdds;
    tGrassStarter = gSaveBlock2Ptr->challenges.grassStarter;
    tWaterStarter = gSaveBlock2Ptr->challenges.waterStarter;
    tFireStarter = gSaveBlock2Ptr->challenges.fireStarter;
    tStarterAffectsRival = gSaveBlock2Ptr->challenges.starterAffectsRival;
    tLevelCap = gSaveBlock2Ptr->challenges.levelCap;
    tGauntletMode = gSaveBlock2Ptr->challenges.gauntletMode;
    tNoBattleItems = gSaveBlock2Ptr->challenges.noBattleItems;
    tXPMultiplier = gSaveBlock2Ptr-> challenges.xpMultiplier;
}

static void DrawChallengesPg1()
{  
    LimitedEncounters_DrawChoices(tLimitedEncounters);
    SpeciesClause_DrawChoices(tSpeciesClause);
    PermaDeath_DrawChoices(tPermaDeath);
    ForceSetMode_DrawChoices(tForceSetMode);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void DrawChallengesPg2()
{
    InfiniteCandy_DrawChoices(tInfiniteCandy);
    Repellant_DrawChoices(tRepellant);
    StartingMoney_DrawChoices(tStartingMoney);
    Shinyodds_DrawChoices(tShinyOdds);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void DrawChallengesPg3()
{  
    GrassStarter_DrawChoices(tGrassStarter);
    WaterStarter_DrawChoices(tWaterStarter);
    FireStarter_DrawChoices(tFireStarter);
    StarterAffectsRival_DrawChoices(tStarterAffectsRival);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void DrawChallengesPg4()
{
    LevelCap_DrawChoices(tLevelCap);
    GauntletMode_DrawChoices(tGauntletMode);
    NoBattleItems_DrawChoices(tNoBattleItems);
    XPMultiplier_DrawChoices(tXPMultiplier);
    HighlightChallengeMenuItem(tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void (*const sDrawChallenges[PAGE_COUNT])() = {DrawChallengesPg1,DrawChallengesPg2,DrawChallengesPg3,DrawChallengesPg4};
static void (*const sDrawChoices[PAGE_COUNT][ROWS_OPTIONS-1])(u16 selection) = 
{
    {LimitedEncounters_DrawChoices,SpeciesClause_DrawChoices,PermaDeath_DrawChoices,ForceSetMode_DrawChoices},
    {InfiniteCandy_DrawChoices,Repellant_DrawChoices,StartingMoney_DrawChoices,Shinyodds_DrawChoices},
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
    u32 i;
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
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_CLR);
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
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, TILE_TOP_CORNER_L);
        LoadBgTiles(2, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, TILE_TOP_CORNER_L);
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
        PutWindowTilemap(WIN_CONFIRM);
        FillWindowPixelBuffer(WIN_CONFIRM, PIXEL_FILL(1));
        AddTextPrinterParameterized(WIN_CONFIRM, FONT_NORMAL, gText_ChallengesConfirm, 8, 1, TEXT_SKIP_DRAW, NULL);
        DrawBgWindowFramesConfirm();
        CopyWindowToVram(WIN_CONFIRM, COPYWIN_FULL);
    case 10:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 11:
        taskId = CreateTask(Task_ChallengeMenuFadeIn, 0);
        tMenuSelection = 0;
        ReadAllCurrentSettings();
        sDrawChallenges[sCurrPage]();
        gMain.state++;
        break;
    case 12:
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

static void save()
{
    gSaveBlock2Ptr->challenges.limitedEncounters = tLimitedEncounters;
    gSaveBlock2Ptr->challenges.speciesClause = tSpeciesClause;
    gSaveBlock2Ptr->challenges.permaDeath = tPermaDeath;
    gSaveBlock2Ptr->challenges.forceSetMode = tForceSetMode;
    gSaveBlock2Ptr->challenges.infiniteCandy = tInfiniteCandy;
    gSaveBlock2Ptr->challenges.repellant = tRepellant;
    gSaveBlock2Ptr->challenges.startingMoney = tStartingMoney;
    gSaveBlock2Ptr->challenges.shinyOdds = tShinyOdds;
    gSaveBlock2Ptr->challenges.grassStarter = tGrassStarter;
    gSaveBlock2Ptr->challenges.waterStarter = tWaterStarter;
    gSaveBlock2Ptr->challenges.fireStarter = tFireStarter;
    gSaveBlock2Ptr->challenges.starterAffectsRival = tStarterAffectsRival;
    gSaveBlock2Ptr->challenges.levelCap = tLevelCap;
    gSaveBlock2Ptr->challenges.gauntletMode = tGauntletMode;
    gSaveBlock2Ptr->challenges.noBattleItems = tNoBattleItems;
    gSaveBlock2Ptr->challenges.xpMultiplier = tXPMultiplier;
    return;
}

static void Task_ChangePage(u8 taskId)
{
    DrawHeaderText(taskId);
    PutWindowTilemap(WIN_CHALLENGES);
    DrawChallengeMenuTexts();
    sDrawChallenges[sCurrPage]();
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
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        if (tMenuSelection == (CONFIRM_INDEX))
        {
            gTasks[taskId].func = Task_ChallengeMenuConfirm;
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
            gTasks[taskId].func = Task_ChallengeMenuConfirm;
        }
    }
    else if (JOY_REPEAT(DPAD_UP))
    {
        if (tMenuSelection > 0)
            tMenuSelection--;
        else
            tMenuSelection = (CONFIRM_INDEX);
        HighlightChallengeMenuItem(tMenuSelection);
        if(isInDetails) 
        {
            sArrowPressed = TRUE;
            DrawHeaderText(taskId);
        }
    }
    else if (JOY_REPEAT(DPAD_DOWN))
    {
        if (tMenuSelection < (CONFIRM_INDEX))
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
    else if (JOY_REPEAT(DPAD_RIGHT)&&tMenuSelection !=CONFIRM_INDEX)
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
    else if (JOY_REPEAT(DPAD_LEFT)&&tMenuSelection !=CONFIRM_INDEX)
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
    save();
    gTasks[taskId].func = Task_ChallengeMenuFadeOut;
}

static void Task_ChallengeMenuConfirm(u8 taskId)
{
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_NONE);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0,0));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0,0));
    CreateYesNoMenu(&sChallengeMenuWinTemplates[WIN_CONFIRM_YESNO],TILE_TOP_CORNER_L,7,1);
    ShowBg(2);
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_ChallengeMenuConfirmProcessInput;
}

static void Task_ChallengeMenuConfirmProcessInput(u8 taskId)
{
    s8 result = Menu_ProcessInputNoWrapClearOnChoose();
    switch (result)
    {
    case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_ChallengeMenuSave;
        break;
    case 1:
    case MENU_B_PRESSED:
        HideBg(2);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        HighlightChallengeMenuItem(tMenuSelection);
        DrawBgWindowFrames();
        gTasks[taskId].func = Task_ChangePage;
        break;
    }
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
    u8 dst[21];
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


    widthLeft = GetStringWidth(FONT_NORMAL, leftText, 0);
    widthMid = GetStringWidth(FONT_NORMAL, middleText, 0);
    widthRight = GetStringWidth(FONT_NORMAL, rightText, 0);

    widthMid -= 94;
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    DrawChallengeMenuChoice(leftText, 104, ypos, styles[0]);
    DrawChallengeMenuChoice(middleText, xMid, ypos, styles[1]);
    DrawChallengeMenuChoice(rightText, GetStringRightAlignXOffset(FONT_NORMAL, rightText, 198), ypos, styles[2]);
}

static void LimitedEncounters_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_LIMITEDENCOUNTERS));
}

static void SpeciesClause_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_SPECIESCLAUSE));
}

static void PermaDeath_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_PERMADEATH));
}

static void ForceSetMode_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_FORCESETMODE));
}

static void InfiniteCandy_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_INFINITECANDY));
}

static void Repellant_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_REPELLANT));
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
static void StartingMoney_DrawChoices(u16 selection)
{
    u8 text[21];
    s32 number,i,j;
    i=0;
    j=0;
    while (TRUE)
    {
        if (selection < 9 * (i + 1))
        {
            number = (selection - (9 * i)+1) * sPowersOfTen[j];
            break;
        }
        i++;
        j++;
    }
    if (number == 100000000)
        number = 99999999;
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];
    for(j=7;j>=0;j--)
    {
        if(number/sPowersOfTen[j])
            text[i++] = (number / sPowersOfTen[j])%10 + CHAR_0;   
    }
    text[i++] = CHAR_CURRENCY;
    while(i<21)
        text[i++] = CHAR_SPACER;
    text[i] = EOS;
    DrawChallengeMenuChoice(text, 104, YPOS(MENUITEM_STARTINGMONEY), 1);
}

static void Shinyodds_DrawChoices(u16 selection)
{
    u8 text[21];
    s32 number,i,j;
    i=0;
    j=0;
    number = 1;
    for(i;i<selection;i++)
    {
        number*=2;
    }
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];
    text[i++] = CHAR_1;    
    text[i++] = CHAR_SLASH; 
    for(j=5;j>=0;j--)
    {
        if(number/sPowersOfTen[j])
            text[i++] = (number / sPowersOfTen[j])%10 + CHAR_0;   
    }
    while(i<21)
        text[i++] = CHAR_SPACER;
    text[i] = EOS;  
    DrawChallengeMenuChoice(text, 104, YPOS(MENUITEM_SHINYODDS), 1); 
}

static void GrassStarter_DrawChoices(u16 selection)
{
    u8 text[21];
    u8 speciesName[POKEMON_NAME_LENGTH + 1];
    u8 speciesNameDst[POKEMON_NAME_LENGTH + 1];
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
    StringCopyPadded(speciesNameDst,speciesName,CHAR_SPACER,10);
    StringAppend(text,speciesNameDst);
    DrawChallengeMenuChoice(text, 104, YPOS(MENUITEM_GRASSSTARTER), 1);
}

static void WaterStarter_DrawChoices(u16 selection)
{
    u8 text[19];
    u8 speciesName[POKEMON_NAME_LENGTH + 1];
    u8 speciesNameDst[POKEMON_NAME_LENGTH + 1];
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
    StringCopyPadded(speciesNameDst,speciesName,CHAR_SPACER,10);
    StringAppend(text,speciesNameDst);
    DrawChallengeMenuChoice(text, 104, YPOS(MENUITEM_WATERSTARTER), 1);
}

static void FireStarter_DrawChoices(u16 selection)
{
    u8 text[19];
    u8 speciesName[POKEMON_NAME_LENGTH + 1];
    u8 speciesNameDst[POKEMON_NAME_LENGTH + 1];
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
    StringCopyPadded(speciesNameDst,speciesName,CHAR_SPACER,10);
    StringAppend(text,speciesNameDst);
    DrawChallengeMenuChoice(text, 104, YPOS(MENUITEM_FIRESTARTER), 1);
}

static void StarterAffectsRival_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_STARTERAFFECTSRIVAL));
}

static void LevelCap_DrawChoices(u16 selection)
{
    ChallengesDraw3Choices(selection, gText_ChallengesNo,gText_ChallengesYes,gText_Extreme,YPOS(MENUITEM_LEVELCAP));
}

static void GauntletMode_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_GAUNTLETMODE));
}

static void NoBattleItems_DrawChoices(u16 selection)
{
    ChallengesDraw2Choices(selection, gText_ChallengesNo,gText_ChallengesYes,YPOS(MENUITEM_NOBATTLEITEMS));
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
    DrawChallengeMenuChoice(text, 128, YPOS(MENUITEM_XPMULTIPLIER), 1);
}

static void DrawHeaderText(u8 taskId)
{
    u32 i, widthChallenges;
    u8 pageDots[9] = _(""); // Array size should be at least (2 * PAGE_COUNT) -1
    u8 pageNameStr[30];
    StringCopy(pageNameStr, gText_Challenge);
    switch (sCurrPage)
    {
    case 0:
        StringAppend(pageNameStr, gText_Nuzlocke);
        break;
    case 1:
        StringAppend(pageNameStr, gText_Convenience);
        break;
    case 2:
        StringAppend(pageNameStr, gText_Starter);
        break;
    case 3:
        StringAppend(pageNameStr, gText_Difficulty);
        break;
    }
    widthChallenges = GetStringWidth(FONT_NORMAL, pageNameStr, 0);
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
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, pageNameStr, 8, 1, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, pageDots, GetStringRightAlignXOffset(FONT_NORMAL, pageDots, 198), 1, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav, 8, 17, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_ChallengeDetails, 8, 33, TEXT_SKIP_DRAW, NULL);
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

    items = ROWS_OPTIONS - 1;
    FillWindowPixelBuffer(WIN_CHALLENGES, PIXEL_FILL(1));
    for (i = 0; i < items; i++)
        AddTextPrinterParameterized(WIN_CHALLENGES, FONT_NORMAL, sChallengeMenuItemsNames[((ROWS_OPTIONS - 1) * sCurrPage) + i], 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_CHALLENGES, FONT_NORMAL, gText_Confirm4, 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}



static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  2*ROWS_HEADER,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  2*ROWS_HEADER,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  2*ROWS_HEADER+1,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  2*ROWS_HEADER+1, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  2*ROWS_HEADER+1,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  2*ROWS_HEADER+2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  2*ROWS_HEADER+2, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  2*ROWS_HEADER+2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  2*ROWS_HEADER+3,  1, 2*ROWS_OPTIONS,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  2*ROWS_HEADER+3,  1, 2*ROWS_OPTIONS,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);
    CopyBgTilemapBufferToVram(1);
}

static void DrawBgWindowFramesConfirm(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(2, TILE_TOP_CORNER_L,  1,  14,  1,  1,  7);
    FillBgTilemapBufferRect(2, TILE_TOP_EDGE,      2,  14, 27,  1,  7);
    FillBgTilemapBufferRect(2, TILE_TOP_CORNER_R, 28,  14,  1,  1,  7);
    FillBgTilemapBufferRect(2, TILE_LEFT_EDGE,     1,  15,  1,  4,  7);
    FillBgTilemapBufferRect(2, TILE_RIGHT_EDGE,   28,  16,  1,  4,  7);
    FillBgTilemapBufferRect(2, TILE_BOT_CORNER_L,  1,  19,  1,  1,  7);
    FillBgTilemapBufferRect(2, TILE_BOT_EDGE,      2,  19, 27,  1,  7);
    FillBgTilemapBufferRect(2, TILE_BOT_CORNER_R, 28,  19,  1,  1,  7);
    CopyBgTilemapBufferToVram(2);
}
