#pragma once
#include <string>

struct Settings
{
	void LoadForExe(wchar_t* exeName, wchar_t* settingsPath);
	
	bool m_applyMod = false;
	bool m_touchPad = false;
	bool m_pressPad = false;
	bool m_mapToDPad = false;
	double m_deadzone = 0.15f;
};

extern Settings g_settings;

