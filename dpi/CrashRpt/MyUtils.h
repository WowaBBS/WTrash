#pragma once

template<typename T> T *GetLibraryFunction(HMODULE hLib, LPCSTR pszFunc)
{
   return reinterpret_cast<T*>(GetProcAddress(hLib,pszFunc));
}

template<typename T> T *GetModuleFunction(LPCTSTR pszModule, LPCSTR pszFunc)
{
   return reinterpret_cast<T*>(GetProcAddress(GetModuleHandle(pszModule),pszFunc));
}

#define GET_LIBRARY_FUNCTION(hLib,Func) GetLibraryFunction<decltype(Func)>(hLib,#Func)
#define GET_MODULE_FUNCTION(pszModule,Func) GetModuleFunction<decltype(Func)>(pszModule,#Func)

HMODULE LoadSystemLibrary(LPCTSTR pszName)
{
   TCHAR szPath[MAX_PATH];
   UINT Length=GetSystemDirectory(szPath, _countof(szPath));
   if(Length<1 || Length+1+lstrlen(pszName)>=_countof(szPath))
      return nullptr;
   PathAppend(szPath,pszName);
   return LoadLibrary(szPath);
}
