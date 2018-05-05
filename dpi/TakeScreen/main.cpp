#include "ScreenCap.h"
#include <iostream>

DWORD getParentPID(DWORD pid)
{
   HANDLE h = NULL;
   PROCESSENTRY32 pe = {0};
   DWORD ppid = 0;
   pe.dwSize = sizeof(PROCESSENTRY32);
   h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if(Process32First(h, &pe)) 
      do 
      {
         if(pe.th32ProcessID==pid) 
         {
            ppid = pe.th32ParentProcessID;
            break;
         }
      }
      while(Process32Next(h, &pe));
   
   CloseHandle(h);
   return (ppid);
}

int main(int argc, char* argv[])
{
  std::cout<<"Hello world!\n";
  
  DWORD pid = GetCurrentProcessId();
  std::cout<<"Current PID="<<pid<<"\n";
  pid = getParentPID(pid);
  std::cout<<"Parent PID="<<pid<<"\n";
  
  CScreenCapture ScreenCapture;
  ScreenshotInfo ssi;
  if(!ScreenCapture.TakeDesktopScreenshot(
    _T(".\\"),
    ssi, //ScreenshotInfo& 
    SCREENSHOT_TYPE_MAIN_WINDOW, //SCREENSHOT_TYPE_VIRTUAL_SCREEN, // SCREENSHOT_TYPE type
    pid, //85928, //92180, // DWORD dwProcessId = 
    SCREENSHOT_FORMAT_PNG, // SCREENSHOT_IMAGE_FORMAT fmt=
    95, // int nJpegQuality
    FALSE, // BOOL bGrayscale=
    0 //int nIdStartFrom=
  ))
    std::cout<<"[Error]\n";
  else
    std::cout<<"Ok\n";
  
  return 0;
}
