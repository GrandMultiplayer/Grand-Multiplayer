#ifndef __CCONFIG_H__
#define __CCONFIG_H__
#include <string>

class CConfig
{
	static struct Connection
	{
		std::string Ip;
		int			Port;
	} Connection;

	static struct Information
	{
		std::string Name;
	} Information;

	static struct Debug
	{
		bool		UI;		// Show the debug ui or not.
		bool		Log;	// Print debug info to console or not.
	} Debug;

	static struct Game
	{
		bool		Editor;
	} Game;

	static struct Chat
	{
		bool		Visible;
		int			ChatFontSize; // -5 > 5
		int			VisibleMessages; // 5 > 50
	} Chat;

public:
	static bool Read();
	static void Save();
	
	static std::string		GetIp() { return Connection.Ip; }
	static void				SetIp(const std::string ip) { Connection.Ip = ip; }

	static int				GetPort() { return Connection.Port; }
	static void				SetConPort(const int port) { Connection.Port = port; }

	static std::string		GetName() { return Information.Name; }
	static void				SetName(const std::string name) { Information.Name = name; }

	static bool				IsDebugUI() { return Debug.UI; }
	static void				SetDebugUI(const bool state) { Debug.UI = state; }

	static bool				IsDebugLog() { return Debug.Log; }
	static void				SetDebugLog(const bool state) { Debug.Log = state; }

	static bool				GetEditor() { return Game.Editor; }
	static void				SetEditor(const bool toggle) { Game.Editor = toggle; }

	static bool				IsChatVisible() { return Chat.Visible; }
	static void				SetChatVisible(const bool state) { Chat.Visible = state; }
	
	static const int		GetChatSize() { return Chat.ChatFontSize; }
	static void				SetChatSize(const int size) { Chat.ChatFontSize = size; }

	static const int		GetVisibleMessageCount() { return Chat.VisibleMessages; }
	static void				SetVisibleMessageCount(const int count) { Chat.VisibleMessages = count; }

	static void				SetupDefaults();
};

#endif