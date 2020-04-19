// Ini
#include "IniReader.h"

// GrandM
#include "Common.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "Config.h"

bool Config::Read()
{
	INIReader Config("Config.ini");

	if (Config.ParseError() < 0)
	{
		LOG_ERROR("[CConfig] Can't read configuration file");
		return false;
	}
	else
	{
		Connection.Ip = Config.Get("Connection", "ip", "127.0.0.1");
		Connection.Port = Config.GetInteger("Connection", "port", 2322);
		Connection.Pass = Config.Get("Connection", "password", "");
		Connection.Announce = Config.GetBoolean("Connection", "announce", false);

		Limits.MaxPlayers = Config.GetInteger("Limits", "players", 50);
		Limits.MaxFPS = Config.GetInteger("Limits", "fps", 100);
		Limits.tickRate = Config.GetInteger("Limits", "tickrate", 10);

		Information.Name = Config.Get("Information", "Name", "GrandM Server (" VERSION ")");
		Information.Gamemode = Config.Get("Information", "Gamemode", "Freeroam");
		Information.Website = Config.Get("Information", "Website", "");

		Plugins.Plugins = Config.Get("Plugins", "plugins", "Lua");

		LOG_PRINT("[CConfig] Loaded config");
		return true;
	}
	return false;
}