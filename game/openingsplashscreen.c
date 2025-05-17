
#include"main.h"
#include"openingsplash.h"





void drawsplash(void) {
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    static uint64_t brightnessmodifier;
    static BOOL blink;

    //if (WaitForSingleObject(essentialsloadedevent, 0) != WAIT_OBJECT_0) {
    //    return;
    //}

    //reset any state specific to this gamestate if this gamestate has been re-entered.
    if (perfdata.totalframesrendered > (lastframeseen + 1))
    {
        localframecount = 0;
        ginputenabled = FALSE;
    }
    if (localframecount == 10) playgamesound(&splash);


    if (perfdata.totalframesrendered % 30 == 0) blink = !blink;

    memset(vidset.backbuffer.Memory, 0, GAME_CANVAS_MEMSIZE);


    if (WaitForSingleObject(essentialsloadedevent, 0) != WAIT_OBJECT_0) 
    {
        if (blink) 
        {
            Blitstringtobuffer("\xf2", &vidset.origfont, &(PIXEL32){200, 200, 200, 255}, RESOLUTIONWIDTH - 5, RESOLUTIONHEIGHT - 7);
        }

    }
    else
    {
        ginputenabled = TRUE;
    }


    brightnessmodifier++;

    if (brightnessmodifier < 255) {
        Blitstringtobuffer("maxpainop presents", &vidset.origfont, &((PIXEL32) { (uint8_t)(255 - brightnessmodifier), (uint8_t)(255 - brightnessmodifier), (uint8_t)(255 - brightnessmodifier), 255 }), (RESOLUTIONWIDTH / 2) - ((18 * 6) / 2), 100);
    }

    if (localframecount >= 260) {

        if (WaitForSingleObject(assetloadingthread, 0) == WAIT_OBJECT_0)
        {
            DWORD threadexitcode = ERROR_SUCCESS;
            GetExitCodeThread(assetloadingthread, &threadexitcode);
            if (threadexitcode != ERROR_SUCCESS) {
                LogMessageA(LOGLEVELERROR, "[%s] asset loading thread failed with error code 0x%08lx!", __FUNCTION__, threadexitcode);
                maingamerunning = false;
                MessageBoxA(vidset.ggamewindow, "Asset loading failed check log", "Error", MB_OK | MB_ICONERROR);
                return;
            }           

            prevgamestate = currentgamestate;
            currentgamestate = GAMESTATE_TITLESCREEN;
        }

    }
    localframecount++;
    lastframeseen = perfdata.totalframesrendered;
}



void PPI_openingsplash() {
    if (gameinput.choosekeydown && !gameinput.choosekeywasdown) {

        if (WaitForSingleObject(assetloadingthread, 0) == WAIT_OBJECT_0)
        {
            DWORD threadexitcode = ERROR_SUCCESS;

            GetExitCodeThread(assetloadingthread, &threadexitcode);

            if (threadexitcode == ERROR_SUCCESS) {
                prevgamestate = currentgamestate;
                currentgamestate = GAMESTATE_TITLESCREEN;
            }
        }
    }
}