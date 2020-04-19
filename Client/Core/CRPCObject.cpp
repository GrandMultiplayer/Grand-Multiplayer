#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "Functions.h"
#include "CVector3.h"
#include "CObject.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCObject.h"

void CRPCObject::Create(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCObject::Create.");

	int entity;
	uint32_t model;
	CVector3 position;
	CVector3 rotation;
	bool dynamic;

	bitStream->Read(entity);
	bitStream->Read(model);
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);
	bitStream->Read(rotation.x);
	bitStream->Read(rotation.y);
	bitStream->Read(rotation.z);
	bitStream->Read(dynamic);

	if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
	{
		LOG_WARNING("[CRPCObject::Create] Object entity %i has invalid model %i.", entity, model);
		return;
	}

	CObject newObject(entity, model, position, rotation, dynamic);
	g_Objects.push_back(newObject);
}

void CRPCObject::SetTextureVariation(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, textureindex;

	bitStream->Read(entity);
	bitStream->Read(textureindex);

	LOG_DEBUG("CRPCObject::SetTextureVariation [%i, %i].", entity, textureindex);

	for (int i = 0; i < g_Objects.size(); i++)
	{
		if (g_Objects[i].GetID() == entity)
		{
			g_Objects[i].SetTextureVariation(textureindex);
			return;
		}
	}
}