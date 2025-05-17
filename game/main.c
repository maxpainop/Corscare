

#include "main.h"
#include"exityesnoscreen.h"
#include"options.h"
#include"overworld.h"
#include"lvl1.h"
#include"overwritesaveyesnoscreen.h"
#include"titlescreen.h"
#include"openingsplash.h"
#include "miniz.h"
#include"stb_vorbis.h"
#pragma warning(disable: 28251)
int maximizedstate = 0;



//GAMESTATE desiredgamestate;
//__m128i data;

BOOL windowhasfocus;
CRITICAL_SECTION logcritsec; //this used to be a handle data type but changed it to critical section to avoid warning ,used to sync logging between multiple threads
REGPARAM regparam;
IXAudio2* xaudio;
uint8_t xaudioSFXsourcevoiceselector;
IXAudio2MasteringVoice* xaudiomasteringvoice;

int gFontCharacterPixelOffset[] = {
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
        93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
    //     !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /  0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
94, 64, 87, 66, 67, 68, 70, 85, 72, 73, 71, 77, 88, 74, 91, 92, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 86, 84, 89, 75, 90, 93,
//  @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
65, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 80, 78, 81, 69, 76,
//  `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  ..
62, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 82, 79, 83, 63, 93,
//  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,
//  .. .. .. .. .. .. .. .. .. .. .. �  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. �  .. .. .. ..
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 96, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 95, 93, 93, 93, 93,
//  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,
//  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. F2 .. .. .. .. .. .. .. .. .. .. .. .. ..
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 97, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93
};





//////////////////////////////////////////////////////////////////^window stuff^///////////////////////////////////////////////////////////////////////
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    // PlaySound("C:\\Users\\maaz-\\source\\repos\\game\\assets\\bassloop.wav", NULL, SND_FILENAME | SND_ASYNC );
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
    //

    int64_t framestart = 0;
    int64_t frameend = 0;
    int64_t elapsedmicrosecondsperframe = 0;
    int64_t elapsedmsperframeaccumulator = 0;
    int64_t elapsedmsperframeaccumulatorcooked = 0;
    HMODULE NtDLLmodulehandle = NULL;
    MSG Message = { 0 };
    HANDLE processhandle = GetCurrentProcess();

#pragma warning(suppress: 6031)
    InitializeCriticalSectionAndSpinCount(&logcritsec, 0x400);


    essentialsloadedevent = CreateEventA(NULL, TRUE, FALSE, "essentialsloadedevent");

    if (loadregistryparams() != ERROR_SUCCESS) return 0;
    vidset.userscreenWidth = GetSystemMetrics(SM_CXSCREEN);
    vidset.userscreenHeight = GetSystemMetrics(SM_CYSCREEN);
    //if ((NtDLLmodulehandle = GetModuleHandleA("ntdll.dll")) == NULL) {
    //    MessageBoxA(NULL, "coudlny load ntdll.dll", "ERROR", MB_ICONERROR | MB_OK);
    //    return 0;
    //}
    //if ((NtQueryTimerResolution = (_NtQueryTimerResolution)GetProcAddress(NtDLLmodulehandle, "NtQueryTimerResolution")) == NULL) {
    //    MessageBoxA(NULL, "coudlny fnid ntquerytimer res function in ntdll.dll", "ERROR", MB_ICONERROR | MB_OK);
    //    return 0;
    //}

   /* NtQueryTimerResolution(&perfdata.minimumtimerresolution, &perfdata.maximumtimerresolution, &perfdata.currenttimerresolution);*/

    GetSystemInfo(&perfdata.systeminfo);
    GetSystemTimeAsFileTime((FILETIME*)&perfdata.previoussystemtime);

    if (gamerunning() == TRUE) {
        MessageBoxA(NULL, "Game Already Running!", "Error!!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


    if (timeBeginPeriod(1) == TIMERR_NOCANDO) {
        MessageBoxA(NULL, "timer error!", "Error!!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    if (SetPriorityClass(processhandle, HIGH_PRIORITY_CLASS) == 0)
    {
        MessageBoxA(NULL, "failed to set process priority!", "Error!!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST) == 0) {
        MessageBoxA(NULL, "failed to set thread priority!", "Error!!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


    if (Creategamewindow() != 0) return 0;



    if ((assetloadingthread = CreateThread(NULL, 0, assetloadingthreadproc, NULL, 0, NULL)) == NULL)
    {
        MessageBoxA(NULL, "create thread failed", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }



    if (initsoundengine() != S_OK) {
        MessageBoxA(NULL, "sound engine initialization failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    QueryPerformanceFrequency((LARGE_INTEGER*)&perfdata.perffreq);

    vidset.backbuffer.BitmapInfo.bmiHeader.biSize = sizeof(vidset.backbuffer.BitmapInfo.bmiHeader);
    vidset.backbuffer.BitmapInfo.bmiHeader.biWidth = RESOLUTIONWIDTH;
    vidset.backbuffer.BitmapInfo.bmiHeader.biHeight = RESOLUTIONHEIGHT;
    vidset.backbuffer.BitmapInfo.bmiHeader.biBitCount = BPP;
    vidset.backbuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    vidset.backbuffer.BitmapInfo.bmiHeader.biPlanes = 1;
    vidset.backbuffer.Memory = VirtualAlloc(NULL, GAME_CANVAS_MEMSIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    
    if(vidset.backbuffer.Memory == NULL){//malloc canvas & return error on faliure
        MessageBoxA(NULL, "Valloc error :(", "game has crashed", MB_ICONERROR| MB_OK);
        return (0);
    }

    //memset(vidset.backbuffer.Memory, 0x7F, GAME_CANVAS_MEMSIZE);
        
  //////////////////////////////////////////////////////////////////////////////////////////////////     
    
    initglobals();



    if (initializeplayer() != ERROR_SUCCESS) {
        MessageBoxA(NULL, "sprite load faliure :(", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    initializelevel();

    initializeenemy();

    while (maingamerunning==TRUE) {
        QueryPerformanceCounter((LARGE_INTEGER*) & framestart);
        while (PeekMessageA(&Message, vidset.ggamewindow, 0, 0, PM_REMOVE)) {
            DispatchMessageA(&Message);
        }

        ProcessPlayerInput();
        Resolutionmanager();
        RenderFrameGraphics();
        QueryPerformanceCounter((LARGE_INTEGER*) & frameend);
        elapsedmicrosecondsperframe = frameend - framestart;
        elapsedmicrosecondsperframe *= 1000000;
        elapsedmicrosecondsperframe /= perfdata.perffreq;
        perfdata.totalframesrendered++;
        elapsedmsperframeaccumulator += elapsedmicrosecondsperframe;
        
        while (elapsedmicrosecondsperframe < TARGETMSPERFRAME) {
            QueryPerformanceCounter((LARGE_INTEGER*)&frameend);
            elapsedmicrosecondsperframe = frameend - framestart;
            elapsedmicrosecondsperframe *= 1000000;
            elapsedmicrosecondsperframe /= perfdata.perffreq;
            QueryPerformanceCounter((LARGE_INTEGER*)&frameend);
            if (elapsedmicrosecondsperframe < (TARGETMSPERFRAME * 0.75f)) Sleep(1);
        }

        elapsedmsperframeaccumulatorcooked += elapsedmicrosecondsperframe;


        if ((perfdata.totalframesrendered % calcavgfpseveryxframes) == 0) {
            GetSystemTimeAsFileTime((FILETIME*) &perfdata.currentsystime);
            GetProcessTimes(processhandle,
                &perfdata.processcreationtime,
                &perfdata.processexittime,
                (FILETIME*)&perfdata.currentkernelCPUtime,
                (FILETIME*)&perfdata.currentuserCPUtime);

             (int64_t)perfdata.CPUpercent = (perfdata.currentkernelCPUtime - perfdata.previouskernelCPUtime) + \
                 (perfdata.currentuserCPUtime - perfdata.previoususerCPUtime);
             
             perfdata.CPUpercent /= (perfdata.currentsystime - perfdata.previoussystemtime);

             perfdata.CPUpercent /= perfdata.systeminfo.dwNumberOfProcessors;

             perfdata.CPUpercent *= 100;



            GetProcessHandleCount(processhandle, &perfdata.handlecount);

            perfdata.Rawfpsavg = 1.0f / ((elapsedmsperframeaccumulator / calcavgfpseveryxframes) * 0.000001f);
            perfdata.Cookedfpsavg= 1.0f / ((elapsedmsperframeaccumulatorcooked / calcavgfpseveryxframes) * 0.000001f);

            elapsedmsperframeaccumulator = 0;
            elapsedmsperframeaccumulatorcooked = 0;
            perfdata.previouskernelCPUtime = perfdata.currentkernelCPUtime;
            perfdata.previouskernelCPUtime = perfdata.currentkernelCPUtime;
            perfdata.previoussystemtime = perfdata.currentsystime;
        }
    }

    return 0;
}

LRESULT CALLBACK MainWndProc(HWND winhandle,UINT uMsg,WPARAM wParam,LPARAM lParam){
   
    LRESULT ret = 0;

    switch (uMsg)
    {
        case WM_CLOSE:
        {
            maingamerunning = FALSE;
            PostQuitMessage(0);
            break;
        }    
        case WM_ACTIVATE:
        {
            if (wParam == 0)
            {
                windowhasfocus = FALSE;
            }
            else 
            {
                windowhasfocus = TRUE;
            }
            //ShowCursor(FALSE);
            break;
        }

        default:
            ret =  DefWindowProcA(winhandle, uMsg, wParam, lParam);
    }
    return ret;
}

DWORD Creategamewindow(void) {
    DWORD ret = 0;
    WNDCLASSEXA wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = 0;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

#ifdef _DEBUG
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
#else
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
#endif
    wc.lpszMenuName = NULL;
    wc.lpszClassName = GAME_NAME"winclass";


    if (!RegisterClassExA(&wc))
    {
        ret = GetLastError();
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    vidset.ggamewindow = CreateWindowExA(0, wc.lpszClassName, GAME_NAME, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, vidset.userscreenWidth /6, vidset.userscreenHeight/14, windowreswidth, windowresheight, NULL, NULL, GetModuleHandle(NULL), NULL);
    SetWindowPos(vidset.ggamewindow, NULL, 0, 0, windowreswidth, windowresheight, SWP_NOZORDER | SWP_NOMOVE);
    ShowWindow(vidset.ggamewindow, 1);
    UpdateWindow(vidset.ggamewindow);
    if (vidset.ggamewindow == NULL)
    {
        ret = GetLastError();
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    vidset.monitorinfo.cbSize =  sizeof(MONITORINFO) ;

    if (GetMonitorInfoA(MonitorFromWindow(vidset.ggamewindow, MONITOR_DEFAULTTOPRIMARY), &vidset.monitorinfo) == 0) {
        ret = ERROR_MONITOR_NO_DESCRIPTOR;
        goto Exit;
    }



    if (vidset.currentwindowheight > vidset.currentwindowheight || vidset.currentwindowwidth > vidset.currentwindowwidth) {
        LogMessageA(LOGLEVELERROR,"[%s] the window width/height from registry was larger than current monitor size. resetting" __FUNCTION__);
        vidset.currentwindowheight = vidset.userscreenHeight;
        vidset.currentwindowwidth = vidset.userscreenWidth;
    }
    for (int i = 1; i < 12; i++) {
        if (RESOLUTIONWIDTH * i > vidset.userscreenWidth) {
            vidset.maxscalefactor = i - 1;
            break;
        }
    }

    if (regparam.scalefactor == 0) {
        vidset.currscalefactor = vidset.maxscalefactor;
    }
    else
    {
        vidset.currscalefactor = regparam.scalefactor;
    }

    /*LogMessageA(LOGLEVELINFO, "[%s]current scale factor is %d max scale facor is %d "__FUNCTION__, vidset.currscalefactor, vidset.maxscalefactor);*/



     
Exit:

    return ret;
}

BOOL gamerunning(void) {
    HANDLE Mutex = NULL;

    Mutex = CreateMutexA(NULL,FALSE, GAME_NAME"_Mutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS) return(TRUE);
    else return(FALSE);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------//


/////////////////////////////////////////////////////////logging/////////////////////////////////////////////////////////////////////////////////
DWORD loadregistryparams(void) {

    DWORD result = ERROR_SUCCESS;

    HKEY regkey = NULL;
    DWORD regdisposition = 0;
    DWORD regbytesread = sizeof(DWORD);
    result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS,NULL,&regkey, &regdisposition);
   
    if (result != ERROR_SUCCESS) 
    {
        LogMessageA(LOGLEVELERROR, "[%s] regcreatekey failed with error code 0x%08lx!", __FUNCTION__, result);
        goto Exit;
    }
    if (regdisposition == REG_CREATED_NEW_KEY) {
        LogMessageA(LOGLEVELINFO, "[%s] registry key did not resist; created new key hkcu\\software\\%s", __FUNCTION__, GAME_NAME);
    }
    else {
        LogMessageA(LOGLEVELINFO, "[%s] opened existing regkey hkcu\\software\\%s", __FUNCTION__, GAME_NAME);
    }
    result = RegGetValueA(regkey,NULL, "loglevel", RRF_RT_DWORD, NULL, (BYTE*)&regparam.loglevel, &regbytesread);
    
    if (result != ERROR_SUCCESS) {
        if (result == ERROR_FILE_NOT_FOUND) {
            result = ERROR_SUCCESS;
            LogMessageA(LOGLEVELINFO, "[%s]  regvalue loglevel not found using 0  ", __FUNCTION__);
            regparam.loglevel = LOGLEVELNONE;
        }
        else {
            LogMessageA(LOGLEVELINFO, "[%s] failed to read loglevel reg value", __FUNCTION__, result);
            goto Exit;
        }
    }
    LogMessageA(LOGLEVELINFO, "[%s] loglevel is %d", __FUNCTION__, regparam.loglevel);



    //////////////////////////////////////////////////////////////////////////
    //result = RegGetValueA(regkey, NULL, "scalefactor", RRF_RT_DWORD, NULL, (BYTE*)&regparam.scalefactor, &regbytesread);
    //
    //if (result == ERROR_FILE_NOT_FOUND) {
    //    result = ERROR_SUCCESS;
    //    LogMessageA(LOGLEVELINFO, "[%s]  regvalue scalefactor not found using 0  ", __FUNCTION__);
    //    regparam.scalefactor = 0;
    //}
    //else 
    //{
    //    LogMessageA(LOGLEVELINFO, "[%s] failed to read loglevel reg value", __FUNCTION__, result);
    //    goto Exit;
    //}
    //LogMessageA(LOGLEVELINFO, "[%s] scalefactor is %d", __FUNCTION__, regparam.scalefactor);
    ////////////////////////////////////////////////////////////////


    result = RegGetValueA(regkey, NULL, "SFXvolume", RRF_RT_DWORD, NULL,&regparam.SFXvolume, &regbytesread);

    if (result != ERROR_SUCCESS) {
        if (result == ERROR_FILE_NOT_FOUND)
        {
            result = ERROR_SUCCESS;
            LogMessageA(LOGLEVELINFO, "[%s]  regvalue sfxvolume not found using 0.5  ", __FUNCTION__);
            regparam.SFXvolume = 50;
        }
    }
    else 
    {
        LogMessageA(LOGLEVELINFO, "[%s] failed to read sfxvolume reg value error code : 0x%08lx!", __FUNCTION__, result);
    }

    LogMessageA(LOGLEVELINFO, "[%s] sfxvolume is %.1f", __FUNCTION__, (float)(regparam.SFXvolume/100.0f));

    sfxvolume = (float)(regparam.SFXvolume / 100.0f);

    ////////////////////////////////////////////////////////////////
    result = RegGetValueA(regkey, NULL, "musicvolume", RRF_RT_DWORD, NULL, (BYTE*)&regparam.musicvolume, &regbytesread);

    if (result != ERROR_SUCCESS) {
        if (result == ERROR_FILE_NOT_FOUND)
        {
            result = ERROR_SUCCESS;
            LogMessageA(LOGLEVELINFO, "[%s]  regvalue musicvolume not found using 0.5  ", __FUNCTION__);
            regparam.musicvolume = 50;
        }
    }
    else {
        LogMessageA(LOGLEVELINFO, "[%s] failed to read musicvolume reg value error code : 0x%08lx!", __FUNCTION__, result);
    }
    LogMessageA(LOGLEVELINFO, "[%s] musicvolume is %.1f", __FUNCTION__, (float)(regparam.musicvolume / 100.0f));

    musicvolume = (float)(regparam.musicvolume / 100.0f);


Exit:
    
    if (regkey) RegCloseKey(regkey);
    return(result);
}

DWORD saveregparams(void) {
    DWORD result = ERROR_SUCCESS;

    HKEY regkey = NULL;
    DWORD regdisposition = 0;
    DWORD localsfxvolume = (DWORD)(sfxvolume * 100.0f);
    DWORD localmusicvolume = (DWORD)(musicvolume * 100.0f);
    
    
    result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &regkey, &regdisposition);

    if (result != ERROR_SUCCESS)
    {
        LogMessageA(LOGLEVELERROR, "[%s] regcreatekey failed with error code 0x%08lx!", __FUNCTION__, result);
        goto Exit;
    }

    LogMessageA(LOGLEVELINFO, "[%s] reg key opened to save", __FUNCTION__);

    result = RegSetValueExA(regkey, "SFXvolume", 0, REG_DWORD,(const BYTE*)&localsfxvolume, sizeof(DWORD));
    if (result != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] failed to log sfxvolume into registry",__FUNCTION__);
        goto Exit;
    }

    LogMessageA(LOGLEVELINFO, "[%s] sfx volume saved %d", __FUNCTION__,localsfxvolume);

    result = RegSetValueExA(regkey, "musicvolume", 0, REG_DWORD,(const BYTE*)&localmusicvolume, sizeof(DWORD));
    if (result != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] failed to log musicvolume into registry", __FUNCTION__);
        goto Exit;
    }
    LogMessageA(LOGLEVELINFO, "[%s] music volume saved %d", __FUNCTION__, localmusicvolume);
    
Exit:
    if (regkey)
    {
        RegCloseKey(regkey);
    }
    return result;

}

void LogMessageA(_In_ DWORD loglevel, _In_ char* message, _In_ ...) {
    size_t msglen = strlen(message);
    SYSTEMTIME time = { 0 };
    HANDLE logfilehandle = INVALID_HANDLE_VALUE;
    DWORD endoffile = 0;;
    DWORD numofbyteswritten = 0;
    char datetimestring[96] = {0};
    char severitystring[8] = { 0 };
    char formattedstring[4096] = { 0 };
    int error = 0;
    if (regparam.loglevel < loglevel) {
        return;
    }
    if (msglen < 1 || msglen > 4096) {
        return;
    }
    switch (loglevel)
    {
        case LOGLEVELNONE:
        {
            return;
        }
        case LOGLEVELINFO: 
        {
            strcpy_s(severitystring, sizeof(severitystring), "[INFO]");

            break;
        }
        case LOGLEVELWARNING:
        {
            strcpy_s(severitystring, sizeof(severitystring), "[WARN]");

            break;
        }
        case LOGLEVELERROR:
        {
            strcpy_s(severitystring, sizeof(severitystring), "[ERROR]");

            break;
        }
        case LOGLEVELDEBUG:
        {
            strcpy_s(severitystring, sizeof(severitystring), "[DEBUG]");

            break;
        }
        default:
        {

        }
    }

    GetLocalTime(&time);  

    va_list argpointer = NULL;
    va_start(argpointer, message);


    _vsnprintf_s(formattedstring,sizeof(formattedstring),_TRUNCATE,message, argpointer);

    va_end(argpointer);

    error = _snprintf_s(datetimestring, sizeof(datetimestring), _TRUNCATE, "\r\n[%02u/%02u/%u %02u:%02u:%02u.%03u]", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);


    EnterCriticalSection(&logcritsec);

    if((logfilehandle = CreateFileA(LOGFILENAME, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
        return;
    }
    endoffile = SetFilePointer(logfilehandle, 0, NULL, FILE_END);

    WriteFile(logfilehandle, datetimestring, (DWORD)strlen(datetimestring),&numofbyteswritten,NULL) ;
    WriteFile(logfilehandle, severitystring, (DWORD)strlen(severitystring), &numofbyteswritten, NULL);
    WriteFile(logfilehandle, formattedstring, (DWORD)strlen(formattedstring), &numofbyteswritten, NULL);

    if (logfilehandle != INVALID_HANDLE_VALUE) CloseHandle(logfilehandle);

    LeaveCriticalSection(&logcritsec);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------//






/////////////////////////////////////////////////////////////////misc////////////////////////////////////////////////////////////////////////

void Resolutionmanager() {
    RECT rect;

    if (GetClientRect(vidset.ggamewindow, &rect))
    {
        vidset.currentwindowwidth = rect.right - rect.left;
        vidset.currentwindowheight= rect.bottom - rect.top;
    }
}

DWORD initializeplayer(void) {
    DWORD error = ERROR_SUCCESS;



    player.ScreenposX = 96;
    player.ScreenposY = 170;
    player.worldpos.X = 96;
    player.worldpos.Y = 170;
    //camera.X = 0;
    //camera.Y = 264;

    rightplayerrect.size.x = 3;
    rightplayerrect.size.y = 25;

    leftplayerrect.size.x = 3;
    leftplayerrect.size.y = 25;

    feetplayerrect.size.x = 8;
    feetplayerrect.size.y = 1;

    headplayerrect.size.x = 10;
    headplayerrect.size.y = 3;
    
   
    player.direction = FACING_RIGHT_0;

    return(error);
}

void initializeenemy(void) 
{
    skeletonenemy01.worldposx = 663; //start position
    skeletonenemy01.worldposy = 200;
    skeletonenemy01.hitbox.size.x = 25;
    skeletonenemy01.hitbox.size.y = 26;
    skeletonenemy01.direction = -1;
}

void initializelevel(void) {


    //overworld (lvl1)
    overworldrect.size.x = 2400;
    overworldrect.size.y = 256;
    overworldrect.position.x = 0;
    overworldrect.position.y = 0;
    overworldrect.leveloffset = -480;


    //lvl2
    leveltworect.size.x = 720;
    leveltworect.size.y = 256;
    leveltworect.position.x = 0;
    leveltworect.position.y = 0;
    leveltworect.leveloffset = -480;

    currentarea = overworldrect;

}

void Blit32bppbitmaptobuffer(_In_ GAMEBITMAP* gamebitmap, _In_ int32_t x, int32_t y, _In_ int16_t brightnessadjustment) {
    // Define the screen dimensions
    const int32_t SCREEN_WIDTH = RESOLUTIONWIDTH;
    const int32_t SCREEN_HEIGHT = RESOLUTIONHEIGHT;


    int32_t startingscreenpixel = ((SCREEN_WIDTH * SCREEN_HEIGHT) - SCREEN_WIDTH) - (SCREEN_WIDTH * y) + x;

    int32_t startingbitmappixel = ((gamebitmap->BitmapInfo.bmiHeader.biWidth * gamebitmap->BitmapInfo.bmiHeader.biHeight) -
        gamebitmap->BitmapInfo.bmiHeader.biWidth);



    int32_t memoryoffset = 0;
    int32_t bitmapoffset = 0;
    PIXEL32 bitmappixel = { 0 };

    // Check if the sprite is entirely off bounds of the screen
    if (x > SCREEN_WIDTH || x + gamebitmap->BitmapInfo.bmiHeader.biWidth < 0 || y > SCREEN_HEIGHT || y + gamebitmap->BitmapInfo.bmiHeader.biHeight < 0) 
    {
        return;
    }

    for (int16_t YPixel = 0; YPixel < gamebitmap->BitmapInfo.bmiHeader.biHeight; YPixel++) {
        for (int16_t XPixel = 0; XPixel < gamebitmap->BitmapInfo.bmiHeader.biWidth; XPixel++) {
            // Check if the current pixel is off bounds of the screen
            if (x + XPixel >= SCREEN_WIDTH || x + XPixel < 0 ||
                y + YPixel >= SCREEN_HEIGHT || y + YPixel < 0) {
                continue;
            }

            memoryoffset = startingscreenpixel + XPixel - (SCREEN_WIDTH * YPixel);
            bitmapoffset = startingbitmappixel + XPixel - (gamebitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            memcpy_s(&bitmappixel, sizeof(PIXEL32), (PIXEL32*)gamebitmap->Memory + bitmapoffset, sizeof(PIXEL32));
            
            if (bitmappixel.alpha == 255) {
                //clamps btw 0-255
                //min (upper, max(x,lower)

                bitmappixel.red     = (uint8_t) min(255, max((bitmappixel.red + brightnessadjustment), 0));
                bitmappixel.green   = (uint8_t) min(255, max((bitmappixel.green + brightnessadjustment), 0));
                bitmappixel.blue    = (uint8_t) min(255, max((bitmappixel.blue + brightnessadjustment), 0));

                memcpy_s((PIXEL32*)vidset.backbuffer.Memory + memoryoffset, sizeof(PIXEL32), &bitmappixel, sizeof(PIXEL32));

            }
        }
    }
}

void blittilemaptobuffer(_In_ GAMEBITMAP* gamebitmap, _In_ int16_t brightnessadjustment) {

    int32_t startingscreenpixel = ((RESOLUTIONWIDTH * RESOLUTIONHEIGHT) - RESOLUTIONWIDTH);
   

   int32_t startingbitmappixel = ((gamebitmap->BitmapInfo.bmiHeader.biWidth * gamebitmap->BitmapInfo.bmiHeader.biHeight) - \
        gamebitmap->BitmapInfo.bmiHeader.biWidth) + (int)camera.X - (gamebitmap->BitmapInfo.bmiHeader.biWidth * (int)camera.Y);




    int32_t memoryoffset = 0;
    int32_t bitmapoffset = 0;

#ifdef AVX
    __m256i bitmapoctapixel;
    for (int16_t YPixel = 0; YPixel < RESOLUTIONHEIGHT;YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < RESOLUTIONWIDTH;XPixel += 8)
        {
            memoryoffset = startingscreenpixel + XPixel - (RESOLUTIONWIDTH * YPixel);

            bitmapoffset = startingbitmappixel + XPixel - (gamebitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            bitmapoctapixel = _mm256_loadu_si256((PIXEL32*)gamebitmap->Memory + bitmapoffset);           //causes warnings



            _mm256_store_si256((PIXEL32*)vidset.backbuffer.Memory + memoryoffset, bitmapoctapixel);



        }

    }
#elif defined SSE2
    __m128i bitmapquadpixel;

    for (int16_t YPixel = 0; YPixel < RESOLUTIONHEIGHT;YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < RESOLUTIONWIDTH;XPixel += 4)
        {
            memoryoffset = startingscreenpixel + XPixel - (RESOLUTIONWIDTH * YPixel);

            bitmapoffset = startingbitmappixel + XPixel - (gamebitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            bitmapquadpixel = _mm_load_si128((PIXEL32*)gamebitmap->Memory + bitmapoffset);

            _mm_store_si128((PIXEL32*)vidset.backbuffer.Memory + memoryoffset, bitmapquadpixel);
        }

    }
#else

    PIXEL32 bitmappixel = { 0 };

    for (int16_t YPixel = 0; YPixel < RESOLUTIONHEIGHT;YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < RESOLUTIONWIDTH;XPixel++)
        {
            memoryoffset = startingscreenpixel + XPixel - (RESOLUTIONWIDTH * YPixel);
            bitmapoffset = startingbitmappixel + XPixel - (gamebitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            memcpy_s(&bitmappixel, sizeof(PIXEL32), (PIXEL32*)gamebitmap->Memory + bitmapoffset, sizeof(PIXEL32));

            //clamps btw 0-255
            //min (upper, max(x,lower)

            bitmappixel.red     = (uint8_t) min(255, max((bitmappixel.red + brightnessadjustment), 0));
            bitmappixel.green   = (uint8_t) min(255, max((bitmappixel.green + brightnessadjustment), 0));
            bitmappixel.blue    = (uint8_t) min(255, max((bitmappixel.blue + brightnessadjustment), 0));

            memcpy_s((PIXEL32*)vidset.backbuffer.Memory + memoryoffset, sizeof(PIXEL32), &bitmappixel, sizeof(PIXEL32));

        }

    }

#endif

} 

void Blitstringtobuffer(_In_ char* string, _In_ GAMEBITMAP* fontsheet,_In_ PIXEL32* color,  _In_ uint32_t x, uint32_t y) {
    int charW = fontsheet->BitmapInfo.bmiHeader.biWidth / FONT_SHEET_CHARPERROW;
    int charH = fontsheet->BitmapInfo.bmiHeader.biHeight;
    int bytesperchar = (charW * charH * (fontsheet->BitmapInfo.bmiHeader.biBitCount / 8));
    int len = (int)strlen(string);
    GAMEBITMAP stringbitmap = { 0 };
    stringbitmap.BitmapInfo.bmiHeader.biBitCount = BPP;
    stringbitmap.BitmapInfo.bmiHeader.biHeight = charH;
    stringbitmap.BitmapInfo.bmiHeader.biWidth = charW * len;
    stringbitmap.BitmapInfo.bmiHeader.biPlanes = 1;
    stringbitmap.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    stringbitmap.Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytesperchar * len);
    
    for (int c = 0; c < len; c++) {
        int startingfontsheetbyte = 0;
        int fontsheetoffset = 0;
        int stringbitmapoffset = 0;
        PIXEL32 fontsheetpixel = { 0 };
        startingfontsheetbyte = (fontsheet->BitmapInfo.bmiHeader.biWidth * fontsheet->BitmapInfo.bmiHeader.biHeight) - fontsheet->BitmapInfo.bmiHeader.biWidth + (charW * gFontCharacterPixelOffset[(unsigned char)string[c]]);

        for (int ypix = 0; ypix < charH; ypix++) {
            for (int xpix = 0; xpix < charW; xpix++) {
                fontsheetoffset = startingfontsheetbyte + xpix - (fontsheet->BitmapInfo.bmiHeader.biWidth * ypix);
                stringbitmapoffset = (c * charW) + (stringbitmap.BitmapInfo.bmiHeader.biWidth * stringbitmap.BitmapInfo.bmiHeader.biHeight) - \
                    stringbitmap.BitmapInfo.bmiHeader.biWidth + xpix - (stringbitmap.BitmapInfo.bmiHeader.biWidth) * ypix;

                memcpy_s(&fontsheetpixel, sizeof(PIXEL32), (PIXEL32*)fontsheet->Memory + fontsheetoffset, sizeof(PIXEL32));
                fontsheetpixel.red = color->red;
                fontsheetpixel.green = color->green;
                fontsheetpixel.blue = color->blue;



                memcpy_s((PIXEL32*)stringbitmap.Memory + stringbitmapoffset, sizeof(PIXEL32), &fontsheetpixel, sizeof(PIXEL32));
            }
        }
    }

    Blit32bppbitmaptobuffer(&stringbitmap, x, y, 0);

    if (stringbitmap.Memory) HeapFree(GetProcessHeap(), 0, stringbitmap.Memory);
}

void playgamesound(_In_ GAMESOUND* gamesound) {
    xaudioSFXsourcevoice[xaudioSFXsourcevoiceselector]->lpVtbl->SubmitSourceBuffer(xaudioSFXsourcevoice[xaudioSFXsourcevoiceselector], &gamesound->buffer, NULL);
    xaudioSFXsourcevoice[xaudioSFXsourcevoiceselector]->lpVtbl->Start(xaudioSFXsourcevoice[xaudioSFXsourcevoiceselector], 0, XAUDIO2_COMMIT_NOW);
    xaudioSFXsourcevoiceselector++;
    if (xaudioSFXsourcevoiceselector > (NUMBEROFSFXSOURCEVOICES - 1)) xaudioSFXsourcevoiceselector = 0;
}

//void Blit32bppbitmaptobuffer(_In_ GAMEBITMAP* gamebitmap, _In_ int32_t x, int32_t y) {
//
//
//    int32_t startingscreenpixel = ((RESOLUTIONWIDTH * RESOLUTIONHEIGHT) - RESOLUTIONWIDTH) - (RESOLUTIONWIDTH * y) + x ;
//    
//    int32_t startingbitmappixel = ((gamebitmap->BitmapInfo.bmiHeader.biWidth * gamebitmap->BitmapInfo.bmiHeader.biHeight) - \
//            gamebitmap->BitmapInfo.bmiHeader.biWidth);
//
//
//
//    //player.playerspritedimx = gamebitmap->BitmapInfo.bmiHeader.biWidth;
//    //player.playerspritedimy = gamebitmap->BitmapInfo.bmiHeader.biHeight;
//    int32_t memoryoffset = 0;
//    int32_t bitmapoffset = 0;
//    PIXEL32 bitmappixel = { 0 };
//    PIXEL32 bgpixel = { 0 };
//
//
//    for (int16_t YPixel = 0; YPixel < gamebitmap->BitmapInfo.bmiHeader.biHeight;YPixel++)
//    {
//
//        for (int16_t XPixel = 0; XPixel < gamebitmap->BitmapInfo.bmiHeader.biWidth;XPixel++)
//        {
//            if ((x < -2) || (x > RESOLUTIONWIDTH - gamebitmap->BitmapInfo.bmiHeader.biWidth) ||
//                (y < 1) || (y > RESOLUTIONHEIGHT - gamebitmap->BitmapInfo.bmiHeader.biHeight)) 
//            {
//
//                if (x < -2) {
//                    if (XPixel < - x) 
//                    {
//                        break;
//                    }
//                }
//                
//                else if (x > RESOLUTIONWIDTH - gamebitmap->BitmapInfo.bmiHeader.biWidth)
//                {
//                    if (XPixel > RESOLUTIONWIDTH - x - 1) 
//                    {
//                        break;
//                    }
//                }
//
//                if (y < 1) 
//                {
//                    if (YPixel < -y ) 
//                    {
//                        break;
//                    }
//
//                    else if (y > RESOLUTIONHEIGHT - gamebitmap->BitmapInfo.bmiHeader.biHeight-50) 
//                    {
//                        if (YPixel > RESOLUTIONHEIGHT - y - 1)
//                        {
//                            break;
//                        }
//                    }
//                }
//
//
//
//            }
//
//            memoryoffset = startingscreenpixel + XPixel - (RESOLUTIONWIDTH * YPixel);
//
//            bitmapoffset = startingbitmappixel + XPixel - (gamebitmap->BitmapInfo.bmiHeader.biWidth * YPixel);
//
//            memcpy_s(&bitmappixel, sizeof(PIXEL32), (PIXEL32*)gamebitmap->Memory + bitmapoffset, sizeof(PIXEL32));
//            if (bitmappixel.alpha == 255) 
//            {
//                memcpy_s((PIXEL32*)vidset.backbuffer.Memory + memoryoffset, sizeof(PIXEL32), &bitmappixel, sizeof(PIXEL32));
//            }
//
//        }
//
//    }
//
//}


//---------------------------------------------------------------------------------------------------------------------------------------------------//


DWORD load32bppbitmapfrommemory(_In_ void* buffer, _Inout_ GAMEBITMAP* gamebitmap) {
    
    DWORD error = ERROR_SUCCESS;
    WORD bitmapheader = 0;
    DWORD pixeldataoffset = 0;
    //DWORD numberofbytesread = 2;


    memcpy(&bitmapheader, buffer, sizeof(WORD));


    if (bitmapheader != 0x4d42)//"BM" backwards
    {
        error = ERROR_INVALID_DATA;
        return error;
    }

    memcpy(&pixeldataoffset, (BYTE*)buffer + 0xA,sizeof(DWORD));

    memcpy(&gamebitmap->BitmapInfo.bmiHeader, (BYTE*)buffer + 0xE, sizeof(BITMAPINFOHEADER));


    gamebitmap->Memory = (BYTE*)buffer + pixeldataoffset;


    if (error == ERROR_SUCCESS) LogMessageA(LOGLEVELINFO, "[%s] loading successful", __FUNCTION__);
    else LogMessageA(LOGLEVELERROR, "[%s] loading FAILED", __FUNCTION__);
    return(error);
}


HRESULT initsoundengine(void) {
    HRESULT result = S_OK;
    WAVEFORMATEX sfxwaveformat = { 0 };
    WAVEFORMATEX musicwaveformat = { 0 };
    result = CoInitializeEx(NULL, COINIT_MULTITHREADED);


    if (result != S_OK) {
        LogMessageA(LOGLEVELERROR, "[%s] coinitializeex failed", __FUNCTION__, result);
        goto Exit;
    }

    result = XAudio2Create(&xaudio, 0,XAUDIO2_ANY_PROCESSOR);
    if (FAILED(result)) {
        LogMessageA(LOGLEVELERROR, "[%s] audiocreate failed", __FUNCTION__, result);
        goto Exit;
    }
    result = xaudio->lpVtbl->CreateMasteringVoice(xaudio, &xaudiomasteringvoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL, 0);
    if (FAILED(result)) {
        LogMessageA(LOGLEVELERROR, "[%s] CreateMasteringVoice failed", __FUNCTION__, result);
        goto Exit;
    }
    sfxwaveformat.wFormatTag = WAVE_FORMAT_PCM;
    sfxwaveformat.nChannels = 1; //mono
    sfxwaveformat.nSamplesPerSec = 44100;
    sfxwaveformat.nAvgBytesPerSec = sfxwaveformat.nSamplesPerSec * sfxwaveformat.nChannels * 2;
    sfxwaveformat.nBlockAlign = sfxwaveformat.nChannels * 2;
    sfxwaveformat.wBitsPerSample = 16;
    sfxwaveformat.cbSize = 0x6164;
    for (int count = 0; count < NUMBEROFSFXSOURCEVOICES; count++) {
        result = xaudio->lpVtbl->CreateSourceVoice(xaudio, &xaudioSFXsourcevoice[count], &sfxwaveformat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
        if (result != S_OK) {
            LogMessageA(LOGLEVELERROR, "[%s] CreateSourceVoice for SFX failed", __FUNCTION__, result);
            goto Exit;
        }
        xaudioSFXsourcevoice[count]->lpVtbl->SetVolume(xaudioSFXsourcevoice[count], sfxvolume, XAUDIO2_COMMIT_NOW);
    }
    musicwaveformat.wFormatTag = WAVE_FORMAT_PCM;
    musicwaveformat.nChannels = 2; //stereo
    musicwaveformat.nSamplesPerSec = 44100;
    musicwaveformat.nAvgBytesPerSec = musicwaveformat.nSamplesPerSec * musicwaveformat.nChannels * 2;
    musicwaveformat.nBlockAlign = musicwaveformat.nChannels * 2;
    musicwaveformat.wBitsPerSample = 16;
    musicwaveformat.cbSize = 0;
    result = xaudio->lpVtbl->CreateSourceVoice(xaudio, &xaudiomusicsourcevoice, &musicwaveformat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
    if (result != S_OK) {
        LogMessageA(LOGLEVELERROR, "[%s] CreateSourceVoice for MUSIC failed", __FUNCTION__, result);
        goto Exit;
    }
    xaudiomusicsourcevoice->lpVtbl->SetVolume(xaudiomusicsourcevoice, musicvolume, XAUDIO2_COMMIT_NOW);



Exit:

    return(result);

}


DWORD loadwavfrommemory(_In_ void* buffer, _Inout_ GAMESOUND* gamesound) 
{
    DWORD error = ERROR_SUCCESS;
    //DWORD numberofbytesread;
    DWORD RIFF = 0;
    uint16_t dataoffset = 0;
    DWORD datachunksearcher = 0;
    BOOL datachunkfound = FALSE;
    DWORD datachunksize = 0;

    //HANDLE filehandle = INVALID_HANDLE_VALUE;

    //if ((filehandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] createfileA failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    memcpy(&RIFF, buffer, sizeof(DWORD));

    //if (ReadFile(filehandle, &RIFF, sizeof(DWORD), &numberofbytesread, NULL) == 0) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] ReadFile failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    if (RIFF != 0x46464952)//"RIFF" backwards
    {
        error = ERROR_FILE_INVALID;
        LogMessageA(LOGLEVELERROR, "[%s] first 4 bytes check, filetype check failed ", __FUNCTION__, error);
        goto Exit;
    }

    //if (SetFilePointer(filehandle, 20, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] SetFilePointer failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    memcpy(&gamesound->waveformat, (BYTE*)buffer + 20, sizeof(WAVEFORMATEX));

    //if (ReadFile(filehandle, &gamesound->waveformat, sizeof(WAVEFORMATEX), &numberofbytesread, NULL) == 0) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] ReadFile (second one)  failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    if (gamesound->waveformat.nBlockAlign != ((gamesound->waveformat.nChannels * gamesound->waveformat.wBitsPerSample) / 8) ||
        (gamesound->waveformat.wFormatTag != WAVE_FORMAT_PCM) || (gamesound->waveformat.wBitsPerSample != 16))
    {
        error = ERROR_DATATYPE_MISMATCH;
        LogMessageA(LOGLEVELERROR, "[%s] this wav data did not meet format specifications, only PCM format allowed, 44.1khz, 16bps", __FUNCTION__, error);
        goto Exit;
    }

    while (datachunkfound == FALSE)
    {
        //if (SetFilePointer(filehandle, dataoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
        //    error = GetLastError();
        //    LogMessageA(LOGLEVELERROR, "[%s] SetFilePointer failed", __FUNCTION__, error);
        //    goto Exit;
        //}
        //if (ReadFile(filehandle, &datachunksearcher, sizeof(DWORD), &numberofbytesread, NULL) == 0) {
        //    error = GetLastError();
        //    LogMessageA(LOGLEVELERROR, "[%s] ReadFile (second one)  failed", __FUNCTION__, error);
        //    goto Exit;
        //}

        memcpy(&datachunksearcher, (BYTE*)buffer + dataoffset, sizeof(DWORD));

        if (datachunksearcher == 0x61746164)
        {//spells letters data backwards
            datachunkfound = true;
            break;
        }
        else
        {
            dataoffset += 4;
        }
        if (dataoffset > 256)
        {
            error = ERROR_DATATYPE_MISMATCH;
            LogMessageA(LOGLEVELERROR, "[%s] data not found", __FUNCTION__, error);
            goto Exit;
        }
    }

    

    //if (SetFilePointer(filehandle, dataoffset + 4, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] SetFilePointer failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    memcpy(&datachunksize, (BYTE*)buffer + dataoffset + 4, sizeof(DWORD));

    //if (ReadFile(filehandle, &datachunksize, sizeof(DWORD), &numberofbytesread, NULL) == 0) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] ReadFile (third one)  failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    //gamesound->buffer.pAudioData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, datachunksize);

    //if (gamesound->buffer.pAudioData == NULL) {
    //    error = ERROR_NOT_ENOUGH_MEMORY;
    //    LogMessageA(LOGLEVELERROR, "[%s] heapalloc  failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    gamesound->buffer.Flags = XAUDIO2_END_OF_STREAM;
    gamesound->buffer.AudioBytes = datachunksize;


    //if (SetFilePointer(filehandle, dataoffset + 8, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] SetFilePointer failed", __FUNCTION__, error);
    //    goto Exit;
    //}
    //if (ReadFile(filehandle, gamesound->buffer.pAudioData, datachunksize, &numberofbytesread, NULL) == 0) {
    //    error = GetLastError();
    //    LogMessageA(LOGLEVELERROR, "[%s] ReadFile (fourth one)  failed", __FUNCTION__, error);
    //    goto Exit;
    //}

    gamesound->buffer.pAudioData = (BYTE*)buffer + dataoffset + 8;


Exit:
    if (error == ERROR_SUCCESS) LogMessageA(LOGLEVELINFO, "[%s] successfully loaded from memory .", __FUNCTION__);
    else LogMessageA(LOGLEVELERROR, "[%s] failed to load wav file from memory. Error code: 0x%08lx!", __FUNCTION__, error);

    //if (filehandle && (filehandle != INVALID_HANDLE_VALUE)) CloseHandle(filehandle);
    return(error);
}


DWORD loadtilemapfrommemory(_In_ void* buffer,_In_ uint32_t buffersize , _Inout_ TILEMAP* tilemap) {
    DWORD error = ERROR_SUCCESS;
    DWORD bytesread = 0;
    char* cursor = NULL;
    char tempbuffer[16] = { 0 };
    uint16_t rows;
    uint16_t cols;
    int tile_size = 8;
    static int iterationcounter = 0;


    if ((cursor = strstr(buffer, "width=")) == NULL) {
        error = ERROR_INVALID_DATA;
        LogMessageA(LOGLEVELERROR, "[%s] cant locate width attribute error code 0x%08lx", __FUNCTION__, error);
        goto Exit;
    }


    bytesread = 0;

    for (;;) {

        if (bytesread > 8) {
            //shoud have foundn opening quotationmark by now
            error = ERROR_INVALID_DATA;
            LogMessageA(LOGLEVELERROR, "[%s] cant locate opening quote b4 width attribute error code 0x%08lx", __FUNCTION__, error);
            goto Exit;
        }
        if (*cursor == '\"') {
            cursor++;
            break;
        }
        else
        {
            cursor++;
        }


        bytesread++;


    }

    bytesread = 0;

    for (int count = 0; count < 6;count++) {
        if (*cursor == '\"') {
            cursor++;
            break;
        }
        else {
            tempbuffer[count] = *cursor;
            cursor++;
        }
    }

    tilemap->width = atoi(tempbuffer);

    if (tilemap->width == 0) {
        error = ERROR_INVALID_DATA;
        LogMessageA(LOGLEVELERROR, "[%s] width attribute is 0 lol error code 0x%08lx", __FUNCTION__, error);
        goto Exit;
    }

    memset(tempbuffer, 0, sizeof(tempbuffer));

    if ((cursor = strstr(buffer, "height=")) == NULL) {
        error = ERROR_INVALID_DATA;
        LogMessageA(LOGLEVELERROR, "[%s] cant locate height attribute error code 0x%08lx", __FUNCTION__, error);
        goto Exit;
    }


    bytesread = 0;

    for (;;) {

        if (bytesread > 8) {
            //shoud have foundn opening quotationmark by now
            error = ERROR_INVALID_DATA;
            LogMessageA(LOGLEVELERROR, "[%s] cant locate opening quote b4 height attribute error code 0x%08lx", __FUNCTION__, error);
            goto Exit;
        }
        if (*cursor == '\"') {
            cursor++;
            break;
        }
        else
        {
            cursor++;
        }


        bytesread++;


    }

    bytesread = 0;

    for (int count = 0; count < 6;count++) {
        if (*cursor == '\"') {
            cursor++;
            break;
        }
        else {
            tempbuffer[count] = *cursor;
            cursor++;
        }
    }

    tilemap->height = (uint16_t)atoi(tempbuffer);

    if (tilemap->height == 0) {
        error = ERROR_INVALID_DATA;
        LogMessageA(LOGLEVELERROR, "[%s] height attribute is 0 lol error code 0x%08lx", __FUNCTION__, error);
        goto Exit;
    }


    LogMessageA(LOGLEVELINFO, "[%s] tilemap dimensions: %dx%d", __FUNCTION__, tilemap->width, tilemap->height);

    rows = tilemap->height;
    cols = tilemap->width;


    tilemap->map = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, rows * sizeof(void*));


    if (tilemap->map == NULL)
    {
        error = ERROR_OUTOFMEMORY;
        LogMessageA(LOGLEVELERROR, "[%s] HeapAlloc error code 0x%08lx", __FUNCTION__, error);
        goto Exit;
    }

    for (uint16_t count = 0; count < tilemap->height;count++) {

        tilemap->map[count] = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cols * sizeof(void*));

        if (tilemap->map[count] == NULL) {

            error = ERROR_OUTOFMEMORY;
            LogMessageA(LOGLEVELERROR, "[%s] HeapAlloc error code 0x%08lx", __FUNCTION__, error);
            goto Exit;

        }

    }

    bytesread = 0;

    memset(tempbuffer, 0, sizeof(tempbuffer));


    if ((cursor = strstr(buffer, ",")) == NULL)
    {
        error = ERROR_INVALID_DATA;
        LogMessageA(LOGLEVELERROR, "[%s] cant find comma error code 0x%08lx", __FUNCTION__, error);
        goto Exit;
    }


    while (*cursor != 0xA && *cursor != 0xD)
    {
        if (bytesread > 4) {
            error = ERROR_INVALID_DATA;
            LogMessageA(LOGLEVELERROR, "[%s] cant find newline at beginnign error code 0x%08lx", __FUNCTION__, error);
            goto Exit;
        }


        bytesread++;
        cursor--;
    }

    cursor++;

    for (uint16_t row = 0; row < rows;row++)
    {
        for (uint16_t col = 0; col < cols; col++)
        {
            memset(tempbuffer, 0, sizeof(tempbuffer));

        skip:

            if (*cursor == '\r' || *cursor == '\n')
            {
                cursor++;
                goto skip;
            }

            for (int count = 0; count < 8; count++)
            {
                if (*cursor == ',' || *cursor == '<')
                {
                    if (((tilemap->map[row][col]) = atoi(tempbuffer)) == 0)
                    {
                        error = ERROR_INVALID_DATA;
                        LogMessageA(LOGLEVELERROR, "[%s] atoi failed while converting mapdata error code 0x%08lx", __FUNCTION__, error);
                        goto Exit;

                    }
                    cursor++;
                    break;
                }
                tempbuffer[count] = *cursor;
                cursor++;
            }
        }
    }

    if (iterationcounter == 0) {
        overworldrects = malloc(sizeof(Rect) * tilemap->width * tilemap->height);

        if (overworldrects == NULL) {

        }
        else {
            for (int y = 0; y < tilemap->height; y++) {
                for (int x = 0; x < tilemap->width; x++) {
                    if (tilemap->map[y][x] != PASSABLETILE) {
                        int index = y * tilemap->width + x;
                        overworldrects[index].position.x = (float)(x * tile_size);
                        overworldrects[index].position.y = (float)(y * tile_size);
                        overworldrects[index].size.x = (float)tile_size;
                        overworldrects[index].size.y = (float)tile_size;
                        overworldrects[index].type = tilemap->map[y][x];
                    }
                }
            }

        }
    }

    if (iterationcounter == 1) {
        lvl2rects = malloc(sizeof(Rect) * tilemap->width * tilemap->height);

        if (lvl2rects == NULL) {

        }
        else {
            for (int y = 0; y < tilemap->height; y++) {
                for (int x = 0; x < tilemap->width; x++) {
                    if (tilemap->map[y][x] != PASSABLETILE) {
                        int index = y * tilemap->width + x;
                        lvl2rects[index].position.x = (float)(x * tile_size);
                        lvl2rects[index].position.y = (float)(y * tile_size);
                        lvl2rects[index].size.x = (float)tile_size;
                        lvl2rects[index].size.y = (float)tile_size;
                        lvl2rects[index].type = tilemap->map[y][x];
                    }
                }
            }

        }
    }
    

Exit:


    HeapFree(GetProcessHeap(), 0, tilemap->map);
    iterationcounter++;
    return(error);
}

//DWORD loadtilemapfrommemory(_In_ void* buffer, _In_ uint32_t buffersize, _Inout_ TILEMAP* tilemap) {
//    DWORD error = ERROR_SUCCESS;
//    DWORD bytesread = 0;
//    char* cursor = NULL;
//    char tempbuffer[16] = { 0 };
//    uint16_t rows;
//    uint16_t cols;
//   // int tile_size = 1; // Set tile_size to 1 for 1x1 tiles
//    static int iterationcounter = 0;
//
//    if ((cursor = strstr(buffer, "width=")) == NULL) {
//        error = ERROR_INVALID_DATA;
//        LogMessageA(LOGLEVELERROR, "[%s] cant locate width attribute error code 0x%08lx", __FUNCTION__, error);
//        goto Exit;
//    }
//
//
//    bytesread = 0;
//
//    for (;;) {
//
//        if (bytesread > 8) {
//            //shoud have foundn opening quotationmark by now
//            error = ERROR_INVALID_DATA;
//            LogMessageA(LOGLEVELERROR, "[%s] cant locate opening quote b4 width attribute error code 0x%08lx", __FUNCTION__, error);
//            goto Exit;
//        }
//        if (*cursor == '\"') {
//            cursor++;
//            break;
//        }
//        else
//        {
//            cursor++;
//        }
//
//
//        bytesread++;
//
//
//    }
//
//    bytesread = 0;
//
//    for (int count = 0; count < 6;count++) {
//        if (*cursor == '\"') {
//            cursor++;
//            break;
//        }
//        else {
//            tempbuffer[count] = *cursor;
//            cursor++;
//        }
//    }
//
//    tilemap->width = atoi(tempbuffer);
//
//    if (tilemap->width == 0) {
//        error = ERROR_INVALID_DATA;
//        LogMessageA(LOGLEVELERROR, "[%s] width attribute is 0 lol error code 0x%08lx", __FUNCTION__, error);
//        goto Exit;
//    }
//
//    memset(tempbuffer, 0, sizeof(tempbuffer));
//
//    if ((cursor = strstr(buffer, "height=")) == NULL) {
//        error = ERROR_INVALID_DATA;
//        LogMessageA(LOGLEVELERROR, "[%s] cant locate height attribute error code 0x%08lx", __FUNCTION__, error);
//        goto Exit;
//    }
//
//
//    bytesread = 0;
//
//    for (;;) {
//
//        if (bytesread > 8) {
//            //shoud have foundn opening quotationmark by now
//            error = ERROR_INVALID_DATA;
//            LogMessageA(LOGLEVELERROR, "[%s] cant locate opening quote b4 height attribute error code 0x%08lx", __FUNCTION__, error);
//            goto Exit;
//        }
//        if (*cursor == '\"') {
//            cursor++;
//            break;
//        }
//        else
//        {
//            cursor++;
//        }
//
//
//        bytesread++;
//
//
//    }
//
//    bytesread = 0;
//
//    for (int count = 0; count < 6;count++) {
//        if (*cursor == '\"') {
//            cursor++;
//            break;
//        }
//        else {
//            tempbuffer[count] = *cursor;
//            cursor++;
//        }
//    }
//
//    tilemap->height = (uint16_t)atoi(tempbuffer);
//
//    if (tilemap->height == 0) {
//        error = ERROR_INVALID_DATA;
//        LogMessageA(LOGLEVELERROR, "[%s] height attribute is 0 lol error code 0x%08lx", __FUNCTION__, error);
//        goto Exit;
//    }
//
//
//    LogMessageA(LOGLEVELINFO, "[%s] tilemap dimensions: %dx%d", __FUNCTION__, tilemap->width, tilemap->height);
//
//    rows = tilemap->height;
//    cols = tilemap->width;
//
//
//    tilemap->map = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, rows * sizeof(void*));
//
//
//    if (tilemap->map == NULL)
//    {
//        error = ERROR_OUTOFMEMORY;
//        LogMessageA(LOGLEVELERROR, "[%s] HeapAlloc error code 0x%08lx", __FUNCTION__, error);
//        goto Exit;
//    }
//
//    for (uint16_t count = 0; count < tilemap->height;count++) {
//
//        tilemap->map[count] = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cols * sizeof(void*));
//
//        if (tilemap->map[count] == NULL) {
//
//            error = ERROR_OUTOFMEMORY;
//            LogMessageA(LOGLEVELERROR, "[%s] HeapAlloc error code 0x%08lx", __FUNCTION__, error);
//            goto Exit;
//
//        }
//
//    }
//
//    bytesread = 0;
//
//    memset(tempbuffer, 0, sizeof(tempbuffer));
//
//
//    if ((cursor = strstr(buffer, ",")) == NULL)
//    {
//        error = ERROR_INVALID_DATA;
//        LogMessageA(LOGLEVELERROR, "[%s] cant find comma error code 0x%08lx", __FUNCTION__, error);
//        goto Exit;
//    }
//
//
//    while (*cursor != 0xA && *cursor != 0xD)
//    {
//        if (bytesread > 4) {
//            error = ERROR_INVALID_DATA;
//            LogMessageA(LOGLEVELERROR, "[%s] cant find newline at beginnign error code 0x%08lx", __FUNCTION__, error);
//            goto Exit;
//        }
//
//
//        bytesread++;
//        cursor--;
//    }
//
//    cursor++;
//
//    for (uint16_t row = 0; row < rows;row++)
//    {
//        for (uint16_t col = 0; col < cols; col++)
//        {
//            memset(tempbuffer, 0, sizeof(tempbuffer));
//
//        skip:
//
//            if (*cursor == '\r' || *cursor == '\n')
//            {
//                cursor++;
//                goto skip;
//            }
//
//            for (int count = 0; count < 8; count++)
//            {
//                if (*cursor == ',' || *cursor == '<')
//                {
//                    if (((tilemap->map[row][col]) = (uint8_t)atoi(tempbuffer)) == 0)
//                    {
//                        error = ERROR_INVALID_DATA;
//                        LogMessageA(LOGLEVELERROR, "[%s] atoi failed while converting mapdata (The tilemap cannot contain any tiles with the value 0, because atoi cannot differentiate between 0 and failure.)error code 0x%08lx", __FUNCTION__, error);
//                        goto Exit;
//
//                    }
//                    cursor++;
//                    break;
//                }
//
//                tempbuffer[count] = *cursor;
//                cursor++;
//            }
//        }
//    }
//
//    if (iterationcounter == 0) {
//        int tile_size = 8; // Set tile_size to 1 for 1x1 tiles
//        overworldrects = malloc(sizeof(Rect) * tilemap->width * tilemap->height);
//
//        if (overworldrects == NULL) {
//
//        }
//        else {
//            for (int y = 0; y < tilemap->height; y++) {
//                for (int x = 0; x < tilemap->width; x++) {
//                    if (tilemap->map[y][x] != PASSABLETILE) {
//                        int index = y * tilemap->width + x;
//                        overworldrects[index].position.x = (float)(x * tile_size);
//                        overworldrects[index].position.y = (float)(y * tile_size);
//                        overworldrects[index].size.x = (float)tile_size;
//                        overworldrects[index].size.y = (float)tile_size;
//                        overworldrects[index].type = tilemap->map[y][x];
//                    }
//                }
//            }
//
//        }
//    }
//
//    if (iterationcounter == 1) {
//        int tile_size = 4; // Set tile_size to 1 for 1x1 tiles
//        lvl2rects = malloc(sizeof(Rect) * tilemap->width * tilemap->height);
//
//        if (lvl2rects == NULL) {
//
//        }
//        else {
//            for (int y = 0; y < tilemap->height; y++) {
//                for (int x = 0; x < tilemap->width; x++) {
//                    if (tilemap->map[y][x] != PASSABLETILE) {
//                        int index = y * tilemap->width + x;
//                        lvl2rects[index].position.x = (float)(x * tile_size);
//                        lvl2rects[index].position.y = (float)(y * tile_size);
//                        lvl2rects[index].size.x = (float)tile_size;
//                        lvl2rects[index].size.y = (float)tile_size;
//                        lvl2rects[index].type = tilemap->map[y][x];
//                    }
//                }
//            }
//
//        }
//    }
//
//Exit:
//
//    HeapFree(GetProcessHeap(), 0, tilemap->map);
//    iterationcounter++;
//    return(error);
//}
//---------------------------------------------------------------------------------------------------------------------------------------------------//


void RenderFrameGraphics(void) {
    switch (currentgamestate)
    {   
    case GAMESTATE_OPENINGSPLASH:
    {
        drawsplash();
        break;
    }

    case GAMESTATE_TITLESCREEN:
    {
        drawtitlescreen();
        break;
    }
    case GAMESTATE_OVERWORLD:
    {
        drawoverworld();
        break;
    }
    case GAMESTATE_LEVELONE:
    {
        drawlevelone();
        break;
    }
    case GAMESTATE_EXITYESNOSCREEN:
    {
        drawexityesnoscreen();
        break;
    }
    case GAMESTATE_OPTIONSSCREEN:
    {
        drawoptionsscreen();
        break;
    }
    case GAMESTATE_SAVEDATAOVERWRITE:
    {
        drawoverwritesavedatayesno();
        break;
    }
    default:
    {

    }

    }

    HDC Devicecontext = GetDC(vidset.ggamewindow);
    

    StretchDIBits(Devicecontext, 0, 0, vidset.currentwindowwidth, vidset.currentwindowheight, 0, 0, RESOLUTIONWIDTH, RESOLUTIONHEIGHT, vidset.backbuffer.Memory, &vidset.backbuffer.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);


    if (perfdata.displaydebuginfo == true) {//TEMPORARY
        SelectObject(Devicecontext, (HFONT)GetStockObject(ANSI_FIXED_FONT));
        char debugbuffer[64] = { 0 };
        sprintf_s(debugbuffer, sizeof(debugbuffer), "fps cooked: %0.1f", perfdata.Cookedfpsavg);
        TextOutA(Devicecontext, 0, 0, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "fps raw: %0.1f", perfdata.Rawfpsavg);
        TextOutA(Devicecontext, 0, 15, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "handles: %lu", perfdata.handlecount);
        TextOutA(Devicecontext, 0, 30, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "current window height: %d", vidset.currentwindowheight);
        TextOutA(Devicecontext, 0, 45, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "current window width: %d", vidset.currentwindowwidth);
        TextOutA(Devicecontext, 0, 60, debugbuffer, (int)strlen(debugbuffer));
      //  sprintf_s(debugbuffer, sizeof(debugbuffer), "min timer res: %.02f", perfdata.minimumtimerresolution/10000.0f);//
       // TextOutA(Devicecontext, 0, 75, debugbuffer, (int)strlen(debugbuffer));
       // sprintf_s(debugbuffer, sizeof(debugbuffer), "max timer res: %.02f", perfdata.maximumtimerresolution / 10000.0f);
       // TextOutA(Devicecontext, 0, 90, debugbuffer, (int)strlen(debugbuffer));
       // sprintf_s(debugbuffer, sizeof(debugbuffer), "cur timer res: %.02f", perfdata.currenttimerresolution / 10000.0f);
        //TextOutA(Devicecontext, 0, 105, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "cpu usage: %.01f%%", perfdata.CPUpercent);
        TextOutA(Devicecontext, 0, 150, debugbuffer, (int)strlen(debugbuffer));
        //sprintf_s(debugbuffer, sizeof(debugbuffer), "frames rendered: %l", perfdata.totalframesrendered);
        //TextOutA(Devicecontext, 0, 150, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "screen position x of player: %.3f", player.ScreenposX);
        TextOutA(Devicecontext, 0, 75, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "screen position y of player: %.3f", player.ScreenposY);
        TextOutA(Devicecontext, 0, 90, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "player facing: %d", player.facing);
        TextOutA(Devicecontext, 0, 105, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "worlspos X : %.3f", player.worldpos.X);
        TextOutA(Devicecontext, 0, 120, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "worldpos Y : %.3f", player.worldpos.Y);
        //TextOutA(Devicecontext, 0, 135, debugbuffer, (int)strlen(debugbuffer));
        //sprintf_s(debugbuffer, sizeof(debugbuffer), "playerrect x : %.2f", rightplayerrect.position.x);
        //TextOutA(Devicecontext, 0, 165, debugbuffer, (int)strlen(debugbuffer));
        //sprintf_s(debugbuffer, sizeof(debugbuffer), "playerrect Y : %.2f", rightplayerrect.position.y);
        TextOutA(Devicecontext, 0, 185, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "camera X : %.3f", camera.X);
        TextOutA(Devicecontext, 0, 200, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "camera Y : %.3f", camera.Y);
        TextOutA(Devicecontext, 0, 215, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "gravity : %.3f", player.gravity);
        TextOutA(Devicecontext, 0, 230, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "floorlevel : %.2f", player.floorlevel);
        TextOutA(Devicecontext, 0, 245, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "level: %d", currentgamestate);
        TextOutA(Devicecontext, 0, 260, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "feetplayer rect Y: %.3f", feetplayerrect.position.y);
        TextOutA(Devicecontext, 0, 275, debugbuffer, (int)strlen(debugbuffer));
        sprintf_s(debugbuffer, sizeof(debugbuffer), "playerhealth %d", player.HP);
        TextOutA(Devicecontext, 0, 290, debugbuffer, (int)strlen(debugbuffer));
    }


    ReleaseDC(vidset.ggamewindow, Devicecontext);
}

void ProcessPlayerInput(void) {
    
    if ((ginputenabled == FALSE) || (windowhasfocus == FALSE)) return;

    int msgboxid;

    gameinput.f3keydown = GetAsyncKeyState(VK_F3);
    gameinput.Escapekeydown = GetAsyncKeyState(VK_ESCAPE);
    gameinput.Leftkeydown = GetAsyncKeyState(VK_LEFT)|GetAsyncKeyState('A');
    gameinput.Rightkeydown = GetAsyncKeyState(VK_RIGHT)|GetAsyncKeyState('D');
    gameinput.Upkeydown = GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W') | GetAsyncKeyState('X') ;
    gameinput.Downkeydown = GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S');
    gameinput.choosekeydown = GetAsyncKeyState(VK_RETURN) | GetAsyncKeyState('X') | GetAsyncKeyState('Z');
    gameinput.zkeydown = GetAsyncKeyState('Z');
    gameinput.xkeydown = GetAsyncKeyState('X');







//-----------------------------------misc functions--------------------------------*3=====B  
    if (gameinput.Escapekeydown) {
        msgboxid = MessageBoxA(NULL, "Would you like to exit?", "", MB_ICONSTOP | MB_YESNO);
        if(msgboxid == 6) SendMessageA(vidset.ggamewindow, WM_CLOSE, 0, 0);
    }
    //if (gameinput.f3keydown && !gameinput.debugkeywasdown) {
    //    perfdata.displaydebuginfo = !perfdata.displaydebuginfo;
    //}
//--------------------------------------------------------------------------------*3=====B


    switch (currentgamestate)
    {

    case GAMESTATE_OPENINGSPLASH:
    {
        PPI_openingsplash();
        break;
    }

    case GAMESTATE_TITLESCREEN:
    {
        PPI_Titlescreen();
        break;
    }
    case GAMESTATE_OPTIONSSCREEN:
    {
        PPI_options();
        break;
    }
    case GAMESTATE_OVERWORLD:
    {
        PPI_overworld();    
        break;
    }
    case GAMESTATE_LEVELONE:
    {
        PPI_lvl1();
        currentarea = leveltworect;
        break;
    }
    case GAMESTATE_EXITYESNOSCREEN:
    {
        PPI_exityesno();
        break;
    }
    case GAMESTATE_SAVEDATAOVERWRITE:
    {
        PPI_overwritesavedatayesno();
        break;
    }
    default:
    {

    }

    }

    gameinput.debugkeywasdown = gameinput.f3keydown;
    gameinput.Leftkeywasdown = gameinput.Leftkeydown;
    gameinput.Rightkeywasdown = gameinput.Rightkeydown;
    gameinput.Upkeywasdown = gameinput.Upkeydown;
    gameinput.Downkeywasdown = gameinput.Downkeydown;
    gameinput.choosekeywasdown = gameinput.choosekeydown;
    gameinput.zkeywasdown = gameinput.zkeydown;
}

DWORD loadoggfrommemory(_In_ void* buffer, _In_ uint64_t buffersize, _Inout_ GAMESOUND* gamesound)
{

    
    DWORD ret = ERROR_SUCCESS;
    int samplesdecoded = 0;
    int channels = 0;
    int samplerate = 0;
    short* decodedaudio = NULL;



    samplesdecoded = stb_vorbis_decode_memory(buffer, (int)buffersize, &channels, &samplerate, &decodedaudio);

    if (samplesdecoded < 1) {
        ret = ERROR_BAD_COMPRESSION_BUFFER;
        LogMessageA(LOGLEVELERROR, "[%s] stb_vorbis_decode_memory failed error code 0x%08lx", __FUNCTION__, ret);
        goto Exit;
    }

    gamesound->waveformat.wFormatTag = WAVE_FORMAT_PCM;
    gamesound->waveformat.nChannels = channels;
    gamesound->waveformat.nSamplesPerSec = samplerate;
    gamesound->waveformat.nAvgBytesPerSec = gamesound->waveformat.nSamplesPerSec * gamesound->waveformat.nChannels * 2;
    gamesound->waveformat.nBlockAlign = gamesound->waveformat.nChannels * 2;
    gamesound->waveformat.wBitsPerSample = 16;
    gamesound->buffer.Flags = XAUDIO2_END_OF_STREAM;
    gamesound->buffer.AudioBytes = samplesdecoded * gamesound->waveformat.nChannels * 2;
    gamesound->buffer.pAudioData = (const BYTE*)decodedaudio;




Exit:
    return ret;
}

void playgamemusic(_In_ GAMESOUND* gamesound) {
    
    xaudiomusicsourcevoice->lpVtbl->Stop(xaudiomusicsourcevoice, 0, 0);
    gamesound->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    xaudiomusicsourcevoice->lpVtbl->FlushSourceBuffers;
    xaudiomusicsourcevoice->lpVtbl->SubmitSourceBuffer(xaudiomusicsourcevoice, &gamesound->buffer, NULL);
    xaudiomusicsourcevoice->lpVtbl->Start(xaudiomusicsourcevoice, 0, XAUDIO2_COMMIT_NOW);

}




////////////////////////////////////////////////////////////loading things//////////////////////////////////////////////////////////////////////


DWORD loadassetfromarchive(_In_ char* archivename, _In_ char* assetfilename, _In_ RESOURCETYPE resourcetype, _Inout_ void* resource) {
    
    DWORD error = ERROR_SUCCESS;
    mz_zip_archive archive = { 0 };
    BYTE* decompressedbuffer = NULL;
    size_t decompressedsize = 0;
    BOOL filefoundinarchive = FALSE;


    if ((mz_zip_reader_init_file(&archive, archivename, 0)) == FALSE) {
        error = mz_zip_get_last_error(&archive);
        LogMessageA(LOGLEVELERROR, "[%s] mz_zip_reader_init_file failed with 0x%08lx on %s", __FUNCTION__, error, archivename);
        goto exit;
    }

    //iterate file till we find the one we lookin for

    for (int i = 0; i < (int)mz_zip_reader_get_num_files(&archive); i++) 
    {
        mz_zip_archive_file_stat compressedfilestats = { 0 };
        if(mz_zip_reader_file_stat(&archive,i,&compressedfilestats)==MZ_FALSE)
        {
            error = mz_zip_get_last_error(&archive);
            LogMessageA(LOGLEVELERROR, "[%s] mz_zip_reader_file_stat failed with 0x%08lx archive: %s  file: %s", __FUNCTION__, error, archivename,assetfilename);
            goto exit;
        }
        if (_stricmp(compressedfilestats.m_filename, assetfilename) == 0) {
            filefoundinarchive = TRUE;
            if ((decompressedbuffer = mz_zip_reader_extract_to_heap(&archive, i, &decompressedsize, 0)) == NULL) {
                error = mz_zip_get_last_error(&archive);
                LogMessageA(LOGLEVELERROR, "[%s] mz_zip_reader_extract_to_heap failed with 0x%08lx archive: %s  file: %s", __FUNCTION__, error, archivename, assetfilename);
                goto exit;
            }
            break;
        }
    }
    if (filefoundinarchive == FALSE) {
        error = ERROR_FILE_NOT_FOUND;
        LogMessageA(LOGLEVELERROR, "[%s] file: %s not found in archive", __FUNCTION__,assetfilename);
        goto exit;
    }

    switch (resourcetype)
    {
        case TYPE_WAV:
        {
            error = loadwavfrommemory(decompressedbuffer, resource);

            break;
        }
        case TYPE_OGG:
        {
            error = loadoggfrommemory(decompressedbuffer,decompressedsize ,resource);
            break;
        }
        case TYPE_TILEMAP:
        {
            error = loadtilemapfrommemory(decompressedbuffer, (uint32_t)decompressedsize, resource);
            break;
        }
        case TYPE_BMPX:
        {
            error = load32bppbitmapfrommemory(decompressedbuffer, resource);
            break;
        }

        default:
        {
            error = FILE_TYPE_UNKNOWN;
            goto exit;
        }

    }



exit:

    mz_zip_reader_end(&archive);
     
    return error;

}

DWORD assetloadingthreadproc(_In_ LPVOID lpparam) {//do NOT let this make any gui elements for some reason
    
    DWORD error = ERROR_SUCCESS;

    //////////////////////////////////////////////////////////////////////////////////////essentials(font and splash sound)


    if ((loadassetfromarchive(ASSET_FILE, "6x7font.bmpx", TYPE_BMPX, &vidset.origfont)) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading font failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "splash.wav", TYPE_WAV, &splash) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    SetEvent(essentialsloadedevent);//esentials have been loaded

    //////////////////////////////////////////////////////////////////////////////////////miscellaneous


    if ((loadassetfromarchive(ASSET_FILE, "titlescreenbackground.bmpx", TYPE_BMPX, &vidset.bgbackbuff)) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading titlescreenbackground.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "skel1.bmpx", TYPE_BMPX, &skeletonenemy01.skeleton[0])) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading titlescreenbackground.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "skel2.bmpx", TYPE_BMPX, &skeletonenemy01.skeleton[1])) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading titlescreenbackground.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "skel3.bmpx", TYPE_BMPX, &skeletonenemy01.skeleton[2])) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading titlescreenbackground.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "skel4.bmpx", TYPE_BMPX, &skeletonenemy01.skeleton[3])) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading titlescreenbackground.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "skel5.bmpx", TYPE_BMPX, &skeletonenemy01.skeleton[4])) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading titlescreenbackground.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "skel6.bmpx", TYPE_BMPX, &skeletonenemy01.skeleton[5])) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading titlescreenbackground.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "lvl1.bmpx", TYPE_BMPX, &overworldgamemap.gamebitmap)) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading verticaltest.bmpx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "lvl2.bmpx", TYPE_BMPX, &level2gamemap.gamebitmap)) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading verticaltest.bmpx failed", __FUNCTION__);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "select.wav", TYPE_WAV, &menunav) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading select.wav failed", __FUNCTION__);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "blipselect.wav", TYPE_WAV, &choose) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading blipselect.wav failed", __FUNCTION__);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "badselection.wav", TYPE_WAV, &badselection) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading badselection.wav failed", __FUNCTION__);
        return 0;
    }

  
//////////////////////////////////////////////////////////////////////////////////////player sprites and such


    if (loadassetfromarchive(ASSET_FILE, "loop.ogg", TYPE_OGG, &level01music) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading loop.ogg failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "lvl1.tmx", TYPE_TILEMAP, &overworldgamemap.tilemap)) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading verticaltest.tmx failed", __FUNCTION__);
        return 0;
    }

    if ((loadassetfromarchive(ASSET_FILE, "lvl2.tmx", TYPE_TILEMAP, &level2gamemap.tilemap)) != ERROR_SUCCESS) {
        LogMessageA(LOGLEVELERROR, "[%s] loading verticaltest.tmx failed", __FUNCTION__);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "jane.bmpx", TYPE_BMPX, &player.Sprite[suit_0][FACING_RIGHT_0]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "right_walk1.bmpx", TYPE_BMPX, &player.Sprite[suit_0][FACING_RIGHT_1]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "right_walk2.bmpx", TYPE_BMPX, &player.Sprite[suit_0][FACING_RIGHT_2]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "facing_left_0.bmpx", TYPE_BMPX, &player.Sprite[suit_0][FACING_LEFT_0]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "left_walk1.bmpx", TYPE_BMPX, &player.Sprite[suit_0][FACING_LEFT_1]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "left_walk2.bmpx", TYPE_BMPX, &player.Sprite[suit_0][FACING_LEFT_2]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "newjump0.bmpx", TYPE_BMPX, &player.Sprite[suit_0][JUMPING_UP_0]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "newjump1.bmpx", TYPE_BMPX, &player.Sprite[suit_0][JUMPING_UP_1]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "newjump0L.bmpx", TYPE_BMPX, &player.Sprite[suit_0][JUMPING_UP_0L]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }



    if (loadassetfromarchive(ASSET_FILE, "newjump1L.bmpx", TYPE_BMPX, &player.Sprite[suit_0][JUMPING_UP_1L]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "Attack1.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK1]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }



    if (loadassetfromarchive(ASSET_FILE, "Attack2.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK2]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }



    if (loadassetfromarchive(ASSET_FILE, "Attack3.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK3]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "Attack4.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK4]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "Attack1L.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK1L]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "Attack2L.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK2L]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "Attack3L.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK3L]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "Attack4L.bmpx", TYPE_BMPX, &player.Sprite[suit_0][ATK4L]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "crouchleft.bmpx", TYPE_BMPX, &player.Sprite[suit_0][CROUCHLEFT]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    if (loadassetfromarchive(ASSET_FILE, "crouchright.bmpx", TYPE_BMPX, &player.Sprite[suit_0][CROUCHRIGHT]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "sword_extension_rightside.bmpx", TYPE_BMPX, &player.Sprite[suit_0][SWORD_EXTENSION_RIGHTSIDE]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "sword_extension_rightside_2.bmpx", TYPE_BMPX, &player.Sprite[suit_0][SWORD_EXTENSION_RIGHTSIDE_2]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "sword_extension_leftside.bmpx", TYPE_BMPX, &player.Sprite[suit_0][SWORD_EXTENSION_LEFTSIDE]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "sword_extension_leftside_2.bmpx", TYPE_BMPX, &player.Sprite[suit_0][SWORD_EXTENSION_LEFTSIDE_2]) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "hitboxR.bmpx", TYPE_BMPX, &hitboxR) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }



    if (loadassetfromarchive(ASSET_FILE, "hitboxL.bmpx", TYPE_BMPX, &hitboxL) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }



    if (loadassetfromarchive(ASSET_FILE, "hitboxF.bmpx", TYPE_BMPX, &hitboxF) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }

    if (loadassetfromarchive(ASSET_FILE, "hitboxH.bmpx", TYPE_BMPX, &hitboxH) != ERROR_SUCCESS) {
        MessageBoxA(NULL, "loading sound splash.wav failed :/", "game has crashed", MB_ICONERROR | MB_OK);
        return 0;
    }


    return error;
}

void initglobals(void)
{
    currentgamestate = GAMESTATE_OPENINGSPLASH;
    maingamerunning = TRUE;
    ginputenabled = FALSE;
    gfadetransitioning = FALSE;
}