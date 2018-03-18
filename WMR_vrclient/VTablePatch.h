#pragma once

template<class T> struct FunctionPatch
{
	T m_symbol;
	void* m_replaced;
	int m_entry = -1;
};

void** VTable(void* pVRSystem);

int GetNumberEntries(void** vtable);

void InstallWritableVtable(void* system);

template<class T> void PatchVTable(void* pVRSystem, void* pRefNoOverride, void* pOverride, FunctionPatch<T>& out)
{
	void** vtable = VTable(pRefNoOverride);
	void** compvtable = VTable(pOverride);
	void** tgtVtable = VTable(pVRSystem);

	for (int i = 0; i < GetNumberEntries(vtable); i++)
	{
		if (vtable[i] != compvtable[i])
		{
			out.m_entry = i;
			out.m_symbol = (T)tgtVtable[out.m_entry];
			tgtVtable[out.m_entry] = out.m_replaced = compvtable[i];
			return;
		}
	}
}

template<class T> void RemovePatchVTable(void* pVRSystem, const FunctionPatch<T>& out)
{
	if (out.m_entry == -1)
		return;

	void** vtable = VTable(pVRSystem);
	if (vtable[out.m_entry] == out.m_replaced)
		vtable[out.m_entry] = out.m_symbol;
}