#include <vector>

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

// Hook
#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

// GrandM
#include "APIStructs.h"
#include "GUI.h"
#include "CVector3.h"
#include "CMaths.h"
#include "CLocalPlayer.h"
#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CCheckpoint.h"

CCheckpoint::CCheckpoint(const int entity, const CVector3 position, const float radius, const Color color, const int reserved)
{
	//CEntity
	game.entity = -1;
	game.created = false;
	game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	game.vehicle = -1;

	identity.entityId = entity;
	identity.type = eCheckpoint;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;

	//CCheckpoint
	transform.Position = position;

	data.radius = radius;
	data.color = color;
	data.reserved = reserved;

	data.pointTo = { 0, 0, 0 };
	data.type = 40;

	data.color = { 255, 255, 255, 255 };

	data.nearHeight = 10.0f;
	data.farHeight = 20.0f;

	data.triggered = false;

	LOG_DEBUG("[CCheckpoint] Created checkpoint entity %i.", identity.entityId);
}

CCheckpoint::~CCheckpoint()
{
	identity.entityId = -1;
	identity.type = Type::eNill;
}

CCheckpoint::CCheckpoint(const CCheckpoint & e)
{
	//CEntity
	identity = e.identity;
	game = e.game;
	world = e.world;
	tag = e.tag;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	health = e.health;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	interpolation = e.interpolation;
	sync = e.sync;

	//CCheckpoint
	data = e.data;
	game = e.game;
}

CCheckpoint & CCheckpoint::operator=(const CCheckpoint &e)
{
	//CEntity
	identity = e.identity;
	game = e.game;
	world = e.world;
	tag = e.tag;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	health = e.health;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	interpolation = e.interpolation;
	sync = e.sync;

	//CCheckpoint
	data = e.data;
	game = e.game;

	return (*this);
}

void CCheckpoint::Destroy()
{
	LOG_DEBUG("[CCheckpoint] Removing checkpoint entity %i.", identity.entityId);

	if (game.entity != -1)
		Hide();
}

void CCheckpoint::Show()
{
	Hide();

	game.entity = GRAPHICS::CREATE_CHECKPOINT(data.type, transform.Position.x, transform.Position.y, transform.Position.z, data.pointTo.x, data.pointTo.y, data.pointTo.z, data.radius, data.color.Red, data.color.Green, data.color.Blue, data.color.Alpha, data.reserved);
	GRAPHICS::SET_CHECKPOINT_CYLINDER_HEIGHT(game.entity, data.nearHeight, data.farHeight, data.radius);
}

void CCheckpoint::Hide()
{
	if (game.entity != -1)
	{
		GRAPHICS::DELETE_CHECKPOINT(game.entity);
		game.entity = -1;
	}
}

void CCheckpoint::Pulse()
{
	if (game.entity != -1)
	{
		CVector3 position = CLocalPlayer::GetPosition();

		float distance = Math::GetDistanceBetweenPoints2D(position.x, position.y, transform.Position.x, transform.Position.y);

		if ((distance <= (data.radius / 2) && position.z > transform.Position.z - 2.0f && position.z < transform.Position.z + data.nearHeight) && !data.triggered)
		{
			data.triggered = true;

			RakNet::BitStream bitstream;
			bitstream.Write(identity.entityId);
			bitstream.Write(CLocalPlayer::GetID());
			CNetworkManager::GetRPC().Signal("OnPlayerEnterCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
		}

		if ((distance > (data.radius / 2) || position.z < transform.Position.z - 2.0f || position.z > transform.Position.z + data.nearHeight) && data.triggered)
		{
			data.triggered = false;

			RakNet::BitStream bitstream;
			bitstream.Write(identity.entityId);
			bitstream.Write(CLocalPlayer::GetID());
			CNetworkManager::GetRPC().Signal("OnPlayerExitCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
		}
	}
}

void CCheckpoint::SetHeight(const float nearHeight, const float farHeight)
{
	data.nearHeight = nearHeight;
	data.farHeight = farHeight;

	GRAPHICS::SET_CHECKPOINT_CYLINDER_HEIGHT(game.entity, data.nearHeight, data.farHeight, data.radius);

	// If the above fails this will work below, really need chat for testing at this point
	/*if(game.checkpoint != -1)
		Show();*/
}

void CCheckpoint::SetPosition(CVector3 position)
{
	transform.Position = position;

	if (game.entity != -1)
		Show();
}

void CCheckpoint::PointTo(const CVector3 pointTo)
{
	data.pointTo = pointTo;

	if (game.entity != -1)
		Show();
}

void CCheckpoint::SetType(const int type)
{
	data.type = type;

	if (game.entity != -1)
		Show();
}

void CCheckpoint::SetColor(const Color color)
{
	data.color = color;

	if (game.entity != -1)
		Show();
}