#include <winsock2.h>
#include <Windows.h>
#include <string>

// Include
#include "IniReader.h"
#include "IniWriter.h"
#include "ini.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "CConfig.h"

struct CConfig::Connection CConfig::Connection;
struct CConfig::Information CConfig::Information;
struct CConfig::Game CConfig::Game;
struct CConfig::Debug CConfig::Debug;
struct CConfig::Chat CConfig::Chat;

bool CConfig::Read()
{
	char buffer[MAX_PATH];//always use MAX_PATH for filepaths
	GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));
	
	std::string filePath = buffer;
	filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size());

	INIReader Config(filePath + "Config.ini");

	const int p = Config.ParseError();
	if (p < 0)
	{
		LOG_ERROR("[CConfig] Can't read configuration file.");

		SetupDefaults();
		return false;
	}
	else
	{
		Connection.Ip = Config.Get("Connection", "ip", "127.0.0.1");
		Connection.Port = Config.GetInteger("Connection", "port", 2322);
		
		Information.Name = Config.Get("Information", "name", "A Grand Player");

		Game.Editor = Config.GetBoolean("Game", "editor", false);
		
		Debug.UI = Config.GetBoolean("Debug", "UI", false);
		Debug.Log = Config.GetBoolean("Debug", "Log", false);

		Chat.Visible = Config.GetBoolean("Chat", "Visible", true);
		Chat.ChatFontSize = Config.GetInteger("Chat", "FontSize", 0);
		Chat.VisibleMessages = Config.GetInteger("Chat", "VisibleMessages", 10);

		std::string favs = Config.Get("Server", "favorites", "");

		LOG_DEBUG("[CConfig] Loaded config.");
		return true;
	}

	LOG_ERROR("[CConfig] Error %s, Failed to load config.", p);
	return false;
}

void CConfig::Save() {
	LOG_DEBUG("[CConfig] Saving Config.");

	char buffer[MAX_PATH];		//always use MAX_PATH for filepaths
	GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));

	std::string filePath = buffer;
	filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size());

	INIWriter Config(filePath + "Config.ini");

	Config.WriteString("Connection", "ip", Connection.Ip);
	Config.WriteInteger("Connection", "port", Connection.Port);

	Config.WriteString("Information", "name", Information.Name);

	Config.WriteBoolean("Game", "editor", Game.Editor);

	Config.WriteBoolean("Debug", "UI", Debug.UI);
	Config.WriteBoolean("Debug", "Log", Debug.Log);

	Config.WriteBoolean("Chat", "Visible", Chat.Visible);
	Config.WriteInteger("Chat", "FontSize", Chat.ChatFontSize);
	Config.WriteInteger("Chat", "VisibleMessages", Chat.VisibleMessages);
}

void CConfig::SetupDefaults()
{
	LOG_PRINT("[CConfig] Setting up default config.");

	Connection.Ip = "127.0.0.1";
	Connection.Port = 2322;

	Information.Name = "A Grand Player";
	
	Debug.UI = false;

	Game.Editor = false;
	
	Chat.Visible = true;

	CConfig::Save();
}