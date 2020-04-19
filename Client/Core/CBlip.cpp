//Self
#include "CBlip.h"

// RakNet
#include <BitStream.h>
#include <PacketPriority.h>
#include <RakNetTypes.h>
#include <MessageIdentifiers.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

CBlip::CBlip()
{
	//CEntity
	game.entity = NULL;
	game.created = false;
	game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	game.vehicle = -1;

	identity.entityId = -1;
	identity.type = Type::eBlip;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;

	//CBlip
	hasBlip = false;

	transform.Position = CVector3::null;
}

CBlip::CBlip(const int id, const CVector3 pos)
{
	//CEntity
	game.entity = NULL;
	game.created = false;
	game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	game.vehicle = -1;

	identity.entityId = id;
	identity.type = eBlip;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;
	
	//CBlip
	hasBlip = true;

	transform.Position = pos;
}

CBlip::~CBlip()
{
	//Remove();

	identity.entityId = -1;
	identity.type = eNill;

	game.entity = NULL;

	hasBlip = false;

	transform.Position = CVector3::null;
}

CBlip::CBlip(const CBlip & e)
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

	//CBlip
	data = e.data;
	game = e.game;
	hasBlip = e.hasBlip;
}

CBlip & CBlip::operator=(const CBlip &e)
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

	//CBlip
	data = e.data;
	game = e.game;
	hasBlip = e.hasBlip;

	return (*this);
}

void CBlip::Create(const int id)
{
	identity.entityId = id;
	identity.type = eBlip;

	hasBlip = true;
}

void CBlip::CreateBlip(Entity ent)
{
	if (UI::DOES_BLIP_EXIST(game.entity))
		Remove();

	if (!ENTITY::DOES_ENTITY_EXIST(ent))
		return;

	if (hasBlip && data.visible)
	{
		game.entity = UI::ADD_BLIP_FOR_ENTITY(ent);
		//UI::SET_BLIP_AS_FRIENDLY(Game.Blip, true);

		UI::SET_BLIP_SPRITE(game.entity, data.sprite);
		UI::SET_BLIP_COLOUR(game.entity, data.color);

		UI::SET_BLIP_SCALE(game.entity, 1.0f);

		UI::SET_BLIP_CATEGORY(game.entity, 2);

		UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM((char*)data.text.c_str());
		UI::END_TEXT_COMMAND_SET_BLIP_NAME(game.entity);

		if(data.shortRange)
			UI::SET_BLIP_AS_SHORT_RANGE(game.entity, true);
	}
}

void CBlip::CreateBlip()
{
	if (UI::DOES_BLIP_EXIST(game.entity))
		Remove();

	if (hasBlip && data.visible)
	{
		game.entity = UI::ADD_BLIP_FOR_COORD(transform.Position.x, transform.Position.y, transform.Position.z);
		UI::SET_BLIP_AS_FRIENDLY(game.entity, true);

		UI::SET_BLIP_SPRITE(game.entity, data.sprite);
		UI::SET_BLIP_COLOUR(game.entity, data.color);

		UI::SET_BLIP_SCALE(game.entity, 1.0f);

		UI::SET_BLIP_CATEGORY(game.entity, 2);

		UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM((char *)data.text.c_str());
		UI::END_TEXT_COMMAND_SET_BLIP_NAME(game.entity);

		UI::SET_BLIP_AS_SHORT_RANGE(game.entity, data.shortRange);
	}
}

void CBlip::Remove()
{
	if (UI::DOES_BLIP_EXIST(game.entity))
	{
		UI::REMOVE_BLIP(&game.entity);
	}
}

void CBlip::Show(Entity ent)
{
	data.visible = true;

	CreateBlip(ent);
}

void CBlip::Show()
{
	data.visible = true;

	CreateBlip();
}

void CBlip::Hide()
{
	data.visible = false;

	Remove();
}

void CBlip::SetColor(const int color)
{
	data.color = color;

	if (UI::DOES_BLIP_EXIST(game.entity) && data.visible)
	{
		UI::SET_BLIP_COLOUR(game.entity, data.color);
	}
}

void CBlip::SetSprite(const int sprite)
{
	data.sprite = sprite;

	if (UI::DOES_BLIP_EXIST(game.entity) && data.visible)
	{
		UI::SET_BLIP_SPRITE(game.entity, data.sprite);
	}
}

void CBlip::SetPosition(const CVector3 pos)
{
	transform.Position = pos;

	if (UI::DOES_BLIP_EXIST(game.entity) && data.visible)
	{
		UI::SET_BLIP_COORDS(game.entity, pos.x, pos.y, pos.z);
	}
}

void CBlip::SetText(const std::string text)
{
	data.text = text;

	if (UI::DOES_BLIP_EXIST(game.entity) && data.visible)
	{
		UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM((char *)text.c_str());
		UI::END_TEXT_COMMAND_SET_BLIP_NAME(game.entity);
	}
}

void CBlip::SetShortRange(const bool state)
{
	data.shortRange = state;

	if (UI::DOES_BLIP_EXIST(game.entity) && data.visible)
	{
		UI::SET_BLIP_AS_SHORT_RANGE(game.entity, state);
	}
}