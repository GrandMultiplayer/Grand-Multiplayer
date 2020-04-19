#include <vector>
#include <string>

// RakNet
#include <BitStream.h>
#include <PacketPriority.h>
#include <MessageIdentifiers.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "Functions.h"
#include "APIStructs.h"
#include "SharedUtility.h"
#include "CVector3.h"
#include "CMaths.h"
#include "CNPC.h"
#include "GamePed.h"
#include "CVisual.h"
#include "CNetworkManager.h"
#include "CLocalPlayer.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

CNPC::CNPC(const int entity, const uint32_t model, const CVector3 position, const CVector3 rotation)
{
	if (!STREAMING::IS_MODEL_IN_CDIMAGE(data.model.hash) || !STREAMING::IS_MODEL_VALID(data.model.hash))
	{
		LOG_DEBUG("[CNPC] NPC entity %i has invalid model %i", identity.entityId, data.model.hash);
		return;
	}

	//CEntity
	game.entity = NULL;
	game.created = false;
	game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	game.vehicle = -1;

	identity.entityId = entity;
	identity.type = eNPC;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;

	//CNPC
	data.model.hash = model;
	transform.Position = position;

	transform.Rotation = rotation;

	LOG_DEBUG("[CNPC] Added NPC entity %i.", identity.entityId);

	network.lastSyncSent = std::chrono::system_clock::now();
	network.synchronized = true;
}

CNPC::~CNPC()
{
	identity.entityId = -1;
	identity.type = Type::eNill;
}

CNPC::CNPC(const CNPC & e)
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

	//CNPC
	data = e.data;
	game = e.game;
	network = e.network;
}

CNPC & CNPC::operator=(const CNPC &e)
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

	//CNPC
	data = e.data;
	game = e.game;
	network = e.network;

	return (*this);
}

bool CNPC::CreateNpc()
{
	if (game.created)
	{
		LOG_DEBUG("[CNPC] NPC entity %i already created.", identity.entityId);
		return false;
	}

	if (!STREAMING::IS_MODEL_IN_CDIMAGE(data.model.hash) || !STREAMING::IS_MODEL_VALID(data.model.hash))
	{
		LOG_WARNING("[CNPC] Tried to create NPC entity %i, but model does not exist!", identity.entityId);
		return false;
	}
	
	LOG_DEBUG("[CNPC] Creating NPC entity %i.", identity.entityId);
	
	STREAMING::REQUEST_MODEL(data.model.hash);

	if (!STREAMING::HAS_MODEL_LOADED(data.model.hash))
		return false;

	game.entity = PED::CREATE_PED(26, data.model.hash, transform.Position.x, transform.Position.y, transform.Position.z, transform.Rotation.z, false, true);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(data.model.hash);

	ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(CLocalPlayer::GetPed(), game.entity, false);
	ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(game.entity, CLocalPlayer::GetPed(), false);

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, transform.Position.x, transform.Position.y, transform.Position.z, false, false, false);

	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(game.entity, true);
	PED::SET_PED_FLEE_ATTRIBUTES(game.entity, 0, 0);
	PED::SET_PED_COMBAT_ATTRIBUTES(game.entity, 17, true);
	PED::SET_PED_CAN_RAGDOLL(game.entity, false);
	PED::UNREGISTER_PEDHEADSHOT(game.entity);

	WEAPON::SET_PED_DROPS_WEAPONS_WHEN_DEAD(game.entity, false);

	BRAIN::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(game.entity, true);

	Hash relationshipGroup;
	PED::ADD_RELATIONSHIP_GROUP("GRANDMPLAYER", &relationshipGroup);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(game.entity, relationshipGroup);

	PED::SET_PED_DEFAULT_COMPONENT_VARIATION(game.entity);
	PED::SET_PED_CAN_EVASIVE_DIVE(game.entity, false);
	PED::SET_PED_CAN_BE_TARGETTED(game.entity, true);
	PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(game.entity, CLocalPlayer::GetPlayer(), true);
	PED::SET_PED_GET_OUT_UPSIDE_DOWN_VEHICLE(game.entity, false);
	PED::SET_PED_AS_ENEMY(game.entity, false);
	PED::SET_CAN_ATTACK_FRIENDLY(game.entity, true, true);

	// Set Component Variations
	for (int i = 0; i < SizeOfArray(data.model.components); i++)
	{
		GTAV::GamePed::SetPedComponentVariation(game.entity, i, data.model.components[i].drawableid, data.model.components[i].textureid, data.model.components[i].paletteid);
	}

	// Set Headblend
	GTAV::GamePed::SetPedHeadBlend(game.entity, data.model.headBlend.shapeFirst, data.model.headBlend.shapeSecond, data.model.headBlend.shapeThird, data.model.headBlend.skinFirst, data.model.headBlend.skinSecond, data.model.headBlend.skinThird, data.model.headBlend.shapeMix, data.model.headBlend.skinMix, data.model.headBlend.thirdMix);

	// Set Head 
	for (int i = 0; i < SizeOfArray(data.model.headOverlays); i++)
	{
		GTAV::GamePed::SetPedHeadOverlayColor(game.entity, i, data.model.headOverlays[i].index, data.model.headOverlays[i].colorType, data.model.headOverlays[i].colorID, data.model.headOverlays[i].secondColorID, data.model.headOverlays[i].opacity);
	}

	// Set Ped Props
	for (int i = 0; i < SizeOfArray(data.model.props); i++)
	{
		GTAV::GamePed::SetPedProp(game.entity, i, data.model.props[i].drawableid, data.model.props[i].textureid);
	}

	// Set Face Features
	for (int i = 0; i < SizeOfArray(data.model.faceFeatures); i++)
	{
		GTAV::GamePed::SetPedFaceFeature(game.entity, i, data.model.faceFeatures[i].scale);
	}

	blip.CreateBlip(game.entity);

	//PED::SET_PED_MOVEMENT_CLIPSET(game.npc, "move_jump@weapons@rpg", 0);

	game.created = true;
	LOG_DEBUG("[CNPC] Created NPC entity %i.", identity.entityId);

	return true;
}

void CNPC::Destroy()
{
	LOG_DEBUG("[CNPC] Removing NPC entity %i.", identity.entityId);

	if (game.created)
	{
		blip.Remove();

		if (ENTITY::DOES_ENTITY_EXIST(game.entity))
			ENTITY::DELETE_ENTITY(&game.entity);

		game.created = false;
	}
}

void CNPC::Delete()
{
	if (game.created)
	{
		blip.Remove();

		if (ENTITY::DOES_ENTITY_EXIST(game.entity))
			ENTITY::DELETE_ENTITY(&game.entity);

		game.created = false;
	}

	game.entity = NULL;
}

void CNPC::Pulse()
{
	ShowNametag();
}

void CNPC::SetModelComponent(const int index, const int drawableid, const int textureid, const int paletteid)
{
	data.model.components[index].drawableid = drawableid;
	data.model.components[index].textureid = textureid;
	data.model.components[index].paletteid = paletteid;

	if (game.created)
		GTAV::GamePed::SetPedComponentVariation(game.entity, index, drawableid, textureid, paletteid);
}

void CNPC::SetModelHeadBlend(const int shapeFirst, const float shapeMix, const int shapeSecond, const int shapeThird, const int skinFirst, const float skinMix, const int skinSecond, const int skinThird, const float thirdMix)
{
	data.model.headBlend.shapeFirst = shapeFirst;
	data.model.headBlend.shapeMix = shapeMix;
	data.model.headBlend.shapeSecond = shapeSecond;
	data.model.headBlend.shapeThird = shapeThird;
	data.model.headBlend.skinFirst = skinFirst;
	data.model.headBlend.skinMix = skinMix;
	data.model.headBlend.skinSecond = skinSecond;
	data.model.headBlend.skinThird = skinThird;
	data.model.headBlend.thirdMix = thirdMix;

	if (game.created)
		GTAV::GamePed::SetPedHeadBlend(game.entity, shapeFirst, shapeSecond, shapeThird, skinFirst, skinSecond, skinThird, shapeMix, skinMix, thirdMix);
}

void CNPC::SetModelHeadOverlay(const int index, const int type, const int colorType, const int colorID, const int secondColorID, const float opacity)
{
	data.model.headOverlays[index].index = type;
	data.model.headOverlays[index].colorType = colorType;
	data.model.headOverlays[index].colorID = colorID;
	data.model.headOverlays[index].secondColorID = secondColorID;
	data.model.headOverlays[index].opacity = opacity;

	if (game.created)
		GTAV::GamePed::SetPedHeadOverlayColor(game.entity, index, index, colorType, colorID, secondColorID, opacity);
}

void CNPC::SetModelProp(const int index, const int drawableid, const int textureid)
{
	data.model.props[index].drawableid = drawableid;
	data.model.props[index].textureid = textureid;

	if (game.created)
		GTAV::GamePed::SetPedProp(game.entity, index, drawableid, textureid);
}

void CNPC::SetModelFaceFeature(const int index, const float scale)
{ 
	data.model.faceFeatures[index].scale = scale;

	if (game.created)
		GTAV::GamePed::SetPedFaceFeature(game.created, index, scale);
}

void CNPC::ShowNametag()
{
	if (!game.created)
		return;
	
	CVector3 localPos = CLocalPlayer::GetPosition();
	if (!ENTITY::IS_ENTITY_OCCLUDED(game.entity) && 
		Math::GetDistanceBetweenPoints3D(transform.Position.x, transform.Position.y, transform.Position.z, localPos.x, localPos.y, localPos.z) < tag.distance && !ENTITY::IS_ENTITY_DEAD(game.entity, 0))
	{
		float width = 0.03f;
		float height = 0.0065f;
		float border = 0.001f;

		float sX, sY;
		Vector3 Coordinates = PED::GET_PED_BONE_COORDS(game.entity, eBone::SKEL_Head, 0.0f, 0.0f, 0.0f);
		GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(Coordinates.x, Coordinates.y, Coordinates.z + 0.3, &sX, &sY);

		// Text Nametag
		if (tag.textVisible)
			CVisual::TextDraw(tag.text.c_str(), sX, (sY - 0.05f), 0.4f, 2, tag.textColor.Red, tag.textColor.Green, tag.textColor.Blue, tag.textColor.Alpha, true, false, true);

		
		if (tag.healthVisible)
		{
			float health = ENTITY::GET_ENTITY_HEALTH(game.entity);
			float armour = PED::GET_PED_ARMOUR(game.entity);

			if (health <= 100)
				health = 0;
			else
				health = (health - 100) / 100;

			if (armour <= 100)
				armour = 0;
			else
				armour = (armour - 100) / 100;

			// Helath/Armour outline
			GRAPHICS::DRAW_RECT(sX, sY - 0.01, width + (border * 2), height + (border * 2), 0, 0, 0, 200, 0);

			// Helath/Armour background
			GRAPHICS::DRAW_RECT(sX, sY - 0.01, width, height, 150, 150, 150, 255, 0);

			if (armour > 0)
			{
				// Armour forground
				GRAPHICS::DRAW_RECT(sX - width / 2 * (1 - armour), sY - 0.01, width * armour, height, 0, 76, 153, 255, 0);

				// Helath outline
				GRAPHICS::DRAW_RECT(sX, sY - 0.01 + 0.008f, width + (border * 2), height + (border * 2), 0, 0, 0, 200, 0);
				// Helath background
				GRAPHICS::DRAW_RECT(sX, sY - 0.01 + 0.008f, width, height, 150, 150, 150, 255, 0);
				// Health forground
				GRAPHICS::DRAW_RECT(sX - width / 2 * (1 - health), sY - 0.01 + 0.008f, width * health, height, 0, 153, 76, 255, 0);
			}
			else
			{
				// Health forground
				GRAPHICS::DRAW_RECT(sX - width / 2 * (1 - health), sY - 0.01, width * health, height, 0, 153, 76, 255, 0);
			}
		}
	}

	// Would require the ped to be given to the game network system.
	/*if (!UI::_HAS_HEAD_DISPLAY_LOADED(game.npc))
	{
		int display = UI::_CREATE_MP_GAMER_TAG(game.npc, "My NPC", false, false, "", 1);
		UI::_SET_HEAD_DISPLAY_STRING(display, "My NPC");
		UI::_SET_HEAD_DISPLAY_FLAG(display, HeadDisplayFlag::TextWithOutline, true);
		UI::_SET_HEAD_DISPLAY_FLAG(display, HeadDisplayFlag::HealthBar, true);
		UI::_SET_HEAD_DISPLAY_FLAG_COLOR(display, HeadDisplayFlag::TextWithOutline, 0);
		UI::_SET_HEAD_DISPLAY_HEALTH_BAR_COLOR(display, 0);
		UI::_SET_HEAD_DISPLAY_FLAG_COLOR(display, HeadDisplayFlag::TextWithOutline, 255);
		UI::_SET_HEAD_DISPLAY_FLAG_COLOR(display, HeadDisplayFlag::HealthBar, 255);
	}*/
}

void CNPC::SetArmour(const int armour)
{
	data.armour = armour;

	if (game.created && ENTITY::DOES_ENTITY_EXIST(game.entity))
	{
		PED::SET_PED_ARMOUR(game.entity, armour);
	}
}

void CNPC::Revive()
{
	health.Set(200);

	PED::REVIVE_INJURED_PED(game.entity);
	PED::RESURRECT_PED(game.entity);
	PLAYER::RESET_PLAYER_ARREST_STATE(game.entity);
	ENTITY::SET_ENTITY_HEALTH(game.entity, health.Get());
	PED::RESET_PED_VISIBLE_DAMAGE(game.entity);
}