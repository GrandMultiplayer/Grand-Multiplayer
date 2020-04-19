#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <iomanip>
#pragma comment(lib, "IPHLPAPI.lib")

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>

#include "Pattern.h"
#include "hook/enums.h"
#include "hook/types.h"
#include "hook/main.h"

#include "APIStructs.h"
#include "GUI.h"
#include "Vehicle.h"
#include "Scripts.h"
#include "CDiscord.h"
#include "Common.h"
#include "DirectXHook.h"
#include "CefMainContext.h"
#include "CConfig.h"
#include "CCore.h"
#include "CLocalPlayer.h"
#include "CGui.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CMain.h"


static eGameState *	gameState;

//static eGameState* m_gameState;
static uint64_t m_worldPtr;
static uint64_t m_replayPtr;
static BlipList* m_blipList;
Vehicles vehExt;

eGameState CMain::GetGameState()
{
	return *gameState;
}

BlipList* CMain::GetBlipList()
{
	return m_blipList;
}

uint64_t CMain::GetWorldPtr()
{
	return m_worldPtr;
}

uint64_t CMain::GetReplayPtr()
{
	return m_replayPtr;
}

void CMain::Start()
{
	// Get Vehicle Offsets
	vehExt.Init();

	LOG_DEBUG("Initializing Grand Multiplayer Client Version %s", VERSION);
	GetMACaddress();
	LOG_DEBUG("Your Key is %s", CLocalPlayer::GetSecretKey().c_str());

	CGui::SetVersion(VERSION);

	if (getGameVersion() < eGameVersion::VER_1_0_1493_1_STEAM)
	{
		LOG_ERROR("Invalid game version please update your game.");
		MessageBoxA(NULL, "Invalid game version please update your game./", INFO_NAME, MB_ICONEXCLAMATION);
	}

//	CConfig::Read();
	
	CCore::Initialize();
	
	CLocalPlayer::MainMenu();
	
	while (true)
	{
		CCore::OnGameTick();
	}
}

void CMain::Cleaner()
{
	static unsigned long LastCleanUp = timeGetTime();

	while (true)
	{
		if (timeGetTime() >= LastCleanUp + 1000)
		{
			CCore::CleanUp();			// World Clean Up
			Scripts::Pulse();			// Script Clean Up
			LastCleanUp = timeGetTime();
		}

		CCore::CleanUpTick();
	}
}

/* Pattern Scanning */
void CMain::FailPatterns(const char* name, Memory::pattern ptn)
{
	LOG_DEBUG("[CMain] finding %s", name);
	//Cleanup();
}

void CMain::InitializeGame(HMODULE hModule)
{
	// Get game state
	if (auto gameStatePattern = "83 3D ? ? ? ? ? 8A D9 74 0A"_Scan)
	{
		gameState = gameStatePattern.add(2).rip(5).as<decltype(gameState)>();
		LOG_ADDRESS("[CMain] gameState", gameState);
	}
	else
		LOG_ERROR("[CMain] Unable to find gameState");

	// Skip game logos
	LOG_DEBUG("[CMain] Skipping game logos.");
	auto p_gameLogos = Memory::pattern("70 6C 61 74 66 6F 72 6D 3A");
	Memory::putVP<uint8_t>(p_gameLogos.count(1).get(0).get<void>(0), 0xC3);
	//auto p_gameLogos = Memory::pattern("70 6C 61 74 66 6F 72 6D 3A 2F 6D 6F 76 69 65 73 2F 72 6F 63 6B 73 74 61 72 5F 6C 6F 67 6F 73 00");
	//Memory::putVP<uint8_t>(p_gameLogos.count(1).get(0).get<void>(0), 0xC3);

	CConfig::Read();

	// Get Game resolution
	static uint32_t* g_resolution;
	char * location = Memory::pattern("C7 05 ? ? ? ? 00 05 00 00").count(1).get(0).get<char>(2);

	g_resolution = (uint32_t*)(*(int32_t*)location + location + 8);

	CGui::SetResolution(Vector2(g_resolution[0], g_resolution[1]));
	LOG_DEBUG("[CMain] Game Resolution %i x %i", g_resolution[0], g_resolution[1]);
	
	// init Direct3d hook
	if (!g_D3DHook.InitializeHooks())
		LOG_ERROR("Failed to Initialize Direct3d Hooks");

	LOG_DEBUG("[CMain] Init Discord");
	CDiscord::discordInit();

	LOG_DEBUG("[CMain] Setting SP pattern.");
	Memory::put<uint8_t>(Memory::get_pattern("48 83 3D ? ? ? ? 00 88 05 ? ? ? ? 75 0B", 8), 0x90);
	
	// Watchdog
	LOG_DEBUG("[CMain] Setting Watchdog pattern");
	Memory::return_function(Memory::pattern("48 83 64 24 30 00 83 4C 24 28 FF 33 D2 48").count(1).get(0).get<void>(-4));

	// parallelize streaming (force 'disable parallel streaming' flag off)
	LOG_DEBUG("[CMain] Disabling parallelize streaming");
	Memory::put<uint8_t>(Memory::get_pattern("C0 C6 05 ? ? ? ? 01 44 88 35", 7), 0);

	// Mute loading screen audio.
	LOG_DEBUG("[CMain] Mutting Loadng screen Audio pattern");
	Memory::return_function(Memory::get_pattern("41 B8 97 96 11 96", -0x9A));
	
	//auto p_gameState = Memory::pattern("83 3D ? ? ? ? ? 8A D9 74 0A");
	auto p_worldPtr = Memory::pattern("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07");
	auto p_replayPtr = Memory::pattern("48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D ? ? ? ?");
	auto p_blipList = Memory::pattern("4C 8D 05 ? ? ? ? 0F B7 C1");
	
	//Wait for Menu or Game
	while (true) 
	{
		if (GetGameState() == GameStateLoadingSP_MP || GetGameState() == GameStatePlaying)
			break;

		Sleep(1000);
	}
	
	// Get world pointer
	location = p_worldPtr.count(1).get(0).get<char>(0);
	location == nullptr ? FailPatterns("world Pointer", p_worldPtr) : m_worldPtr = reinterpret_cast<uint64_t>(location) + *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(location) + 3) + 7;

	// Get replay interface pointer
	location = p_replayPtr.count(1).get(0).get<char>(0);
	location == nullptr ? FailPatterns("replayinterface Pointer", p_replayPtr) : m_replayPtr = reinterpret_cast<uint64_t>(location) + *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(location) + 3) + 7;

	// Get blip list
	location = p_blipList.count(1).get(0).get<char>(0);
	location == nullptr ? FailPatterns("blip List", p_blipList) : m_blipList = (BlipList*)(location + *reinterpret_cast<int*>(location + 3) + 7);

	// replace default loading of SP Content with loading of MP DLC.
	location = Memory::pattern("75 0D BA E2 99 8F 57").count(1).get(0).get<char>(0);
	Memory::nop(location, 2);
	Memory::put(location + 3, 0xBCC89179); // GROUP_MAP
	
	//Wait for Game
	while (true)
	{
		if (GetGameState() != GameStateLoadingSP_MP && GetGameState() == GameStatePlaying)
			break;

		Sleep(1000);
	}

	scriptRegister(hModule, CMain::Start);
	scriptRegisterAdditionalThread(hModule, CMain::Addit);

	LOG_DEBUG("[CMain] Initialized Game.");
}

void CMain::GetMACaddress()
{
	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);                  // [in] size of receive data buffer
	if (dwStatus != ERROR_SUCCESS)
	{
		LOG_ERROR("GetAdaptersInfo failed. err=%d\n", GetLastError());
		return;
	}

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to  current adapter info


	std::string key;

	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::setw(2);
	for (int i = 0; i < pAdapterInfo->AddressLength; i++)
	{
		ss << (int)pAdapterInfo->Address[i];
	}

	pAdapterInfo = pAdapterInfo->Next;

	if (pAdapterInfo)
	{
		PIP_ADAPTER_INFO p = pAdapterInfo->Next;

		if (p)
			pAdapterInfo = p;

		ss << std::hex << std::setfill('0') << std::setw(2);
		for (int i = 0; i < pAdapterInfo->AddressLength; i++)
		{
			ss << (int)pAdapterInfo->Address[i];
		}
		key += ss.str();
	}

	CLocalPlayer::SetSecretKey(key);
}

void CMain::Addit()
{
	while (true)
	{
		WAIT(1000);
	}
}