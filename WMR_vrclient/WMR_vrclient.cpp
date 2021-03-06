#include "stdafx.h"
#include "windows.h"

#include "VRSystemPatch/openvr019.h"
#include "ivrclientcore.h"

#include "Settings.h"
#include <fstream>

extern "C"
{
	typedef void* (*HmdSystemFactoryFn)(const char *pInterfaceName, int *pReturnCode);
	HmdSystemFactoryFn g_HmdSystemFactory;
	typedef void* (*VRClientCoreFactoryFn)(const char *pInterfaceName, int *pReturnCode);
	VRClientCoreFactoryFn g_VRClientCoreFactory;
}

#if _WIN64
#define DLL_CALL __stdcall
#else
#define DLL_CALL
#endif

std::ofstream g_log;
HMODULE g_lib;

void LoadRealDLL(wchar_t* path, int len)
{
	wchar_t* slash = wcsrchr(path, '\\');
	wchar_t dllname[20];
		
	wcscpy_s(dllname, 20, slash + 1);

	slash[1] = 0;
	wcscat_s(path, len, L"WMR_output.txt");
	g_log = std::ofstream(path, std::ios_base::app);

	slash[1] = 0;
	wcscat_s(path, len, L"real_");
	wcscat_s(path, len, dllname);
	
	g_lib = LoadLibrary(path);

	if (!g_lib)
	{
		g_log << "Failed to open library " << path << "\n";
	}

	g_HmdSystemFactory = (HmdSystemFactoryFn)GetProcAddress(g_lib, "HmdSystemFactory");
	g_VRClientCoreFactory = (VRClientCoreFactoryFn)GetProcAddress(g_lib, "VRClientCoreFactory");
	
	slash[1] = 0;
	wcscat_s(path, len, L"WMR_vr_settings.json");

	wchar_t filename[1024];
	GetModuleFileName(nullptr, filename, 1024);
	wchar_t* exeName = wcsrchr(filename, '\\') + 1;

	g_settings.LoadForExe(exeName, path);
}

void UnloadRealDLL()
{
	FreeLibrary(g_lib);

	g_log.close();
}

void PatchClientCore(void* clientCore);
void PatchVRSystem(void* system, const char*);

extern "C"
{
	__declspec(dllexport) void* DLL_CALL VRClientCoreFactory(const char *pInterfaceName, int *pReturnCode)
	{
		vr::IVRClientCore* clientCore = (vr::IVRClientCore*)g_VRClientCoreFactory(pInterfaceName, pReturnCode);
		
		if (g_settings.m_applyMod)
		{
			PatchClientCore(clientCore);
		
			//patch the latest version of the interface - there's an alternative way of getting interfaces as a function table that I haven't fully figured out
			//luckily it seems to call through to the current interface... 
			vr::EVRInitError err;
			clientCore->GetGenericInterface(vr::IVRSystem_Version, &err);
		}

		return clientCore;
	}

	__declspec(dllexport) void* DLL_CALL HmdSystemFactory(const char *pInterfaceName, int *pReturnCode)
	{
		return g_HmdSystemFactory(pInterfaceName, pReturnCode);
	}
}