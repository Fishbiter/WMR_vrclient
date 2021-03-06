// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

void RemovePatchVRSystem();
void LoadRealDLL(WCHAR* path, int len);
void UnloadRealDLL();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		WCHAR filename[1024];
		GetModuleFileName(hModule, filename, 1024);
		LoadRealDLL(filename, 1024);
		break;
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		RemovePatchVRSystem();
		UnloadRealDLL();
        break;
    }
    return TRUE;
}

