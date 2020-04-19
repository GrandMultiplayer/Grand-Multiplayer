#include <vector>

// RakNet
#include <BitStream.h>
#include <PacketPriority.h>
#include <RakNetTypes.h>
#include <MessageIdentifiers.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "CVector3.h"
#include "CMaths.h"
#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CObject.h"

CObject::CObject(const int entity, const int hash, CVector3 position, CVector3 rotation, const bool dynamic)
{
	if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash) || !STREAMING::IS_MODEL_VALID(hash))
	{
		LOG_DEBUG("[CObject] Object entity %i has invalid model %i.", identity.entityId, hash);
		return;
	}

	//CEntity
	game.entity = NULL;
	game.created = false;
	game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	game.vehicle = -1;

	identity.entityId = entity;
	identity.type = eObject;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;

	//CObject
	data.model.hash = hash;
	data.model.dynamic = dynamic;
	transform.Position = position;
	transform.Rotation = rotation;

	data.model.textureIndex = 0;

	network.lastSyncSent = timeGetTime();
	network.synchronized = true;

	STREAMING::REQUEST_MODEL(data.model.hash);

	LOG_DEBUG("[CObject] Added Object entity %i.", identity.entityId);
}

CObject::~CObject()
{
	identity.entityId = -1;
	identity.type = Type::eNill;
}

CObject::CObject(const CObject & e)
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

	//CObject
	data = e.data;
	game = e.game;
	network = e.network;
	interpolation = e.interpolation;
}

CObject & CObject::operator=(const CObject &e)
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

	//CObject
	data = e.data;
	game = e.game;
	network = e.network;
	interpolation = e.interpolation;

	return (*this);
}

bool CObject::CreateObject()
{
	if (game.created)
	{
		LOG_DEBUG("[CObject] Object entity %i already created.", identity.entityId);
		return false;
	}

	if (STREAMING::IS_MODEL_IN_CDIMAGE(data.model.hash) && STREAMING::IS_MODEL_VALID(data.model.hash))
	{
		LOG_DEBUG("[CObject] Creating Object entity %i.", identity.entityId);

		STREAMING::REQUEST_MODEL(data.model.hash);

		if (!STREAMING::HAS_MODEL_LOADED(data.model.hash))
			return false;

		game.entity = OBJECT::CREATE_OBJECT_NO_OFFSET(data.model.hash, transform.Position.x, transform.Position.y, transform.Position.z, false, true, data.model.dynamic);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(data.model.hash);

		blip.CreateBlip(game.entity);

		ENTITY::SET_ENTITY_ROTATION(game.entity, transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, 0, true);

		if (!data.model.dynamic)
			ENTITY::FREEZE_ENTITY_POSITION(game.entity, true);

		game.created = true;

		SetTargetData();

		LOG_DEBUG("[CObject] Created Object entity %i.", identity.entityId);
		return true;
	}

	LOG_WARNING("[CObject] Tried to create %i, but model %s does not exist!", identity.entityId, data.model.hash);
	return false;
}

void CObject::Destroy()
{
	LOG_DEBUG("[CObject] Removing Object entity %i.", identity.entityId);

	if (game.created)
	{
		blip.Remove();

		if (ENTITY::DOES_ENTITY_EXIST(game.entity))
			ENTITY::DELETE_ENTITY(&game.entity);

		game.created = false;
	}

	identity.entityId = -1;
}

void CObject::Delete()
{
	if (game.created)
	{
		blip.Remove();

		if (ENTITY::DOES_ENTITY_EXIST(game.entity))
			ENTITY::DELETE_ENTITY(&game.entity);

		game.created = false;
	}

	game.entity = NULL;

	if (CNetworkManager::GetInterface()->GetMyGUID() == game.physicsAssignee)
	{
		RakNet::BitStream bitstream;
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(RakNet::UNASSIGNED_RAKNET_GUID);
		CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
	}
}

void CObject::Pulse()
{
	if (game.created && identity.entityId != -1 && data.model.dynamic)
	{
		if (CNetworkManager::GetInterface()->GetMyGUID() != game.physicsAssignee)
		{
			Interpolate();
		}
		else if (CNetworkManager::GetInterface()->GetMyGUID() == game.physicsAssignee)
		{
			if (timeGetTime() > network.lastSyncSent + (1000.0f / 50))
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

				CNetworkManager::GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, CNetworkManager::GetSystemAddress(), false);

				network.lastSyncSent = timeGetTime();
			}
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

	network.lastSyncReceived = timeGetTime();
}

void CObject::Interpolate()
{
	SetTargetPosition();
	SetTargetRotation();
	SetTargetData();
}

void CObject::UpdateTargetPosition()
{
	if (game.created)
	{
		unsigned long CurrentTime = timeGetTime();
		unsigned int InterpolationTime = CurrentTime - network.lastSyncReceived;

		// Get our position
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
		CVector3 CurrentPosition(Coordinates.x, Coordinates.y, Coordinates.z);

		// Set the target position
		CVector3 TargetPosition(transform.Position.x, transform.Position.y, transform.Position.z);
		interpolation.position.target = TargetPosition;

		// Calculate the relative error
		interpolation.position.error = TargetPosition - CurrentPosition;

		// Apply the error over 400ms (i.e. 1/4 per 100ms)
		interpolation.position.error *= Math::Lerp<const float>(0.25f, Math::UnlerpClamped(100, InterpolationTime, 400), 1.0f);

		// Get the interpolation interval
		interpolation.position.startTime = CurrentTime;
		interpolation.position.finishTime = (CurrentTime + InterpolationTime);

		// Initialize the interpolation
		interpolation.position.lastAlpha = 0.0f;
	}
}

void CObject::SetTargetPosition()
{
	if (interpolation.position.finishTime != 0 && game.created)
	{
		// Get our position
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
		CVector3 vecCurrentPosition = { Coordinates.x, Coordinates.y, Coordinates.z };

		// Get the factor of time spent from the interpolation start to the current time.
		unsigned long CurrentTime = timeGetTime();
		float fAlpha = Math::Unlerp(interpolation.position.startTime, CurrentTime, interpolation.position.finishTime);

		// Don't let it overcompensate the error
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.5f);

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
		if (CVector3::Distance(vecCurrentPosition, interpolation.position.target) > 750.0f)
		{
			// Abort all interpolation
			interpolation.position.finishTime = 0;
			vecNewPosition = interpolation.position.target;
		}

		// Set our new position
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, vecNewPosition.x, vecNewPosition.y, vecNewPosition.z, false, false, false);
		ENTITY::SET_ENTITY_VELOCITY(game.entity, transform.Velocity.x, transform.Velocity.y, transform.Velocity.z);
		ENTITY::SET_ENTITY_ROTATION(game.entity, transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, 2, true);
	}
}

void CObject::UpdateTargetRotation()
{
	if (game.created)
	{
		unsigned int interpolationtime = timeGetTime() - network.lastSyncReceived;
		unsigned long CurrentTime = timeGetTime();

		// Get our rotation
		Vector3 CurrentRotationVec = ENTITY::GET_ENTITY_ROTATION(game.entity, 2);
		CVector3 CurrentRotation(CurrentRotationVec.x, CurrentRotationVec.y, CurrentRotationVec.z);

		// Set the target rotation
		CVector3 TargetRotation = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
		interpolation.rotation.target = TargetRotation;

		// Get the error
		interpolation.rotation.error.x = Math::GetOffsetDegrees(CurrentRotation.x, TargetRotation.x);
		interpolation.rotation.error.y = Math::GetOffsetDegrees(CurrentRotation.y, TargetRotation.y);
		interpolation.rotation.error.z = Math::GetOffsetDegrees(CurrentRotation.z, TargetRotation.z);
		//interpolation.rotation.error = TargetRotation - CurrentRotation;
		interpolation.rotation.error *= Math::Lerp < const float >(0.40f, Math::UnlerpClamped(100, interpolationtime, 400), 1.0f);

		// Get the interpolation interval
		interpolation.rotation.startTime = CurrentTime;
		interpolation.rotation.finishTime = (CurrentTime + interpolationtime);

		// Initialize the interpolation
		interpolation.rotation.lastAlpha = 0.0f;
	}
}

void CObject::SetTargetRotation()
{
	if (game.created)
	{
		if (interpolation.rotation.finishTime != 0)
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

			// Calculate the new position
			CVector3 vecNewRotation = CurrentRotation + vecCompensation;

			// Set our new position
			ENTITY::SET_ENTITY_ROTATION(game.entity, vecNewRotation.x, vecNewRotation.y, vecNewRotation.z, 2, true);
		}
	}
}

void CObject::SetTargetData()
{
	if (game.created)
	{
		OBJECT::_SET_OBJECT_TEXTURE_VARIANT(game.entity, data.model.textureIndex);
	}
}

void CObject::SetTextureVariation(const int textureIndex)
{
	data.model.textureIndex = textureIndex;

	if (game.created)
		OBJECT::_SET_OBJECT_TEXTURE_VARIANT(game.entity, textureIndex);
}