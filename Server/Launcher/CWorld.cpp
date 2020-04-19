// System

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"

// GrandM
#include "Server.h"

//Self
#include "CWorld.h"

struct CWorld::sTime CWorld::Time;
struct CWorld::sWeather CWorld::Weather;

bool CWorld::blackout = false;

std::vector<CWorld::IPL> CWorld::g_IPLs;

void CWorld::Init()
{
	Time.hour = 12;
	Time.minute = 00;
	Time.second = 00;
	Weather.Weather = "CLEAR";
	Weather.snow = false;
}

void CWorld::SetTime(int hour, int minute, int second)
{
	Time.hour = hour;
	Time.minute = minute;
	Time.second = second;

	RakNet::BitStream bitstream;
	bitstream.Write(Time.hour);
	bitstream.Write(Time.minute);
	bitstream.Write(Time.second);

	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetTime", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CWorld::SetWeather(std::string weather)
{
	Weather.Weather = weather;

	RakNet::BitStream bitstream;
	bitstream.Write(RakNet::RakString(weather.c_str()));

	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetWeather", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CWorld::SetGroundSnow(const bool state)
{
	Weather.snow = state;

	RakNet::BitStream bitstream;
	bitstream.Write(state);

	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetGroundSnow", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CWorld::LoadIPL(std::string ipl)
{
	bool exists = false;

	for (unsigned int i = 0; i < g_IPLs.size(); i++)
	{
		if (ipl.compare(g_IPLs[i].ipl) == 0)
		{
			if (!g_IPLs[i].enabled)
				g_IPLs[i].enabled = true;

			exists = true;
			break;
		}
	}

	if (!exists)
	{
		IPL newIPL;
		newIPL.ipl = ipl;
		newIPL.enabled = true;
		g_IPLs.push_back(newIPL);
	}

	RakNet::BitStream bitstream;
	bitstream.Write(RakNet::RakString(ipl.c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("LoadIPL", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CWorld::UnloadIPL(std::string ipl)
{
	bool exists = false;

	for (unsigned int i = 0; i < g_IPLs.size(); i++)
	{
		if (ipl.compare(g_IPLs[i].ipl) == 0)
		{
			if (g_IPLs[i].enabled)
				g_IPLs[i].enabled = false;

			exists = true;
		}
	}

	if (!exists)
	{
		IPL newIPL;
		newIPL.ipl = ipl;
		newIPL.enabled = false;
		g_IPLs.push_back(newIPL);
	}

	RakNet::BitStream bitstream;
	bitstream.Write(RakNet::RakString(ipl.c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("UnloadIPL", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CWorld::SetBlackout(const bool state)
{
	blackout = state;

	RakNet::BitStream bitstream;
	bitstream.Write(state);

	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlackout", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}
