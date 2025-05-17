#include"main.h"
#include"fadetransition.h"

void fadetransition(void) {

    static uint64_t localframecount;
    static uint64_t lastframeseen;
    static int16_t brightnessmod;

    if (perfdata.totalframesrendered > (lastframeseen + 1)) {
        localframecount = 0;
    }


    brightnessmod-=4;


    if (brightnessmod < 255) {
       

        //Blit32bppbitmaptobuffer(&player.Sprite[suit_0][player.direction + player.spriteindex], player.ScreenposX, player.ScreenposY, brightnessmod);
        //Blit32bppbitmaptobuffer(vidset.backbuffer.Memory, player.ScreenposX, player.ScreenposY, brightnessmod);

       // Blitstringtobuffer("maxpainop presents", &vidset.origfont, &((PIXEL32) { (uint8_t)(255 - brightnessmodifier), (uint8_t)(255 - brightnessmodifier), (uint8_t)(255 - brightnessmodifier), 255 }), (RESOLUTIONWIDTH / 2) - ((18 * 6) / 2), 100);
    }


    if (perfdata.displaydebuginfo)
    {
        Blit32bppbitmaptobuffer(&hitboxR, rightplayerrect.position.x - camera.X, rightplayerrect.position.y - camera.Y,0);
        Blit32bppbitmaptobuffer(&hitboxL, leftplayerrect.position.x - camera.X, leftplayerrect.position.y - camera.Y,0);
        Blit32bppbitmaptobuffer(&hitboxF, feetplayerrect.position.x - camera.X, feetplayerrect.position.y - camera.Y, 0);
        Blit32bppbitmaptobuffer(&hitboxH, headplayerrect.position.x - camera.X, headplayerrect.position.y - camera.Y, 0);

    }

    if (localframecount >= 260) {

            prevgamestate = currentgamestate;
           // currentgamestate = GAMESTATE_TITLESCREEN; //need to change this !!

    }

    localframecount++;
    lastframeseen = perfdata.totalframesrendered;
}