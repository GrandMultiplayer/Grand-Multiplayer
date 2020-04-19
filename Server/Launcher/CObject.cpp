// Self
#include "CObject.h"

// System

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "MessageIdentifiers.h"

// GrandM
#include "CVector3.h"

#include "Server.h"

#include "CPlayer.h"
#include "CBlip.h"

CObject::CObject(const uint32_t hash, CVector3 position, CVector3 rotation, bool dynamic)
{
	// Assign current count as entity id
	identity.entityId = CEntity::count;
	// Increment the count for the next entity id
	CEntity::count++;

	identity.type = GrandM::EntityType::Object;

	data.Model.hash = hash;
	data.Model.dynamic = dynamic;

	transform.Position = position;
	transform.Rotation = rotation;
}

CObject::~CObject()
{
	identity.entityId = -1;
	identity.type = GrandM::Nill;
}

CObject::CObject(const CObject & e)
{
	//CEntity
	identity = e.identity;
	world = e.world;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	sync = e.sync;

	//CObject
	data = e.data;
	blip = e.blip;
	network = e.network;
}

CObject & CObject::operator=(const CObject &e)
{
	//CEntity
	identity = e.identity;
	world = e.world;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	sync = e.sync;

	//CObject
	data = e.data;
	blip = e.blip;
	network = e.network;

	return (*this);
}

void CObject::Pulse()
{
	if (data.Model.dynamic)
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(network.lastSync.time_since_epoch()).count() + (1000 / Server::GetServer()->GetSyncRate())
			<= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
		{
			RakNet::BitStream bitstream;
			bitstream.Write((unsigned char)ID_PACKET_OBJECT);

			bitstream.Write(identity.entityId);

			bitstream.Write(transform.Position.x);
			bitstream.Write(transform.Position.y);
			bitstream.Write(transform.Position.z);

			bitstream.Write(transform.Velocity.x);
			bitstream.Write(transform.Velocity.y);
			bitstream.Write(transform.Velocity.z);

			bitstream.Write(transform.Rotation.x);
			bitstream.Write(transform.Rotation.y);
			bitstream.Write(transform.Rotation.z);

			if (!g_Players.empty())
			{
				for (int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetDimension() == world.dim)
					{
						if ((g_Players[i].IsScriptedCameraActive() && CVector3::Distance2D(g_Players[i].GetScriptCameraPosition(), transform.Position) <= 1000.0f)
							|| (CVector3::Distance2D(g_Players[i].GetPosition(), transform.Position) <= 1000.0f))
						{
							Server::GetServer()->GetNetworkManager()->GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, g_Players[i].GetGUID(), false);
						}
					}
				}
			}
			
			network.lastSync = std::chrono::system_clock::now();
		}
	}
}

void CObject::Update(RakNet::Packet *packet)
{
	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);

	bitstream.Read(identity.entityId);

	bitstream.Read(transform.Position.x);
	bitstream.Read(transform.Position.y);
	bitstream.Read(transform.Position.z);

	bitstream.Read(transform.Velocity.x);
	bitstream.Read(transform.Velocity.y);
	bitstream.Read(transform.Velocity.z);

	bitstream.Read(transform.Rotation.x);
	bitstream.Read(transform.Rotation.y);
	bitstream.Read(transform.Rotation.z);
}

void CObject::SyncData(RakNet::RakNetGUID requester)
{
	RakNet::BitStream bitstream;

#pragma region Blip
	if (blip.HasBlip())
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

		blip.SyncData(requester);
	}
#pragma endregion

	// Assignment Data
	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(network.physicsAssignee);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(data.Model.texture);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetTextureVariation", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(health);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetHealth", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
}

void CObject::SetAssignee(RakNet::RakNetGUID assignee)
{
	network.physicsAssignee = assignee;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(assignee);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Server::GetServer()->GetNetworkManager()->GetInterface()->GetSystemAddressFromIndex(0), true, false);
}