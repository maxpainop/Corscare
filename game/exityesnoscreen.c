#include "main.h"
#include "exityesnoscreen.h"


MENUITEM gMI_exityesno_yes = { "YES", (RESOLUTIONWIDTH / 2) - ((3 * 6) / 2),100, menuitem_exityesno_yes };
MENUITEM gMI_exityesno_no = { "NO", (RESOLUTIONWIDTH / 2) - ((2 * 6) / 2),115, menuitem_exityesno_no };
MENUITEM* gMI_exityesnooptions[] = { &gMI_exityesno_yes,&gMI_exityesno_no };
MENU gmenu_exityesno = { "EXIT?",1,_countof(gMI_exityesnooptions),gMI_exityesnooptions };





void drawexityesnoscreen(void) {
    PIXEL32 white = { 255,230,255,255 };
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    memset(vidset.backbuffer.Memory, 0, GAME_CANVAS_MEMSIZE);
    Blitstringtobuffer(gmenu_exityesno.name, &vidset.origfont, &white, (RESOLUTIONWIDTH / 2) - ((5 * 6) / 2), 80);
    Blitstringtobuffer(gmenu_exityesno.items[0]->name, &vidset.origfont, &white, gmenu_exityesno.items[0]->x - 1, gmenu_exityesno.items[0]->y);
    Blitstringtobuffer(gmenu_exityesno.items[1]->name, &vidset.origfont, &white, gmenu_exityesno.items[1]->x, gmenu_exityesno.items[1]->y);

    Blitstringtobuffer("»", &vidset.origfont, &white, gmenu_exityesno.items[gmenu_exityesno.selecteditem]->x - 6, gmenu_exityesno.items[gmenu_exityesno.selecteditem]->y);
}

void PPI_exityesno(void) {
    int msgboxid;
    //////////////////////////////////////////////////////////////////////
    if (gameinput.Escapekeydown) {
        msgboxid = MessageBoxA(NULL, "Would you like to exit?", "", MB_ICONSTOP | MB_YESNO);
        if (msgboxid == 6) SendMessageA(vidset.ggamewindow, WM_CLOSE, 0, 0);
    }
    //if (gameinput.f3keydown && !gameinput.debugkeywasdown) {
    //    perfdata.displaydebuginfo = !perfdata.displaydebuginfo;
    //}
    /////////////////////////////////////////////////////////////////////
    if (gameinput.f3keydown && !gameinput.debugkeywasdown) perfdata.displaydebuginfo = !perfdata.displaydebuginfo;
    if (gameinput.Downkeydown && !gameinput.Downkeywasdown) if (gmenu_exityesno.selecteditem < gmenu_exityesno.itemcount - 1)  gmenu_exityesno.selecteditem++, playgamesound(&menunav);
    if (gameinput.Upkeydown && !gameinput.Upkeywasdown) if (gmenu_exityesno.selecteditem > 0) gmenu_exityesno.selecteditem--, playgamesound(&menunav);
    /////////////////////////////////////////////////////////////
    if (gameinput.choosekeydown && !gameinput.choosekeywasdown) {
        gmenu_exityesno.items[gmenu_exityesno.selecteditem]->action();
        playgamesound(&choose);
    }
}

void menuitem_exityesno_yes(void) {
    SendMessageA(vidset.ggamewindow, WM_CLOSE, 0, 0);
}

void menuitem_exityesno_no(void) {
    currentgamestate = prevgamestate;
    prevgamestate = GAMESTATE_EXITYESNOSCREEN;
}