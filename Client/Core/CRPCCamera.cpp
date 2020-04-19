#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "CVector3.h"
#include "APIStructs.h"
#include "GUI.h"

#include "CLocalPlayer.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CPickup.h"
#include "CNPC.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCCamera.h"


void CRPCCamera::SetCameraPosition(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	CVector3 position;

	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);

	LOG_DEBUG("CRPCCamera::SetCameraPosition");

	STREAMING::LOAD_SCENE(position.x, position.y, position.z);
	CLocalPlayer::SetScriptedCameraActive(true);
	CAM::SET_CAM_COORD(CLocalPlayer::GetScriptedCamera(), position.x, position.y, position.z);
}

void CRPCCamera::SetCameraRotation(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	CVector3 rotation;

	bitStream->Read(rotation.x);
	bitStream->Read(rotation.y);
	bitStream->Read(rotation.z);

	LOG_DEBUG("CRPCCamera::SetCameraRotation");

	CLocalPlayer::SetScriptedCameraActive(true);
	CAM::SET_CAM_ROT(CLocalPlayer::GetScriptedCamera(), rotation.x, rotation.y, rotation.z, 2);
}

void CRPCCamera::ResetCamera(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCCamera::ResetCamera");

	CLocalPlayer::SetScriptedCameraActive(false);
}

void CRPCCamera::FirstPersonOnly(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCCamera::FirstPersonOnly");

	CLocalPlayer::SetFirstPersonOnly();
}

void CRPCCamera::ThirdPersonOnly(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCCamera::ThirdPersonOnly");

	CLocalPlayer::SetThirdPersonOnly();
}

void CRPCCamera::AttachCamera(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	int entity, type = -1;
	CVector3 offset;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(offset.x);
	bitStream->Read(offset.y);
	bitStream->Read(offset.z);

	if (entity == -1)
	{
		LOG_DEBUG("[CRPCCamera::AttachCamera] Invalid entity type.");
		return;
	}

	CLocalPlayer::SetScriptedCameraActive(true);

	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				if (g_Players[i].IsCreated())
					CAM::ATTACH_CAM_TO_ENTITY(CLocalPlayer::GetScriptedCamera(), g_Players[i].GetPed(), offset.x, offset.y, offset.z, true);
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				if (g_Npcs[i].IsCreated())
					CAM::ATTACH_CAM_TO_ENTITY(CLocalPlayer::GetScriptedCamera(), g_Npcs[i].GetEntity(), offset.x, offset.y, offset.z, true);
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				if (g_Vehicles[i].IsCreated())
					CAM::ATTACH_CAM_TO_ENTITY(CLocalPlayer::GetScriptedCamera(), g_Vehicles[i].GetEntity(), offset.x, offset.y, offset.z, true);
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				if (g_Objects[i].IsCreated())
					CAM::ATTACH_CAM_TO_ENTITY(CLocalPlayer::GetScriptedCamera(), g_Objects[i].GetEntity(), offset.x, offset.y, offset.z, true);
				return;
			}
		}
		break;
	default:
		CLocalPlayer::SetScriptedCameraActive(false);
		LOG_DEBUG("[CRPCCamera::AttachCamera] Invalid entity type.");
		return;
	}
}

void CRPCCamera::DetachCamera(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCCamera::DetachCamera");

	CLocalPlayer::SetScriptedCameraActive(true);
	CAM::DETACH_CAM(CLocalPlayer::GetScriptedCamera());
}