#ifndef __CGUI_H__
#define __CGUI_H__
//#define _CEFDISABLED
#include <WinSock2.h>
#include <Windows.h>

#include <chrono>
#include <string>
#include <vector>

#include "hook/types.h"

class CGui
{
private:
	static bool displayLoadingScreen;
	static bool displayMainMenu;
	static bool displayChat;

	static bool displayCursor;

	static std::vector<std::string> frames;

	static Vector2 resolution;

public:
	static bool displayGameSettings;

	static void Init();

	static const bool IsLoadingScreen() { return displayLoadingScreen; }
	// tOM = toggle other menus.
	static void ShowLoadingScreen(const bool state, const bool tOM = false);

	static const bool IsMainMenu() { return displayMainMenu; }
	// tOM = toggle other menus.
	static void ShowMainMenu(const bool state, const bool tOM = false);

	static const bool IsChat() { return displayChat; }
	// tOM = toggle other menus.
	static void ShowChat(const bool state, const bool tOM = false);

	static void SetVersion(std::string version);

	static void ShowConnectionError(std::string error, std::string message);

	static void SetLoadingText(std::string text);

	static void ShowChatMessage(std::string message);

	static void Refresh();

	static void ClearChat();
	static void ClearFrame();

	static void SetConnected(const bool state);

	static void GiveFocus(const bool hasCursor);
	static const bool IsCursorVisible() { return displayCursor; };

	static const bool DoesFrameExist(std::string frame);
	static void CreateFrame(std::string frame);
	static void RemoveFrame(std::string frame);
	static void LoadFrame(std::string frame, std::string url, const bool remote = false);

	static void LoadURL(std::string frame, std::string url, const bool remote = false, const bool resource = false);

	static Vector2 GetResolution();
	static void SetResolution(Vector2 res);
	static void SetResolution(const int x, const int y);

	class Debug
	{
	public:
		static void UpdateDebug();

		static float inbound;
		static float outbound;
		static float loss;

		static int fps;
		static int ping;

	private:
		static std::chrono::system_clock::time_point timeNow;
		static std::chrono::system_clock::time_point timeLast;
		static std::chrono::duration<double, std::milli> timeTook;
	};
};

#endif