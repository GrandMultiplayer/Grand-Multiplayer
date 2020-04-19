// System
#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#else
#define _BSD_SOURCE
#include <dlfcn.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <iostream>

// Raknet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "RakPeerInterface.h"

// GrandM
#include <Common.h>

#include "Config.h"
#include "RPCManager.h"

#include "Master.h"

#include "CWorld.h"

#include "CBlip.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"
#include "CCheckpoint.h"
#include "CPickup.h"
#include "CAPI.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "Server.h"

Config* g_Config;

std::unique_ptr<Server> Server::pServer = nullptr;

std::vector<CPlayer> g_Players;
std::vector<CVehicle> g_Vehicles;
std::vector<CObject> g_Objects;
std::vector<CNPC> g_Npcs;
std::vector<CCheckpoint> g_Checkpoints;
std::vector<CBlip> g_Blips;
std::vector<CPickup> g_Pickups;
std::vector<CAPI> g_ApiModules;

Server::Server()
{
	// Maximum amount of packets sent per second
	p_SyncRate = TICKRATE;
	// Unused, but it's supposed to limit the server FPS to 100.
	p_MaxFPS = 100;
	// If true, it shows the FPS in the console window. (windows only)
	p_ShowFPS = true;

	// Construct CConfig
	g_Config = new Config();

	// Construct CNetworkManager
	g_NetworkManager = std::unique_ptr<NetworkManager>(new NetworkManager);

	CWorld::Init();

	timeNow = std::chrono::system_clock::now();
	timeLast = std::chrono::system_clock::now();
	timeTook = timeNow - timeLast;

	LOG_PRINT("[Server] Constructed");
}


Server::~Server()
{
	SAFE_DELETE(g_Config);

	LOG_PRINT("[Server] Deconstructed");
}

#define USEAPI

bool Server::Load(int argc, char ** argv)
{

#ifdef _WIN32
	// Set Window Title containing the modname, modversion and build type
	SetConsoleTitle(L"" INFO_NAME "(" VERSION ") - " INFO_BUILD);
#endif

	if (!g_Config)
	{
		LOG_ERROR("[CConfig] Invalid");
		getc(stdin);
		return 1;
	}

	if (!g_Config->Read())
	{
		LOG_ERROR("[CConfig] Could not read config file");
		getc(stdin);
		return 1;
	}

	if (!GetNetworkManager()->Start())
	{
		LOG_ERROR("[CNetworkManager] Could not be started");
		getc(stdin);
		return 1;
	}

	// Register RPG Messages
	RPC::Manager::RegisterRPCMessages();

	// Split the plugin string for each defined plugin and insert into the pool.
	CAPI NewModule;
	std::string module;
	printf("Plugins: %s", g_Config->GetPlugins().c_str());
	for (unsigned int c = 0; c < g_Config->GetPlugins().size(); c++)
	{
		if (g_Config->GetPlugins()[c] != ' ')
		{
			module.push_back(g_Config->GetPlugins()[c]);
		}
		else
		{
			struct _stat64 filebuffer;
			if (stat(std::string("./plugin/" + module + LIBRARY_EXTENSION).c_str(), &filebuffer) == 0)
			{
				NewModule.SetModuleName(module);
				g_ApiModules.push_back(NewModule);
			}
			else
			{
				LOG_ERROR("Plugin %s not found.", module.c_str());
			}
			
			module = "";
		}

		if (c == g_Config->GetPlugins().size() - 1)
		{
			struct _stat64 filebuffer;
			if (stat(std::string("./plugin/" + module + LIBRARY_EXTENSION).c_str(), &filebuffer) == 0)
			{
				NewModule.SetModuleName(module);
				g_ApiModules.push_back(NewModule);
			}
			else
			{
				LOG_ERROR("Plugin %s not found.", module);
			}
		}
	}

#ifdef USEAPI
	// Load plugin modules
	for (unsigned int i = 0; i < g_ApiModules.size(); i++)
	{
		g_ApiModules[i].Load();
	}

	// Call Initialize function on our API
	for (unsigned int i = 0; i < g_ApiModules.size(); i++)
	{
		if (!g_ApiModules[i].Initialize())
		{
			LOG_ERROR("[CAPI]  %s could not be initialized.", g_ApiModules[i].ModuleName());
		}
	}
#endif

	p_Active = true;
	return true;
}

void Server::Stop()
{
	// Call Close function on our API
	for (unsigned int i = 0; i < g_ApiModules.size(); i++)
	{
		if (!g_ApiModules[i].Close())
		{
			LOG_ERROR("[CAPI]  %s could not be closed.", g_ApiModules[i].ModuleName());
			getc(stdin);
		}
	}
	p_Active = false;
}

void Server::Update()
{
	timeNow = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> work_time = timeNow - timeLast;

	if (work_time.count() + timeTook.count() >= 1000.0f / g_Config->GetFPS())
	{
		timeLast = std::chrono::system_clock::now();

		// Keep CNetworkManager active
		GetNetworkManager()->Pulse();

		// Pulse all players
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			g_Players[i].PulseMovement();
			g_Players[i].Pulse();
		}

		// Pulse all vehicles
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			//if (g_Vehicles[i].GetOccupant(0) != -1)
			if (g_Vehicles[i].GetAssignee() != RakNet::UNASSIGNED_RAKNET_GUID)
				g_Vehicles[i].PulseMovement();
		}

		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			g_Objects[i].Pulse();
		}

		for (unsigned int i = 0; i < g_ApiModules.size(); i++)
		{
			g_ApiModules[i].OnTick();
		}

		// Show FPS in console window (windows only)
		if (p_ShowFPS)
			ShowFPS();

		// Lag Compensator, Adding this to the work_time will compensate for how long it takes to get threw the itteration
		// and ensures the fps set in the config unless somthing is seriously bottlenecking the server.
		timeTook = timeLast - std::chrono::system_clock::now();
	}
	else
	{
		float timeLeft = (1000.0f / g_Config->GetFPS()) - (work_time.count() + timeTook.count());

#ifdef _WIN32
		//Sleep((1000.0f / g_Config->GetFPS()) / 2);
		Sleep(timeLeft);
#else
		//usleep((1000.0f / g_Config->GetFPS() * 1000)/2);
		usleep(timeLeft * 1000);
#endif
	}
}

void Server::ShowFPS()
{
#ifdef _WIN32
	unsigned long Time = timeGetTime();
	unsigned long TimeElapsed = (Time - p_LastFPSUpdateTime);

	if (TimeElapsed >= 1000)
	{
		p_FPS = p_FrameCount;

		char title[128];
		sprintf_s(title, "" INFO_NAME "(" VERSION ") - " INFO_BUILD " %dfps", p_FPS);
		SetConsoleTitleA(title);

		p_FrameCount = 0;
		p_LastFPSUpdateTime = Time;
	}
	p_FrameCount++;
#endif
}

#ifndef _WIN32
unsigned int timeGetTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_usec / 1000;
}
#endif

void Server::Input(std::atomic<bool>& run)
{
	std::string buffer;

	while (run.load())
	{
		std::getline(std::cin, buffer);

		if (buffer == "quit")
		{
			puts("Quitting. (Temp Disabled)");
			//QUIT
			continue;
		}
		if (buffer == "clear")
		{
			Utility::GetConsole()->Clear();
			continue;
		}
		else if (buffer == "pingmaster")
		{
			Master::UpdateMaster();
			continue;
		}
		else if (buffer == "stats")
		{
			/*RakNetStatistics rssSender;
			RakNetStatistics rssReceiver;
			unsigned short numSystems;
			char *message;

			g_Server->GetNetworkManager()->GetInterface()->GetConnectionList(0, &numSystems);
			if (numSystems > 0) {
				for (int i = 0; i < numSystems; i++)
				{
					g_Server->GetNetworkManager()->GetInterface()->GetStatistics(g_Server->GetNetworkManager()->GetInterface()->GetSystemAddressFromIndex(i), &rssSender);
					StatisticsToString(&rssSender, message, 2);
					printf("==== System %i ====\n", i + 1);
					printf("%s\n\n", message);
				}
			}*/


			PRINT(ConsoleForeground::CYAN, false, "==== Pools ====");
			PRINT(ConsoleForeground::CYAN, false, "%i Players.", g_Players.size());
			PRINT(ConsoleForeground::CYAN, false, "+ %i Vehicles.", g_Vehicles.size());
			PRINT(ConsoleForeground::CYAN, false, "+ %i Objects.", g_Objects.size());
			PRINT(ConsoleForeground::CYAN, false, "+ %i NPCs.", g_Npcs.size());
			PRINT(ConsoleForeground::CYAN, false, "+ %i Checkpoints.", g_Checkpoints.size());
			PRINT(ConsoleForeground::CYAN, false, "+ %i Pickups.", g_Pickups.size());
			PRINT(ConsoleForeground::CYAN, false, "+ %i Blips.", g_Blips.size());
			continue;
		}
		else if (buffer == "playerlist")
		{
			RakNet::SystemAddress systems[10];
			unsigned short numConnections = g_Config->GetMaxPlayers();
			Server::GetServer()->GetNetworkManager()->GetInterface()->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);

			if (numConnections > 0)
			{
				PRINT(ConsoleForeground::CYAN, false, "%i Players Currently Connected\n", numConnections);

				for (unsigned int i = 0; i < numConnections; i++)
				{
					PRINT(ConsoleForeground::CYAN, false, "- %i. %s\n", i + 1, systems[i].ToString(true));
				}
			}
			else
			{
				PRINT(ConsoleForeground::CYAN, false, "No Players Currently Connected.");
			}
			continue;
		}
		else if (buffer.compare(0, 6, "plugin") == 0)
		{

			if (buffer.length() == 11 && buffer.compare(7, 4, "list") == 0)
			{
				PRINT(ConsoleForeground::CYAN, false, "==== Plugins ====");
				if (!g_ApiModules.empty())
				{
					for (int i = 0; i < g_ApiModules.size(); i++)
					{
						if (g_ApiModules[i].IsLoaded())
						{
							PRINT(ConsoleForeground::CYAN, false, "+ %s loaded.", g_ApiModules[i].ModuleName().c_str());
						}
						else
						{
							PRINT(ConsoleForeground::CYAN, false, "+ %s not loaded.", g_ApiModules[i].ModuleName().c_str());
						}
					}
				}
			}
			else if (buffer.length() >= 11 && buffer.compare(7, 4, "load") == 0)
			{
				if (buffer.length() > 12)
				{
					char plugin[128];
					buffer.copy(plugin, buffer.length() - 12, 12);
					bool newPlugin = true;

					for (int i = 0; i < g_ApiModules.size(); i++)
					{
						std::string plug = g_ApiModules[i].ModuleName();
						if (plug.compare(plugin) == 0)
						{
							if (!g_ApiModules[i].IsLoaded())
							{
								g_ApiModules[i].Load();
								g_ApiModules[i].Initialize();
							}
							else
							{
								PRINT(ConsoleForeground::YELLOW, false, "Plugin %s is already loaded.", plugin);
							}

							newPlugin = false;
							break;
						}
					}

					if (newPlugin)
					{
						CAPI NewModule;
						std::string module = plugin;

						struct _stat64 filebuffer;
						if (stat(std::string("./plugin/" + module + LIBRARY_EXTENSION).c_str(), &filebuffer) == 0)
						{
							const int index = g_ApiModules.size();

							NewModule.SetModuleName(module);
							g_ApiModules.push_back(NewModule);

							if (g_ApiModules[index].Load())
							{
								g_ApiModules[index].Initialize();
							}
						}
						else
						{
							PRINT(ConsoleForeground::YELLOW, false, "Could not find a plugin named %s", plugin);
						}
					}
				}
			}
			else if (buffer.length() >= 13 && buffer.compare(7, 6, "unload") == 0)
			{
				if (buffer.length() > 14)
				{
					char plugin[128];
					buffer.copy(plugin, buffer.length() - 14, 14);

					for (int i = 0; i < g_ApiModules.size(); i++)
					{
						std::string plug = g_ApiModules[i].ModuleName();
						if (plug.compare(plugin) == 0)
						{
							if (g_ApiModules[i].IsLoaded())
							{
								g_ApiModules[i].Close();
								g_ApiModules[i].Unload();
							}
							else
							{
								PRINT(ConsoleForeground::YELLOW, false, "Plugin %s is not currently loaded.", plugin);
							}
							break;
						}
					}
				}
			}
			else if (buffer.length() >= 13 && buffer.compare(7, 6, "reload") == 0)
			{
				if (buffer.length() > 14)
				{
					// Unload
					char plugin[128];
					buffer.copy(plugin, buffer.length() - 14, 14);
					bool newPlugin = true;

					for (int i = 0; i < g_ApiModules.size(); i++)
					{
						std::string plug = g_ApiModules[i].ModuleName();
						if (plug.compare(plugin) == 0)
						{
							if (g_ApiModules[i].IsLoaded())
							{
								g_ApiModules[i].Close();
								g_ApiModules[i].Unload();
							}

							g_ApiModules[i].Load();
							g_ApiModules[i].Initialize();

							if (g_ApiModules[i].IsLoaded())
								return PRINT(ConsoleForeground::GREEN, false, "Plugin %s Reloaded.", plugin);

							newPlugin = false;
							break;
						}
					}

					if (newPlugin)
					{
						CAPI NewModule;
						std::string module = plugin;

						struct _stat64 filebuffer;
						if (stat(std::string("./plugin/" + module + LIBRARY_EXTENSION).c_str(), &filebuffer) == 0)
						{
							const int index = g_ApiModules.size();

							NewModule.SetModuleName(module);
							g_ApiModules.push_back(NewModule);

							if (g_ApiModules[index].Load())
							{
								g_ApiModules[index].Initialize();
							}
						}
						else
						{
							PRINT(ConsoleForeground::YELLOW, false, "Could not find a plugin named %s", plugin);
						}
					}
					//LOG_PRINT("Plugin %s failed to reload.", plugin);
				}
			}
			continue;
		}
		else if (buffer[0] == '/')
		{
			/*for (int i = 0; i < g_ApiModules.size(); i++)
			{
				void *Instance = g_ApiModules[i].GetInstance();
				API::Network::OnPlayerCommand(Instance, -1, buffer.c_str());
			}*/
		}
		else if (!buffer.empty())
		{
			RakNet::RakString textstring(buffer.c_str());

			RakNet::BitStream bitstream;
			bitstream.Write(textstring);
			bitstream.Write(RakNet::RakString("CHAR_DEFAULT"));
			bitstream.Write(1);
			bitstream.Write(210);
			bitstream.Write(RakNet::RakString("~r~SERVER"));
			bitstream.Write(RakNet::RakString("Chat"));
			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowMessageAboveMap", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
		}
	}
}