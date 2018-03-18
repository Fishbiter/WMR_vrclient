#include "stdafx.h"

#include "VRSystemPatch/openvr019.h"
#include "ivrclientcore.h"
#include "VTablePatch.h"

using namespace vr;

IVRClientCore* g_clientCore;

void PatchVRSystem(void* pVRSystem, const char* interfaceVersion);

class MyClientCore : public IVRClientCore
{
public:
	virtual EVRInitError Init(vr::EVRApplicationType eApplicationType, const char *pStartupInfo) { return EVRInitError(); }
	virtual void Cleanup() {}
	virtual EVRInitError IsInterfaceVersionValid(const char *pchInterfaceVersion) { return EVRInitError(); }
	virtual void *GetGenericInterface(const char *pchNameAndVersion, EVRInitError *peError) { return nullptr; }
	virtual bool BIsHmdPresent() { return false; }
	virtual const char *GetEnglishStringForHmdError(vr::EVRInitError eError) {return "";}
	virtual const char *GetIDForVRInitError(vr::EVRInitError eError) { return ""; }
} g_MyClientCore;

typedef void*(__thiscall *GetGenericInterfaceFn)(IVRClientCore*, const char*, EVRInitError *);
FunctionPatch<GetGenericInterfaceFn> g_realGetGenericInterfaceFn;

class ReplaceGetGenericInterface : public MyClientCore
{
	virtual void *GetGenericInterface(const char *pchNameAndVersion, EVRInitError *peError)
	{
		void* pInterface = g_realGetGenericInterfaceFn.m_symbol(this, pchNameAndVersion, peError);
		if (pInterface)
		{
			PatchVRSystem(pInterface, pchNameAndVersion);
		}
		return pInterface;
	}
} g_replaceGetGenericInterface;


typedef void (__thiscall *CleanupFn)(IVRClientCore*);
FunctionPatch<CleanupFn> g_realCleanupFn;

class ReplaceCleanup : public MyClientCore
{
	virtual void Cleanup()
	{
		RemovePatchVTable(this, g_realGetGenericInterfaceFn);
		RemovePatchVTable(this, g_realCleanupFn);
		g_realCleanupFn.m_symbol(this);
		g_clientCore = nullptr;
	}
} g_replaceCleanup;

void PatchClientCore(void* clientCore)
{
	if (g_clientCore)
		return;
	
	g_clientCore = (IVRClientCore*)clientCore;
	
	InstallWritableVtable(g_clientCore);

	PatchVTable(g_clientCore, &g_MyClientCore, &g_replaceGetGenericInterface, g_realGetGenericInterfaceFn);
	PatchVTable(g_clientCore, &g_MyClientCore, &g_replaceCleanup, g_realCleanupFn);
}

