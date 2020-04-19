// Self
#include "CNPC.h"

// System


// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

// GrandM
#include "SharedUtility.h"

#include "NetworkManager.h"
#include "Server.h"
#include "CBlip.h"

CNPC::CNPC(const uint32_t model, const CVector3 position, const CVector3 rotation)
{
	// Assign current count as entity id
	identity.entityId = CEntity::count;
	// Increment the count for the next entity id
	CEntity::count++;

	identity.type = GrandM::EntityType::NPC;

	data.Model.hash = model;

	transform.Position = position;
	transform.Rotation = rotation;

	network.lastSync = std::chrono::system_clock::now();
}

CNPC::~CNPC()
{
	identity.entityId = -1;
	identity.type = GrandM::Nill;
}

CNPC::CNPC(const CNPC & e)
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

	//CNPC
	data = e.data;
	blip = e.blip;
	network = e.network;
}

CNPC & CNPC::operator=(const CNPC &e)
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

	//CNPC
	data = e.data;
	blip = e.blip;
	network = e.network;

	return (*this);
}

void CNPC::Pulse()
{
	// Nothing yet as no AI is synced atm for NPCs
}

void CNPC::Update(RakNet::Packet *packet)
{
	// Nothing yet as no AI is synced atm for NPCs
}


void CNPC::SyncData(RakNet::RakNetGUID requester)
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

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(RakNet::RakString(tag.text.c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametag", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.textVisible);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.healthVisible);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetHealthbarVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.distance);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagDistance", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.textColor.Red);
	bitstream.Write(tag.textColor.Green);
	bitstream.Write(tag.textColor.Blue);
	bitstream.Write(tag.textColor.Alpha);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(data.Model.headBlend.shapeFirst);
	bitstream.Write(data.Model.headBlend.shapeMix);
	bitstream.Write(data.Model.headBlend.shapeSecond);
	bitstream.Write(data.Model.headBlend.shapeThird);
	bitstream.Write(data.Model.headBlend.skinFirst);
	bitstream.Write(data.Model.headBlend.skinMix);
	bitstream.Write(data.Model.headBlend.skinSecond);
	bitstream.Write(data.Model.headBlend.skinThird);
	bitstream.Write(data.Model.headBlend.thirdMix);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadBlend", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	for (unsigned int i = 0; i < SizeOfArray(data.Model.components); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.components[i].drawableid);
		bitstream.Write(data.Model.components[i].paletteid);
		bitstream.Write(data.Model.components[i].textureid);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedComponent", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	for (unsigned int i = 0; i < SizeOfArray(data.Model.faceFeatures); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.faceFeatures[i].scale);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedFaceFeature", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	for (unsigned int i = 0; i < SizeOfArray(data.Model.headOverlays); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.headOverlays[i].index);
		bitstream.Write(data.Model.headOverlays[i].colorType);
		bitstream.Write(data.Model.headOverlays[i].colorID);
		bitstream.Write(data.Model.headOverlays[i].secondColorID);
		bitstream.Write(data.Model.headOverlays[i].opacity);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadOverlay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	for (unsigned int i = 0; i < SizeOfArray(data.Model.props); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.props[i].drawableid);
		bitstream.Write(data.Model.props[i].textureid);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedProp", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(health);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetHealth", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
}

void CNPC::SetAssignee(RakNet::RakNetGUID assignee)
{
	network.physicsAssignee = assignee;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(assignee);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Server::GetServer()->GetNetworkManager()->GetInterface()->GetSystemAddressFromIndex(0), true, false);
}

void CNPC::Revive()
{
	dead = false;
	health.Set(200);
}
