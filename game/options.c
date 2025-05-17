#include"main.h"
#include"options.h"

MENUITEM gMI_options_sfxvolume = { "SFX Volume:", (RESOLUTIONWIDTH / 2) - ((11 * 6) / 2),100, menuitem_options_sfxvolume };
MENUITEM gMI_options_musicvolume = { "Music Volume:", (RESOLUTIONWIDTH / 2) - ((13 * 6) / 2),115, menuitem_options_musicvolume };
MENUITEM gMI_options_back = { "BACK", (RESOLUTIONWIDTH / 2) - ((4 * 6) / 2),130, menuitem_options_back };
MENUITEM* gMI_optionsoptions[] = { &gMI_options_sfxvolume,&gMI_options_musicvolume,&gMI_options_back };
MENU gmenu_options = { "OPTIONS",0,_countof(gMI_optionsoptions),gMI_optionsoptions };



void drawoptionsscreen(void) {
    PIXEL32 white = { 230,255,230,255 };
    PIXEL32 gray = { 155,130,155,255 };
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    memset(vidset.backbuffer.Memory, 0, GAME_CANVAS_MEMSIZE);
    Blitstringtobuffer(gmenu_options.name, &vidset.origfont, &white, (RESOLUTIONWIDTH / 2) - ((8 * 6) / 2), 80);
    Blitstringtobuffer(gmenu_options.items[0]->name, &vidset.origfont, &white, gmenu_options.items[0]->x, gmenu_options.items[0]->y);
    Blitstringtobuffer(gmenu_options.items[1]->name, &vidset.origfont, &white, gmenu_options.items[1]->x, gmenu_options.items[1]->y);
    Blitstringtobuffer(gmenu_options.items[2]->name, &vidset.origfont, &white, gmenu_options.items[2]->x, gmenu_options.items[2]->y);
    Blitstringtobuffer("»", &vidset.origfont, &white, gmenu_options.items[gmenu_options.selecteditem]->x - 6, gmenu_options.items[gmenu_options.selecteditem]->y);
    for (int vol = 0; vol < 10; vol++) {
        if (vol >= (int)(sfxvolume * 10))
        {
            Blitstringtobuffer("\xf2", &vidset.origfont, &gray, 280 + (vol * 6), 100);
        }
        else
        {
            Blitstringtobuffer("\xf2", &vidset.origfont, &white, 280 + (vol * 6), 100);
        }
    }
    for (int vol = 0; vol < 10; vol++) {
        if (vol >= (int)(musicvolume * 10))
        {
            Blitstringtobuffer("\xf2", &vidset.origfont, &gray, 280 + (vol * 6), 115);
        }
        else
        {
            Blitstringtobuffer("\xf2", &vidset.origfont, &white, 280 + (vol * 6), 115);
        }
    }
}

void PPI_options(void) {
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
    if (gameinput.Downkeydown && !gameinput.Downkeywasdown) if (gmenu_options.selecteditem < gmenu_options.itemcount - 1)  gmenu_options.selecteditem++, playgamesound(&menunav);
    if (gameinput.Upkeydown && !gameinput.Upkeywasdown) if (gmenu_options.selecteditem > 0) gmenu_options.selecteditem--, playgamesound(&menunav);
    /////////////////////////////////////////////////////////////
    if (gameinput.choosekeydown && !gameinput.choosekeywasdown) {
        gmenu_options.items[gmenu_options.selecteditem]->action();
        playgamesound(&choose);
    }
}

void menuitem_options_sfxvolume(void) {
    sfxvolume += 0.1f;
    if ((int)(sfxvolume * 10) > 10) sfxvolume = 0.0f;
    for (int i = 0; i < NUMBEROFSFXSOURCEVOICES; i++) {
        xaudioSFXsourcevoice[i]->lpVtbl->SetVolume(xaudioSFXsourcevoice[i], sfxvolume, XAUDIO2_COMMIT_NOW);
    }
}

void menuitem_options_musicvolume(void) {
    musicvolume += 0.1f;
    if ((int)(musicvolume * 10) > 10) musicvolume = 0.0f;
    xaudiomusicsourcevoice->lpVtbl->SetVolume(xaudiomusicsourcevoice, musicvolume, XAUDIO2_COMMIT_NOW);
}

void menuitem_options_back(void) {
    prevgamestate = currentgamestate;
    currentgamestate = GAMESTATE_TITLESCREEN;
    if (saveregparams() != ERROR_SUCCESS)
    {
        LogMessageA(LOGLEVELERROR, "[%s] saveregparams failed haha fuck you!!!!1111!!", __FUNCTION__);
    }
}