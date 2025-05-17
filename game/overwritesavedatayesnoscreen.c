#include"main.h"
#include"overwritesaveyesnoscreen.h"


MENUITEM gMI_overwriteyesno_yes = { "YES", (RESOLUTIONWIDTH / 2) - ((3 * 6) / 2),100, menuitem_overwriteyesno_yes };
MENUITEM gMI_overwriteyesno_no = { "NO", (RESOLUTIONWIDTH / 2) - ((2 * 6) / 2),115, menuitem_overwriteyesno_no };
MENUITEM* gMI_overwriteyesnooptions[] = { &gMI_overwriteyesno_yes,&gMI_overwriteyesno_no };
MENU gmenu_overwriteyesno = { "overwrite saved data?",1,_countof(gMI_overwriteyesnooptions),gMI_overwriteyesnooptions };

  
void drawoverwritesavedatayesno(void) {
    PIXEL32 white = { 255,230,255,255 };
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    memset(vidset.backbuffer.Memory, 0, GAME_CANVAS_MEMSIZE);
    Blitstringtobuffer(gmenu_overwriteyesno.name, &vidset.origfont, &white, (RESOLUTIONWIDTH / 2) - ((21 * 6) / 2), 80);
    Blitstringtobuffer(gmenu_overwriteyesno.items[0]->name, &vidset.origfont, &white, gmenu_overwriteyesno.items[0]->x - 1, gmenu_overwriteyesno.items[0]->y);
    Blitstringtobuffer(gmenu_overwriteyesno.items[1]->name, &vidset.origfont, &white, gmenu_overwriteyesno.items[1]->x, gmenu_overwriteyesno.items[1]->y);

    Blitstringtobuffer("»", &vidset.origfont, &white, gmenu_overwriteyesno.items[gmenu_overwriteyesno.selecteditem]->x - 6, gmenu_overwriteyesno.items[gmenu_overwriteyesno.selecteditem]->y);
}



void PPI_overwritesavedatayesno(void) {
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
    if (gameinput.Downkeydown && !gameinput.Downkeywasdown) if (gmenu_overwriteyesno.selecteditem < gmenu_overwriteyesno.itemcount)  gmenu_overwriteyesno.selecteditem++, playgamesound(&menunav);
    if (gameinput.Upkeydown && !gameinput.Upkeywasdown) if (gmenu_overwriteyesno.selecteditem > 0) gmenu_overwriteyesno.selecteditem--, playgamesound(&menunav);
    /////////////////////////////////////////////////////////////
    if (gameinput.choosekeydown && !gameinput.choosekeywasdown) {
        gmenu_overwriteyesno.items[gmenu_overwriteyesno.selecteditem]->action();
        playgamesound(&choose);
    }
}

void menuitem_overwriteyesno_yes(void) {
    //to be written
}


void menuitem_overwriteyesno_no(void) {
    currentgamestate = prevgamestate;
    prevgamestate = GAMESTATE_SAVEDATAOVERWRITE;
}