#pragma once



#pragma warning(push, 3)

#include <Windows.h>

#include <stdint.h>

#include <stdio.h>

#include <string.h>

#include <math.h>

#include <stdbool.h>

#include<xaudio2.h> //audio library

#include <Psapi.h> //process status api

#include <time.h>

#pragma comment(lib,"Winmm.lib")

#pragma comment(lib,"XAudio2.lib")

//#define AVX
#include "tiles.h"

#ifdef AVX
#include <immintrin.h>
#elif defined SSE2
#include<emmintrin.h>
#endif
#pragma warning(pop)


#pragma region 

#define MAX_RAY_LENGTH 25

#define RAY_COUNT 8

#define TILEBLOCKDIMENSION 16

#define GRAVITY 15 // define the gravitational acceleration constant

#define GAME_NAME "CORSCARE"

#define windowreswidth 1440+18

#define windowresheight 768+47

#define RESOLUTIONWIDTH 480

#define RESOLUTIONHEIGHT 256

#define BPP 32

#define calcavgfpseveryxframes 120

#define GAME_CANVAS_MEMSIZE (RESOLUTIONWIDTH * RESOLUTIONHEIGHT * (BPP/8))

#define TARGETMSPERFRAME 16667

#define suit_0 0

#define suit_1 1

#define suit_2 2

#define FACING_LEFT_0				0
									
#define FACING_LEFT_1				1
									
#define FACING_LEFT_2				2
									
#define FACING_RIGHT_0				3
									
#define FACING_RIGHT_1				4
									
#define FACING_RIGHT_2				5
									
#define JUMPING_UP_0				6
									
#define JUMPING_UP_1				7
									
#define JUMPING_UP_2				8
									
#define JUMPING_UP_0L				9
									
#define JUMPING_UP_1L				10
									
#define JUMPING_UP_2L				11
									
#define ATK1						12
									
#define ATK2						13
									
#define ATK3						14
									
#define ATK1L						15
									
#define ATK2L						16
									
#define ATK3L						17
									
#define CROUCHLEFT					18
									
#define CROUCHRIGHT					19

#define SWORD_EXTENSION_RIGHTSIDE	20

#define ATK4						21

#define SWORD_EXTENSION_RIGHTSIDE_2 22

#define ATK4L						23

#define SWORD_EXTENSION_LEFTSIDE	24

#define SWORD_EXTENSION_LEFTSIDE_2  25

#define DIRECTIONLEFT 0

#define DIRECTIONRIGHT 1

#define FONT_SHEET_CHARPERROW 98

#define LOGLEVELNONE 0

#define LOGLEVELINFO 1

#define LOGLEVELWARNING 2

#define LOGLEVELERROR 3

#define LOGLEVELDEBUG 4

#define NUMBEROFSFXSOURCEVOICES 4

#define LOGFILENAME GAME_NAME ".log"

#define ASSET_FILE "assets.dat" 

#pragma endregion//all variable definitions

typedef enum GAMESTATE {
	GAMESTATE_OPENINGSPLASH,
	GAMESTATE_TITLESCREEN,
	GAMESTATE_OVERWORLD,
	GAMESTATE_LEVELONE,
	GAMESTATE_OPTIONSSCREEN,
	GAMESTATE_EXITYESNOSCREEN,
	GAMESTATE_SAVEDATAOVERWRITE,
}GAMESTATE;

typedef enum RESOURCETYPE {

	TYPE_WAV,
	TYPE_OGG,
	TYPE_TILEMAP,
	TYPE_BMPX

}RESOURCETYPE;

typedef LONG(NTAPI* _NtQueryTimerResolution) (OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);
_NtQueryTimerResolution NtQueryTimerResolution;

typedef struct UPOINT {			//an unsigned point meaning it cannot be negative

	float X;
	float Y;

}UPOINT;

typedef struct GAMEBITMAP {		//game bitmap containing bitmap information and pointer to memory (memory contains image)
	
	BITMAPINFO BitmapInfo;
	void* Memory;

}GAMEBITMAP;

typedef struct GAMESOUND {		//game sound to do with xaudio2

	WAVEFORMATEX waveformat;
	XAUDIO2_BUFFER buffer;

}GAMESOUND;

typedef struct PIXEL32 {		//pixel32 storing rgba data

	//each 1 byte
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;

}PIXEL32;

typedef struct gameperfdata {	//performance statistics usually shown in debugging
	uint64_t totalframesrendered;
	float Rawfpsavg;
	float Cookedfpsavg;
	int64_t perffreq;
	bool displaydebuginfo;
	DWORD handlecount;
	SYSTEM_INFO systeminfo;
	int64_t currentsystime;
	int64_t previoussystemtime;
	FILETIME processcreationtime;
	FILETIME processexittime;
	int64_t currentuserCPUtime;
	int64_t previoususerCPUtime;
	int64_t currentkernelCPUtime;
	int64_t previouskernelCPUtime;
	double CPUpercent;

	//LONG minimumtimerresolution;
	//LONG maximumtimerresolution;
	//LONG currenttimerresolution;
}PERFORMANCEDATA;

typedef struct VIDEOSETTINGS {	//video settings containing misc screen related stuff
	long currentwindowwidth;
	long currentwindowheight;
	int userscreenWidth;
	int userscreenHeight;
	HWND ggamewindow;
	GAMEBITMAP backbuffer;
	GAMEBITMAP origfont;
	MONITORINFO monitorinfo;
	GAMEBITMAP backgroundbackbuffer;
	GAMEBITMAP bgbackbuff;
	int maxscalefactor;
	int currscalefactor;
}VIDSET;

typedef struct PLAYER {
	GAMEBITMAP Sprite[3][25];
	float ScreenposX;
	float ScreenposY;
	UPOINT worldpos;
	uint8_t direction;
	uint8_t spriteindex;
	int32_t HP;
	int playerspritedimx;
	int playerspritedimy; 
	int jumpstatus;				// jump status; 0=not jumping, 1=ascending, 2=descending
	float gravity;
	UPOINT velocity;			//velocity of horizontal jump
	int facing;					//which direction hes facing
	int jumping;				//which direction hes jumping
	BOOL canwalk;
	BOOL iswalking;
	BOOL isattacking;
	BOOL crouching;
	float floorlevel;
}PLAYER;

typedef struct GAMEINPUT {
	short f3keydown;
	short Escapekeydown;
	short Leftkeydown;
	short Rightkeydown;
	short Upkeydown;
	short Downkeydown;
	int16_t debugkeywasdown;
	int16_t Leftkeywasdown;
	int16_t Rightkeywasdown;
	int16_t Upkeywasdown;
	int16_t Downkeywasdown;
	int16_t choosekeydown;
	int16_t choosekeywasdown;
	int16_t zkeydown;
	int16_t xkeydown;
	int16_t zkeywasdown;
	bool attackanim;
	bool zpressed;
	int attackanimindex;
	BOOL hasplayermovedsinceportal;
	BOOL iframes;
	int iframecount;

}GAMEINPUT;

typedef struct REGPARAM {
  DWORD loglevel;
  DWORD SFXvolume;
  DWORD musicvolume;
  DWORD scalefactor;
} REGPARAM;


typedef struct MENUITEM			//for when adding menu items to a menu
{
	char* name;
	int16_t x;
	int16_t y;
	void (*action)(void);

}MENUITEM;

typedef struct MENU				//for when making a menu
{
	char* name;
	int8_t selecteditem;
	int8_t itemcount;
	MENUITEM** items;

}MENU;

typedef struct TILEMAP {		//tilemap for getting collision map of a level
	uint16_t width;
	uint16_t height;
	uint8_t** map;				//2d array to store collision info

}TILEMAP;

typedef struct GAMEMAP {		//map for each level, containing tilemap for collisions and bitmap for the level art
	TILEMAP tilemap;
	GAMEBITMAP gamebitmap;
}GAMEMAP;


typedef struct Vec2 {			//glorified point
	float x, y;
}Vec2;

typedef struct Ray {			//has origin direction and length for casting rays
	Vec2 origin;
	Vec2 direction;
	float length;
}Ray;

typedef struct Rect {			//rectangle
	Vec2 position;
	Vec2 size;
	int type;					//for tile descriptions; like passable, dangerous, stair etc.
	int leveloffset;			//camera offset specific to each level to ensure proper camera scrolling
}Rect;	

typedef struct ENEMY_SKELETON {		//struct for enemy skeleton
	GAMEBITMAP skeleton[6];
	int worldposx;
	int worldposy;
	int direction;
	Rect hitbox;
} ENEMY_SKELETON;

///////////////////////////////////////////////////////miscallaneous?
BOOL maingamerunning;

PERFORMANCEDATA perfdata;

VIDSET vidset;

GAMEINPUT gameinput;

BOOL gameinprogress;

PLAYER player;

ENEMY_SKELETON skeletonenemy01; //probably going to get rid of this, was used to test skeleton sprite animation

///////////////////////////////////////////////////////////////////////// sound/sfx
GAMESOUND menunav;
GAMESOUND choose;
GAMESOUND badselection;
GAMESOUND splash;
GAMESOUND level01music;

float sfxvolume;
float musicvolume;

///////////////////////////////////////////////////////////////////////// links to an enum of gamestates
GAMESTATE currentgamestate;
GAMESTATE prevgamestate;

//////////////////////////////////////////////////////////////////////////
IXAudio2SourceVoice* xaudioSFXsourcevoice[NUMBEROFSFXSOURCEVOICES];
IXAudio2SourceVoice* xaudiomusicsourcevoice;

//event gets signalled after most essential assets have been loaded(essential means assets required to render splash screen)
HANDLE essentialsloadedevent;
HANDLE assetloadingthread;



/////////////////////////////////////////////////////////////////////////function definitions

LRESULT CALLBACK MainWndProc(												//main windowproc function, sets up basis of entire game
	HWND winhandle, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam);				

DWORD Creategamewindow(void);												//initializes game window as well as all corresponding things

BOOL gamerunning(void);														//prevents multiple instances of game to run simultaneously

void ProcessPlayerInput();													//switches to each corresponding PPI function based on gamestate, sets up keyboard input

void RenderFrameGraphics();													//switches to each corresponding draw function based on gamestate, shows debug info, and stretches pixels to proper aspect ratio 

void Resolutionmanager();													//gets screen dimensions

DWORD Load32bppbitmapfromfile(												// no longer used, gonna want to delete it
	_In_ char* filename,
	_Inout_ GAMEBITMAP* gamebitmap);					

DWORD initializeplayer(void);												//set up player stats like health damage size etc.

void initializeenemy(void);

DWORD loadassetfromarchive(													//load assets from the archive created using miniz (if theres issues with this make sure you actually added to the archive)
	_In_ char* archivename, 
	_In_ char* assetfilename, 
	_In_ RESOURCETYPE resourcetype, 
	_Inout_ void* resource);

void Blit32bppbitmaptobuffer(												//might need to change coz hi
	_In_ GAMEBITMAP* gamebitmap, 
	_In_ int32_t x, 
	int32_t y, 
	_In_ int16_t brightnessadjustment);

void Blitstringtobuffer(													//as described, blits a string to the buffer
	_In_ char* string, 
    _In_ GAMEBITMAP* fontsheet, 
	_In_ PIXEL32* color, 
	_In_ uint32_t x, 
    uint32_t y);

void blittilemaptobuffer(													//as described, takes game bitmap of a level and blits it to the screen buffer
	_In_ GAMEBITMAP* gamebitmap, 
	_In_ int16_t brightnessadjustment);			

DWORD loadregistryparams(void);												//reads registry parameters in the corscare windows registry (my regparams are read and corresponding actions performed here)

DWORD saveregparams(void);													//creates registry parameters in the corscare windows registry (my regparams are created here)

void LogMessageA(_In_ DWORD loglevel, _In_ char* message, _In_ ...);		//logger function that takes in log danger level, a message, and other params, and writes to corscare.txt

DWORD Loadtilemap(_In_ char* filename,_Inout_ TILEMAP* tilemap);			// no longer used, gonna want to delete it

bool pointInRect(Vec2 p, Rect r);											//part of collision.c, returns true or false based on if a point is inside a given rectangle

bool rectsOverlap(Rect r1, Rect r2);										//part of collision.c, returns true or false based on if 2 rectangles overlap

Vec2 intersectRect(Ray r, Rect rect);										//part of collision.c, defined but never used, Returns the point at which the ray intersects the rectangle, or (0, 0) if there is no intersection

Vec2 castRay(Ray r, Rect* tiles, int numTiles);								//part of collision.c, as described, casts a ray

bool checkRectWithinDistance(Rect rect, Ray ray, float distance);			//part of collision.c, defined but never used, checks whether a ray(see definition in declaration of struct) is within a certain distance of a rectangle

void initializelevel(void);													//initialize various level parameters like size player position etc.

void playgamesound(_In_ GAMESOUND* gamesound);								//call this to play game sound effects

void playgamemusic(_In_ GAMESOUND* gamesound);								//call this to play game music

HRESULT initsoundengine(void);												//initializes xaudio2 sound engine

DWORD loadwavfromfile(_In_ char* filename, _Inout_ GAMESOUND* gamesound);	// no longer used, gonna want to delete it

DWORD loadwavfrommemory(_In_ void* buffer, _Inout_ GAMESOUND* gamesound);	//loads wav music/sfx from memory

DWORD loadoggfrommemory(													//loads ogg sfx from memory
	_In_ void* buffer,
	_In_ uint64_t buffersize, 
	_Inout_ GAMESOUND* gamesound);	

DWORD loadoggfromfile(_In_ char* filename, _Inout_ GAMESOUND* gamesound);	// no longer used, gonna want to delete it

DWORD loadtilemapfrommemory(												//loads tilemaps from memory
	_In_ void* buffer, 
	_In_ uint32_t buffersize, 
	_Inout_ TILEMAP* tilemap);	

DWORD load32bppbitmapfrommemory(											//loads bitmaps from memory
	_In_ void* buffer, 
	_Inout_ GAMEBITMAP* gamebitmap);					

DWORD assetloadingthreadproc(_In_ LPVOID lpparam);							//loads assets on a seperate thread {or something}

void initglobals(void);														//initializes globals to a specific value if required

/////////////////////////////////////////////////////////////////////////


////////////PPI
Rect rightplayerrect;    //player's right hitbox used for collision detection with enemies and the level obstacles
Rect leftplayerrect;     //player's left  hitbox used for collision detection with enemies and the level obstacles
Rect feetplayerrect;     //player's foot  hitbox used for collision detection with enemies and the level obstacles
Rect headplayerrect;	 //player's head  hitbox used for collision detection with enemies and the level obstacles
					    
UPOINT camera;			 //camera for scrolling and following player throughout level, type upoint which is described at declaration of upoint struct
					    
GAMEBITMAP hitboxR;		 //player's right hitbox bitmap may want to move these types of things to a seperate, graphics header later
GAMEBITMAP hitboxL;		 //player's left  hitbox bitmap may want to move these types of things to a seperate, graphics header later
GAMEBITMAP hitboxF;		 //player's foot  hitbox bitmap may want to move these types of things to a seperate, graphics header later
GAMEBITMAP hitboxH;		 //player's head  hitbox bitmap may want to move these types of things to a seperate, graphics header later

GAMEBITMAP redzone;		 //not currently sure what these are for yet, might be obsolete because they dont have any other references in the code than the one here
GAMEBITMAP yellowzone;	 //not currently sure what these are for yet, might be obsolete because they dont have any other references in the code than the one here


GAMEMAP overworldgamemap;//game map containing tilemap and bitmap of the overworld level. might want to move this to a separate graphics header later
GAMEMAP level2gamemap;

Rect* overworldrects;			 //rectangle data type and variable, used to create hitboxes
Rect* lvl2rects;

BOOL ginputenabled;		 //check if input is allowed currently, use to prevent input during level changes and loading
BOOL gfadetransitioning; //bool to check if level is transitioning, so that fading can be done in draw functions

