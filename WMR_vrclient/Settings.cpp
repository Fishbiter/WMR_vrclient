#include "stdafx.h"
#include "Settings.h"
#include "picojson/picojson.h"
#include <fstream>
#include "VRSystemPatch/openvr019.h"

Settings g_settings;

extern std::ofstream g_log;

template<class T> void Set(picojson::value& node, T& member, const char* name)
{
	auto value = node.get(name);
	if (value.is<T>())
	{
		member = value.get<T>();
	}
	g_log << name << "=" << member << "\n";
}

void Settings::LoadForExe(wchar_t* exeName, wchar_t* settingsPath)
{
	char exeNameUtf8[1024];
	BOOL unused;
	char def = '0';
	WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS, exeName, -1, exeNameUtf8, 1024, &def, &unused);
	
	g_log << "Starting WMR_vrclient for \"" << exeNameUtf8 << "\"\n";

	picojson::value exes;
	std::ifstream infile(settingsPath);
	std::string error = picojson::parse(exes, infile);

	if (!error.empty())
	{
		g_log << "Misformated json - bailing: " << error << "\n";
		m_applyMod = false;
		return;
	}

	if (exes.is<picojson::object>())
	{
		auto node = exes.get(exeNameUtf8);
		if (!node.is<picojson::object>())
		{
			g_log << "No settings for exe " << exeNameUtf8 << " using default\n";
			node = exes.get("default");
		}

		Set(exes, m_deadzone, "deadzone");

		if(node.is<picojson::object>())
		{
			Set(node, m_applyMod, "applyMod");
			if (m_applyMod)
			{
				Set(node, m_touchPad, "touchPad");
				Set(node, m_pressPad, "pressPad");
				Set(node, m_mapToDPad, "mapToDPad");
				Set(node, m_deadzone, "deadzone");
			}
		}
	}
}
