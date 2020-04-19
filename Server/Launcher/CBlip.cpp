
// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "MessageIdentifiers.h"

// GrandM
#include "Server.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "CBlip.h"

CBlip::CBlip()
{
	identity.entityId = -1;
	identity.type = GrandM::EntityType::Blip;
}

CBlip::CBlip(CVector3 position)
{
	// Assign current count as entity id
	identity.entityId = CEntity::count;
	// Increment the count for the next entity id
	CEntity::count++;

	identity.type = GrandM::EntityType::Blip;

	data.hasBlip = true;
	transform.Position = position;
}

CBlip::~CBlip()
{
	identity.entityId = -1;
	identity.type = GrandM::Nill;
}

CBlip::CBlip(const CBlip & e)
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

	//CBlip
	data = e.data;
}

CBlip & CBlip::operator=(const CBlip &e)
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

	//CBlip
	data = e.data;

	return (*this);
}

void CBlip::CreateE(const int entityId, GrandM::EntityType type)
{
	identity.type = type;
	identity.entityId = entityId;

	data.hasBlip = true;
}

void CBlip::SyncData(RakNet::RakNetGUID user)
{
	RakNet::BitStream bitstream;
	
	PRINT(ConsoleForeground::MAGENTA, false, "%i %i", identity.entityId, identity.type);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(data.color);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(data.sprite);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipSprite", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

	if (!data.text.empty())
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(RakNet::RakString(data.text.c_str()));
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipText", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
	}

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(data.isShortRange);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipShortRange", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

	if (data.visible)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowBlip", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
	}
}

void CBlip::Show()
{
	data.visible = true;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowBlip", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
}
void CBlip::Hide()
{
	data.visible = false;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("HideBlip", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
}

void CBlip::SetColor(const int color) 
{ 
	data.color = color;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(color);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
}

void CBlip::SetSprite(const int sprite)
{ 
	data.sprite = sprite;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(sprite);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipSprite", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
}

void CBlip::SetText(const std::string text)
{
	data.text = text;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(RakNet::RakString(text.c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipText", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
}

void CBlip::SetShortRanged(const bool state)
{
	data.isShortRange = state;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(state);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlipShortRange", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
}