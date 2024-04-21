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

#define tMenuSelection data[0]
#define tLevelCap data[1]
#define tPermaDeath data[2]
#define tLimitedEncounters data[3]
#define tSpeciesClause data[4]
#define tNoBattleItems data[5]
#define tForceSetMode data[6]
#define tCustomStarter data[7]
#define tXPMultiplier data[8]
#define tInfiniteCandy data[9]
#define tRepellant data[10]

// Menu items Pg1
 enum
 {
     MENUITEM_LEVELCAP,
     MENUITEM_PERMADEATH,
     MENUITEM_LIMITEDENCOUNTERS,
     MENUITEM_SPECIESCLAUSE,
     MENUITEM_NOBATTLEITEMS,
     MENUITEM_FORCESETMODE,
     MENUITEM_CONFIRM,
     MENUITEM_COUNT,
 };
 
// Menu items Pg2
enum
{
    MENUITEM_CUSTOMSTARTER,
    MENUITEM_XPMULTIPLIER,
    MENUITEM_INFINITECANDY,
    MENUITEM_REPELLANT,
    MENUITEM_CONFIRM_PG2,
    MENUITEM_COUNT_PG2,
};


enum
{
    WIN_HEADER,
    WIN_CHALLENGES,
};
//Page 1
#define YPOS_LEVELCAP    (MENUITEM_LEVELCAP * 16)
#define YPOS_PERMADEATH  (MENUITEM_PERMADEATH * 16)
#define YPOS_LIMITEDENCOUNTERS  (MENUITEM_LIMITEDENCOUNTERS * 16)
#define YPOS_SPECIESCLAUSE        (MENUITEM_SPECIESCLAUSE * 16)
#define YPOS_NOBATTLEITEMS   (MENUITEM_NOBATTLEITEMS * 16)
#define YPOS_FORCESETMODE    (MENUITEM_FORCESETMODE * 16)


//Pg2
#define YPOS_CUSTOMSTARTER        (MENUITEM_CUSTOMSTARTER * 16)
#define YPOS_XPMULTIPLIER      (MENUITEM_XPMULTIPLIER * 16)
#define YPOS_INFINITECANDY      (MENUITEM_INFINITECANDY * 16)
#define YPOS_REPELLANT      (MENUITEM_REPELLANT * 16)

#define MAX_XP_MULTIPLIER 25
#define PAGE_COUNT  2

static void Task_ChallengeMenuFadeIn(u8 taskId);
static void Task_ChallengeMenuProcessInput(u8 taskId);
static void Task_ChallengeMenuFadeIn_Pg2(u8 taskId);
static void Task_ChallengeMenuProcessInput_Pg2(u8 taskId);
static void Task_ChallengeMenuSave(u8 taskId);
static void Task_ChallengeMenuFadeOut(u8 taskId);
static void HighlightChallengeMenuItem(u8 selection);
static u8 LevelCap_ProcessInput(u8 selection);
static void LevelCap_DrawChoices(u8 selection);
static u8   CustomStarter_ProcessInput(u8 selection);
static void CustomStarter_DrawChoices(u8 selection);
static u8   XPMultiplier_ProcessInput(u8 selection);
static void XPMultiplier_DrawChoices(u8 selection);
static u8 PermaDeath_ProcessInput(u8 selection);
static void PermaDeath_DrawChoices(u8 selection);
static u8 LimitedEncounters_ProcessInput(u8 selection);
static void LimitedEncounters_DrawChoices(u8 selection);
static u8 SpeciesClause_ProcessInput(u8 selection);
static void SpeciesClause_DrawChoices(u8 selection);
static u8 ForceSetMode_ProcessInput(u8 selection);
static void ForceSetMode_DrawChoices(u8 selection);
static u8 NoBattleItems_ProcessInput(u8 selection);
static void NoBattleItems_DrawChoices(u8 selection);
static u8   InfiniteCandy_ProcessInput(u8 selection);
static void InfiniteCandy_DrawChoices(u8 selection);
static u8   Repellant_ProcessInput(u8 selection);
static void Repellant_DrawChoices(u8 selection);
static void DrawHeaderText(u8 taskId);
static void DrawChallengeMenuTexts(void);
static void DrawBgWindowFrames(void);

EWRAM_DATA static bool8 sArrowPressed = FALSE;
EWRAM_DATA static u8 sCurrPage = 0;
EWRAM_DATA static u8 isInDetails = 0;

static const u16 sChallengeMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text.gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/interface/option_menu_equals_sign.4bpp");

static const u8 *const sChallengeMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_LEVELCAP]         = gText_LevelCap,
    [MENUITEM_PERMADEATH]       = gText_PermaDeath,
    [MENUITEM_LIMITEDENCOUNTERS]= gText_LimitedEncounters,
    [MENUITEM_SPECIESCLAUSE]    = gText_SpeciesClause,
    [MENUITEM_NOBATTLEITEMS]    = gText_NoBattleItems,
    [MENUITEM_FORCESETMODE]     = gText_ForceSetMode,
    [MENUITEM_CONFIRM]           = gText_Confirm2,
};

static const u8 *const sChallengeMenuItemsNames_Pg2[MENUITEM_COUNT_PG2] =
{
    [MENUITEM_CUSTOMSTARTER]     = gText_CustomStarter,
    [MENUITEM_XPMULTIPLIER]      = gText_XPMultiplier,
    [MENUITEM_INFINITECANDY]     = gText_InfiniteCandy,
    [MENUITEM_REPELLANT]         = gText_Repellant,
    [MENUITEM_CONFIRM_PG2]        = gText_Confirm2,
};


static const struct WindowTemplate sChallengeMenuWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    [WIN_CHALLENGES] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 5,
        .width = 26,
        .height = 14,
        .paletteNum = 1,
        .baseBlock = 0x36
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

static void ReadAllCurrentSettings(u8 taskId)
{
    gTasks[taskId].tMenuSelection = 0;
    gTasks[taskId].tLevelCap = gSaveBlock2Ptr->challenges.levelCap;
    gTasks[taskId].tPermaDeath = gSaveBlock2Ptr->challenges.permaDeath;
    gTasks[taskId].tLimitedEncounters = gSaveBlock2Ptr->challenges.limitedEncounters;
    gTasks[taskId].tSpeciesClause = gSaveBlock2Ptr->challenges.speciesClause;
    gTasks[taskId].tNoBattleItems = gSaveBlock2Ptr->challenges.noBattleItems;
    gTasks[taskId].tForceSetMode = gSaveBlock2Ptr->challenges.forceSetMode;
    gTasks[taskId].tCustomStarter = 0;
    gTasks[taskId].tXPMultiplier = gSaveBlock2Ptr-> challenges.xpMultiplier;
    gTasks[taskId].tInfiniteCandy = gSaveBlock2Ptr->challenges.infiniteCandy;
    gTasks[taskId].tRepellant = gSaveBlock2Ptr->challenges.repellant;
}

static void DrawChallengesPg1(u8 taskId)
{  
    ReadAllCurrentSettings(taskId);
    LevelCap_DrawChoices(gTasks[taskId].tLevelCap);
    PermaDeath_DrawChoices(gTasks[taskId].tPermaDeath);
    LimitedEncounters_DrawChoices(gTasks[taskId].tLimitedEncounters);
    SpeciesClause_DrawChoices(gTasks[taskId].tSpeciesClause);
    NoBattleItems_DrawChoices(gTasks[taskId].tNoBattleItems);
    ForceSetMode_DrawChoices(gTasks[taskId].tForceSetMode);
    HighlightChallengeMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}

static void DrawChallengesPg2(u8 taskId)
{
    ReadAllCurrentSettings(taskId);
    CustomStarter_DrawChoices(gTasks[taskId].tCustomStarter);
    XPMultiplier_DrawChoices(gTasks[taskId].tXPMultiplier);
    InfiniteCandy_DrawChoices(gTasks[taskId].tInfiniteCandy);
    Repellant_DrawChoices(gTasks[taskId].tRepellant);
    HighlightChallengeMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_CHALLENGES, COPYWIN_FULL);
}


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
    {
        switch(sCurrPage)
        {
        case 0:
            taskId = CreateTask(Task_ChallengeMenuFadeIn, 0);
            DrawChallengesPg1(taskId);
            break;
        case 1:
            taskId = CreateTask(Task_ChallengeMenuFadeIn_Pg2, 0);
            DrawChallengesPg2(taskId);
            break;            
        }
        gMain.state++;
        break;
    }
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
    gSaveBlock2Ptr->challenges.levelCap = gTasks[taskId].tLevelCap;
    gSaveBlock2Ptr->challenges.permaDeath = gTasks[taskId].tPermaDeath;
    gSaveBlock2Ptr->challenges.limitedEncounters = gTasks[taskId].tLimitedEncounters;
    gSaveBlock2Ptr->challenges.speciesClause = gTasks[taskId].tSpeciesClause;
    gSaveBlock2Ptr->challenges.noBattleItems = gTasks[taskId].tNoBattleItems;
    gSaveBlock2Ptr->challenges.forceSetMode = gTasks[taskId].tForceSetMode;
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
    gSaveBlock2Ptr->challenges.xpMultiplier = gTasks[taskId].tXPMultiplier;
    gSaveBlock2Ptr->challenges.infiniteCandy = gTasks[taskId].tInfiniteCandy;
    gSaveBlock2Ptr->challenges.repellant = gTasks[taskId].tRepellant;
    return;
}

static void Task_ChangePage(u8 taskId)
{
    save(taskId);
    DrawHeaderText(taskId);
    PutWindowTilemap(1);
    DrawChallengeMenuTexts();
    switch(sCurrPage)
    {
    case 0:
        DrawChallengesPg1(taskId);
        gTasks[taskId].func = Task_ChallengeMenuFadeIn;
        break;
    case 1:
        DrawChallengesPg2(taskId);
        gTasks[taskId].func = Task_ChallengeMenuFadeIn_Pg2;
        break;
    }
}

static void Task_ChallengeMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_ChallengeMenuProcessInput;
}

static void Task_ChallengeMenuProcessInput(u8 taskId)
{
    if ((JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON)) && !isInDetails)
    {
        FillWindowPixelBuffer(WIN_CHALLENGES, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_CHALLENGES, FALSE);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tMenuSelection == MENUITEM_CONFIRM)
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
        gTasks[taskId].func = Task_ChallengeMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if(!isInDetails)
        {
            if (gTasks[taskId].tMenuSelection > 0)
                gTasks[taskId].tMenuSelection--;
            else
                gTasks[taskId].tMenuSelection = MENUITEM_CONFIRM;
            HighlightChallengeMenuItem(gTasks[taskId].tMenuSelection);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if(!isInDetails)
        {
            if (gTasks[taskId].tMenuSelection < MENUITEM_CONFIRM)
                gTasks[taskId].tMenuSelection++;
            else
                gTasks[taskId].tMenuSelection = 0;
            HighlightChallengeMenuItem(gTasks[taskId].tMenuSelection);
        }
    }
    else
    {
        u8 previousChallenge;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_LEVELCAP:
            previousChallenge = gTasks[taskId].tLevelCap;
            gTasks[taskId].tLevelCap = LevelCap_ProcessInput(gTasks[taskId].tLevelCap);

            if (previousChallenge != gTasks[taskId].tLevelCap)
                LevelCap_DrawChoices(gTasks[taskId].tLevelCap);
            break;
        case MENUITEM_PERMADEATH:
            previousChallenge = gTasks[taskId].tPermaDeath;
            gTasks[taskId].tPermaDeath = PermaDeath_ProcessInput(gTasks[taskId].tPermaDeath);

            if (previousChallenge != gTasks[taskId].tPermaDeath)
                PermaDeath_DrawChoices(gTasks[taskId].tPermaDeath);
            break;
        case MENUITEM_LIMITEDENCOUNTERS:
            previousChallenge = gTasks[taskId].tLimitedEncounters;
            gTasks[taskId].tLimitedEncounters = LimitedEncounters_ProcessInput(gTasks[taskId].tLimitedEncounters);

            if (previousChallenge != gTasks[taskId].tLimitedEncounters)
                LimitedEncounters_DrawChoices(gTasks[taskId].tLimitedEncounters);
            break;
        case MENUITEM_SPECIESCLAUSE:
            previousChallenge = gTasks[taskId].tSpeciesClause;
            gTasks[taskId].tSpeciesClause = SpeciesClause_ProcessInput(gTasks[taskId].tSpeciesClause);

            if (previousChallenge != gTasks[taskId].tSpeciesClause)
                SpeciesClause_DrawChoices(gTasks[taskId].tSpeciesClause);
            break;
        case MENUITEM_NOBATTLEITEMS:
            previousChallenge = gTasks[taskId].tNoBattleItems;
            gTasks[taskId].tNoBattleItems = NoBattleItems_ProcessInput(gTasks[taskId].tNoBattleItems);

            if (previousChallenge != gTasks[taskId].tNoBattleItems)
                NoBattleItems_DrawChoices(gTasks[taskId].tNoBattleItems);
            break;
        case MENUITEM_FORCESETMODE:
            previousChallenge = gTasks[taskId].tForceSetMode;
            gTasks[taskId].tForceSetMode = ForceSetMode_ProcessInput(gTasks[taskId].tForceSetMode);

            if (previousChallenge != gTasks[taskId].tForceSetMode)
                ForceSetMode_DrawChoices(gTasks[taskId].tForceSetMode);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_CHALLENGES, COPYWIN_GFX);
        }
    }
}
static void Task_ChallengeMenuFadeIn_Pg2(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_ChallengeMenuProcessInput_Pg2;
}


static void Task_ChallengeMenuProcessInput_Pg2(u8 taskId)
{   
    
    if ((JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON)) && !isInDetails)
    {
        FillWindowPixelBuffer(WIN_CHALLENGES, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_CHALLENGES, FALSE);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_ChangePage;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tMenuSelection == MENUITEM_CONFIRM_PG2)
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
        gTasks[taskId].func = Task_ChallengeMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if(isInDetails && gTasks[taskId].tMenuSelection == MENUITEM_CUSTOMSTARTER)
        {
            switch (gTasks[taskId].tCustomStarter)
            {
            case 0:
                if(gSaveBlock2Ptr -> challenges.grassStarter <= NUM_SPECIES)
                {
                    gSaveBlock2Ptr -> challenges.grassStarter +=1;
                } else 
                {
                    gSaveBlock2Ptr -> challenges.grassStarter = 1;
                }
                break;
            case 1:
                if(gSaveBlock2Ptr -> challenges.waterStarter <= NUM_SPECIES)
                {
                    gSaveBlock2Ptr -> challenges.waterStarter +=1;
                } else 
                {
                    gSaveBlock2Ptr -> challenges.waterStarter = 1;
                }
                break;
            case 2:
                if(gSaveBlock2Ptr -> challenges.fireStarter <= NUM_SPECIES)
                {
                    gSaveBlock2Ptr -> challenges.fireStarter +=1;
                } else 
                {
                    gSaveBlock2Ptr -> challenges.fireStarter = 1;
                }
                break;
            default:
                break;
            }
            sArrowPressed = TRUE;
            CustomStarter_DrawChoices(gTasks[taskId].tCustomStarter);
        } else if(!isInDetails)
        {

            if (gTasks[taskId].tMenuSelection > 0)
                gTasks[taskId].tMenuSelection--;
            else
                gTasks[taskId].tMenuSelection = MENUITEM_CONFIRM_PG2;
            HighlightChallengeMenuItem(gTasks[taskId].tMenuSelection);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if(isInDetails && gTasks[taskId].tMenuSelection == MENUITEM_CUSTOMSTARTER)
        {
            switch (gTasks[taskId].tCustomStarter)
            {
            case 0:
                if(gSaveBlock2Ptr -> challenges.grassStarter >= 1)
                {
                    gSaveBlock2Ptr -> challenges.grassStarter -=1;
                } else 
                {
                    gSaveBlock2Ptr -> challenges.grassStarter = 386;
                }
                break;
            case 1:
                if(gSaveBlock2Ptr -> challenges.waterStarter >= 1)
                {
                    gSaveBlock2Ptr -> challenges.waterStarter -=1;
                } else 
                {
                    gSaveBlock2Ptr -> challenges.waterStarter = 386;
                }
                break;
            case 2:
                if(gSaveBlock2Ptr -> challenges.fireStarter >= 1)
                {
                    gSaveBlock2Ptr -> challenges.fireStarter -=1;
                } else 
                {
                    gSaveBlock2Ptr -> challenges.fireStarter = 386;
                }
                break;
            default:
                break;
            }
            sArrowPressed = TRUE;
            CustomStarter_DrawChoices(gTasks[taskId].tCustomStarter);
        } else if(!isInDetails)
        {
            if (gTasks[taskId].tMenuSelection < MENUITEM_CONFIRM_PG2)
                gTasks[taskId].tMenuSelection++;
            else
                gTasks[taskId].tMenuSelection = 0;
            HighlightChallengeMenuItem(gTasks[taskId].tMenuSelection);
        }   
    }
    else
    {
        u8 previousChallenge;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_CUSTOMSTARTER:
            previousChallenge = gTasks[taskId].tCustomStarter;
            gTasks[taskId].tCustomStarter = CustomStarter_ProcessInput(gTasks[taskId].tCustomStarter);

            if (previousChallenge != gTasks[taskId].tCustomStarter)
                CustomStarter_DrawChoices(gTasks[taskId].tCustomStarter);
            break;
        case MENUITEM_XPMULTIPLIER:
            previousChallenge = gTasks[taskId].tXPMultiplier;
            gTasks[taskId].tXPMultiplier = XPMultiplier_ProcessInput(gTasks[taskId].tXPMultiplier);

            if (previousChallenge != gTasks[taskId].tXPMultiplier)
                XPMultiplier_DrawChoices(gTasks[taskId].tXPMultiplier);
            break;
        case MENUITEM_INFINITECANDY:
            previousChallenge = gTasks[taskId].tInfiniteCandy;
            gTasks[taskId].tInfiniteCandy = InfiniteCandy_ProcessInput(gTasks[taskId].tInfiniteCandy);
            if (previousChallenge != gTasks[taskId].tInfiniteCandy)
                InfiniteCandy_DrawChoices(gTasks[taskId].tInfiniteCandy);
            break;
        case MENUITEM_REPELLANT:
            previousChallenge = gTasks[taskId].tRepellant;
            gTasks[taskId].tRepellant = Repellant_ProcessInput(gTasks[taskId].tRepellant);

            if (previousChallenge != gTasks[taskId].tRepellant)
                Repellant_DrawChoices(gTasks[taskId].tRepellant);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_CHALLENGES, COPYWIN_GFX);
        }
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
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(index * 16 + 40, index * 16 + 56));
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

static u8 LevelCap_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void LevelCap_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_LEVELCAP, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_LEVELCAP, styles[1]);
}

static u8 PermaDeath_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void PermaDeath_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_PERMADEATH, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_PERMADEATH, styles[1]);
}

static u8 LimitedEncounters_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void LimitedEncounters_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_LIMITEDENCOUNTERS, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_LIMITEDENCOUNTERS, styles[1]);
}

static u8 SpeciesClause_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void SpeciesClause_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_SPECIESCLAUSE, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_SPECIESCLAUSE, styles[1]);
}

static u8 NoBattleItems_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void NoBattleItems_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_NOBATTLEITEMS, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_NOBATTLEITEMS, styles[1]);
}

static u8 ForceSetMode_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void ForceSetMode_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_FORCESETMODE, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_FORCESETMODE, styles[1]);
}


static u8 CustomStarter_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void CustomStarter_DrawChoices(u8 selection)
{
    s32 widthGrass, widthWater, widthFire, xWater;
    u8 grassStarterText[19];
    u8 waterStarterText[19];
    u8 fireStarterText[19];
    u16 grassStarter, waterStarter, fireStarter;
    u8 styles[3];
    u8 i = 0;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    if(gSaveBlock2Ptr->challenges.grassStarter == SPECIES_NONE)
    {
        gSaveBlock2Ptr->challenges.grassStarter = SpeciesToNationalPokedexNum(SPECIES_TREECKO);
    }
    grassStarter = gSaveBlock2Ptr->challenges.grassStarter;
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        grassStarterText[i] = gText_FrameTypeNumber[i];
    grassStarterText[i++]= CHAR_G;
    grassStarterText[i++]= CHAR_COLON;
    if(grassStarter / 100)
    {
        grassStarterText[i++] = grassStarter / 100 + CHAR_0;
    }
    if(grassStarter / 10)
    {
        grassStarterText[i++] = (grassStarter / 10)%10 + CHAR_0;
    }
    grassStarterText[i++] = grassStarter % 10 + CHAR_0;
    grassStarterText[i++] = CHAR_SPACER;
    grassStarterText[i] = EOS;
    i=0;

    if(gSaveBlock2Ptr->challenges.waterStarter == SPECIES_NONE)
    {
        gSaveBlock2Ptr->challenges.waterStarter = SpeciesToNationalPokedexNum(SPECIES_MUDKIP);
    }
    waterStarter = gSaveBlock2Ptr->challenges.waterStarter;
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        waterStarterText[i] = gText_FrameTypeNumber[i];
    waterStarterText[i++]= CHAR_W;
    waterStarterText[i++]= CHAR_COLON;
    if(waterStarter / 100)
    {
        waterStarterText[i++] = waterStarter / 100 + CHAR_0;
    }
    if(waterStarter / 10)
    {
        waterStarterText[i++] = (waterStarter / 10)%10 + CHAR_0;
    }
    waterStarterText[i++] = waterStarter % 10 + CHAR_0;
    waterStarterText[i++] = CHAR_SPACER;
    waterStarterText[i] = EOS;
    i=0;

    if(gSaveBlock2Ptr->challenges.fireStarter == SPECIES_NONE)
    {
        gSaveBlock2Ptr->challenges.fireStarter = SpeciesToNationalPokedexNum(SPECIES_TORCHIC);
    }
    fireStarter = gSaveBlock2Ptr->challenges.fireStarter;
    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        fireStarterText[i] = gText_FrameTypeNumber[i];
    fireStarterText[i++]= CHAR_F;
    fireStarterText[i++]= CHAR_COLON;
    if(fireStarter / 100)
    {
        fireStarterText[i++] = fireStarter / 100 + CHAR_0;
    }
    if(fireStarter / 10)
    {
        fireStarterText[i++] = (fireStarter / 10)%10 + CHAR_0;
    }
    fireStarterText[i++] = fireStarter % 10 + CHAR_0;
    fireStarterText[i++] = CHAR_SPACER;
    fireStarterText[i] = EOS;
    i=0;
    widthGrass = GetStringWidth(FONT_NORMAL, grassStarterText, 0);
    widthWater = GetStringWidth(FONT_NORMAL, waterStarterText, 0);
    widthFire = GetStringWidth(FONT_NORMAL, fireStarterText, 0);
    widthWater -=94;
    xWater = (widthGrass - widthWater - widthFire) / 2 + 104;
    DrawChallengeMenuChoice(grassStarterText, 104, YPOS_CUSTOMSTARTER, styles[0]);
    DrawChallengeMenuChoice(waterStarterText, xWater, YPOS_CUSTOMSTARTER, styles[1]);
    DrawChallengeMenuChoice(fireStarterText, GetStringRightAlignXOffset(FONT_NORMAL, fireStarterText, 198), YPOS_CUSTOMSTARTER, styles[2]);
}

static u8 XPMultiplier_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < MAX_XP_MULTIPLIER)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = MAX_XP_MULTIPLIER;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void XPMultiplier_DrawChoices(u8 selection)
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

static u8 InfiniteCandy_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void InfiniteCandy_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_INFINITECANDY, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_INFINITECANDY, styles[1]);
}

static u8 Repellant_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Repellant_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawChallengeMenuChoice(gText_ChallengesNo, 104, YPOS_REPELLANT, styles[0]);
    DrawChallengeMenuChoice(gText_ChallengesYes, GetStringRightAlignXOffset(FONT_NORMAL, gText_ChallengesYes, 198), YPOS_REPELLANT, styles[1]);
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
        StringAppend(pageDots, gText_Space);
        StringAppend(pageDots, gText_ChallengeDetails);
        xMid = (8 + widthChallenges + 5);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_Challenge, 8, 1, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, pageDots, xMid, 1, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav, GetStringRightAlignXOffset(FONT_NORMAL, gText_PageNav, 198), 1, TEXT_SKIP_DRAW, NULL);
        CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
    }
    else
    {
        if (sCurrPage == 0)
        {
            switch (gTasks[taskId].tMenuSelection)
            {
            case MENUITEM_LEVELCAP:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_LevelCapDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_PERMADEATH:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PermaDeathDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_LIMITEDENCOUNTERS:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_LimitedEncountersDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_SPECIESCLAUSE:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_SpeciesClauseDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_NOBATTLEITEMS:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_NoBattleItemsDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_FORCESETMODE:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_ForceSetModeDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            }
        }
        else
        {
            switch (gTasks[taskId].tMenuSelection)
            {

            case MENUITEM_CUSTOMSTARTER:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_CustomStarterDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_XPMULTIPLIER:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_XPMultiplierDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_INFINITECANDY:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_InfiniteCandyDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            case MENUITEM_REPELLANT:
                AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_RepellantDetails, 8, 1, TEXT_SKIP_DRAW, NULL);
                break;
            }
        }
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
        menu = sChallengeMenuItemsNames;
        break;
    case 1:
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
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  3,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  3, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  3,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  4, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
