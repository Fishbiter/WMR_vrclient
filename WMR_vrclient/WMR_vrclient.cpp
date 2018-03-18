#include "stdafx.h"
#include "windows.h"

#include "openvr.h"
#include "ivrclientcore.h"

#include "Settings.h"
#include <fstream>

void PatchVRSystem(void* pVRSystem, const char* interfaceVersion);

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

	g_log.close();
}

void UnloadRealDLL()
{
	FreeLibrary(g_lib);
}

extern "C"
{
	__declspec(dllexport) void* DLL_CALL VRClientCoreFactory(const char *pInterfaceName, int *pReturnCode)
	{
		if (g_settings.m_applyMod)
		{
			vr::IVRClientCore* clientCore = (vr::IVRClientCore*)g_VRClientCoreFactory(pInterfaceName, pReturnCode);
			
			vr::EVRInitError eError;
			vr::IVRSystem* system = (vr::IVRSystem*)clientCore->GetGenericInterface(g_settings.m_systemVersion.c_str(), &eError);
		
			PatchVRSystem(system, pInterfaceName);
		
			return clientCore;
		}
		
		return g_VRClientCoreFactory(pInterfaceName, pReturnCode);
	}

	__declspec(dllexport) void* DLL_CALL HmdSystemFactory(const char *pInterfaceName, int *pReturnCode)
	{
		return g_HmdSystemFactory(pInterfaceName, pReturnCode);
	}
}