#pragma once

#include <windows.h>

bool VerifyOSVersion(
   DWORD Major,BYTE MajorOperator,
   DWORD Minor,BYTE MinorOperator,
   DWORD Build,BYTE BuildOperator
)
{
   OSVERSIONINFOEX osvi={sizeof(osvi)};

   osvi.dwMajorVersion=Major;
   osvi.dwMinorVersion=Minor;
   osvi.dwBuildNumber =Build;

   ULONGLONG ConditionMask=0;
   VER_SET_CONDITION(ConditionMask, VER_MAJORVERSION ,MajorOperator);
   VER_SET_CONDITION(ConditionMask, VER_MINORVERSION ,MinorOperator);
   VER_SET_CONDITION(ConditionMask, VER_BUILDNUMBER  ,BuildOperator);

   return VerifyVersionInfo(
      &osvi,
      VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER,
      ConditionMask)!=FALSE;
}

bool VerifyOSVersion(
   DWORD Major,BYTE MajorOperator,
   DWORD Minor,BYTE MinorOperator
)
{
   OSVERSIONINFOEX osvi={sizeof(osvi)};

   osvi.dwMajorVersion=Major;
   osvi.dwMinorVersion=Minor;

   ULONGLONG ConditionMask=0;
   VER_SET_CONDITION(ConditionMask, VER_MAJORVERSION ,MajorOperator);
   VER_SET_CONDITION(ConditionMask, VER_MINORVERSION ,MinorOperator);

   return VerifyVersionInfo(
      &osvi,
      VER_MAJORVERSION | VER_MINORVERSION,
      ConditionMask)!=FALSE;
}

bool CheckOSVersion      (DWORD Major, DWORD Minor             ) { return VerifyOSVersion(Major,VER_EQUAL         ,Minor,VER_EQUAL                                 ); }
bool CheckOSVersion      (DWORD Major, DWORD Minor, DWORD Build) { return VerifyOSVersion(Major,VER_EQUAL         ,Minor,VER_EQUAL        , Build,VER_EQUAL        ); }
bool CheckOSVersionLater (DWORD Major, DWORD Minor             ) { return VerifyOSVersion(Major,VER_GREATER_EQUAL ,Minor,VER_GREATER_EQUAL                         ); }
bool CheckOSVersionLater (DWORD Major, DWORD Minor, DWORD Build) { return VerifyOSVersion(Major,VER_GREATER_EQUAL ,Minor,VER_GREATER_EQUAL, Build,VER_GREATER_EQUAL); }

template<DWORD Major, BYTE MajorOperator, DWORD Minor, BYTE MinorOperator, DWORD Build, BYTE BuildOperator> bool VerifyOSVersion() { static bool Res=VerifyOSVersion(Major, MajorOperator, Minor, MinorOperator, Build, BuildOperator); return Res; }
template<DWORD Major, BYTE MajorOperator, DWORD Minor, BYTE MinorOperator                                 > bool VerifyOSVersion() { static bool Res=VerifyOSVersion(Major, MajorOperator, Minor, MinorOperator                      ); return Res; }

template<DWORD Major, DWORD Minor             > bool CheckOSVersion      () { static bool Res=CheckOSVersion      (Major, Minor       ); return Res; }
template<DWORD Major, DWORD Minor, DWORD Build> bool CheckOSVersion      () { static bool Res=CheckOSVersion      (Major, Minor, Build); return Res; }
template<DWORD Major, DWORD Minor             > bool CheckOSVersionLater () { static bool Res=CheckOSVersionLater (Major, Minor       ); return Res; }
template<DWORD Major, DWORD Minor, DWORD Build> bool CheckOSVersionLater () { static bool Res=CheckOSVersionLater (Major, Minor, Build); return Res; }

bool IsWindowsXP                         () { return VerifyOSVersion<5,VER_EQUAL,1,VER_GREATER_EQUAL>(); }
bool IsWindowsVista                      () { return CheckOSVersion      < 6,0      >(); }
bool IsWindows7                          () { return CheckOSVersion      < 6,1      >(); }
bool IsWindows8                          () { return CheckOSVersion      < 6,2      >(); }
bool IsWindows8_1                        () { return CheckOSVersion      < 6,3      >(); }
bool IsWindows10                         () { return CheckOSVersion      <10,0      >(); }
bool IsWindows10AnniversaryUpdate        () { return CheckOSVersion      <10,0,14393>(); }
bool IsWindowsXPOrLater                  () { return CheckOSVersionLater < 5,1      >(); }
bool IsWindowsVistaOrLate                () { return CheckOSVersionLater < 6,0      >(); }
bool IsWindows7OrLater                   () { return CheckOSVersionLater < 6,1      >(); }
bool IsWindows8OrLater                   () { return CheckOSVersionLater < 6,2      >(); }
bool IsWindows8_1OrLater                 () { return CheckOSVersionLater < 6,3      >(); }
bool IsWindows10OrLater                  () { return CheckOSVersionLater <10,0      >(); }
bool IsWindows10AnniversaryUpdateOrLater () { return CheckOSVersionLater <10,0,14393>(); }
