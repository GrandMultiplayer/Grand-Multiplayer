#include <iostream>
#include <vector>

// RakNet
#include <RakNetTypes.h>

#include <SharedUtility.h>

#ifdef __GRANDMSERVER__
#include "../Server/Launcher/Server.h"
#include "../Server/Launcher/CEntity.h"
#include "../Server/Launcher/CPlayer.h"
#include "../Server/Launcher/CVehicle.h"
#include "../Server/Launcher/CObject.h"
#include "../Server/Launcher/CNPC.h"

// Logger
#include "../Server/Launcher/Console.h"
#include "../Server/Launcher/General.h"
#include "../Server/Launcher/Log.h"
#endif
#ifdef __GRANDMCLIENT__
#include "../Client/Core/CNetworkManager.h"
#include "../Client/Core/CEntity.h"
#include "../Client/Core/CPlayer.h"
#include "../Client/Core/CVehicle.h"
#include "../Client/Core/CObject.h"
#include "../Client/Core/CNPC.h"
#include "../Client/Core/CCore.h"

// Logger
#include "../Client/Core/Console.h"
#include "../Client/Core/General.h"
#include "../Client/Core/Log.h"
#endif

#include "SyncManager.h"
#include "VehicleSyncManager.h"

namespace SyncManager
{
	Sync syncmessages[] = {
#pragma region Entity
	{ "EntityHealth", EntityHealth },
#pragma endregion
#pragma region Vehicle
	{ "VehicleEngineState", SyncManager::Vehicle::EngineState },
	{ "VehicleEngineHealth", SyncManager::Vehicle::EngineHealth },
	{ "VehicleFuelHealth", SyncManager::Vehicle::FuelHealth },
	{ "VehicleHeadlightState", SyncManager::Vehicle::HeadlightState },
	{ "VehicleHighbeamState", SyncManager::Vehicle::HighbeamState },
	{ "VehicleRoofState", SyncManager::Vehicle::RoofState },
	{ "VehicleTaxiLight", SyncManager::Vehicle::TaxiLight },
	{ "VehicleLeftIndicator", SyncManager::Vehicle::LeftIndicator },
	{ "VehicleRightIndicator", SyncManager::Vehicle::RightIndicator },
	{ "VehicleInteriorLight", SyncManager::Vehicle::InteriorLight },
	{ "VehicleHorn", SyncManager::Vehicle::Horn },
	{ "VehicleSirenSound", SyncManager::Vehicle::SirenSound },
	{ "VehicleBoostActive", SyncManager::Vehicle::BoostActive }
#pragma endregion
	};

	void Init()
	{
		for (unsigned int i = 0; i < SizeOfArray(syncmessages); i++)
		{
#ifdef __GRANDMSERVER__
			Server::GetServer()->GetNetworkManager()->GetRPC().RegisterSlot(syncmessages[i].name, syncmessages[i].functionPointer, 0);
#endif
#ifdef __GRANDMCLIENT__
			CNetworkManager::GetRPC().RegisterSlot(syncmessages[i].name, syncmessages[i].functionPointer, 0);
#endif
		}
	}

	void EntityHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet)
	{
		int type = -1, entityid = -1, val;

		bitStream->Read(type);
		bitStream->Read(entityid);
		bitStream->Read(val);
		
		switch (type)
		{
#ifdef __GRANDMSERVER__
		case GrandM::EntityType::Player:
#endif
#ifdef __GRANDMCLIENT__
		case CEntity::ePlayer:
#endif
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetID() == entityid)
				{
					g_Players[i].SetHealth(val, packet->guid);
					return;
				}
			}
			break;
		}
#ifdef __GRANDMSERVER__
		case GrandM::EntityType::Vehicle:
#endif
#ifdef __GRANDMCLIENT__
		case CEntity::eVehicle:
#endif
		{
			for (unsigned int i = 0; i < g_Vehicles.size(); i++)
			{
				if (g_Vehicles[i].GetID() == entityid)
				{
					g_Vehicles[i].SetHealth(val, packet->guid);
					return;
				}
			}
			break;
		}
#ifdef __GRANDMSERVER__
		case GrandM::EntityType::Object:
#endif
#ifdef __GRANDMCLIENT__
		case CEntity::eObject:
#endif
		{
			for (unsigned int i = 0; i < g_Objects.size(); i++)
			{
				if (g_Objects[i].GetID() == entityid)
				{
#ifdef __GRANDMSERVER__
					g_Objects[i].SetHealth(val, packet->guid);
#endif
					return;
				}
			}
			break;
		}
#ifdef __GRANDMSERVER__
		case GrandM::EntityType::NPC:
#endif
#ifdef __GRANDMCLIENT__
		case CEntity::eNPC:
#endif
		{
			for (unsigned int i = 0; i < g_Npcs.size(); i++)
			{
				if (g_Npcs[i].GetID() == entityid)
				{
					g_Npcs[i].SetHealth(val, packet->guid);
					return;
				}
			}
			break;
		}
		default:
			break;
		}
	}
}