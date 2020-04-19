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
#include "../Client/Core/CVehicle.h"
#include "../Client/Core/CCore.h"

// Logger
#include "../Client/Core/Console.h"
#include "../Client/Core/General.h"
#include "../Client/Core/Log.h"
#endif

#include "VehicleSyncManager.h"

namespace SyncManager
{
	namespace Vehicle 
	{
		void EngineState(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetEngineState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void EngineHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{

			int type = -1, entityid = -1;
			float val = 0;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetEngineHealth(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				LOG_ERROR("<SyncManager::EngineHealth> recived invalid type %i", type);
				break;
			}
		}

		void FuelHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			float val = 0;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetFuelTankHealth(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void HeadlightState(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetHeadlightState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void HighbeamState(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetHighbeamState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void RoofState(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetRoofState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void TaxiLight(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetTaxiLightState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void LeftIndicator(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
#ifdef __GRANDMSERVER__
						g_Vehicles[i].SetLeftIndicatorState(val, packet->guid);
#endif
#ifdef __GRANDMCLIENT__
						g_Vehicles[i].SetIndicatorState(val, 1, packet->guid);
#endif
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void RightIndicator(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
#ifdef __GRANDMSERVER__
						g_Vehicles[i].SetRightIndicatorState(val, packet->guid);
#endif
#ifdef __GRANDMCLIENT__
						g_Vehicles[i].SetIndicatorState(val, 1, packet->guid);
#endif
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void InteriorLight(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetInteriorLightState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void Horn(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetHornState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void SirenSound(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetSirenSoundState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void BoostActive(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int type = -1, entityid = -1;
			bool val = false;

			bitStream->Read(type);
			bitStream->Read(entityid);
			bitStream->Read(val);
			
			switch (type)
			{
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
						g_Vehicles[i].SetBoostState(val, packet->guid);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}
	}
}