#ifdef _WIN32
#include <SharedUtility.h>
#else
#include <sys/time.h>
#endif

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

// GrandM
#include "Server.h"
#include "NetworkSync.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"
#include "CCheckpoint.h"
#include "CPickup.h"
#include "CBlip.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "CEntity.h"

int CEntity::count = 0;

bool CEntity::IsNull()
{
	return (identity.entityId == -1);
}

void CEntity::SetPosition(const CVector3 position)
{
	transform.Position = position;
	RakNet::BitStream bitstream;

	switch (identity.type)
	{
	case GrandM::EntityType::Nill:
	{
		LOG_ERROR("[Entity] Attempted to set the position of an unknown entity type, entity %i", identity.entityId);
		//case GrandM::EntityType::Player:
		//	bitstream.Write(-1);
		//	bitstream.Write(position.x);
		//	bitstream.Write(position.y);
		//	bitstream.Write(position.z);
		//	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPosition", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Network.guid, false , false);
		break;
	}
	default:
	{
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(position.x);
		bitstream.Write(position.y);
		bitstream.Write(position.z);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPosition", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
		break;
	}
	}
}

void CEntity::SetRotation(const CVector3 rotation)
{
	transform.Rotation = rotation;
	RakNet::BitStream bitstream;

	switch (identity.type)
	{
	case GrandM::EntityType::Nill:
	{
		LOG_ERROR("[Entity] Attempted to set the rotation of an unknown entity type, entity %i", identity.entityId);
		/*case GrandM::EntityType::Player:
			bitstream.Write(Identity.entityId);
			bitstream.Write(rotation.x);
			bitstream.Write(rotation.y);
			bitstream.Write(rotation.z);
			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPosition", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Network.guid, false, false);*/
		break;
	}
	default:
	{
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(rotation.x);
		bitstream.Write(rotation.y);
		bitstream.Write(rotation.z);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPosition", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
		break;
	}
	}
}

void CEntity::SetDimension(const int dim)
{
	world.dim = dim;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(dim);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetDimension", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

	//NetworkSync::SyncEntities(Network.guid, world.dim);
}

CEntity CEntity::GetLastAttacker()
{
	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetID() == lastAttacker)
		{
			return g_Players[i];
		}
	}

	return CEntity();
}
// this  could take a type now to make it faster
CEntity* CEntity::GetEntityFromID(const int id)
{
	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == id)
		{
			return &g_Vehicles[i];
		}
	}

	for (unsigned int i = 0; i < g_Npcs.size(); i++)
	{
		if (g_Npcs[i].GetID() == id)
		{
			return &g_Npcs[i];
		}
	}

	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetID() == id)
		{
			return &g_Players[i];
		}
	}

	for (unsigned int i = 0; i < g_Objects.size(); i++)
	{
		if (g_Objects[i].GetID() == id)
		{
			return &g_Objects[i];
		}
	}

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
	{
		if (g_Checkpoints[i].GetID() == id)
		{
			return &g_Checkpoints[i];
		}
	}

	for (unsigned int i = 0; i < g_Pickups.size(); i++)
	{
		if (g_Pickups[i].GetID() == id)
		{
			return &g_Pickups[i];
		}
	}

	for (unsigned int i = 0; i < g_Blips.size(); i++)
	{
		if (g_Blips[i].GetID() == id)
		{
			return &g_Blips[i];
		}
	}

	return new CEntity();
}

GrandM::EntityType CEntity::GetEntityTypeFromID(const int id)
{
	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == id)
		{
			return GrandM::EntityType::Vehicle;
		}
	}

	for (unsigned int i = 0; i < g_Npcs.size(); i++)
	{
		if (g_Npcs[i].GetID() == id)
		{
			return GrandM::EntityType::NPC;
		}
	}

	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetID() == id)
		{
			return GrandM::EntityType::Player;
		}
	}

	for (unsigned int i = 0; i < g_Objects.size(); i++)
	{
		if (g_Objects[i].GetID() == id)
		{
			return GrandM::EntityType::Object;
		}
	}

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
	{
		if (g_Checkpoints[i].GetID() == id)
		{
			return GrandM::EntityType::Checkpoint;
		}
	}

	for (unsigned int i = 0; i < g_Pickups.size(); i++)
	{
		if (g_Pickups[i].GetID() == id)
		{
			return GrandM::EntityType::Pickup;
		}
	}

	for (unsigned int i = 0; i < g_Blips.size(); i++)
	{
		if (g_Blips[i].GetID() == id)
		{
			return GrandM::EntityType::Blip;
		}
	}

	return GrandM::EntityType::Nill;
}

void CEntity::SetNametag(const std::string nametag)
{ 
	tag.text = nametag;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(RakNet::RakString(nametag.c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametag", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CEntity::SetNametagVisible(const bool visible)
{ 
	tag.textVisible = visible;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(visible);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CEntity::SetNametagDistance(const float distance)
{ 
	tag.distance = distance;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(distance);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagDistance", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CEntity::SetHealthbarVisible(const bool visible)
{ 
	tag.healthVisible = visible;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(visible);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetHealthbarVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CEntity::SetNametagColor(const Color color)
{ 
	tag.textColor = color;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(color.Red);
	bitstream.Write(color.Green);
	bitstream.Write(color.Blue);
	bitstream.Write(color.Alpha);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CEntity::SetViewDistance(const float distance) 
{
	world.streamDistance = distance; 

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(distance);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetViewDistance", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
}

void CEntity::UpdateMovement(RakNet::Packet *packet)
{
	int id, type;

	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);
	bitstream.Read(type);
	bitstream.Read(id);

	bitstream.Read(transform.Position.x);
	bitstream.Read(transform.Position.y);
	bitstream.Read(transform.Position.z);

	bitstream.Read(transform.Rotation.x);
	bitstream.Read(transform.Rotation.y);
	bitstream.Read(transform.Rotation.z);

	bitstream.Read(transform.Velocity.x);
	bitstream.Read(transform.Velocity.y);
	bitstream.Read(transform.Velocity.z);

	bitstream.Read(forwardSpeed);
	bitstream.Reset();

	packet->deleteData;
}

void CEntity::PulseMovement()
{
	uint64_t now = getticktime();

	if(sync.lastMovementSync + (1000 / Server::GetServer()->GetSyncRate()) <= now && identity.entityId != -1)
	{
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)ID_PACKET_MOVEMENT);

		bitstream.Write(identity.type);
		bitstream.Write(identity.entityId);

		bitstream.Write(transform.Position.x);
		bitstream.Write(transform.Position.y);
		bitstream.Write(transform.Position.z);

		bitstream.Write(transform.Rotation.x);
		bitstream.Write(transform.Rotation.y);
		bitstream.Write(transform.Rotation.z);

		bitstream.Write(transform.Velocity.x);
		bitstream.Write(transform.Velocity.y);
		bitstream.Write(transform.Velocity.z);

		bitstream.Write(forwardSpeed);

		if (!g_Players.empty())
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetDimension() == world.dim && identity.entityId != g_Players[i].GetID())
				{
					if ((g_Players[i].IsScriptedCameraActive() && CVector3::Distance2D(g_Players[i].GetScriptCameraPosition(), transform.Position) <= 1000.0f)
						|| (CVector3::Distance2D(g_Players[i].GetPosition(), transform.Position) <= 1000.0f))
					{
						// If you want phys assignment checking on a type then you need to override this function
						// As we don't want to store that in the centity object as it causes API issues.
						Server::GetServer()->GetNetworkManager()->GetInterface()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, g_Players[i].GetGUID(), false);
					}
				}
			}
		}

		sync.lastMovementSync = now;
	}
}