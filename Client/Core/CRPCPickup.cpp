#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "Functions.h"
#include "CVector3.h"
#include "CPickup.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCPickup.h"

void CRPCPickup::Create(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPickup::Create.");

	int entity;
	uint32_t model;
	CVector3 position;

	bitStream->Read(entity);
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);
	bitStream->Read(model);

	if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
	{
		LOG_WARNING("[CRPCPickup::Create] Object entity %i has invalid model %i.", entity, model);
		return;
	}

	CPickup newPickup(entity);
	newPickup.SetPosition(position);
	newPickup.SetModel(model);
	g_Pickups.push_back(newPickup);
}

void CRPCPickup::SetPickupVisible(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPickup::SetPickupVisible.");

	int entity;
	bool state;

	bitStream->Read(entity);
	bitStream->Read(state);

	for (unsigned int i = 0; i < g_Pickups.size(); i++)
	{
		if (g_Pickups[i].GetID() == entity)
		{
			g_Pickups[i].SetVisible(state);
			return;
		}
	}
}

void CRPCPickup::SetPickupModel(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPickup::SetPickupModel.");

	int entity;
	uint32_t model;

	bitStream->Read(entity);
	bitStream->Read(model);

	for (unsigned int i = 0; i < g_Pickups.size(); i++)
	{
		if (g_Pickups[i].GetID() == entity)
		{
			g_Pickups[i].SetModel(model);
			return;
		}
	}
}

void CRPCPickup::SetPickupRespawnDelay(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPickup::SetPickupRespawnDelay.");

	int entity, time;

	bitStream->Read(entity);
	bitStream->Read(time);

	for (unsigned int i = 0; i < g_Pickups.size(); i++)
	{
		if (g_Pickups[i].GetID() == entity)
		{
			g_Pickups[i].SetRespawnDelay(time);
			return;
		}
	}
}