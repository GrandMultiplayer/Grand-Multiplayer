// System
#include <string>
#include <vector>

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"

// GrandM
#include "CVector3.h"

#include "Server.h"
#include "CBlip.h"

// Self
#include "CPickup.h"

CPickup::CPickup(const CVector3 position)
{
	// Assign current count as entity id
	identity.entityId = CEntity::count;
	// Increment the count for the next entity id
	CEntity::count++;

	identity.type = GrandM::EntityType::Pickup;

	transform.Position = position;
}

CPickup::~CPickup()
{
	identity.entityId = -1;
	identity.type = GrandM::Nill;
}

CPickup::CPickup(const CPickup & e)
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

	//CPickup
	data = e.data;
	blip = e.blip;
}

CPickup & CPickup::operator=(const CPickup &e)
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

	//CPickup
	data = e.data;
	blip = e.blip;

	return (*this);
}

void CPickup::SyncData(RakNet::RakNetGUID user)
{
	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(data.visible);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(data.respawnDelay);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupRespawnDelay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
}