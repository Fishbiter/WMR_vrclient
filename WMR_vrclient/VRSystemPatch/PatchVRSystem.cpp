#include "stdafx.h"

#define DECLARE_PATCH_VERSION(x) namespace IVRSystem_##x \
{ \
void PatchVRSystem(void* pVRSystem, const char* interfaceVersion);\
void RemovePatchVRSystem();\
}

DECLARE_PATCH_VERSION(007);
DECLARE_PATCH_VERSION(009);
DECLARE_PATCH_VERSION(010);
DECLARE_PATCH_VERSION(011);
DECLARE_PATCH_VERSION(012);
DECLARE_PATCH_VERSION(014);
DECLARE_PATCH_VERSION(015);
DECLARE_PATCH_VERSION(016);
DECLARE_PATCH_VERSION(017);
DECLARE_PATCH_VERSION(019);

void PatchVRSystem(void* pVRSystem, const char* interfaceVersion)
{
	IVRSystem_007::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_009::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_010::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_011::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_012::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_014::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_015::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_016::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_017::PatchVRSystem(pVRSystem, interfaceVersion);
	IVRSystem_019::PatchVRSystem(pVRSystem, interfaceVersion);
}

void RemovePatchVRSystem()
{
	IVRSystem_007::RemovePatchVRSystem();
	IVRSystem_009::RemovePatchVRSystem();
	IVRSystem_010::RemovePatchVRSystem();
	IVRSystem_011::RemovePatchVRSystem();
	IVRSystem_012::RemovePatchVRSystem();
	IVRSystem_014::RemovePatchVRSystem();
	IVRSystem_015::RemovePatchVRSystem();
	IVRSystem_016::RemovePatchVRSystem();
	IVRSystem_017::RemovePatchVRSystem();
	IVRSystem_019::RemovePatchVRSystem();
}