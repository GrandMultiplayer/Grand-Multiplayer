#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "CVector3.h"
#include "APIStructs.h"
#include "GUI.h"
#include "CLocalPlayer.h"
#include "CStreamer.h"
#include "GamePed.h"

// Entities
#include "CEntity.h"
#include "CPlayer.h"
#include "CNPC.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CCheckpoint.h"
#include "CPickup.h"
#include "CBlip.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCEntity.h"

void CRPCEntity::Destroy(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity = -1, type = -1;

	bitStream->Read(entity);
	bitStream->Read(type);

	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				g_Players[i].Destroy();
				g_Players.erase(g_Players.begin() + i);
				g_Players.shrink_to_fit();
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].Destroy();
				g_Vehicles.erase(g_Vehicles.begin() + i);
				g_Vehicles.shrink_to_fit();
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].Destroy();
				g_Objects.erase(g_Objects.begin() + i);
				g_Objects.shrink_to_fit();
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].Destroy();
				g_Npcs.erase(g_Npcs.begin() + i);
				g_Npcs.shrink_to_fit();
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].Destroy();
				g_Checkpoints.erase(g_Checkpoints.begin() + i);
				g_Checkpoints.shrink_to_fit();
				return;
			}
		}
		break;
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				//g_Blips[i].Destroy();
				g_Blips.erase(g_Blips.begin() + i);
				g_Blips.shrink_to_fit();
				return;
			}
		}
		break;
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].Destroy();
				g_Pickups.erase(g_Pickups.begin() + i);
				g_Pickups.shrink_to_fit();
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCEntity::Destroy: Entity %i  is not a valid type.", entity);
	}
}

void CRPCEntity::SetPosition(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	CVector3 position;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);

	LOG_DEBUG("CRPCEntity::SetPosition [%i].", entity);

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			// Force everything to stream out
			CStreamer::ForceStreamOut();
			// Load location
			STREAMING::LOAD_SCENE(position.x, position.y, position.z);
			// Stream in everything at the position

			CVector3 pos;
			if (CLocalPlayer::IsScriptedCameraActive())
			{
				Vector3 camPos = CAM::GET_CAM_COORD(CLocalPlayer::GetScriptedCamera());
				pos = { camPos.x, camPos.y, camPos.z };
			}

			CStreamer::StreamObjectsIn(position, CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
			CStreamer::StreamVehiclesIn(position, CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
			CStreamer::StreamPlayersIn(position, CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
			CStreamer::StreamNpcIn(position, CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
			CStreamer::StreamPickupIn(position, CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
			// Send player to the position
			ENTITY::SET_ENTITY_COORDS(CLocalPlayer::GetPed(), position.x, position.y, position.z, false, false, false, false);
			return;
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetPosition(position);
					return;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetPosition(position);
				return;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetPosition(position);
				return;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetPosition(position);
				return;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetPosition(position);
				return;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetPosition(position);
				return;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetPosition(position);
				return;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetPosition: Entity %i  is not a valid Entity.", entity);
		break;
	}
}

void CRPCEntity::SetRotation(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	CVector3 rotation;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(rotation.x);
	bitStream->Read(rotation.y);
	bitStream->Read(rotation.z);

	LOG_DEBUG("CRPCEntity::SetRotation [%i].", entity);

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			ENTITY::SET_ENTITY_ROTATION(CLocalPlayer::GetPed(), rotation.x, rotation.y, rotation.z, 2, true);
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetRotation(rotation);
					return;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetRotation(rotation);
				return;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetRotation(rotation);
				return;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetRotation(rotation);
				return;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetRotation(rotation);
				return;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetRotation(rotation);
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetRotation(rotation);
				return;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetRotation: Entity %i  is not a valid Entity.", entity);
		break;
	}
}

void CRPCEntity::SetPhysicsAssignment(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity = -1, type = -1;
	RakNet::RakNetGUID guid;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(guid);

	//LOG_DEBUG("CRPCEntity::SetPhysicsAssignment [%i, %i, %s].", entity, type, guid.ToString());

	switch (type)
	{
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetAssignee(guid);
				return;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetAssignee(guid);
				return;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetAssignee(guid);
				return;
			}
		}
	default:
		LOG_WARNING("CRPCEntity::SetPhysicsAssignment: Entity Type %i is not a valid Entity type.", entity);
		break;
	}
}

void CRPCEntity::PedComponent(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, componentid, drawableid, paletteid, textureid;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(componentid);
	bitStream->Read(drawableid);
	bitStream->Read(paletteid);
	bitStream->Read(textureid);

	LOG_DEBUG("CRPCEntity::PedComponent [%i].", entity);

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			GTAV::GamePed::SetPedComponentVariation(CLocalPlayer::GetPed(), componentid, drawableid, textureid, paletteid);
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetModelComponent(componentid, drawableid, textureid, paletteid);
					return;
				}
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetModelComponent(componentid, drawableid, textureid, paletteid);
				return;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::PedComponent: Entity %i is not a valid Entity type.", entity);
		break;
	}
}

void CRPCEntity::PedHeadBlend(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, shapeFirst, shapeSecond, shapeThird, skinFirst, skinSecond, skinThird;
	float shapeMix, skinMix, thirdMix;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(shapeFirst);
	bitStream->Read(shapeMix);
	bitStream->Read(shapeSecond);
	bitStream->Read(shapeThird);
	bitStream->Read(skinFirst);
	bitStream->Read(skinMix);
	bitStream->Read(skinSecond);
	bitStream->Read(skinThird);
	bitStream->Read(thirdMix);

	LOG_DEBUG("CRPCEntity::PedHeadBlend [%i].", entity);

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			GTAV::GamePed::SetPedHeadBlend(CLocalPlayer::GetPed(), shapeFirst, shapeSecond, shapeThird, skinFirst, skinSecond, skinThird, shapeMix, skinMix, thirdMix);
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetModelHeadBlend(shapeFirst, shapeSecond, shapeThird, skinFirst, skinSecond, skinThird, shapeMix, skinMix, thirdMix);
					return;
				}
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetModelHeadBlend(shapeFirst, shapeSecond, shapeThird, skinFirst, skinSecond, skinThird, shapeMix, skinMix, thirdMix);
				return;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::PedHeadBlend: Entity %i is not a valid Entity type.", entity);
		break;
	}
}

void CRPCEntity::PedHeadOverlay(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, overlayid, index, colorType, colorID, secondColorID;
	float opacity;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(overlayid);
	bitStream->Read(index);
	bitStream->Read(colorType);
	bitStream->Read(colorID);
	bitStream->Read(secondColorID);
	bitStream->Read(opacity);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::PedHeadOverlay> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			GTAV::GamePed::SetPedHeadOverlayColor(CLocalPlayer::GetPed(), overlayid, index, colorType, colorID, secondColorID, opacity);
			return;
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetModelHeadOverlay(overlayid, index, colorType, colorID, secondColorID, opacity);
					return;
				}
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetModelHeadOverlay(overlayid, index, colorType, colorID, secondColorID, opacity);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::PedHeadOverlay: Entity %i is not a valid Entity type.", entity);
	}
}

void CRPCEntity::PedProp(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, componentid, drawableid, textureid;
	float opacity;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(componentid);
	bitStream->Read(drawableid);
	bitStream->Read(textureid);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::PedProp> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			GTAV::GamePed::SetPedProp(CLocalPlayer::GetPed(), componentid, drawableid, textureid);
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetModelProp(componentid, drawableid, textureid);
					break;
				}
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetModelProp(componentid, drawableid, textureid);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::PedProp: Entity %i is not a valid Entity type.", entity);
	}
}

void CRPCEntity::PedFaceFeature(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, index;
	float scale;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(index);
	bitStream->Read(scale);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::PedFaceFeature> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			GTAV::GamePed::SetPedFaceFeature(CLocalPlayer::GetPed(), index, scale);
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetModelFaceFeature(index, scale);
					break;
				}
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetModelFaceFeature(index, scale);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::PedFaceFeature: Entity %i is not a valid Entity type.", entity);
	}
}

void CRPCEntity::SetViewDistance(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	float distance;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(distance);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetViewDistance> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetViewDistance(distance);
					break;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetViewDistance(distance);
				break;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetViewDistance(distance);
				break;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetViewDistance(distance);
				break;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetViewDistance(distance);
				break;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetViewDistance(distance);
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetViewDistance(distance);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetViewDistance: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetNametag(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	RakNet::RakString nametag;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(nametag);
	
	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetNametag> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetNametag(nametag.C_String());
					break;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetNametag(nametag.C_String());
				break;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetNametag(nametag.C_String());
				break;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetNametag(nametag.C_String());
				break;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetNametag(nametag.C_String());
				break;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetNametag(nametag.C_String());
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetNametag(nametag.C_String());
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetNametag: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetNametagVisible(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	bool visible;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(visible);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetNametagVisible> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetNametagVisible(visible);
					break;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetNametagVisible(visible);
				break;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetNametagVisible(visible);
				break;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetNametagVisible(visible);
				break;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetNametagVisible(visible);
				break;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetNametagVisible(visible);
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetNametagVisible(visible);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetNametagVisible: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetHealthbarVisible(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	bool visible;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(visible);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetHealthbarVisible> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetHealthbarVisible(visible);
					break;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetHealthbarVisible(visible);
				break;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetHealthbarVisible(visible);
				break;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetHealthbarVisible(visible);
				break;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetHealthbarVisible(visible);
				break;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetHealthbarVisible(visible);
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetHealthbarVisible(visible);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetHealthbarVisible: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetNametagDistance(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	float distance;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(distance);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetNametagDistance> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetNametagDistance(distance);
					break;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetNametagDistance(distance);
				break;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetNametagDistance(distance);
				break;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetNametagDistance(distance);
				break;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetNametagDistance(distance);
				break;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetNametagDistance(distance);
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetNametagDistance(distance);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetNametagDistance: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetNametagColor(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;
	Color color;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(color.Red);
	bitStream->Read(color.Green);
	bitStream->Read(color.Blue);
	bitStream->Read(color.Alpha);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetNametagColor> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetNametagColor(color);
					break;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetNametagColor(color);
				break;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				g_Objects[i].SetNametagColor(color);
				break;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetNametagColor(color);
				break;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				g_Checkpoints[i].SetNametagColor(color);
				break;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetNametagColor(color);
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				g_Pickups[i].SetNametagColor(color);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetNametagColor: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, health;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(health);

	if (entity == -1 || type == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetHealth> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetHealth(health, packet->guid);
					return;
				}
			}
		}
		else
		{
			CLocalPlayer::SetHealth(health, packet->guid);
			ENTITY::SET_ENTITY_HEALTH(CLocalPlayer::GetPed(), health);
			return;
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				g_Vehicles[i].SetHealth(health, packet->guid);
				return;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				//g_Objects[i].SetHealth(health, packet->guid);
				return;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetHealth(health, packet->guid);
				return;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetHealth: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetArmour(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, armour;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(armour);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetArmour> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].SetArmour(armour);
					break;
				}
			}
		}
		else
		{
			//CLocalPlayer::SetArmour(armour);
			PED::SET_PED_ARMOUR(CLocalPlayer::GetPed(), armour);
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].SetArmour(armour);
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetArmour: Entity %i  is not a valid Entity.", entity);
	}
}


void CRPCEntity::Revive(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1;

	bitStream->Read(entity);
	bitStream->Read(type);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::Revive> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() != entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					g_Players[i].Revive();
					break;
				}
			}
		}
		else
		{
			CLocalPlayer::SetHealth(CLocalPlayer::GetMaxHealth(), packet->guid);

			PED::REVIVE_INJURED_PED(CLocalPlayer::GetPed());
			PED::RESURRECT_PED(CLocalPlayer::GetPed());
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();
			PLAYER::RESET_PLAYER_ARREST_STATE(CLocalPlayer::GetPed());
			ENTITY::SET_ENTITY_HEALTH(CLocalPlayer::GetPed(), CLocalPlayer::GetMaxHealth());
			PED::RESET_PED_VISIBLE_DAMAGE(CLocalPlayer::GetPed());
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(CLocalPlayer::GetPed(), CLocalPlayer::GetPosition().x, CLocalPlayer::GetPosition().y, CLocalPlayer::GetPosition().z, false, false, false);
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				g_Npcs[i].Revive();
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::Revive: Entity %i  is not a valid Entity.", entity);
	}
}

void CRPCEntity::SetDimension(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, type = -1, dim;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(dim);

	if (entity == -1)
	{
		LOG_DEBUG("<CRPCEntity::SetDimension> recived an invalid entity -1 of type %i.", type);
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		if (CLocalPlayer::GetID() == entity)
		{
			if (CLocalPlayer::GetDimension() != dim)
			{
				CLocalPlayer::SetDimension(dim);
			}
		}
		else
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entity)
				{
					if (CLocalPlayer::GetDimension() != dim)
					{
						g_Players[i].Destroy();
						g_Players.erase(g_Players.begin() + i);
						g_Players.shrink_to_fit();
					}
					break;
				}
			}
		}
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				if (CLocalPlayer::GetDimension() != dim)
				{
					g_Vehicles[i].Destroy();
					g_Vehicles.erase(g_Vehicles.begin() + i);
					g_Vehicles.shrink_to_fit();
				}
				break;
			}
		}
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				if (CLocalPlayer::GetDimension() != dim)
				{
					g_Objects[i].Destroy();
					g_Objects.erase(g_Objects.begin() + i);
					g_Objects.shrink_to_fit();
				}
				break;
			}
		}
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				if (CLocalPlayer::GetDimension() != dim)
				{
					g_Npcs[i].Destroy();
					g_Npcs.erase(g_Npcs.begin() + i);
					g_Npcs.shrink_to_fit();
				}
				break;
			}
		}
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				if (CLocalPlayer::GetDimension() != dim)
				{
					g_Checkpoints[i].Destroy();
					g_Checkpoints.erase(g_Checkpoints.begin() + i);
					g_Checkpoints.shrink_to_fit();
				}
				break;
			}
		}
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				if (CLocalPlayer::GetDimension() != dim)
				{
					//g_Blips[i].Destroy();
					g_Blips.erase(g_Blips.begin() + i);
					g_Blips.shrink_to_fit();
				}
				break;
			}
		}
	case CEntity::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i].GetID() == entity)
			{
				if (CLocalPlayer::GetDimension() != dim)
				{
					g_Pickups[i].Destroy();
					g_Pickups.erase(g_Pickups.begin() + i);
					g_Pickups.shrink_to_fit();
				}
				break;
			}
		}
	default:
		LOG_DEBUG("CRPCEntity::SetDimension: Entity %i  is not a valid Entity.", entity);
	}
}