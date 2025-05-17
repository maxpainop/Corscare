#include"main.h"
#include"overworld.h"
#include "lvl1.h"
#pragma warning(disable: 4244)


int enemyanimationindex =0; //sprite starts blinking when inside function


void PPI_lvl1(void) {
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    int prejumpdirection = 0;
    bool isclimbing = false;
    int numelements = (level2gamemap.gamebitmap.BitmapInfo.bmiHeader.biWidth * level2gamemap.gamebitmap.BitmapInfo.bmiHeader.biHeight) / 64;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (gameinput.zkeydown && !gameinput.zkeywasdown) {
        gameinput.zpressed = true;
    }

    if (gameinput.zpressed) {
        static int startframe = 0;
        static int endframe = 15;
        if (startframe < endframe) {
            gameinput.attackanim = true;
            startframe++;
        }
        else
        {
            gameinput.zpressed = false;
            gameinput.attackanim = false;
            startframe = 0; // Reset startframe to 0 for the next time the function is called  
        }
        gameinput.attackanimindex = startframe;
    }

    if (player.direction >= 0 && player.direction < 3) {
        player.facing = DIRECTIONLEFT;
    }

    if (player.direction >= 3 && player.direction < 6) {
        player.facing = DIRECTIONRIGHT;
    }

    if (perfdata.totalframesrendered > (lastframeseen + 1))
    {
        localframecount = 0;
    }


    if (gameinput.f3keydown && !gameinput.debugkeywasdown) {
        perfdata.displaydebuginfo = !perfdata.displaydebuginfo;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    player.iswalking = false; //set playeriswalking to false so that when he does start walking it can be used as a factor
    player.crouching = false;


    if (gameinput.attackanim && player.facing == DIRECTIONRIGHT)
    {
        player.canwalk = false;

        if (gameinput.attackanimindex < 4)
        {
            player.direction = ATK1;
        }
        else if (gameinput.attackanimindex < 8)
        {
            player.direction = ATK2;
        }
        else if (gameinput.attackanimindex < 12)
        {
            player.direction = ATK3;
        }
        else if (gameinput.attackanimindex < 16)
        {
            player.direction = ATK4;
        }
    }

    else if (gameinput.attackanim && player.facing == DIRECTIONLEFT) //atk left
    {
        player.canwalk = false;

        if (gameinput.attackanimindex < 4)
        {
            player.direction = ATK1L;
        }
        else if (gameinput.attackanimindex < 8)
        {
            player.direction = ATK2L;
        }
        else if (gameinput.attackanimindex < 12)
        {
            player.direction = ATK3L;
        }
        else if (gameinput.attackanimindex < 16)
        {
            player.direction = ATK4L;
        }
    }

    //else  if (gameinput.attackanim && player.facing == DIRECTIONLEFT)//atk left
    //{
    //    player.canwalk = false;
    //    switch (localframecount % 16)
   //    {
    //    case 0:
    //    {
    //        player.direction = 15;
    //        break;
    //    }
    //    case 4:
    //    {
    //        player.direction = 16;
    //        break;
    //    }
    //    case 8:
    //    {
    //        player.direction = 17;
    //        break;
    //    }
    //    case 12:
    //    {
    //        player.direction = 16;
    //        break;
    //    }
    //    default: {}
    //    }
    //}



    //jump
    else if (gameinput.Upkeydown && !gameinput.Upkeywasdown && player.jumpstatus == 0) {


        player.gravity = 3.4f;
        player.jumpstatus = 1;
        moveup(5);
        player.canwalk = false;

        if (gameinput.Leftkeydown && player.facing == DIRECTIONLEFT) {
            player.jumping = 0;
            player.velocity.X = 1;
        }
        if (gameinput.Rightkeydown && player.facing == DIRECTIONRIGHT) {
            player.jumping = 1;
            player.velocity.X = 1.3f;
        }

    }




    //walking left and right
    else if (gameinput.Leftkeydown && player.ScreenposX > 0 && player.canwalk) {
        static int testanim = 0;
        bool canmoveleft = true;

        for (uint64_t i = 0; i < numelements; i++) {
            if (rectsOverlap(leftplayerrect, lvl2rects[i])) {
                if (lvl2rects[i].type == COLLISION) canmoveleft = false;
                canmoveleft = false;
                break;
            }
        }

        if (canmoveleft) {
            moveleft(1.15f);
            player.iswalking = true;

            testanim++;
            if (testanim >= 16) {
                testanim = 0;
            }

            if (testanim < 4) {
                player.direction = 0;
            }
            else if (testanim < 8) {
                player.direction = 1;
            }
            else if (testanim >= 8) {
                player.direction = 2;
            }
        }
        else {
            player.direction = 0;
            testanim = 0;
        }
    }


    else if (gameinput.Rightkeydown && player.ScreenposX < RESOLUTIONWIDTH - 32 && player.canwalk) {
        static int testanim = 0;
        bool canmoveright = true;


        for (uint64_t i = 0; i < numelements; i++) {
            if (rectsOverlap(rightplayerrect, lvl2rects[i]) == true) {
                if (lvl2rects[i].type == COLLISION) canmoveright = false;
                if (lvl2rects[i].type == STAIRTILE) {
                    moveup(1.4);
                    isclimbing = true;
                }
                break;
            }

        }

        if (canmoveright) {
            moveright(1.15f);
            player.iswalking = true;

            testanim++;
            if (testanim >= 16) {
                testanim = 0;
            }

            if (testanim < 4) {
                player.direction = 3;
            }
            else if (testanim < 8) {
                player.direction = 4;
            }
            else if (testanim >= 8) {
                player.direction = 5;
            }
        }
        else {
            player.direction = 3;
            testanim = 0;
        }

    }





    //crouching
    else if (player.facing == DIRECTIONLEFT && gameinput.Downkeydown) {
        player.crouching = true;
        player.canwalk = false;
        player.direction = 18;
    }

    else if (player.facing == DIRECTIONRIGHT && gameinput.Downkeydown) {
        player.crouching = true;
        player.canwalk = false;
        player.direction = 19;
    }

    ///////////////////////////physics//////////////////////////////////
   // if (player.ScreenposY < RESOLUTIONHEIGHT - 32) {//got rid of this so that if the player misses a platform he falls through the floor      


    bool canfall = true;
    bool canjump = true;
    player.floorlevel = level2gamemap.gamebitmap.BitmapInfo.bmiHeader.biHeight - 32;






    feetplayerrect.position.x = player.worldpos.X + 12;
    feetplayerrect.position.y = player.worldpos.Y + 31;





   // if (player.worldpos.Y < level2gamemap.gamebitmap.BitmapInfo.bmiHeader.biHeight - 32) {
        for (uint64_t i = 0; i < numelements; i++) {
            if (rectsOverlap(feetplayerrect, lvl2rects[i]) == true && lvl2rects[i].type == COLLISION)
            {
               // player.floorlevel = feetplayerrect.position.y - 29;
                player.floorlevel = lvl2rects[i].position.y-32; //32 being the distance from head to toe 
                canfall = false;
                break;
            }
        }
  //  }


    if (canfall)
    {
        //if (player.gravity > 0) moveup(player.gravity);
        if (player.gravity <= 0) movedown(-player.gravity);
        player.gravity -= 0.15f;
    }

    if (canjump)
    {
        if (player.gravity > 0) moveup(player.gravity);
    }

    if (!canfall && player.worldpos.Y != player.floorlevel)
    {
        player.worldpos.Y = player.floorlevel;
    }


    // Update player position

    if (player.facing == DIRECTIONLEFT) {
        bool canjumpleft = true;

        for (uint64_t i = 0; i < numelements; i++) {
            if (rectsOverlap(leftplayerrect, lvl2rects[i]) == true) {
                canjumpleft = false;
                break;
            }
        }
        if (canjumpleft) {
            moveleft(player.velocity.X);
        }
    }

    if (player.facing == DIRECTIONRIGHT) {
        bool canjumpright = true;

        for (uint64_t i = 0; i < numelements; i++) {
            if (rectsOverlap(rightplayerrect, lvl2rects[i]) == true) {
                canjumpright = false;
                break;
            }
        }
        if (canjumpright) {
            moveright(player.velocity.X);
        }
    }




    //Update jump status
   // Check for collision with floor
    if (player.worldpos.Y == player.floorlevel) {
        player.gravity = 0;
        player.velocity.X = 0;
        player.jumpstatus = 0;
    }
    else if (player.gravity < 0 && !isclimbing) {
        player.jumpstatus = 2;
    }
    else if (player.gravity > 0 && !isclimbing) {
        player.jumpstatus = 1;
    }
    if (player.gravity <= -3) {
        player.gravity = -3;
    }

    /////////////////////////////////////////////////////////////////////



     ///////////////////////////controlling the direction hes facing when he jumps and lands depending on where he was facing prior to jump /////////////////////////////////

    if (player.jumpstatus > 0) {//jumping while facing right
        switch (player.jumpstatus) {
        case 1:
        {
            if ((gameinput.Rightkeydown) || player.facing == DIRECTIONRIGHT) {
                player.direction = 6;
                break;
            }
            if ((gameinput.Leftkeydown) || player.facing == DIRECTIONLEFT) {
                player.direction = 9;
                break;
            }
            break;
        }
        case 2:
        {

            if ((gameinput.Leftkeydown) || player.facing == DIRECTIONLEFT) {
                player.direction = 10;
                break;
            }

            if ((gameinput.Rightkeydown) || player.facing == DIRECTIONRIGHT) {
                player.direction = 7;
                break;
            }

        }
        default:
        {}
        }
    }


    if (player.worldpos.Y == player.floorlevel && player.gravity == 0 && player.iswalking == false && !gameinput.attackanim && !player.crouching) {//blit sprite to standing after landing a jump

        if (player.facing == DIRECTIONLEFT) {

            player.direction = 0;

        }

        if (player.facing == DIRECTIONRIGHT) {
            player.direction = 3;

        }

    }



    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (player.jumpstatus == 0 && !player.crouching) {
        player.canwalk = true;
    }


skip:

    //detect portal and send value to portalhandler, for now it only sends one, later set it to increase and such
    for (uint64_t i = 0; i < numelements; i++) {
        //if ((rectsOverlap(feetplayerrect, rects[i]) == true || rectsOverlap(leftplayerrect, rects[i]) == true || rectsOverlap(rightplayerrect, rects[i]) == true) && rects[i].type == PORTAL)
        if (gameinput.Upkeydown && !gameinput.Upkeywasdown)
        {
            portalhandler(1);
            break;
        }
    }


    //keeps hitboxes up to date constantly 
    rightplayerrect.position.x = player.worldpos.X + 19;
    leftplayerrect.position.x = player.worldpos.X + 10;
    rightplayerrect.position.y = player.worldpos.Y + 4;
    leftplayerrect.position.y = player.worldpos.Y + 4;


    feetplayerrect.position.x = player.worldpos.X + 12;
    feetplayerrect.position.y = player.worldpos.Y + 31;

    headplayerrect.position.x = player.worldpos.X + 11;
    headplayerrect.position.y = player.worldpos.Y;

    localframecount++;
    lastframeseen = perfdata.totalframesrendered;
    sanitycheck();
}



//new function, enemycontroller. what the function basically does is; first declare initial position and rect of an enemy, or multiple enemies in a level, 
//then, it moves the enemies around, or gives them a specified area to move around in.
//function dosent need any input, all enemies are initially declared in main.h, and only edited and manipulated here
void EnemyAIlevel1(void) 
{
    static uint64_t localframecount;


    skeletonenemy01.worldposx += skeletonenemy01.direction;
    if (skeletonenemy01.worldposx == 500 || skeletonenemy01.worldposx == 663) {
        skeletonenemy01.direction *= -1; // Reverse the direction when the limits are reached
    }
    skeletonenemy01.hitbox.position.x = skeletonenemy01.worldposx + 6;
    skeletonenemy01.hitbox.position.y = skeletonenemy01.worldposy + 12;

    switch (localframecount % 16) {
        {

    case 0:
    {
        if (skeletonenemy01.direction == -1) enemyanimationindex = 2;
        if (skeletonenemy01.direction == 1) enemyanimationindex = 4;
        break;
    }
    case 4:
    {
        if (skeletonenemy01.direction == -1) enemyanimationindex = 1;
        if (skeletonenemy01.direction == 1)  enemyanimationindex = 3;
        break;
    }
    case 8:
    {
        if (skeletonenemy01.direction == -1) enemyanimationindex = 0;
        if (skeletonenemy01.direction == 1)  enemyanimationindex = 5;
        break;
    }

    default: {}
        }

    } //animate skeleton



    if (!gameinput.iframes && rectsOverlap(rightplayerrect, skeletonenemy01.hitbox)) {//when player gets hit by enemy..
        playertakedamage();
        player.gravity = 2.4f;
        player.jumpstatus = 2;
        moveup(2.5);
        player.canwalk = false;
        player.jumping = 0;
        player.velocity.X = -1;
        gameinput.iframes = true;
        gameinput.iframecount = localframecount;
    }




    if (localframecount >= gameinput.iframecount + 90) gameinput.iframes = false;

    localframecount++;
}

void drawlevelone(void) {

    EnemyAIlevel1();
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    static int16_t brightnessmod;

    if (perfdata.totalframesrendered > (lastframeseen + 1)) {
        localframecount = 0;
    }

    brightnessmod = 0;

    if (localframecount == 60) playgamemusic(&level01music);

    //if (localframecount == 0) brightnessmod = -255;


    blittilemaptobuffer(&level2gamemap.gamebitmap, brightnessmod);


    Blit32bppbitmaptobuffer(&player.Sprite[suit_0][player.direction + player.spriteindex], (int32_t)player.ScreenposX, (int32_t)player.ScreenposY, brightnessmod);


    Blit32bppbitmaptobuffer(&skeletonenemy01.skeleton[enemyanimationindex], skeletonenemy01.worldposx - camera.X, skeletonenemy01.worldposy, 0);
    if(player.direction == ATK3) Blit32bppbitmaptobuffer(&player.Sprite[suit_0][SWORD_EXTENSION_RIGHTSIDE], (int32_t)player.ScreenposX + 32, (int32_t)player.ScreenposY, 0);
    if (player.direction == ATK4) Blit32bppbitmaptobuffer(&player.Sprite[suit_0][SWORD_EXTENSION_RIGHTSIDE_2], (int32_t)player.ScreenposX + 32, (int32_t)player.ScreenposY, 0);
    if (player.direction == ATK3L) Blit32bppbitmaptobuffer(&player.Sprite[suit_0][SWORD_EXTENSION_LEFTSIDE], (int32_t)player.ScreenposX -8, (int32_t)player.ScreenposY, 0);
    if (player.direction == ATK4L) Blit32bppbitmaptobuffer(&player.Sprite[suit_0][SWORD_EXTENSION_LEFTSIDE_2], (int32_t)player.ScreenposX -8, (int32_t)player.ScreenposY, 0);


    if (perfdata.displaydebuginfo)
    {
        Blit32bppbitmaptobuffer(&hitboxR, rightplayerrect.position.x - camera.X, rightplayerrect.position.y - camera.Y, 0);
        Blit32bppbitmaptobuffer(&hitboxL, leftplayerrect.position.x - camera.X, leftplayerrect.position.y - camera.Y, 0);
        Blit32bppbitmaptobuffer(&hitboxF, feetplayerrect.position.x - camera.X, feetplayerrect.position.y - camera.Y, 0);
        Blit32bppbitmaptobuffer(&hitboxH, headplayerrect.position.x - camera.X, headplayerrect.position.y - camera.Y, 0);
    }






    localframecount++;
    lastframeseen = perfdata.totalframesrendered;
}
