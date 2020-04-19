// Self
#include "CVehicle.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

//GrandM
#include "SharedUtility.h"
#include "APIStructs.h"

#include "Server.h"
#include "CPlayer.h"
#include "CBlip.h"

// API
#include "CAPI.h"
#include "API_Callback_Entity.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

CVehicle::CVehicle(const uint32_t model, CVector3 position, CVector3 rotation)
{
	// Assign current count as entity id
	identity.entityId = CEntity::count;
	// Increment the count for the next entity id
	CEntity::count += 1;

	identity.type = GrandM::EntityType::Vehicle;

	for (unsigned int i = 0; i < SizeOfArray(data.occupants); i++)
	{
		data.occupants[i] = -1;
	}

	data.model.hash = model;
	transform.Position = position;
	transform.Rotation = rotation;
	data.heading = rotation.z;
}

CVehicle::~CVehicle() 
{
	identity.entityId = -1;
	identity.type = GrandM::Nill;
}

CVehicle::CVehicle(const CVehicle & e)
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

	//CVehicle
	data = e.data;
	blip = e.blip;
	network = e.network;
}

CVehicle & CVehicle::operator=(const CVehicle &e)
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

	//CVehicle
	data = e.data;
	blip = e.blip;
	network = e.network;

	return (*this);
}

void CVehicle::PulseMovement()
{
	uint64_t now = getticktime();

	if (sync.lastMovementSync + (1000 / Server::GetServer()->GetSyncRate()) <= now && identity.entityId != -1)
	{
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

		if (!g_Players.empty())
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetDimension() == world.dim)
				{
					if ((g_Players[i].IsScriptedCameraActive() && CVector3::Distance2D(g_Players[i].GetScriptCameraPosition(), transform.Position) <= 1000.0f)
						|| (CVector3::Distance2D(g_Players[i].GetPosition(), transform.Position) <= 1000.0f))
					{
						if(network.physicsAssignee != g_Players[i].GetGUID()) // Phys Assignment Check
							Server::GetServer()->GetNetworkManager()->GetInterface()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, g_Players[i].GetGUID(), false);
					}
				}
			}
		}

		sync.lastMovementSync = now;
	}
}

/*
void CVehicle::Pulse()
{
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)ID_PACKET_VEHICLE);

#ifndef LOCALTESTING
		bitstream.Write(identity.entityId);
#else
		if (Occupants[0] != -1)
			bitstream.Write(Identity.entityId + 1);
		else
			bitstream.Write(Identity.entityId);
#endif

		bitstream.Write(RakNet::RakString(data.model.name.c_str()));

		bitstream.Write(data.heading);

#ifndef LOCALTESTING
		bitstream.Write(transform.Position.x);
		bitstream.Write(transform.Position.y);
#else
		bitstream.Write(transform.Position.x + 4);
		bitstream.Write(transform.Position.y + 4);
#endif
		bitstream.Write(transform.Position.z);

		bitstream.Write(data.forwardSpeed);

		bitstream.Write(data.gear);
		bitstream.Write(data.rpm);

		bitstream.Write(data.clutch);
		bitstream.Write(data.turbo);
		bitstream.Write(data.acceleration);
		bitstream.Write(data.brake);

		bitstream.Write(data.dashSpeed);
		bitstream.Write(data.steeringAngle);
		bitstream.Write(data.forwardWheelAngle);

		bitstream.Write(transform.Velocity.x);
		bitstream.Write(transform.Velocity.y);
		bitstream.Write(transform.Velocity.z);

		bitstream.Write(transform.Rotation.x);
		bitstream.Write(transform.Rotation.y);
		bitstream.Write(transform.Rotation.z);

		bitstream.Write(data.engine.health);
		bitstream.Write(data.engine.fuelHealth);

		for (unsigned int i = 0; i < SizeOfArray(data.occupants); i++)
		{
			bitstream.Write(data.occupants[i]);
		}

		if (!g_Players.empty())
		{
			for (unsigned int p = 0; p < g_Players.size(); p++)
			{
				if (g_Players[p].GetDimension() == world.dim)
				{
					if ((g_Players[p].IsScriptedCameraActive() && CVector3::Distance2D(g_Players[p].GetScriptCameraPosition(), transform.Position) <= 1000.0f)
						|| (CVector3::Distance2D(g_Players[p].GetPosition(), transform.Position) <= 1000.0f))
					{
						Server::GetServer()->GetNetworkManager()->GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, g_Players[p].GetGUID(), false);
					}
				}
			}
		}

	// Dead Check
	if (health <= 0 && !dead)
	{
		dead = true;

		for (unsigned int i = 0; i < g_ApiModules.size(); i++)
		{
			void *Instance = g_ApiModules[i].GetInstance();
			API::Callback::Entity::OnEntityDeath(Instance, (CEntity)*this, GetLastAttacker());
		}
	}
}
*/

void CVehicle::PulseVehicleData() 
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_PACKET_VEHICLE);

#ifndef LOCALTESTING
	bitstream.Write(identity.entityId);
#else
	if (Occupants[0] != -1)
		bitstream.Write(Identity.entityId + 1);
	else
		bitstream.Write(Identity.entityId);
#endif

	bitstream.Write(data.gear);
	bitstream.Write(data.rpm);

	bitstream.Write(data.clutch);
	bitstream.Write(data.turbo);
	bitstream.Write(data.acceleration);
	bitstream.Write(data.brake);

	bitstream.Write(data.dashSpeed);
	bitstream.Write(data.steeringAngle);
	bitstream.Write(data.forwardWheelAngle);

	for (unsigned int i = 0; i < SizeOfArray(data.occupants); i++)
	{
		bitstream.Write(data.occupants[i]);
	}

	if (!g_Players.empty())
	{
		for (unsigned int p = 0; p < g_Players.size(); p++)
		{
			if (g_Players[p].GetDimension() == world.dim)
			{
				if ((g_Players[p].IsScriptedCameraActive() && CVector3::Distance2D(g_Players[p].GetScriptCameraPosition(), transform.Position) <= 1000.0f)
					|| (CVector3::Distance2D(g_Players[p].GetPosition(), transform.Position) <= 1000.0f))
				{
					Server::GetServer()->GetNetworkManager()->GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, g_Players[p].GetGUID(), false);
				}
			}
		}
	}

	// Dead Check //Move to health set.
	/*if (health <= 0 && !dead)
	{
		dead = true;

		for (unsigned int i = 0; i < g_ApiModules.size(); i++)
		{
			void *Instance = g_ApiModules[i].GetInstance();
			API::Callback::Entity::OnEntityDeath(Instance, (CEntity)*this, GetLastAttacker());
		}
	}*/
}

void CVehicle::UpdateVehicleData(RakNet::Packet *packet)
{
	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);

	bitstream.Read(identity.entityId);

	bitstream.Read(data.gear);
	bitstream.Read(data.rpm);

	bitstream.Read(data.clutch);
	bitstream.Read(data.turbo);
	bitstream.Read(data.acceleration);
	bitstream.Read(data.brake);

	bitstream.Read(data.dashSpeed);
	bitstream.Read(data.steeringAngle);
	bitstream.Read(data.forwardWheelAngle);
}

void CVehicle::SyncData(RakNet::RakNetGUID requester)
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

	// Assignment data
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(network.physicsAssignee);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	if (!data.model.Colors[0].custom)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(1);
		bitstream.Write(data.model.Colors[0].type);
		bitstream.Write(data.model.Colors[0].color);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetStandardColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}
	else
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(1);
		bitstream.Write(data.model.Colors[0].customCol.Red);
		bitstream.Write(data.model.Colors[0].customCol.Green);
		bitstream.Write(data.model.Colors[0].customCol.Blue);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCustomColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	if (!data.model.Colors[1].custom)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(2);
		bitstream.Write(data.model.Colors[1].type);
		bitstream.Write(data.model.Colors[1].color);

		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetStandardColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}
	else
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(2);
		bitstream.Write(data.model.Colors[1].customCol.Red);
		bitstream.Write(data.model.Colors[1].customCol.Green);
		bitstream.Write(data.model.Colors[1].customCol.Blue);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCustomColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(RakNet::RakString(data.plate.text.c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNumberPlate", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	for (int i = 0; i < SizeOfArray(data.model.Mods); i++)
	{
		if (data.model.Mods[i].Edited)
		{
			bitstream.Reset();
			bitstream.Write(identity.entityId);
			bitstream.Write(i);
			bitstream.Write(data.model.Mods[i].index);
			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetMod", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
		}
	}

	data.engine.engineState.Sync(requester, identity.entityId, identity.type);

	data.headlightState.Sync(requester, identity.entityId, identity.type);
	data.highbeamState.Sync(requester, identity.entityId, identity.type);
	
	data.roofState.Sync(requester, identity.entityId, identity.type);

	data.boostActive.Sync(requester, identity.entityId, identity.type);

	data.engine.health.Sync(requester, identity.entityId, identity.type);
	data.engine.fuelHealth.Sync(requester, identity.entityId, identity.type);

	data.taxiLight.Sync(requester, identity.entityId, identity.type);

	data.leftIndicator.Sync(requester, identity.entityId, identity.type);
	data.rightIndicator.Sync(requester, identity.entityId, identity.type);

	data.interiorLight.Sync(requester, identity.entityId, identity.type);

	data.horn.Sync(requester, identity.entityId, identity.type);

	data.sirenSound.Sync(requester, identity.entityId, identity.type);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(data.doorsLockState);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetDoorsLockState", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(data.plate.style);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNumberPlateStyle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	for (unsigned int i = 1; i < SizeOfArray(data.model.Extras); i++)
	{
		if (data.model.Extras[i].edited)
		{
			bitstream.Reset();
			bitstream.Write(identity.entityId);
			bitstream.Write(i);
			bitstream.Write(!data.model.Extras[i].enabled);
			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetExtra", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
		}
	}
	
	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(health);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetHealth", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	if (data.towedVehicle != -1)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(data.towedVehicle);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("TowVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	if (data.cargobobRopeType != -1)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(data.cargobobRopeType);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DeployCargobobRope", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}
}

void CVehicle::SetAssignee(RakNet::RakNetGUID assignee)
{
	network.physicsAssignee = assignee;

	RakNet::BitStream bitstream;
	bitstream.Write(identity.entityId);
	bitstream.Write((int)identity.type);
	bitstream.Write(assignee);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Server::GetServer()->GetNetworkManager()->GetInterface()->GetSystemAddressFromIndex(0), true, false);
}

const int CVehicle::GetColor(const int layer)
{
	if (layer == 1)
	{
		return data.model.Colors[0].color;
	}
	else if (layer == 2)
	{
		return data.model.Colors[1].color;
	}
}

const int CVehicle::GetColorType(const int layer)
{
	if (layer == 1)
	{
		return data.model.Colors[0].type;
	}
	else if (layer == 2)
	{
		return data.model.Colors[1].type;
	}
}

const Color CVehicle::GetCustomColor(const int layer)
{
	if (layer == 1)
	{
		return data.model.Colors[0].customCol;
	}
	else if (layer == 2)
	{
		return data.model.Colors[1].customCol;
	}
}

void CVehicle::SetColor(const int layer, const int painttype, const int color)
{
	if (layer == 1)
	{
		data.model.Colors[0].type = painttype;
		data.model.Colors[0].color = color;
		data.model.Colors[0].custom = false;
	}
	else if(layer == 2)
	{
		data.model.Colors[1].type = painttype;
		data.model.Colors[1].color = color;
		data.model.Colors[1].custom = false;
	}
}

void CVehicle::SetColor(const int layer, const Color color)
{
	if (layer == 1)
	{
		data.model.Colors[0].custom = true;
		data.model.Colors[0].customCol = color;
	}
	else if (layer == 2)
	{
		data.model.Colors[1].custom = true;
		data.model.Colors[1].customCol = color;
	}
}

CEntity CVehicle::GetOccupant(int seat)
{
	if (data.occupants[seat] != -1)
	{
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == data.occupants[seat])
			{
				return (CEntity)g_Players[i];
			}
		}
	}

	return CEntity();
}

CEntity CVehicle::GetTowedVehicle()
{
	if (data.towedVehicle != -1)
	{
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == data.towedVehicle)
			{
				return (CEntity)g_Vehicles[i];
			}
		}
	}

	return CEntity();
}