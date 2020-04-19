// System
#include <vector>

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

// GrandM
#include "APIStructs.h"
#include "CVector3.h"

#include "Server.h"

#include "CBlip.h"

// Self
#include "CCheckpoint.h"

CCheckpoint::CCheckpoint(const CVector3 position, const float radius, const Color color, const int reserved)
{
	// Assign current count as entity id
	identity.entityId = CEntity::count;
	// Increment the count for the next entity id
	CEntity::count++;

	identity.type = GrandM::EntityType::Checkpoint;

	transform.Position = position;
	data.radius = radius;
	data.color = color;
	data.reserved = reserved;
}

CCheckpoint::~CCheckpoint()
{
	identity.entityId = -1;
	identity.type = GrandM::Nill;
}

CCheckpoint::CCheckpoint(const CCheckpoint & e)
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

	//CCheckpoint
	data = e.data;
	blip = e.blip;
}

CCheckpoint & CCheckpoint::operator=(const CCheckpoint &e)
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

	//CCheckpoint
	data = e.data;
	blip = e.blip;

	return (*this);
}

void CCheckpoint::SyncData(RakNet::RakNetGUID requester)
{
	RakNet::BitStream bitstream;

	if (blip.HasBlip())
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

		blip.SyncData(requester);
	}
	
	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(data.nearHeight);
	bitstream.Write(data.farHeight);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCheckpointHeight", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(data.pointTo.x);
	bitstream.Write(data.pointTo.y);
	bitstream.Write(data.pointTo.z);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CheckpointPointTo", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(data.type);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCheckpointType", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
}
