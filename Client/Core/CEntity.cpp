#include <vector>

// RakNet
#include <BitStream.h>
#include <PacketPriority.h>
#include <MessageIdentifiers.h>

#include <SharedUtility.h>
#include "CVector3.h"
#include "CMaths.h"

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

// GrandM
#include "CVisual.h"
#include "CEntity.h"
#include "GamePed.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"
#include "CCheckpoint.h"
#include "CPickup.h"
#include "CBlip.h"
#include "CLocalPlayer.h"
#include "CStreamer.h"
#include "CNetworkManager.h"

#include "./Ped.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

const bool CEntity::operator==(CEntity &e)
{
	return (identity.entityId == e.GetID());
}

const bool CEntity::operator!=(CEntity &e)
{
	return (identity.entityId != e.GetID());
}

bool CEntity::IsNull()
{
	return (identity.entityId == -1);
}

void CEntity::SetPosition(const CVector3 position)
{
	transform.Position = position;

	switch (identity.type)
	{
	case Type::ePlayer:
		break;
	case Type::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i] == *this && g_Vehicles[i].IsCreated())
			{
				bool inVeh = false;
				int seat = -1;

				if (CLocalPlayer::GetVehicleID() == g_Vehicles[i].GetID())
				{
					CVector3 pos;
					if (CLocalPlayer::IsScriptedCameraActive())
					{
						Vector3 camPos = CAM::GET_CAM_COORD(CLocalPlayer::GetScriptedCamera());
						pos = { camPos.x, camPos.y, camPos.z };
					}

					seat = CLocalPlayer::GetSeat();
					CStreamer::StreamVehiclesIn(position, CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
					inVeh = true;
				}

				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(g_Vehicles[i].GetEntity(), position.x, position.y, position.z, false, false, false);

				if (inVeh)
					GTAV::GamePed::PutPedInVehicle(CLocalPlayer::GetPed(), g_Vehicles[i].GetEntity(), seat - 1);

				break;
			}
		}
		break;
	case Type::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i] == *this)
			{
				g_Objects[i].SetPosition(position);

				if (g_Objects[i].IsCreated())
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(g_Objects[i].GetEntity(), position.x, position.y, position.z, false, false, false);
				break;
			}
		}
		break;
	case Type::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i] == *this)
			{
				g_Npcs[i].SetPosition(position);

				if (g_Npcs[i].IsCreated())
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(g_Npcs[i].GetEntity(), position.x, position.y, position.z, false, false, false);
				break;
			}
		}
		break;
	case Type::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i] == *this)
			{
				g_Checkpoints[i].SetPosition(position);
				break;
			}
		}
		break;
	case Type::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i] == *this)
			{
				g_Blips[i].SetPosition(position);
				break;
			}
		}
		break;
	case Type::ePickup:
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			if (g_Pickups[i] == *this)
			{
				g_Pickups[i].SetPosition(position);
				break;
			}
		}
		break;
	default:
		LOG_DEBUG("[CServerEntity::SetPosition] Invalid entity.");
		return;
		break;
	}
}

void CEntity::SetRotation(const CVector3 rotation)
{
	transform.Rotation = rotation;
	switch (identity.type)
	{
	case Type::eVehicle:
		for (int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i] == *this)
			{
				g_Vehicles[i].SetRotation(rotation);

				if (g_Vehicles[i].IsCreated())
					ENTITY::SET_ENTITY_ROTATION(g_Vehicles[i].GetEntity(), rotation.x, rotation.y, rotation.z, 2, false);
				return;
			}
		}
		break;
	case Type::eObject:
		for (int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i] == *this)
			{
				g_Objects[i].SetRotation(rotation);

				if (g_Objects[i].IsCreated())
					ENTITY::SET_ENTITY_ROTATION(g_Objects[i].GetEntity(), rotation.x, rotation.y, rotation.z, 2, true);
				return;
			}
		}
		break;
	case Type::eNPC:
		for (int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i] == *this)
			{
				g_Npcs[i].SetRotation(rotation);

				if (g_Npcs[i].IsCreated())
					ENTITY::SET_ENTITY_ROTATION(g_Objects[i].GetEntity(), rotation.x, rotation.y, rotation.z, 2, true);
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("[CServerEntity::SetRotation] Invalid entity.");
		return;
		break;
	}
}

void CEntity::SetDimension(const int dim)
{
	world.dim = dim;
	//NetworkSync::SyncEntities(Network.guid, world.dim);
}

CEntity* CEntity::GetLastAttacker()
{
	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetID() == lastAttacker)
		{
			return &g_Players[i];
		}
	}

	return nullptr;
}

CEntity* CEntity::GetEntityFromID(const int id)
{
	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == id)
		{
			return &g_Vehicles[i];
		}
	}

	for (unsigned int i = 0; i < g_Npcs.size(); i++)
	{
		if (g_Npcs[i].GetID() == id)
		{
			return &g_Npcs[i];
		}
	}

	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetID() == id)
		{
			return &g_Players[i];
		}
	}

	for (unsigned int i = 0; i < g_Objects.size(); i++)
	{
		if (g_Objects[i].GetID() == id)
		{
			return &g_Objects[i];
		}
	}

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
	{
		if (g_Checkpoints[i].GetID() == id)
		{
			return &g_Checkpoints[i];
		}
	}

	for (unsigned int i = 0; i < g_Pickups.size(); i++)
	{
		if (g_Pickups[i].GetID() == id)
		{
			return &g_Pickups[i];
		}
	}

	for (unsigned int i = 0; i < g_Blips.size(); i++)
	{
		if (g_Blips[i].GetID() == id)
		{
			return &g_Blips[i];
		}
	}

	return nullptr;
}

void CEntity::UpdateMovement(RakNet::Packet *packet)
{
	int id = -1, type = -1;

	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);
	bitstream.Read(type);
	bitstream.Read(id);

	bitstream.Read(transform.Position.x);
	bitstream.Read(transform.Position.y);
	bitstream.Read(transform.Position.z);

	bitstream.Read(transform.Rotation.x);
	bitstream.Read(transform.Rotation.y);
	bitstream.Read(transform.Rotation.z);

	bitstream.Read(transform.Velocity.x);
	bitstream.Read(transform.Velocity.y);
	bitstream.Read(transform.Velocity.z);

	bitstream.Read(forwardSpeed);
	bitstream.Reset();

	packet->deleteData;

	sync.lastSyncReceived = getticktime();

	//Interpolate();

	UpdateTargetPosition();
	UpdateTargetRotation();
}

void CEntity::PulseMovement()
{
	if (!game.created)
		return;

	if (identity.type == Type::ePlayer)
		return;

	if (identity.type == Type::eVehicle)
	{
		const int t_CurrentVehicle = CLocalPlayer::GetVehicleID();
		
		if (CNetworkManager::GetInterface()->GetMyGUID() != game.physicsAssignee || (t_CurrentVehicle != identity.entityId && CLocalPlayer::GetSeat() == 0))
			return;
	}
	
	uint64_t now = getticktime();
	if ((sync.lastMovementSync + (1000 / /*SYNCRATE*/10)) <= now && identity.entityId != -1 && Exists())
	{
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
		Vector3 Rotation = ENTITY::GET_ENTITY_ROTATION(game.entity, 2);
		Vector3 Velocity = ENTITY::GET_ENTITY_VELOCITY(game.entity);

		forwardSpeed = ENTITY::GET_ENTITY_SPEED(game.entity);
		transform.Position = interpolation.position.target = CVector3(Coordinates.x, Coordinates.y, Coordinates.z);
		transform.Rotation = CVector3(Rotation.x, Rotation.y, Rotation.z);
		transform.Velocity = CVector3(Velocity.x, Velocity.y, Velocity.z);

		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)ID_PACKET_MOVEMENT);

		bitstream.Write(identity.type);
		bitstream.Write(identity.entityId);

		bitstream.Write(transform.Position.x);
		bitstream.Write(transform.Position.y);
		bitstream.Write(transform.Position.z);

		bitstream.Write(transform.Rotation.x);
		bitstream.Write(transform.Rotation.y);
		bitstream.Write(transform.Rotation.z);

		bitstream.Write(transform.Velocity.x);
		bitstream.Write(transform.Velocity.y);
		bitstream.Write(transform.Velocity.z);

		bitstream.Write(forwardSpeed);

		CNetworkManager::GetInterface()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, CNetworkManager::GetSystemAddress(), false);

		sync.lastMovementSync = now;
	}
	
	DebugUI();
}

const bool CEntity::Exists()
{
	return ENTITY::DOES_ENTITY_EXIST(game.entity);
}

void CEntity::SetHealth(const int hp, RakNet::RakNetGUID sender) 
{ 
	health.Set(hp, sender, identity.entityId, identity.type);
	
	if (game.created && Exists())
	{
		ENTITY::SET_ENTITY_HEALTH(game.entity, health.Get());
	}
}

void CEntity::Interpolate() 
{
	if (!game.created)
		return;

	if (identity.type == Type::eVehicle && game.physicsAssignee == CNetworkManager::GetInterface()->GetMyGUID())
		return;

	SetTargetPosition();
	SetTargetRotation();
}

void CEntity::UpdateTargetPosition()
{
	if (game.created && Exists())
	{
		unsigned long CurrentTime = timeGetTime();
		unsigned int InterpolationTime = CurrentTime - sync.lastSyncReceived;

		// Get our position
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
		CVector3 CurrentPosition = { Coordinates.x, Coordinates.y, Coordinates.z };

		// Set the target position
		CVector3 TargetPosition = transform.Position;
		interpolation.position.target = TargetPosition;

		// Calculate the relative error
		interpolation.position.error = TargetPosition - CurrentPosition;

		// Get the interpolation interval
		interpolation.position.startTime = CurrentTime;
		interpolation.position.finishTime = (CurrentTime + InterpolationTime);

		// Initialize the interpolation
		interpolation.position.lastAlpha = 0.0f;
	}
}

void CEntity::SetTargetPosition()
{
	bool playerVehicleCheck = true;
	if (identity.type == Type::ePlayer || identity.type == Type::eNPC)
		if (game.vehicle != -1)
			playerVehicleCheck= false;

	if (interpolation.position.finishTime != 0 && game.created && Exists() && playerVehicleCheck)
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
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, vecNewPosition.x, vecNewPosition.y, vecNewPosition.z, false, false, false);
		ENTITY::SET_ENTITY_VELOCITY(game.entity, transform.Velocity.x, transform.Velocity.y, transform.Velocity.z);
	}
}

void CEntity::UpdateTargetRotation()
{
	if (game.created && Exists())
	{
		unsigned long CurrentTime = timeGetTime();
		unsigned int interpolationtime = CurrentTime - sync.lastSyncReceived;

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
		//InterpolationData.Rotation.Error = TargetRotation - CurrentRotation;
		interpolation.rotation.error *= Math::Lerp < const float >(0.40f, Math::UnlerpClamped(100, interpolationtime, 400), 1.0f);

		// Get the interpolation interval
		interpolation.rotation.startTime = CurrentTime;
		interpolation.rotation.finishTime = (CurrentTime + interpolationtime);

		// Initialize the interpolation
		interpolation.rotation.lastAlpha = 0.0f;
	}
}

void CEntity::SetTargetRotation()
{
	bool playerVehicleCheck = true;
	if (identity.type == Type::ePlayer || identity.type == Type::eNPC)
		if (game.vehicle != -1)
			playerVehicleCheck = false;

	if (interpolation.rotation.finishTime != 0 && game.created && Exists() && playerVehicleCheck)
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
		ENTITY::SET_ENTITY_ROTATION(game.entity, vecNewRotation.x, vecNewRotation.y, vecNewRotation.z, 2, true);
	}
}

void CEntity::DebugUI()
{
	CVector3 localPos = CLocalPlayer::GetPosition();
	if (game.created && !ENTITY::IS_ENTITY_OCCLUDED(game.entity) && Math::GetDistanceBetweenPoints3D(transform.Position.x, transform.Position.y, transform.Position.z, localPos.x, localPos.y, localPos.z) < 10.0f)
	{
		float sX, sY;
		Vector3 tPos = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(game.entity, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(game.entity, "seat_dside_f"));
		GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(tPos.x, tPos.y, tPos.z + 0.3f, &sX, &sY);

		std::stringstream os;
		os << "~p~Position: ~w~" << transform.Position.x << ", " << transform.Position.y << ", " << transform.Position.z;

		CVisual::TextDraw(os.str().c_str(), sX, sY, 0.2f, 2, 255, 255, 255, 200, true, false, true);

		// Assignment Text
		GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(tPos.x, tPos.y, tPos.z + 1.0f, &sX, &sY);

		std::stringstream oss;
		oss << "~p~Your GUID: ~w~" << CNetworkManager::GetInterface()->GetMyGUID().ToString() << "~n~~p~Assigned To: ~w~" << game.physicsAssignee.ToString();

		CVisual::TextDraw(oss.str().c_str(), sX, sY, 0.2f, 2, 255, 255, 255, 200, true, false, true);
	}
}