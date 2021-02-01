// dllmain.cpp : 定義 DLL 應用程式的進入點。
#include "framework.h"
#include "MemberHook.h"
#include "IATinject.h"

int IATtargetsCount = 0;
IATtargetLib* IATtargets = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved
                     )
{
	if (dwReason != DLL_PROCESS_ATTACH) //if we arent attaching there is nothing to do
		return TRUE;
	ApplyHooks();
	return TRUE;
}

