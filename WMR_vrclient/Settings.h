#pragma once
#include <string>

struct Settings
{
	void LoadForExe(wchar_t* exeName, wchar_t* settingsPath);

	bool m_applyMod = false;
	bool m_touchPad = false;
	bool m_pressPad = false;
	bool m_mapToDPad = false;
};

extern Settings g_settings;

