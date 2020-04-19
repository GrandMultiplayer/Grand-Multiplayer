#include <vector>

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "Functions.h"
#include "CPlayer.h"
#include "GamePed.h"
#include "APIStructs.h"
#include "SharedUtility.h"
#include "CVector3.h"
#include "CMaths.h"
#include "CNetworkManager.h"
#include "CLocalPlayer.h"
#include "CVisual.h"
#include "Ped.h"
#include "Animations.h"
#include "CVehicle.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

//#define _LOCALTESTING_

CPlayer::CPlayer(const int id, std::string Name)
{
	//CEntity
	game.entity = NULL;
	game.created = false;
	game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	game.vehicle = -1;

	identity.entityId = id;
	identity.type = Type::ePlayer;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;

	//CPlayer
	player.name = Name;
	tag.text = Name;

	data.vehicle.entityID = -1;
	data.vehicle.seat = -1;

	if (CLocalPlayer::GetID() != identity.entityId)
	{
		RequestData();
	}

	LOG_DEBUG("[CPlayer] Added Player [%i][%s]", identity.entityId, player.name.c_str());
}

CPlayer::~CPlayer()
{
	identity.entityId = -1;
	identity.type = Type::eNill;
}

CPlayer::CPlayer(const CPlayer & e)
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

	//CPlayer
	grand = e.grand;
	player = e.player;
	data = e.data;
}

CPlayer & CPlayer::operator=(const CPlayer &e)
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

	//CPlayer
	grand = e.grand;
	player = e.player;
	data = e.data;

	return (*this);
}

bool CPlayer::CreatePed()
{
	if (game.created)
	{
		LOG_WARNING("[CPlayer] Ped for %i already created.", identity.entityId);
		return false;
	}

	if (!STREAMING::IS_MODEL_IN_CDIMAGE(data.model.hash) || !STREAMING::IS_MODEL_VALID(data.model.hash))
	{
		LOG_WARNING("[CPlayer] Tried to create %i, but model <%i> does not exist!", identity.entityId, data.model.hash);
		return false;
	}

	if (STREAMING::IS_MODEL_IN_CDIMAGE(data.model.hash) && STREAMING::IS_MODEL_VALID(data.model.hash))
	{
		LOG_DEBUG("[CPlayer] Creating Ped for Entity %i", identity.entityId);
		STREAMING::REQUEST_MODEL(data.model.hash);

		if (!STREAMING::HAS_MODEL_LOADED(data.model.hash))
			return false;

		game.entity = PED::CREATE_PED(26, data.model.hash, transform.Position.x, transform.Position.y, transform.Position.z, 0.0f, false, true);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(data.model.hash);

		ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(CLocalPlayer::GetPed(), game.entity, false);
		ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(game.entity, CLocalPlayer::GetPed(), false);

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, transform.Position.x, transform.Position.y, transform.Position.z, false, false, false);

		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(game.entity, true);
		PED::SET_PED_FLEE_ATTRIBUTES(game.entity, 0, 0);
		PED::SET_PED_CAN_RAGDOLL(game.entity, true);
		PED::UNREGISTER_PEDHEADSHOT(game.entity);

		//PED::SET_PED_COMBAT_ATTRIBUTES(game.entity, 8, 0);
		PED::SET_PED_COMBAT_ATTRIBUTES(game.entity, 9, 0);
		PED::SET_PED_COMBAT_ATTRIBUTES(game.entity, 13, 0);
		PED::SET_PED_COMBAT_ATTRIBUTES(game.entity, 14, 0);
		PED::SET_PED_COMBAT_ATTRIBUTES(game.entity, 17, 0);
		PED::SET_PED_COMBAT_ATTRIBUTES(game.entity, 46, 0);
		PED::SET_PED_ALERTNESS(game.entity, 0);
		PED::SET_PED_COMBAT_ABILITY(game.entity, 0);
		PED::SET_PED_COMBAT_RANGE(game.entity, 0);
		PED::SET_PED_SEEING_RANGE(game.entity, 0.0);
		PED::SET_PED_HEARING_RANGE(game.entity, 0.0);

		WEAPON::REMOVE_ALL_PED_WEAPONS(game.entity, false);
		WEAPON::SET_PED_DROPS_WEAPONS_WHEN_DEAD(game.entity, false);

		BRAIN::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(game.entity, true);

		WEAPON::SET_PED_INFINITE_AMMO_CLIP(game.entity, true);

		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(game.entity);
		PED::SET_PED_CAN_EVASIVE_DIVE(game.entity, false);
		PED::SET_PED_CAN_BE_TARGETTED(game.entity, true);
		PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(game.entity, CLocalPlayer::GetPlayer(), true);
		PED::SET_PED_GET_OUT_UPSIDE_DOWN_VEHICLE(game.entity, false);
		PED::SET_PED_AS_ENEMY(game.entity, false);
		PED::SET_CAN_ATTACK_FRIENDLY(game.entity, true, true);

		Hash relationshipGroup;
		PED::ADD_RELATIONSHIP_GROUP("GRANDMPLAYER", &relationshipGroup);
		PED::SET_PED_RELATIONSHIP_GROUP_HASH(game.entity, relationshipGroup);

		grand.blip.CreateBlip(game.entity);

		// Set Component Variations
		for (unsigned int i = 0; i < SizeOfArray(data.model.components); i++)
		{
			if (data.model.components[i].drawableid != -1)
				GTAV::GamePed::SetPedComponentVariation(game.entity, i, data.model.components[i].drawableid, data.model.components[i].textureid, data.model.components[i].paletteid);
		}

		// Set Headblend
		GTAV::GamePed::SetPedHeadBlend(game.entity, data.model.headBlend.shapeFirst, data.model.headBlend.shapeSecond, data.model.headBlend.shapeThird, data.model.headBlend.skinFirst, data.model.headBlend.skinSecond, data.model.headBlend.skinThird, data.model.headBlend.shapeMix, data.model.headBlend.skinMix, data.model.headBlend.thirdMix);

		// Set Head 
		for (unsigned int i = 0; i < SizeOfArray(data.model.headOverlays); i++)
		{
			GTAV::GamePed::SetPedHeadOverlayColor(game.entity, i, data.model.headOverlays[i].index, data.model.headOverlays[i].colorType, data.model.headOverlays[i].colorID, data.model.headOverlays[i].secondColorID, data.model.headOverlays[i].opacity);
		}

		// Set Ped Props
		for (unsigned int i = 0; i < SizeOfArray(data.model.props); i++)
		{
			GTAV::GamePed::SetPedProp(game.entity, i, data.model.props[i].drawableid, data.model.props[i].textureid);
		}

		// Set Face Features
		for (unsigned int i = 0; i < SizeOfArray(data.model.faceFeatures); i++)
		{
			GTAV::GamePed::SetPedFaceFeature(game.entity, i, data.model.faceFeatures[i].scale);
		}

		ENTITY::SET_ENTITY_HEALTH(game.entity, health.Get());

		// Aiming Object
		if (!ENTITY::DOES_ENTITY_EXIST(grand.aimingObject))
		{
			STREAMING::REQUEST_MODEL(144995201);//-512779781

			while (!STREAMING::HAS_MODEL_LOADED(144995201))
			{
				WAIT(100);
			}

			grand.aimingObject = OBJECT::CREATE_OBJECT_NO_OFFSET(144995201, data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z, false, false, false);
			ENTITY::SET_ENTITY_COLLISION(grand.aimingObject, false, false);
			ENTITY::SET_ENTITY_ALPHA(grand.aimingObject, 0, true);
		}

		// Follow Object
		if (!ENTITY::DOES_ENTITY_EXIST(grand.followObject))
		{
			STREAMING::REQUEST_MODEL(144995201);

			while (!STREAMING::HAS_MODEL_LOADED(144995201))
			{
				WAIT(100);
			}

			grand.followObject = OBJECT::CREATE_OBJECT_NO_OFFSET(144995201, data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z, false, false, false);
			ENTITY::SET_ENTITY_COLLISION(grand.followObject, false, false);
			ENTITY::SET_ENTITY_ALPHA(grand.followObject, 0, true);
		}

		game.created = true;

		LOG_DEBUG("[CPlayer] Created Ped");
		return true;
	}
}

// Gets the data thats only needed once thats not synced constantly
void CPlayer::RequestData()
{
	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	CNetworkManager::GetRPC().Signal("RequestEntityData", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
}

void CPlayer::Destroy()
{
	LOG_DEBUG("[CPlayer] Removing Player entity %i.", identity.entityId);

	BRAIN::TASK_STAND_STILL(game.entity, 1);

	if (game.created)
	{
		if (ENTITY::DOES_ENTITY_EXIST(game.entity))
			ENTITY::DELETE_ENTITY(&game.entity);

		grand.blip.Remove();

		game.created = false;
	}

	if (ENTITY::DOES_ENTITY_EXIST(grand.followObject))
		ENTITY::DELETE_ENTITY(&grand.followObject);

	if (ENTITY::DOES_ENTITY_EXIST(grand.aimingObject))
		ENTITY::DELETE_ENTITY(&grand.aimingObject);

	identity.entityId = -1;

	data.vehicle.entityID = -1;
	data.vehicle.seat = -1;
}

void CPlayer::Delete()
{
	if (ENTITY::DOES_ENTITY_EXIST(game.entity))
		ENTITY::DELETE_ENTITY(&game.entity);

	grand.blip.Remove();

	game.entity = 0;

	game.created = false;
}

void CPlayer::Pulse()
{
	if (game.created && Exists())
		Interpolate();
}

//void CPlayer::UpdateMovement(RakNet::Packet *packet)
//{
//	int id = -1, type;
//
//	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);
//
//	bitstream.Read(type);
//	bitstream.Read(id);
//
//	if (id == -1)
//		return;
//
//#ifndef _LOCALTESTING_
//	bitstream.Read(transform.Position.x);
//#else
//	bitstream.Read(transform.Position.x);
//	transform.Position.x += 5.f;
//#endif
//	bitstream.Read(transform.Position.y);
//	bitstream.Read(transform.Position.z);
//
//	bitstream.Read(transform.Rotation.x);
//	bitstream.Read(transform.Rotation.y);
//	bitstream.Read(transform.Rotation.z);
//
//	bitstream.Read(transform.Velocity.x);
//	bitstream.Read(transform.Velocity.y);
//	bitstream.Read(transform.Velocity.z);
//
//	bitstream.Read(data.forwardSpeed);
//
//#ifndef _LOCALTESTING_
//	if (CLocalPlayer::GetID() != identity.entityId)
//	{
//#endif
//		if (!data.weapon.aiming && data.task != 309)
//		{
//			UpdateTargetPosition();
//			UpdateTargetRotation();
//		}
//
//		UpdateTargetAnimations();
//		UpdateTargetData();
//#ifndef _LOCALTESTING_
//	}
//#endif
//}

void CPlayer::Update(RakNet::Packet *packet)
{
	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);

	bitstream.Read(identity.entityId);

	RakNet::RakString name;
	bitstream.Read(name);
	player.name = name.C_String();

	bitstream.Read(data.model.hash); // Do we really need to stream this?????

	bitstream.Read(data.vehicle.entityID);
	bitstream.Read(data.vehicle.seat);

	bitstream.Read(data.task);

	bitstream.Read(data.weapon.hash);
	bitstream.Read(data.weapon.reloading);

	bitstream.Read(data.weapon.melee.inMeleeCombat);
	bitstream.Read(data.weapon.melee.targetId);
	bitstream.Read(data.weapon.melee.targetType);

	bitstream.Read(data.weapon.aimPosition.x);
	bitstream.Read(data.weapon.aimPosition.y);
	bitstream.Read(data.weapon.aimPosition.z);

#ifndef _LOCALTESTING_
	if (CLocalPlayer::GetID() != identity.entityId)
	{
#endif
		/*if (data.model.hash != $(data.model.name) && game.created)
			UpdatePlayerModel();*/
#ifndef _LOCALTESTING_
	}
#endif
}

void CPlayer::Interpolate()
{
	SetTargetPosition();
	SetTargetRotation();

	ShowNametag();
}

//void CPlayer::UpdateTargetPosition()
//{
//	if (game.created && Exists())
//	{
//		unsigned long CurrentTime = timeGetTime();
//		unsigned int InterpolationTime = CurrentTime - network.lastSyncReceived;
//
//		// Get our position
//		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
//		CVector3 CurrentPosition = { Coordinates.x, Coordinates.y, Coordinates.z };
//
//		// Set the target position
//		CVector3 TargetPosition = transform.Position;
//		interpolation.position.target = TargetPosition;
//
//		// Calculate the relative error
//		interpolation.position.error = TargetPosition - CurrentPosition;
//
//		// Get the interpolation interval
//		interpolation.position.startTime = CurrentTime;
//		interpolation.position.finishTime = (CurrentTime + InterpolationTime);
//
//		// Initialize the interpolation
//		interpolation.position.lastAlpha = 0.0f;
//	}
//}

void CPlayer::SetTargetPosition()
{
	if (interpolation.position.finishTime != 0 && game.created && data.vehicle.entityID == -1 && Exists())
	{
		// Get our position
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
		CVector3 vecCurrentPosition = { Coordinates.x, Coordinates.y, Coordinates.z };

		// Get the factor of time spent from the interpolation start to the current time.
		unsigned long CurrentTime = timeGetTime();
		float fAlpha = Math::Unlerp(interpolation.position.startTime, CurrentTime, interpolation.position.finishTime);

		// Don't let it overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - interpolation.position.lastAlpha);
		interpolation.position.lastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, interpolation.position.error);

		// If we finished compensating the error, finish it for the next pulse
		if (fAlpha == 1.0f)
			interpolation.position.finishTime = 0;

		// Calculate the new position
		CVector3 vecNewPosition = vecCurrentPosition + vecCompensation;

		// Check if the distance to interpolate is too far
		if (CVector3::Distance(vecCurrentPosition, interpolation.position.target) > 150.0f)
		{
			// Abort all interpolation
			interpolation.position.finishTime = 0;
			vecNewPosition = interpolation.position.target;
		}

		// Set our new position
		if (!data.weapon.aiming)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, vecNewPosition.x, vecNewPosition.y, vecNewPosition.z, false, false, false);
			ENTITY::SET_ENTITY_VELOCITY(game.entity, transform.Velocity.x, transform.Velocity.y, transform.Velocity.z);
		}
		else
		{
			if (CVector3::Distance(vecCurrentPosition, transform.Position) > 0.5f)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, vecNewPosition.x, vecNewPosition.y, vecNewPosition.z, false, false, false);
				ENTITY::SET_ENTITY_VELOCITY(game.entity, transform.Velocity.x, transform.Velocity.y, transform.Velocity.z);
				BRAIN::TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD(game.entity, vecNewPosition.x, vecNewPosition.y, vecNewPosition.z, data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z, 2.0f, 0, 0x3F000000, 0x40800000, 1, 0, 0, 3337513804U);
			}
			else
			{
				BRAIN::TASK_AIM_GUN_AT_COORD(game.entity, data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z, 100, 0, 0);
			}
		}
	}
}

//void CPlayer::UpdateTargetRotation()
//{
//	if (game.created && Exists())
//	{
//		unsigned long CurrentTime = timeGetTime();
//		unsigned int interpolationtime = CurrentTime - network.lastSyncReceived;
//
//		// Get our rotation
//		Vector3 CurrentRotationVec = ENTITY::GET_ENTITY_ROTATION(game.entity, 2);
//		CVector3 CurrentRotation(CurrentRotationVec.x, CurrentRotationVec.y, CurrentRotationVec.z);
//
//		// Set the target rotation
//		CVector3 TargetRotation = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
//		interpolation.rotation.target = TargetRotation;
//
//		// Get the error
//		interpolation.rotation.error.x = Math::GetOffsetDegrees(CurrentRotation.x, TargetRotation.x);
//		interpolation.rotation.error.y = Math::GetOffsetDegrees(CurrentRotation.y, TargetRotation.y);
//		interpolation.rotation.error.z = Math::GetOffsetDegrees(CurrentRotation.z, TargetRotation.z);
//		//InterpolationData.Rotation.Error = TargetRotation - CurrentRotation;
//		interpolation.rotation.error *= Math::Lerp < const float >(0.40f, Math::UnlerpClamped(100, interpolationtime, 400), 1.0f);
//
//		// Get the interpolation interval
//		interpolation.rotation.startTime = CurrentTime;
//		interpolation.rotation.finishTime = (CurrentTime + interpolationtime);
//
//		// Initialize the interpolation
//		interpolation.rotation.lastAlpha = 0.0f;
//	}
//}

void CPlayer::SetTargetRotation()
{
	if (interpolation.rotation.finishTime != 0 && game.created && data.vehicle.entityID == -1 && Exists())
	{
		// Get our rotation
		Vector3 CurrentRotationVec = ENTITY::GET_ENTITY_ROTATION(game.entity, 2);
		CVector3 CurrentRotation(CurrentRotationVec.x, CurrentRotationVec.y, CurrentRotationVec.z);


		// Get the factor of time spent from the interpolation start to the current time.
		unsigned long CurrentTime = timeGetTime();
		float fAlpha = Math::Unlerp(interpolation.rotation.startTime, CurrentTime, interpolation.rotation.finishTime);

		// Don't let it overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - interpolation.rotation.lastAlpha);
		interpolation.rotation.lastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, interpolation.rotation.error);

		// If we finished compensating the error, finish it for the next pulse
		if (fAlpha == 1.0f)
			interpolation.rotation.finishTime = 0;

		// Calculate the new rotation
		CVector3 vecNewRotation = CurrentRotation + vecCompensation;

		// Set our new rotation
		if (!data.weapon.aiming)
		{
			ENTITY::SET_ENTITY_ROTATION(game.entity, vecNewRotation.x, vecNewRotation.y, vecNewRotation.z, 2, true);
		}
	}
}

void CPlayer::UpdateTargetAnimations()
{
	if (game.created && data.vehicle.entityID == -1 && Exists())
	{
		if (IsTargetAnimal())
		{
			std::string dict;
			std::string name;

			if (data.forwardSpeed < 2.0f && data.forwardSpeed > 1.0f)
			{
				Animations::GetAnimalAnimation(data.model.hash, 1, &dict, &name);

				if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
					STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

				if (!ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 3))
					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 8.0f, 10.0f, -1, 1, -8.0f, 1, 1, 1);
			}
			else if (data.forwardSpeed > 2.0f && data.forwardSpeed <= 5.2f)
			{
				Animations::GetAnimalAnimation(data.model.hash, 2, &dict, &name);

				if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
					STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

				if (!ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 3))
					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 8.0f, 10.0f, -1, 1, -8.0f, 1, 1, 1);
			}
			else if (data.forwardSpeed > 5.2f)
			{
				Animations::GetAnimalAnimation(data.model.hash, 3, &dict, &name);

				if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
					STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

				if (!ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 3))
					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 8.0f, 10.0f, -1, 1, -8.0f, 1, 1, 1);
			}
			else if (data.forwardSpeed < 1.0f)
			{
				Animations::GetAnimalAnimation(data.model.hash, 0, &dict, &name);

				if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
					STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

				if (!ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 3))
					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)dict.c_str(), (char*)name.c_str(), 8.0f, 10.0f, -1, 1, -8.0f, 1, 1, 1);
			}
		}
		else
		{
			if (/*data.weapon.justShot*/!data.weapon.melee.swingFinished /*&& data.weapon.melee.animationEnd <= timeGetTime()*/)
			{
				if (ENTITY::GET_ENTITY_ANIM_CURRENT_TIME(game.entity, (char*)data.weapon.melee.meleeDic.c_str(), (char*)data.weapon.melee.meleeAnim.c_str()) >= data.weapon.melee.animationEnd)
				{
					BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.weapon.melee.meleeDic.c_str(), (char*)data.weapon.melee.meleeAnim.c_str(), -4);
					
					data.weapon.melee.swingFinished = true;
					data.weapon.justShot = false;
				}
			}

			if (data.weapon.melee.inMeleeCombat && !data.weapon.justShot && data.weapon.melee.swingFinished)
			{
				int wepClass = GTAV::Weapon::GetWeaponClass(data.weapon.hash);
				std::string AnimDic;
				std::string Anim = Animations::GetMovementAnimation(data.forwardSpeed, false, false, false, data.weapon.aiming );
				if (wepClass == 1) 
					AnimDic = "move_strafe@melee_small_weapon";
				else if (wepClass == 2) 
					AnimDic = "move_strafe@melee_large_weapon";
				else if (wepClass == 0)
				{
					AnimDic = "melee@unarmed@streamed_core_fps"; // this probably should be our main anim dictionary for melee stuffs
					Anim = "idle";
				}

				if (!AnimDic.empty() && !ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)AnimDic.c_str(), (char*)Anim.c_str(), 3))
				{
					if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)AnimDic.c_str()))
						STREAMING::REQUEST_ANIM_DICT((char*)AnimDic.c_str());

					if (!data.weapon.melee.meleeAnim.empty())
						BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.weapon.melee.meleeDic.c_str(), (char*)data.weapon.melee.meleeAnim.c_str(), -4);

					data.weapon.melee.meleeDic = AnimDic;
					data.weapon.melee.meleeAnim = Anim;

					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)AnimDic.c_str(), (char*)Anim.c_str(), 8.0f, 10.0f, -1, 32 | 16 | 1, -8.0f, 1, 1, 1);
				}
			}
			else if (!data.weapon.aiming && !data.weapon.melee.inMeleeCombat && !data.weapon.reloading && data.weapon.melee.swingFinished && !data.jumping)
			{
				// Is Vault/Climbing and not on Ladder
				const unsigned int flag = data.task == 420 && data.task != 47 ? 2147483650U : 2147483649U;

				if (!STREAMING::HAS_ANIM_DICT_LOADED("move_m@generic"))
					STREAMING::REQUEST_ANIM_DICT("move_m@generic");

				if (data.forwardSpeed < 2.0f && data.forwardSpeed > 1.0f && !ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, "move_m@generic", "walk", 3))
				{
					if (!data.onFootAnim.animation.empty())
						BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.dictionary.c_str(), -4);

					data.onFootAnim.dictionary = "move_m@generic";
					data.onFootAnim.animation = "walk";

					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), 8.0f, 10.0f, -1, flag, -8.0f, 1, 1, 1);
				}
				else if (data.forwardSpeed > 2.0f && data.forwardSpeed <= 5.2f && !ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, "move_m@generic", "run", 3))
				{
					if (!data.onFootAnim.animation.empty())
						BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), -4);

					data.onFootAnim.dictionary = "move_m@generic";
					data.onFootAnim.animation = "run";

					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), 8.0f, 10.0f, -1, flag, -8.0f, 1, 1, 1);
				}
				else if (data.forwardSpeed > 5.2f && !ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, "move_m@generic", "sprint", 3))
				{
					if (!data.onFootAnim.animation.empty())
						BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), -4);

					data.onFootAnim.dictionary = "move_m@generic";
					data.onFootAnim.animation = "sprint";

					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), 8.0f, 10.0f, -1, flag, -8.0f, 1, 1, 1);
				}
				else if (data.forwardSpeed < 1.0f && !ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, "move_m@generic", "idle", 3))
				{
					if (!data.onFootAnim.animation.empty())
						BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), -4);

					data.onFootAnim.dictionary = "move_m@generic";
					data.onFootAnim.animation = "idle";

					BRAIN::TASK_PLAY_ANIM(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), 8.0f, 10.0f, -1, flag, -8.0f, 1, 1, 1);
				}
			}
			else if(data.weapon.aiming)
			{
				if (ENTITY::DOES_ENTITY_EXIST(grand.aimingObject))
				{
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(grand.aimingObject, data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z, 0, 0, 0);
				}

				if (data.forwardSpeed >= 0.f)
				{
					if (ENTITY::DOES_ENTITY_EXIST(grand.followObject))
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(grand.followObject, (transform.Position.x + transform.Velocity.x * 1.25f), (transform.Position.y + transform.Velocity.y * 1.25f), (transform.Position.z + transform.Velocity.z * 1.25f), 0, 0, 0);
					}

					if (data.forwardSpeed < 2.f && data.forwardSpeed > .0f && data.weapon.aimMoveSpeed != 1)
					{
						data.weapon.aimMoveSpeed = 1;

						BRAIN::TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY(game.entity, grand.followObject, grand.aimingObject, 1.f, false, 0.5f, 1082130432, true, false, FiringPatternFullAuto);
						BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(game.entity, /*data.forwardSpeed*/0.25f);
					}
					else if (data.forwardSpeed > 2.f && data.forwardSpeed <= 5.2f && data.weapon.aimMoveSpeed != 2)
					{
						data.weapon.aimMoveSpeed = 2;

						BRAIN::TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY(game.entity, grand.followObject, grand.aimingObject, 2.f, false, 0.5f, 1082130432, true, false, FiringPatternFullAuto);
						BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(game.entity, /*data.forwardSpeed*/0.25f);
					}
					else if (data.forwardSpeed > 5.2f && data.weapon.aimMoveSpeed != 5)
					{
						data.weapon.aimMoveSpeed = 5;

						BRAIN::TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY(game.entity, grand.followObject, grand.aimingObject, 5.f, false, 0.5f, 1082130432, true, false, FiringPatternFullAuto);
						BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(game.entity, /*data.forwardSpeed*/0.25f);
					}
					else if (data.forwardSpeed < 1.0f && data.weapon.aimMoveSpeed != 0)
					{
						data.weapon.aimMoveSpeed = 0;

						BRAIN::TASK_AIM_GUN_AT_ENTITY(game.entity, grand.aimingObject, -1, false);
					}


					//if (!BRAIN::GET_IS_TASK_ACTIVE(game.entity, 290) || data.weapon.aimMoveSpeed != data.forwardSpeed) // Aiming
					//{
					//	
					//	// The UGLY way
					//	BRAIN::TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY(game.entity, game.followObject, game.aimingObject, data.weapon.aimMoveSpeed, false, 0.5f, 1082130432, true, false, FiringPatternFullAuto);
					//	BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(game.entity, /*data.forwardSpeed*/0.25f);
					//
					//	// This method would be better to use but its too jittery 
					//	//BRAIN::TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD(game.entity, (transform.Position.x + transform.Velocity.x * 1.25f), (transform.Position.y + transform.Velocity.y * 1.25f), (transform.Position.z + transform.Velocity.z * 1.25f), data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z, data.forwardSpeed, false, 3.0f, 1082130432, true, false, 0, FiringPatternFullAuto);
					//}
				}
				else if (data.forwardSpeed == 0)
				{
					if (!BRAIN::GET_IS_TASK_ACTIVE(game.entity, 290))
					{
						// The UGLY way
						BRAIN::TASK_AIM_GUN_AT_ENTITY(game.entity, grand.aimingObject, -1, false);

						// This method would be better to use but its too jittery
						//BRAIN::TASK_AIM_GUN_AT_COORD(game.entity, data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z, -1, false, false);
					}
					/*
					if (!BRAIN::GET_IS_TASK_ACTIVE(game.entity, 290))
					{
						switch (data.weapon.hash)
						{
						case 615608432:
						case 741814745:
						case 1233104067:
						case 126349499:
						case 600439132:
						case -1420407917:
						case -1169823560:
						case -37975472:
						case -1813897027:
						case -1600701090:
						{
							if (!ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, "weapons@projectile@", "aimlive_m", 3))
							{
								if (!STREAMING::HAS_ANIM_DICT_LOADED("weapons@projectile@")) //weapons@projectile@
									STREAMING::REQUEST_ANIM_DICT("weapons@projectile@");

								BRAIN::TASK_PLAY_ANIM(game.entity, "weapons@projectile@", "aimlive_m", 8.0f, 10.0f, -1, 0, -8.0f, 1, 1, 1);
							}
						}
						break;
						default:
							break;
						}
					}
					*/
				}
			}
		}
	}
	else
	{
		// Drive By / In vehicle related anims.
	}
}

void CPlayer::UpdateTargetData()
{
	if (game.created && Exists())
	{
		if (WEAPON::GET_SELECTED_PED_WEAPON(game.entity) != data.weapon.hash)
		{
			WEAPON::REMOVE_ALL_PED_WEAPONS(game.entity, true);
			WEAPON::GIVE_WEAPON_TO_PED(game.entity, data.weapon.hash, 999, true, true);
		}

		unsigned long CurrentTime = timeGetTime();
		if (data.weapon.reloading && CurrentTime >= data.weapon.lastReload + 2000)
		{
			WEAPON::MAKE_PED_RELOAD(game.entity);

			data.weapon.lastReload = CurrentTime;
			data.weapon.reloading = false;
		}

		if (PED::IS_PED_DEAD_OR_DYING(game.entity, TRUE) && ENTITY::GET_ENTITY_HEALTH(game.entity) <= 0)
		{
			PED::RESURRECT_PED(game.entity);
			PED::CLEAR_PED_BLOOD_DAMAGE(game.entity);
			BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(game.entity);
		}

		if (data.vehicle.entityID > -1 && !GTAV::GamePed::GetVehicle(game.entity))
		{
			BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(game.entity);
			for (unsigned int v = 0; v < g_Vehicles.size(); v++)
			{
				if (g_Vehicles[v].GetID() == data.vehicle.entityID)
				{
					if (ENTITY::DOES_ENTITY_EXIST(g_Vehicles[v].GetEntity()))
						PED::SET_PED_INTO_VEHICLE(game.entity, g_Vehicles[v].GetEntity(), data.vehicle.seat - 1);

					break;
				}
			}
		}
		else if (GTAV::GamePed::GetVehicle(game.entity) && data.vehicle.entityID == -1)
		{
			Vehicle veh = GTAV::GamePed::GetVehicle(game.entity);

			if (ENTITY::DOES_ENTITY_EXIST(veh))
				BRAIN::TASK_LEAVE_VEHICLE(game.entity, veh, 16);
		}

		if (!BRAIN::GET_IS_TASK_ACTIVE(game.entity, data.task))
		{
			if (data.task == 47)
			{
				BRAIN::TASK_CLIMB_LADDER(game.entity, true);
			}
			else if (data.task == 51)
			{
				BRAIN::SET_PED_PATH_CAN_DROP_FROM_HEIGHT(game.entity, true);
			}
			else if (data.task == 161)  //Start Getting In Vehicle
			{
				int index = GTAV::GamePed::GetVehicleIndex(game.entity);
				if (index)
				{
					if (ENTITY::DOES_ENTITY_EXIST(g_Vehicles[index].GetEntity()))
					{
						BRAIN::TASK_OPEN_VEHICLE_DOOR(game.entity, g_Vehicles[index].GetEntity(), 1000, data.vehicle.seat - 1, 2.0f);
					}
				}
			}
			else if (data.task == 199)
			{
#ifndef _LOCALTESTING_
				BRAIN::TASK_VEHICLE_AIM_AT_COORD(game.entity, data.weapon.aimPosition.x, data.weapon.aimPosition.y, data.weapon.aimPosition.z);
#endif
			}
			else if (data.task == 298) //ReloadWeapon
			{
				WEAPON::MAKE_PED_RELOAD(game.entity);
				BRAIN::TASK_RELOAD_WEAPON(game.entity, true);
			}
			else if (data.task == 300) //GetInCover
			{
				if (!PED::IS_PED_IN_COVER(game.entity, 0))
				{
					//BRAIN::TASK_SEEK_COVER_TO_COORDS(playerData[i].pedPed, playerData[i].x, playerData[i].y, playerData[i].z, playerData[i].x, playerData[i].y, playerData[i].z, 0, 0);
					BRAIN::TASK_PUT_PED_DIRECTLY_INTO_COVER(game.entity, transform.Position.x, transform.Position.y, transform.Position.z, -1, 0, 0, 0, 0, 0, 0);
					BRAIN::TASK_STAY_IN_COVER(game.entity);
				}
			}
			else if (data.task == 301) //ExitCover
			{
				BRAIN::TASK_EXIT_COVER(game.entity, 1000, 0, 0, 0);
			}
			else if (data.task == 309) //InCover
			{
				if (!PED::IS_PED_IN_COVER(game.entity, 0))
				{
					BRAIN::TASK_PUT_PED_DIRECTLY_INTO_COVER(game.entity, transform.Position.x, transform.Position.y, transform.Position.z, -1, 0, 0, 0, 0, 0, 0);
					BRAIN::TASK_STAY_IN_COVER(game.entity);
				}
			}
			else if (data.task == 407) //Ragdoll
			{
				PED::SET_PED_TO_RAGDOLL(game.entity, 10000, 10000 * 1000, 0, 0, 0, 0);
			}
			else if (data.task == 420) //Climb/Vault
			{
				BRAIN::TASK_CLIMB(game.entity, true);
			}
			else if (data.task == 421 && !data.jumping) //jump
			{
				if (!PED::IS_PED_JUMPING(game.entity))
				{
					BRAIN::TASK_JUMP(game.entity, false, false, false);
					data.jumping = true;
				}
			}
			else if (data.task == 422) //Fall
			{

			}
			else // Default: Typically when the character is doing movement stuffs
			{
				if(data.task != 421)
					data.jumping = false;
			}
		}
	}


}

bool CPlayer::IsTargetAnimal()
{
	switch (data.model.hash)
	{
	case -832573324:
	case 1462895032:
	case -1430839454:
	case 51016938:
	case 1126154828:
	case 1457690978:
	case -50684386:
	case 1682622302:
	case 402729631:
	case -664053099:
	case -1950698411:
	case 802685111:
	case 1794449327:
	case 1193010354:
	case 1318032802:
	case -1920284487:
	case -1323586730:
	case 111281960:
	case 1125994524:
	case 1832265812:
	case -1384627013:
	case -541762431:
	case -1011537562:
	case 882848737:
	case -1788665315:
	case -745300483:
	case 1015224100:
	case 113504370:
	case -1589092019:
		return true;
	default:
		return false;
	}
}

void CPlayer::UpdatePlayerModel()
{
	if (game.created && Exists())
	{
		if (STREAMING::IS_MODEL_IN_CDIMAGE(data.model.hash) && STREAMING::IS_MODEL_VALID(data.model.hash))
		{
			grand.blip.Remove();

			ENTITY::DELETE_ENTITY(&game.entity);

			STREAMING::REQUEST_MODEL(data.model.hash);
			while (!STREAMING::HAS_MODEL_LOADED(data.model.hash))
				WAIT(0);

			game.entity = PED::CREATE_PED(26, data.model.hash, transform.Position.x, transform.Position.y, transform.Position.z, 0.0f, false, true);

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

			grand.blip.CreateBlip(game.entity);

			LOG_DEBUG("[CPlayer] Updated player entity %i model.", identity.entityId);
			return;
		}
		LOG_DEBUG("[CPlayer] Tried to update player %i model to %i, but model does not exist!", identity.entityId, data.model.hash);
	}
}

void CPlayer::SetModelComponent(const int componentid, const int drawableid, const int textureid, const int paletteid)
{
	data.model.components[componentid].drawableid = drawableid;
	data.model.components[componentid].textureid = textureid;
	data.model.components[componentid].paletteid = paletteid;

	if (game.created && Exists())
		GTAV::GamePed::SetPedComponentVariation(game.entity, componentid, drawableid, textureid, paletteid);
}

void CPlayer::SetModelHeadBlend(const int shapeFirst, const int shapeSecond, const int shapeThird, const int skinFirst, const int skinSecond, const int skinThird, const float shapeMix, const float skinMix, const float thirdMix)
{
	data.model.headBlend.shapeFirst = shapeFirst;
	data.model.headBlend.shapeSecond = shapeSecond;
	data.model.headBlend.shapeThird = shapeThird;
	data.model.headBlend.skinFirst = skinFirst;
	data.model.headBlend.skinSecond = skinSecond;
	data.model.headBlend.skinThird = skinThird;
	data.model.headBlend.shapeMix = shapeMix;
	data.model.headBlend.skinMix = skinMix;
	data.model.headBlend.thirdMix = thirdMix;

	if (game.created && Exists())
		GTAV::GamePed::SetPedHeadBlend(game.entity, shapeFirst, shapeSecond, shapeThird, skinFirst, skinSecond, skinThird, shapeMix, skinMix, thirdMix);
}

void CPlayer::SetModelHeadOverlay(const int overlayid, const int index, const int colorType, const int colorid, const int secondColorid, const float opacity)
{
	data.model.headOverlays[overlayid].index = index;
	data.model.headOverlays[overlayid].colorType = colorType;
	data.model.headOverlays[overlayid].colorID = colorid;
	data.model.headOverlays[overlayid].secondColorID = secondColorid;
	data.model.headOverlays[overlayid].opacity = opacity;

	if (game.created && Exists())
		GTAV::GamePed::SetPedHeadOverlayColor(game.entity, overlayid, index, colorType, colorid, secondColorid, opacity);
}

void CPlayer::SetModelProp(const int componentid, const int drawableid, const int textureid)
{
	data.model.props[componentid].drawableid = drawableid;
	data.model.props[componentid].textureid = textureid;

	if (game.created && Exists())
		GTAV::GamePed::SetPedProp(game.entity, componentid, drawableid, textureid);
}

void CPlayer::SetModelFaceFeature(const int index, const float scale)
{
	data.model.faceFeatures[index].scale = scale;

	if (game.created && Exists())
		GTAV::GamePed::SetPedFaceFeature(game.entity, index, scale);
}

void CPlayer::TaskShoot(const std::string weapon, const int ammo, const CVector3 aim)
{
	if (game.created && Exists())
	{
		int wepClass = GTAV::Weapon::GetWeaponClass(data.weapon.hash);
		//if (data.weapon.melee.inMeleeCombat)
		if (wepClass == 0 || wepClass == 1 || wepClass == 2)
		{
			//if (!data.weapon.melee.swingFinished)
				//return;

			/*
				This is a basic Melee System at this current time untill after 1.0 at which i'll put more detail into melee sync.
			*/

			std::string ourDic;
			std::string ourAnim;
			
			if (wepClass == 0)
			{
				ourDic = "melee@unarmed@streamed_core";
				ourAnim = "heavy_punch_a";
				data.weapon.melee.animationEnd = .9f;
			}
			else if (wepClass == 1)
			{
				ourDic = "melee@small_wpn@streamed_core";
				ourAnim = "small_melee_wpn_short_range_0";
				data.weapon.melee.animationEnd = .3f;
			}
			else if (wepClass == 2)
			{
				ourDic = "melee@large_wpn@streamed_core";
				ourAnim = "short_0_attack";
				data.weapon.melee.animationEnd = .55f;
			}
			
			if (data.weapon.hash == eWeapon::WeaponKnife || data.weapon.hash == eWeapon::WeaponSwitchBlade || data.weapon.hash == eWeapon::WeaponDagger)
			{
				ourDic = "melee@knife@streamed_core_fps";
				ourAnim = "knife_short_range_0";
				data.weapon.melee.animationEnd = .9f;
			}

			// Check for movement anim playing and end it
			if (!data.onFootAnim.animation.empty() && ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), 3))
			{
				BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.onFootAnim.dictionary.c_str(), (char*)data.onFootAnim.animation.c_str(), -4);
			}

			// Check for existing Melee anim playing and end it
			if (!data.weapon.melee.meleeAnim.empty() && ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)data.weapon.melee.meleeDic.c_str(), (char*)data.weapon.melee.meleeAnim.c_str(), 3))
			{
				BRAIN::STOP_ANIM_TASK(game.entity, (char*)data.weapon.melee.meleeDic.c_str(), (char*)data.weapon.melee.meleeAnim.c_str(), -4);
			}

			if (!ENTITY::IS_ENTITY_PLAYING_ANIM(game.entity, (char*)data.weapon.melee.meleeDic.c_str(), (char*)data.weapon.melee.meleeAnim.c_str(), 3))
			{
				if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)ourDic.c_str()))
					STREAMING::REQUEST_ANIM_DICT((char*)ourDic.c_str());

				data.weapon.melee.meleeDic = ourDic;
				data.weapon.melee.meleeAnim = ourAnim;

				BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(game.entity);
				BRAIN::TASK_PLAY_ANIM(game.entity, (char*)data.weapon.melee.meleeDic.c_str(), (char*)data.weapon.melee.meleeAnim.c_str(), 8.0f, 10.0f, -1, 16/*flag*/, -8.0f, 1, 1, 1);

				data.weapon.melee.swingFinished = false;
			}
		}
		else
		{
			//BRAIN::TASK_SHOOT_AT_COORD(game.entity, aim.x, aim.y, aim.z, 2500, FiringPatternSingleShot);
			PED::SET_PED_SHOOTS_AT_COORD(game.entity, aim.x, aim.y, aim.z, true);
		}

		data.weapon.justShot = true;
	}


	//GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coord.fX, coord.fY, coord.fZ + 1.8f, coord2.fX, coord2.fY, coord2.fZ, 0, true, weaponid, playerData[playerid].pedPed, true, false, 999);
}

void CPlayer::ShowNametag()
{
	if (!game.created || !Exists())
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
		// 20 / Distance
		// Text Nametag
		if (tag.textVisible)
			CVisual::TextDraw((char*)tag.text.c_str(), sX, (sY - 0.05f), 0.4f, 2, tag.textColor.Red, tag.textColor.Green, tag.textColor.Blue, tag.textColor.Alpha, true, false, true);

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
}

void CPlayer::SetArmour(const int armour)
{
	data.armour = armour;

	if (game.created && Exists())
	{
		PED::SET_PED_ARMOUR(game.entity, armour);
	}
}

void CPlayer::Revive()
{
	health.Set(200);

	PED::REVIVE_INJURED_PED(game.entity);
	PED::RESURRECT_PED(game.entity);
	PLAYER::RESET_PLAYER_ARREST_STATE(game.entity);
	ENTITY::SET_ENTITY_HEALTH(game.entity, health.Get());
	PED::RESET_PED_VISIBLE_DAMAGE(game.entity);
}
