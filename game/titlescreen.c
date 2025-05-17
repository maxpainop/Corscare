#include "main.h"
#include "titlescreen.h"



MENUITEM gMI_resumegame = { "RESUME", (RESOLUTIONWIDTH / 2) - ((6 * 6) / 2),100, menuitem_titlescreen_resume };
MENUITEM gMI_startnewgame = { "START NEW GAME", (RESOLUTIONWIDTH / 2) - ((14 * 6) / 2),120, menuitem_titlescreen_startnew };
MENUITEM gMI_options = { "OPTIONS", (RESOLUTIONWIDTH / 2) - ((7 * 6) / 2),140, menuitem_titlescreen_options };
MENUITEM gMI_exit = { "EXIT", (RESOLUTIONWIDTH / 2) - ((4 * 6) / 2),160, menuitem_titlescreen_exit };
MENUITEM* gMI_titlescreenitems[] = { &gMI_resumegame, &gMI_startnewgame, &gMI_options, &gMI_exit };
MENU gMenu_titlescreen = { "TITLESCREEN MENU", 0, _countof(gMI_titlescreenitems), gMI_titlescreenitems };


void drawtitlescreen(void) {
    static PIXEL32 white = { 0,0,0,255 };
    static PIXEL32 gray = { 0,0,0,255 };
    static uint64_t localframecount;
    static uint64_t lastframeseen;


    if (perfdata.totalframesrendered > (lastframeseen + 1))
    {
        white.red = 0;
        white.green = 0;
        white.blue = 0;
        gray.red = 0;
        gray.green = 0;
        gray.blue = 0;
    }

    //AARRGGBB
//__stosd(vidset.backbuffer.Memory, 0xFF000000, GAME_CANVAS_MEMSIZE / sizeof(DWORD));
    memset(vidset.backbuffer.Memory, 0, GAME_CANVAS_MEMSIZE);

    if (white.red < 255) {
        white.red += 5;
        white.green += 5;
        white.blue += 5;
    }
    if (gray.green < 100) {
        gray.red += 5;
        gray.green += 5;
        gray.blue += 5;
    }


    Blit32bppbitmaptobuffer(&vidset.bgbackbuff, 0, 0, 0);
    if (!gameinprogress) Blitstringtobuffer(gMenu_titlescreen.items[0]->name, &vidset.origfont, &gray, gMenu_titlescreen.items[0]->x, gMenu_titlescreen.items[0]->y);
    else Blitstringtobuffer(gMenu_titlescreen.items[0]->name, &vidset.origfont, &white, gMenu_titlescreen.items[0]->x, gMenu_titlescreen.items[0]->y);
    for (uint8_t menuitem = 1; menuitem < gMenu_titlescreen.itemcount;menuitem++) {
        Blitstringtobuffer(gMenu_titlescreen.items[menuitem]->name, &vidset.origfont, &white, gMenu_titlescreen.items[menuitem]->x, gMenu_titlescreen.items[menuitem]->y);
    }
    Blitstringtobuffer("»", &vidset.origfont, &white, gMenu_titlescreen.items[gMenu_titlescreen.selecteditem]->x - 6, gMenu_titlescreen.items[gMenu_titlescreen.selecteditem]->y);

    localframecount++;
    lastframeseen = perfdata.totalframesrendered;
}

void PPI_Titlescreen(void) {
    int msgboxid;
    //////////////////////////////////////////////////////////////////////
    if (gameinput.Escapekeydown) {
        msgboxid = MessageBoxA(NULL, "Would you like to exit?", "", MB_ICONSTOP | MB_YESNO);
        if (msgboxid == 6) SendMessageA(vidset.ggamewindow, WM_CLOSE, 0, 0);
    }
    if (gameinput.f3keydown && !gameinput.debugkeywasdown) {
        perfdata.displaydebuginfo = !perfdata.displaydebuginfo;
    }
    /////////////////////////////////////////////////////////////////////
    if (gameinput.f3keydown && !gameinput.debugkeywasdown) perfdata.displaydebuginfo = !perfdata.displaydebuginfo;
    if (gameinput.Downkeydown && !gameinput.Downkeywasdown) if (gMenu_titlescreen.selecteditem < gMenu_titlescreen.itemcount - 1)  gMenu_titlescreen.selecteditem++, playgamesound(&menunav);
    if (gameinput.Upkeydown && !gameinput.Upkeywasdown) if (gMenu_titlescreen.selecteditem > 0) gMenu_titlescreen.selecteditem--, playgamesound(&menunav);
    /////////////////////////////////////////////////////////////
    if (gameinput.choosekeydown && !gameinput.choosekeywasdown) {
        if (gMenu_titlescreen.selecteditem == 0 && !gameinprogress) {
            playgamesound(&badselection);
        }
        else {
            gMenu_titlescreen.items[gMenu_titlescreen.selecteditem]->action();
            playgamesound(&choose);
        }
    }
}

void menuitem_titlescreen_resume(void) {

}

void menuitem_titlescreen_startnew(void) {
    prevgamestate = currentgamestate;
    if (gameinprogress) {
        currentgamestate = GAMESTATE_SAVEDATAOVERWRITE;
    }
    else currentgamestate = GAMESTATE_OVERWORLD;
}

void menuitem_titlescreen_options(void) {
    prevgamestate = currentgamestate;
    currentgamestate = GAMESTATE_OPTIONSSCREEN;
}

void menuitem_titlescreen_exit(void) {
    prevgamestate = currentgamestate;

    currentgamestate = GAMESTATE_EXITYESNOSCREEN;
}