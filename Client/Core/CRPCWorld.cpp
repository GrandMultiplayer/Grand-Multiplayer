// RakNet
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "World.h"
#include "CWorld.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "CRPCWorld.h"

void CRPCWorld::SetTime(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::SetTime.");

	int hour, minute, second;

	bitStream->Read(hour);
	bitStream->Read(minute);
	bitStream->Read(second);

	CWorld::SetTime(hour, minute, second);
}

void CRPCWorld::SetWeather(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::SetWeather.");

	RakNet::RakString weather;

	bitStream->Read(weather);

	//GAMEPLAY::_SET_WEATHER_TYPE_OVER_TIME(weather, time);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
	GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST((char*)weather.C_String());
}

void CRPCWorld::SetGroundSnow(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::SetGroundSnow.");

	bool state;

	bitStream->Read(state);

	GTAV::World::SetGroundSnow(state);
}

void CRPCWorld::LoadIPL(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::LoadIPL.");

	RakNet::RakString ipl;

	bitStream->Read(ipl);

	CWorld::LoadIPL(ipl.C_String());
}

void CRPCWorld::UnloadIPL(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::UnloadIPL.");

	RakNet::RakString ipl;

	bitStream->Read(ipl);

	CWorld::UnloadIPL(ipl.C_String());
}

void CRPCWorld::LoadStringModel(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::LoadStringModel.");

	RakNet::RakString model;

	bitStream->Read(model);

	CWorld::LoadModel(model.C_String());
}

void CRPCWorld::LoadHashModel(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::LoadHashModel.");

	int model;

	bitStream->Read(model);

	CWorld::LoadModel(model);
}

void CRPCWorld::SetBlackout(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCWorld::SetBlackout.");

	bool state;

	bitStream->Read(state);
	
	CWorld::SetBlackout(state);
}