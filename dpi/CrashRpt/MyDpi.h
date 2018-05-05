#pragma once

#include "MyOsDetect.h"
#include "MyUtils.h"
//#include <windows.h>
//#include <windowsx.h>
//#include <shellscalingapi.h>

// See: https://github.com/DBCTRADO/TVTest/blob/master/src/DPIUtil.cpp

WINUSERAPI UINT WINAPI GetDpiForSystem(void);

WINUSERAPI DPI_AWARENESS_CONTEXT WINAPI SetThreadDpiAwarenessContext(
   DPI_AWARENESS_CONTEXT dpiContext
);

#ifndef _DPI_AWARENESS_CONTEXTS_
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
#define DPI_AWARENESS_CONTEXT_UNAWARE              ((DPI_AWARENESS_CONTEXT)-1)
#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE         ((DPI_AWARENESS_CONTEXT)-2)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    ((DPI_AWARENESS_CONTEXT)-3)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#endif

DPI_AWARENESS_CONTEXT MySetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT Context)
{
   if (IsWindows10AnniversaryUpdateOrLater()) {
      auto pSetThreadDpiAwarenessContext =
         GET_MODULE_FUNCTION(TEXT("user32.dll"), SetThreadDpiAwarenessContext);
      if (pSetThreadDpiAwarenessContext != nullptr)
         return pSetThreadDpiAwarenessContext(Context);
   }
   return nullptr;
}

// See: https://code.msdn.microsoft.com/windowsapps/DPI-Tutorial-sample-64134744/view/Discussions#content
typedef enum _PROCESS_DPI_AWARENESS { 
  PROCESS_DPI_UNAWARE            = 0,
  PROCESS_SYSTEM_DPI_AWARE       = 1,
  PROCESS_PER_MONITOR_DPI_AWARE  = 2
} PROCESS_DPI_AWARENESS;

WINUSERAPI HRESULT WINAPI SetProcessDpiAwareness(
  PROCESS_DPI_AWARENESS value
);

HRESULT MySetProcessDpiAwareness(PROCESS_DPI_AWARENESS value)
{
 //if(IsWindows10AnniversaryUpdateOrLater()) { // Unsuppoted in Win10
  //std::cout<<"MySetProcessDpiAwareness\n";
      static auto pSetProcessDpiAwareness =
       //GET_MODULE_FUNCTION(TEXT("user32.dll"), SetProcessDpiAwareness);
         GET_MODULE_FUNCTION(TEXT("Shcore.dll"), SetProcessDpiAwareness);
      if(pSetProcessDpiAwareness!=nullptr)
         return pSetProcessDpiAwareness(value);
 //}
//std::cout<<"Error MySetProcessDpiAwareness\n";
  return 0;
}

#ifndef DPI_ENUMS_DECLARED
typedef enum MONITOR_DPI_TYPE {
   MDT_EFFECTIVE_DPI = 0,
   MDT_ANGULAR_DPI   = 1,
   MDT_RAW_DPI       = 2,
   MDT_DEFAULT       = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;
#endif

STDAPI GetDpiForMonitor(
   HMONITOR hmonitor,
   MONITOR_DPI_TYPE dpiType,
   UINT *dpiX,
   UINT *dpiY
);

int GetMonitorDPI(HMONITOR hMonitor)
{
   if(hMonitor) // && IsWindows8_1OrLater()) { // TODO: Don`t work on windows 10
      if(HMODULE hLib=LoadSystemLibrary(TEXT("shcore.dll")))
      {
         UINT DpiX, DpiY;
         auto pGetDpiForMonitor = GET_LIBRARY_FUNCTION(hLib, GetDpiForMonitor);
         bool fOK=pGetDpiForMonitor &&
            pGetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &DpiX, &DpiY)==S_OK;
         FreeLibrary(hLib);

         if(fOK)
            return DpiY;
      }

   return 0;
}

int GetSystemDPI()
{
   static int SystemDPI = 0;

   if(SystemDPI!=0)
      return SystemDPI;

//DPI_AWARENESS_CONTEXT Old=MySetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
//DPI_AWARENESS_CONTEXT Old=MySetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
   MySetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
   POINT    pt;
 //HRESULT  hr = E_FAIL;

   int DPI;

   // Get the DPI for the main monitor, and set the scaling factor
   pt.x = 1;
   pt.y = 1;
   HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
 //HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
   DPI = GetMonitorDPI(hMonitor);

   static auto pGetDpiForSystem = GET_MODULE_FUNCTION(TEXT("user32.dll"), GetDpiForSystem);
   if(pGetDpiForSystem)
      DPI = pGetDpiForSystem();
   else if(SystemDPI==0)
   {
      if(HDC hdc=GetDC(nullptr))
      {
         SystemDPI=GetDeviceCaps(hdc, LOGPIXELSY);
         ReleaseDC(nullptr, hdc);
      }

      DPI = SystemDPI;
   }
 //MySetThreadDpiAwarenessContext(Old);

   return DPI;
}
