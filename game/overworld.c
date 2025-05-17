#include"main.h"
#include"overworld.h"
#pragma warning(disable: 4244)
int passabletiles[] = {PASSABLETILE};





void drawoverworld(void) {
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    static int16_t brightnessmod;


   if (perfdata.totalframesrendered > (lastframeseen + 1)) {
        localframecount = 0;
   }

   if (localframecount == 60) playgamemusic(&level01music);

   if (localframecount == 0) brightnessmod = -255;

   blittilemaptobuffer(&overworldgamemap.gamebitmap, brightnessmod);


   Blit32bppbitmaptobuffer(&player.Sprite[suit_0][player.direction + player.spriteindex], (int32_t)player.ScreenposX, (int32_t)player.ScreenposY, brightnessmod);
   
  
   if((brightnessmod<0)&& gfadetransitioning == FALSE) brightnessmod+=3;  //triggers the fade from black
   if ((gfadetransitioning == TRUE) && brightnessmod != -255) brightnessmod -= 3; //triggers the fade to black
   if (gfadetransitioning == TRUE && brightnessmod == -255) //triggers when fade transition to black has completed 
   {
       prevgamestate = currentgamestate;
       currentgamestate = GAMESTATE_LEVELONE;
   }
   
   if (perfdata.displaydebuginfo) 
   {                                                                                     
        Blit32bppbitmaptobuffer(&hitboxR, rightplayerrect.position.x- camera.X, rightplayerrect.position.y- camera.Y, 0);
        Blit32bppbitmaptobuffer(&hitboxL, leftplayerrect.position.x - camera.X, leftplayerrect.position.y - camera.Y, 0);
        Blit32bppbitmaptobuffer(&hitboxF, feetplayerrect.position.x - camera.X, feetplayerrect.position.y - camera.Y, 0);
        Blit32bppbitmaptobuffer(&hitboxH, headplayerrect.position.x - camera.X, headplayerrect.position.y - camera.Y, 0);
     }



    localframecount++;
    lastframeseen = perfdata.totalframesrendered;
}


void PPI_overworld(void) {
    static uint64_t localframecount;
    static uint64_t lastframeseen;
    int prejumpdirection = 0;

    int numelements = (overworldgamemap.gamebitmap.BitmapInfo.bmiHeader.biWidth * overworldgamemap.gamebitmap.BitmapInfo.bmiHeader.biHeight)/64;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (gameinput.zkeydown && !gameinput.zkeywasdown) {
         gameinput.zpressed = true;
    }

    if (gameinput.zpressed) {
        static int startframe = 0;
        static int endframe = 16;
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
    }

    if (player.direction >= 0 && player.direction<3) {
        player.facing = DIRECTIONLEFT;
    }

    if (player.direction >= 3 && player.direction<6) {
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

    if (gameinput.attackanim && player.facing == DIRECTIONRIGHT )
    {

        player.canwalk = false;

        switch (localframecount % 16)
        {

        case 0:
        {
            player.direction = 12;
            break;
        }
        case 4:
        {
            player.direction = 13;
            break;
        }
        case 8:
        {
            player.direction = 14;
            break;
        }

        case 12:
        {
            player.direction = 13;
            break;
        }

        default: {}
        }

    }





    else  if (gameinput.attackanim && player.facing == DIRECTIONLEFT)
    {

        player.canwalk = false;

        switch (localframecount % 16)
        {

        case 0:
        {
            player.direction = 15;
            break;
        }
        case 4:
        {
            player.direction = 16;
            break;
        }
        case 8:
        {
            player.direction = 17;
            break;
        }

        case 12:
        {
            player.direction = 16;
            break;
        }

        default: {}
        }

    }


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
            if (rectsOverlap(leftplayerrect, overworldrects[i])) {
                if (overworldrects[i].type == COLLISION) canmoveleft = false;
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
            else if(testanim>=8){
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
            if (rectsOverlap(rightplayerrect, overworldrects[i])==true) {
                if(overworldrects[i].type == COLLISION) canmoveright = false;
                if (overworldrects[i].type == STAIRTILE) {
                    player.ScreenposY -= 8;
                    player.worldpos.Y -= 8;
                    goto skip;
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
        player.floorlevel = overworldgamemap.gamebitmap.BitmapInfo.bmiHeader.biHeight-32;


        feetplayerrect.position.x = player.worldpos.X + 11;
        feetplayerrect.position.y = player.worldpos.Y + 29;

        if (player.worldpos.Y < overworldgamemap.gamebitmap.BitmapInfo.bmiHeader.biHeight-32) {
            for (uint64_t i = 0; i < numelements; i++) {
                if (rectsOverlap(feetplayerrect, overworldrects[i]) == true && overworldrects[i].type == COLLISION)
                {
                   player.floorlevel = feetplayerrect.position.y - 29;
                    canfall = false;
                    break;
                }
            }
        }

        if (player.worldpos.Y < overworldgamemap.gamebitmap.BitmapInfo.bmiHeader.biHeight - 32) {
            for (uint64_t i = 0; i < numelements; i++) {
                if (rectsOverlap(headplayerrect, overworldrects[i]) == true && overworldrects[i].type == COLLISION)
                {
                    //player.floorlevel = feetplayerrect.position.y - 29;
                    canjump = false;
                    break;
                }
            }
        }

        if (canfall) 
        {
             //if (player.gravity > 0) moveup(player.gravity);
             if (player.gravity <=0) movedown(-player.gravity);
            player.gravity -= 0.15f;
        }

        if (canjump)
        {
            if (player.gravity > 0) moveup(player.gravity);
        }

             if (!canfall && player.worldpos.Y != player.floorlevel )
            {
                player.worldpos.Y = player.floorlevel;
            }


             // Update player position

             if (player.facing == DIRECTIONLEFT) {
                 bool canjumpleft = true;
                 
                 for (uint64_t i = 0; i < numelements; i++) {
                     if (rectsOverlap(leftplayerrect, overworldrects[i]) == true) {
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
                     if (rectsOverlap(rightplayerrect, overworldrects[i]) == true) {
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
            else if (player.gravity < 0) {
                player.jumpstatus = 2;
            }
            else if (player.gravity > 0) {
                player.jumpstatus = 1;
            }
         

            if (player.gravity <= -3) {
                player.gravity = -3;
            }

    //} 
    /////////////////////////////////////////////////////////////////////



    ///////////////////////////controlling the direction hes facing when he jumps and lands depending on where he was facing prior to jump /////////////////////////////////

    if (player.jumpstatus > 0 ) {//jumping while facing right
        switch (player.jumpstatus) {
        case 1:
        {
            if ((gameinput.Rightkeydown) || player.facing == DIRECTIONRIGHT ) {
                player.direction = 6;
                break;
            }
            if ( (gameinput.Leftkeydown) || player.facing == DIRECTIONLEFT ) {
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
       // if ((rectsOverlap(feetplayerrect, overworldrects[i]) == true || rectsOverlap(leftplayerrect, overworldrects[i]) == true || rectsOverlap(rightplayerrect, overworldrects[i]) == true) && overworldrects[i].type == PORTAL)
        if(gameinput.Upkeydown && !gameinput.Upkeywasdown)
        {
            portalhandler(1);
            break;
        }
    }

    rightplayerrect.position.x = player.worldpos.X + 19;
    leftplayerrect.position.x = player.worldpos.X + 10;
    rightplayerrect.position.y = player.worldpos.Y + 4;
    leftplayerrect.position.y = player.worldpos.Y + 4;


    feetplayerrect.position.x = player.worldpos.X + 11;
    feetplayerrect.position.y = player.worldpos.Y + 29;

    headplayerrect.position.x = player.worldpos.X + 11;
    headplayerrect.position.y = player.worldpos.Y;


    
    localframecount++;
    lastframeseen = perfdata.totalframesrendered;
    sanitycheck();
}


///////////////////////////////////////////////////////////////////////
void moveleft(float pixels) {

    if (player.ScreenposX > RESOLUTIONWIDTH - 240)
    {
        player.ScreenposX -= pixels;
    }
    else
    {
        if (camera.X <= currentarea.position.x) {
            player.ScreenposX -= pixels;
        }
        else {
            camera.X-=pixels;
        }
    }
    player.worldpos.X -= pixels;


}

void moveright(float pixels) {

    if (player.ScreenposX < RESOLUTIONWIDTH - 240)
    {
        player.ScreenposX += pixels;
    }

    else
    {

        if (camera.X >= currentarea.size.x + currentarea.leveloffset) {
            player.ScreenposX += pixels;
        }
        else {
            camera.X+= pixels;
        }
    }

    player.worldpos.X+=pixels;

}

void moveup(float pixels) {

    if (player.ScreenposY > RESOLUTIONHEIGHT-192)
    {
        player.ScreenposY -= pixels;
    }

    else
    {
        if (camera.Y <= 0) {
            camera.Y = 0;
            player.ScreenposY -= pixels;
        }
        else {
            camera.Y -= pixels;
        }
    }

    player.worldpos.Y -= pixels;
    //feetplayerrect.position.y = player.worldpos.Y + 29;
    
}

void movedown(float pixels) {
    if (player.ScreenposY > RESOLUTIONHEIGHT - 128)
    {
        player.ScreenposY += pixels;
    }
    else
    {
        if (camera.Y >= currentarea.size.y - 256) {
             player.ScreenposY += pixels;
        }
        else {
            camera.Y += pixels;
        }
    }


    if (camera.Y > currentarea.size.y - 256) {
        camera.Y = currentarea.size.y - 256;
    }




    player.worldpos.Y += pixels;
    ////feetplayerrect.position.x = player.worldpos.X + 11;
    //feetplayerrect.position.y = player.worldpos.Y + 29;

}

void sanitycheck(void) {
    player.ScreenposY = player.worldpos.Y - camera.Y;
    player.ScreenposX = player.worldpos.X - camera.X;

}

void portalhandler(int x) {

    if (x == 1)
    {

        gfadetransitioning = TRUE;

    }

}
///////////////////////////////////////////////////////////////////////
