#include <winsock2.h>
#include <Windows.h>
#include <vector>

// RakNet
#include <RakNetTypes.h>

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_sandbox_win.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "Functions.h"
#include "CPlayer.h"
#include "CNPC.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CPickup.h"
#include "CCheckpoint.h"
#include "CStreamer.h"
#include "World.h"
#include "CefMainContext.h"
#include "CGui.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CWorld.h"

CWorld::wTime CWorld::Time;

std::vector<CWorld::IPL> CWorld::g_IPLs;

void CWorld::Initialize()
{

}

void CWorld::Destroy()
{
	LOG_DEBUG("[CWorld] Destroying World");

	// Reset IPLs to their default states
	for (int i = (g_IPLs.size() - 1); i > -1; i--)
	{
		if (g_IPLs[i].defaultState)
		{
			STREAMING::REQUEST_IPL((char*)g_IPLs[i].ipl.c_str());
		}
		else
		{
			STREAMING::REMOVE_IPL((char*)g_IPLs[i].ipl.c_str());
		}

		g_IPLs.erase(g_IPLs.begin() + i);
	}
	// Shrink vector so size is correct.
	g_IPLs.shrink_to_fit();

	// Remove all existing vehicles
	if (!g_Vehicles.empty())
	{
		for (int i = (g_Vehicles.size() - 1); i > -1; i--)
		{
			if (g_Vehicles[i].IsCreated())
				g_Vehicles[i].Destroy();
			g_Vehicles.erase(g_Vehicles.begin() + i);
		}
		// Shrink vector so size is correct.
		g_Vehicles.shrink_to_fit();
	}

	// Remove all existing players
	if (!g_Players.empty())
	{
		for (int i = (g_Players.size() - 1); i > -1; i--)
		{
			if (g_Players[i].IsCreated())
				g_Players[i].Destroy();
			g_Players.erase(g_Players.begin() + i);
		}
		// Shrink vector so size is correct.
		g_Players.shrink_to_fit();
	}

	// Remove all existing objects
	if (!g_Objects.empty())
	{
		for (int i = (g_Objects.size() - 1); i > -1; i--)
		{
			if (g_Objects[i].IsCreated())
				g_Objects[i].Destroy();
			g_Objects.erase(g_Objects.begin() + i);
		}
		// Shrink vector so size is correct.
		g_Objects.shrink_to_fit();
	}

	if (!g_Npcs.empty())
	{
		// Remove all existing npcs
		for (int i = (g_Npcs.size() - 1); i > -1; i--)
		{
			if (g_Npcs[i].IsCreated())
				g_Npcs[i].Destroy();
			g_Npcs.erase(g_Npcs.begin() + i);
		}
		// Shrink vector so size is correct.
		g_Npcs.shrink_to_fit();
	}

	// Remove all existing checkpoints
	if (!g_Checkpoints.empty())
	{
		for (int i = (g_Checkpoints.size() - 1); i > -1; i--)
		{
			g_Checkpoints[i].Destroy();
			g_Checkpoints.erase(g_Checkpoints.begin() + i);
		}
		// Shrink vector so size is correct.
		g_Checkpoints.shrink_to_fit();
	}

	// Remove all existing pickups
	if (!g_Pickups.empty())
	{
		for (int i = (g_Pickups.size() - 1); i > -1; i--)
		{
			g_Pickups[i].Destroy();
			g_Pickups.erase(g_Pickups.begin() + i);
		}
		// Shrink vector so size is correct.
		g_Pickups.shrink_to_fit();
	}

	// Remove all existing blips
	if (!g_Blips.empty())
	{
		for (int i = (g_Blips.size() - 1); i > -1; i--)
		{
			g_Blips[i].Remove();
			g_Blips.erase(g_Blips.begin() + i);
		}
		// Shrink vector so size is correct.
		g_Blips.shrink_to_fit();
	}

	CStreamer::Reset();
	
	GTAV::World::SetGroundSnow(false);
}

void CWorld::LoadIPL(std::string ipl)
{
	bool exists;
	for (unsigned int i = 0; i < g_IPLs.size(); i++) 
	{
		if (ipl.compare(g_IPLs[i].ipl) == 0)
		{
			g_IPLs[i].enabled = true;
			exists = true;
		}
	}

	if (!exists)
	{
		IPL newIPL;
		newIPL.ipl = ipl;
		newIPL.enabled = true;
		newIPL.defaultState = STREAMING::IS_IPL_ACTIVE((char*)ipl.c_str());

		g_IPLs.push_back(newIPL);
	}

	std::string loadmessage = "Syncing with Server - Loading IPL ";
	loadmessage += ipl;
	
	CGui::SetLoadingText(loadmessage);

	STREAMING::REQUEST_IPL((char*)ipl.c_str());
}

void CWorld::UnloadIPL(std::string ipl)
{
	bool exists;
	for (unsigned int i = 0; i < g_IPLs.size(); i++)
	{
		if (ipl.compare(g_IPLs[i].ipl) == 0)
		{
			g_IPLs[i].enabled = true;
			exists = true;
		}
	}

	if (!exists)
	{
		IPL newIPL;
		newIPL.ipl = ipl;
		newIPL.enabled = false;
		newIPL.defaultState = STREAMING::IS_IPL_ACTIVE((char*)ipl.c_str());

		g_IPLs.push_back(newIPL);
	}


	std::string loadmessage = "Syncing with Server - Unloading IPL ";
	loadmessage += ipl;

	CGui::SetLoadingText(loadmessage);


	STREAMING::REMOVE_IPL((char*)ipl.c_str());
}

void CWorld::LoadModel(const std::string model)
{
	int hash = $(model);
	if (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_VALID(hash)) 
	{
		std::string loadmessage = "Syncing with Server - Loading model ";
		loadmessage += model;

		CGui::SetLoadingText(loadmessage);

		LOG_DEBUG("[CWorld::LoadModel] Loading model, %s", model);

		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash))
			WAIT(10);

		LOG_DEBUG("[CWorld::LoadModel] Loaded model, %s", model);
	}
	else
	{
		LOG_WARNING("[CWorld::LoadModel] Attempted to load invalid model, %s", model);
	}
}

void CWorld::LoadModel(const int model)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		std::string loadmessage = "Syncing with Server - Loading model ";
		loadmessage += std::to_string(model);

		CGui::SetLoadingText(loadmessage);

		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
			WAIT(10);
	}
	else
	{
		LOG_WARNING("[CWorld::LoadModel] Attempted to load invalid model, %i", model);
	}
}

void CWorld::SetBlackout(const bool state)
{
	GRAPHICS::_SET_BLACKOUT(state);
}

void CWorld::SetTime(const int hour, const int minute, const int second)
{
	Time.hour = hour;
	Time.minute = minute;
	Time.second = second;

	TIME::SET_CLOCK_TIME(hour, minute, second);
}