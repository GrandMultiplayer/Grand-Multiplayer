#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "Functions.h"
#include "CVector3.h"
#include "CNPC.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCNpc.h"

void CRPCNpc::Create(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCNpc::Create.");

	int entity;
	uint32_t model;
	CVector3 position;
	CVector3 rotation;

	bitStream->Read(entity);
	bitStream->Read(model);
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);
	bitStream->Read(rotation.x);
	bitStream->Read(rotation.y);
	bitStream->Read(rotation.z);
	
	if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
	{
		LOG_WARNING("[CRPCNpc::Create] Object entity %i has invalid model %i.", entity, model);
		return;
	}

	CNPC newNpc(entity, model, position, rotation);
	g_Npcs.push_back(newNpc);
}