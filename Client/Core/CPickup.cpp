#include "CPickup.h"

// RakNet
#include <BitStream.h>
#include <PacketPriority.h>
#include <RakNetTypes.h>
#include <MessageIdentifiers.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "Functions.h"
#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

CPickup::CPickup(const int id)
{
	//CEntity
	game.entity = NULL;
	game.created = false;

	identity.entityId = id;
	identity.type = Type::ePickup;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;
	

	LOG_DEBUG("[CPickup] Added Pickup entity %i.", identity.entityId);
}

CPickup::~CPickup()
{
	identity.entityId = -1;
	identity.type = Type::eNill;
}

CPickup::CPickup(const CPickup & e)
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

	//CPickup
	data = e.data;
	game = e.game;
}

CPickup & CPickup::operator=(const CPickup &e)
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

	//CPickup
	data = e.data;
	game = e.game;

	return (*this);
}

const bool CPickup::Spawn()
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(data.model) && STREAMING::IS_MODEL_VALID(data.model))
	{
		LOG_DEBUG("[CPickup] Creating Pickup entity %i.", identity.entityId);

		STREAMING::REQUEST_MODEL(data.model);
		while (!STREAMING::HAS_MODEL_LOADED(data.model))
			WAIT(0);

		game.entity = OBJECT::CREATE_PICKUP_ROTATE(PickupTypeCustomScript, transform.Position.x, transform.Position.y, transform.Position.z, transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, 0, 0, 0, 0, data.model);

		game.created = true;

		LOG_DEBUG("[CPickup] Created Pickup entity %i.", identity.entityId);
		return true;
	}

	return false;
}

void CPickup::Delete()
{
	if (game.created)
	{
		if (OBJECT::DOES_PICKUP_EXIST(game.entity))
		{
			LOG_DEBUG("[CPickup] Deleted Pickup entity %i.", identity.entityId);
			OBJECT::REMOVE_PICKUP(game.entity);
		}

		game.created = false;
	}
}

void CPickup::Destroy()
{
	Delete();
}

void CPickup::Pulse()
{
	if (game.created)
	{
		if (!data.trigger.triggered)
		{
			if (data.lastCheck + 100 <= timeGetTime())
			{
				if (OBJECT::DOES_PICKUP_EXIST(game.entity))
				{
					if (!OBJECT::DOES_PICKUP_OBJECT_EXIST(game.entity)) // If the Object doesn't exist then its been picked up.
					{
						RakNet::BitStream bitstream;
						bitstream.Write(identity.entityId);
						CNetworkManager::GetRPC().Signal("OnPlayerPickup", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

						if (OBJECT::DOES_PICKUP_EXIST(game.entity))
							OBJECT::REMOVE_PICKUP(game.entity);

						data.trigger.triggered = true;
						data.trigger.triggeredAt = timeGetTime();
					}
				}
			}
			
			return;
		}
		else
		{
			if (data.trigger.triggeredAt + data.trigger.respawnDelay <= timeGetTime() && data.trigger.triggered)
			{
				data.trigger.triggered = false;

				game.entity = OBJECT::CREATE_PICKUP_ROTATE(PickupTypeCustomScript, transform.Position.x, transform.Position.y, transform.Position.z, transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, 0, 0, 0, 0, data.model);
			}
		}
	}
}

void CPickup::SetPosition(const CVector3 position)
{
	transform.Position = position;

	if (game.created)
	{
		if (OBJECT::DOES_PICKUP_EXIST(game.entity))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, transform.Position.x, transform.Position.y, transform.Position.z, false, false, false);
		}
	}
}

void CPickup::SetModel(const uint32_t model) 
{
	data.model = model; 

	if (game.created)
	{
		if (OBJECT::DOES_PICKUP_EXIST(game.entity) && !data.trigger.triggered)
		{
			STREAMING::REQUEST_MODEL(data.model);
			while (!STREAMING::HAS_MODEL_LOADED(data.model))
				WAIT(0);

			Delete();

			game.entity = OBJECT::CREATE_PICKUP_ROTATE(PickupTypeCustomScript, transform.Position.x, transform.Position.y, transform.Position.z, transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, 0, 0, 0, 0, data.model);
		}
	}
}