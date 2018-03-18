#pragma once
#include <string>
#include "openvr.h"

struct Settings
{
	void LoadForExe(wchar_t* exeName, wchar_t* settingsPath);

	bool m_applyMod = false;
	bool m_touchPad = false;
	bool m_pressPad = false;
	bool m_mapToDPad = false;
	double m_patchOffset = 0;
	std::string m_systemVersion = vr::IVRSystem_Version;
};

extern Settings g_settings;

