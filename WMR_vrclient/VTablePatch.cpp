#include "stdafx.h"
#include "VTablePatch.h"
#include "stdlib.h"
#include "stdio.h"

void** VTable(IVRSystem* pVRSystem)
{
	return (*((void****)&pVRSystem))[0];
}

int GetNumberEntries(void** vtable)
{
	int count = 0;
	while (*vtable++) count++;
	return count;
}

void InstallWritableVtable(IVRSystem* system)
{
	void** vtable = VTable(system);
	int size = GetNumberEntries(vtable);
	void** classAsPointerArray = *((void***)&system);
	classAsPointerArray[0] = malloc(size * sizeof(void*));
	memcpy(classAsPointerArray[0], vtable, size * sizeof(void*));
}

