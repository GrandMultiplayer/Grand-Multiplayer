// System
#include <atomic>
#include <thread>

// GrandM
#include <Common.h>
#include "Server.h"

// Shared
#include "Joaat.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#ifndef _WIN32
void Sleep(unsigned int seconds)
{
	sleep(seconds);
}
#endif

int main(int argc, char *argv[])
{
	Utility::GetConsole()->Allocate();

	// Provide the modification, version and gamename to the user.
	PRINT(ConsoleForeground::CYAN, true, "Loading %s v%s server for %s", INFO_NAME, VERSION, INFO_GAME_NAME);

	// Create CServer
	Server::Create();

	// Check if CServer is created
	if (!Server::GetServer())
	{
		LOG_ERROR("[CServer] Invalid");
		getc(stdin);
		return 1;
	}

	// Call OnLoad
	if (!Server::GetServer()->Load(argc, argv))
	{
		LOG_ERROR("[CServer] Could not be started");
		getc(stdin);
		return 1;
	}

	std::atomic<bool> run(true);
	std::thread Input(Server::Input, std::ref(run));

	// Main Server Process Loop
	while (Server::GetServer()->IsActive())
	{
		Server::GetServer()->Update();
	}

	// Call OnUnload
	Input.join();
	Server::GetServer()->Stop();
	return 1;
}