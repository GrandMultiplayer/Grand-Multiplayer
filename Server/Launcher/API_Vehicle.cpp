// Self
#include "API_Vehicle.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Server.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "ModelCache.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace Vehicle
	{
		const char * ThisNamespace = "API::Vehicle::";

		Objects::Entity CreateVehicle(const uint32_t model, const CVector3 position, const CVector3 rotation)
		{
			CVehicle vehicle = CVehicle(model, position, rotation);

			if (vehicle.IsNull())
			{
				LOG_WARNING("Creating a vehicle resulted in null");
				return Objects::Entity();
			}

			Objects::Entity ent;
			ent.SetID(vehicle.GetID());
			ent.SetType(vehicle.GetType());

			g_Vehicles.push_back(vehicle);

			RakNet::BitStream bitstream;
			bitstream.Write(ent.GetID());
			bitstream.Write(model);
			bitstream.Write(position.x);
			bitstream.Write(position.y);
			bitstream.Write(position.z);
			bitstream.Write(rotation.x);
			bitstream.Write(rotation.y);
			bitstream.Write(rotation.z);
			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("InitVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

			ModelCache::AddModel(model);

			return ent;
		}

		const int GetVehicleColor(Entity entity, const int layer)
		{
			if (layer == 1 || layer == 2)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							return veh.GetColor(layer);
						}
					}
					break;
				default:
					LOG_WARNING("[%sGetVehicleColor] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sGetVehicleColor] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sGetVehicleColor] Invalid Layer, Layers are 1 or 2.", ThisNamespace);
			}
		}

		const int GetVehicleColorType(Entity entity, const int layer)
		{
			if (layer == 1 || layer == 2)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							return veh.GetColorType(layer);
						}
					}
					break;
				default:
					LOG_WARNING("[%sGetVehicleColorType] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sGetVehicleColorType] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sGetVehicleColorType] Invalid Layer, Layers are 1 or 2.", ThisNamespace);
			}
		}

		void SetVehicleColor(Entity entity, const int layer, const int painttype, const int color)
		{
			if (layer == 1 || layer == 2)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							veh.SetColor(layer, painttype, color);

							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(layer);
							bitstream.Write(painttype);
							bitstream.Write(color);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetStandardColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetVehicleColor] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sSetVehicleColor] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sSetVehicleColor] Invalid Layer, Layers are 1 or 2.", ThisNamespace);
			}
		}

		const Color GetVehicleRGBColor(Entity entity, const int layer)
		{
			if (layer == 1 || layer == 2)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							return veh.GetCustomColor(layer);
						}
					}
					break;
				default:
					LOG_WARNING("[%sGetVehicleRGBColor] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sGetVehicleRGBColor] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sGetVehicleRGBColor] Invalid Layer, Layers are 1 or 2.", ThisNamespace);
			}
		}

		void SetVehicleRGBColor(Entity entity, const int layer, const Color color)
		{
			if (layer == 1 || layer == 2)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							veh.SetColor(layer, color);

							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(layer);
							bitstream.Write(color.Red);
							bitstream.Write(color.Green);
							bitstream.Write(color.Blue);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCustomColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetVehicleRGBColor] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sSetVehicleRGBColor] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sSetVehicleRGBColor] Invalid Layer, Layers are 1 or 2.", ThisNamespace);
			}
		}

		const char * GetNumberPlate(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetNumberPlate().c_str();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetNumberPlate] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetNumberPlate] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void SetNumberPlate(Entity entity, const char * plate)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						if (std::string(plate).size() > 8)
							plate = std::string(plate).substr(0, 8).c_str(); // ???

						veh.SetNumberPlate(plate);

						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(RakNet::RakString(plate));
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNumberPlate", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetNumberPlate] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetNumberPlate] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		const int GetMod(Entity entity, const int modType)
		{
			if (modType >= 0 && modType <= 49)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							return veh.GetMod(modType);
						}
					}
					break;
				default:
					LOG_WARNING("[%sGetMod] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sGetMod] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sGetMod] Invalid modType %i, must be 0 to 49.", ThisNamespace, entity.GetID());
			}
		}

		void SetMod(Entity entity, const int modType, const int modIndex)
		{
			if (modType >= 0 && modType <= 49)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							veh.SetMod(modType, modIndex);

							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(modType);
							bitstream.Write(modIndex);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetMod", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetMod] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sSetMod] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sSetMod] Invalid modType %i, must be 0 to 49.", ThisNamespace, entity.GetID());
			}
		}

		const bool GetEngineState(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetEngineState();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetEngineState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetEngineState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void SetEngineState(Entity entity, const bool state, const bool instant)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetForcedEngineState(true);
						veh.SetEngineState(state, RakNet::UNASSIGNED_RAKNET_GUID);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetEngineState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetEngineState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		const int GetDoorsLockState(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetDoorsLockState();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetDoorsLockState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetDoorsLockState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void SetDoorsLockState(Entity entity, const int state)
		{
			if (state <= 4 && state >= 0)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							veh.SetDoorsLockState(state);

							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(state);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetDoorsLockState", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetDoorsLockState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sSetDoorsLockState] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sSetDoorsLockState] Invalid Lock State, Valid states are 0 to 4.", ThisNamespace);
			}
		}

		void SetDoorsLockStateForPlayer(Entity entity, const int state, Entity player)
		{
			if (state <= 4 && state >= 0)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							switch (player.GetType())
							{
							case GrandM::EntityType::Player:
								for (unsigned int p = 0; p < g_Players.size(); p++)
								{
									if (g_Players[p] == player)
									{
										RakNet::BitStream bitstream;
										bitstream.Write(entity.GetID());
										bitstream.Write(state);
										Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetDoorsLockState", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[p].GetGUID(), false, false);
										return;
									}
								}
								break;
							default:
								LOG_WARNING("[%sSetDoorsLockState] Entity %i is not of type Player.", ThisNamespace, player);
								break;
							}
							LOG_WARNING("[%sSetDoorsLockState] Invalid Entity(player): %i", ThisNamespace, player);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetDoorsLockState] Entity(entity) %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sSetDoorsLockState] Invalid Entity(entity): %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sSetDoorsLockState] Invalid Lock State, Valid states are 0 to 4.", ThisNamespace, entity.GetID());
			}
		}

		const uint32_t GetVehicleModel(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetModel();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetVehicleModel] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetVehicleModel] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		const int GetNumberPlateStyle(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetNumberPlateStyle();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetNumberPlateStyle] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetNumberPlateStyle] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void SetNumberPlateStyle(Entity entity, const int style)
		{
			if (style >= 0 && style <= 5)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							veh.SetNumberPlateStyle(style);

							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(style);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNumberPlateStyle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetNumberPlateStyle] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sSetNumberPlateStyle] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sSetNumberPlateStyle] Invalid Style %i, Valid styles are 0-5.", ThisNamespace, style);
			}
		}

		const bool GetExtra(Entity entity, const int extra)
		{
			if (extra >= 1 && extra <= 14)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							return veh.GetExtra(extra);
						}
					}
					break;
				default:
					LOG_WARNING("[%sGetExtra] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sGetExtra] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sGetExtra] Invalid Extra: %i, Must be 1-14.", ThisNamespace, extra);
			}
		}

		void SetExtra(Entity entity, const int extra, const bool toggle)
		{
			if (extra >= 1 && extra <= 14)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							veh.SetExtra(extra, toggle);

							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(extra);
							bitstream.Write(toggle);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetExtra", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetExtra] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
				LOG_WARNING("[%sSetExtra] Invalid Entity: %i", ThisNamespace, entity.GetID());
			}
			else
			{
				LOG_WARNING("[%sSetExtra] Invalid Extra: %i, Must be 1-14.", ThisNamespace, extra);
			}
		}

		const float GetEngineHealth(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetEngineHealth();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetEngineHealth] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetEngineHealth] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void SetEngineHealth(Entity entity, const float health)
		{
			if (health <= 1000 && health >= -4000)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							return veh.SetEngineHealth(health, RakNet::UNASSIGNED_RAKNET_GUID);
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetEngineHealth] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
			}
			else
			{
				LOG_WARNING("[%sSetEngineHealth] Invalid health, must be between -4000 - 1000.", ThisNamespace);
			}
			LOG_WARNING("[%sSetEngineHealth] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		const float GetFuelTankHealth(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetFuelTankHealth();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetFuelTankHealth] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetFuelTankHealth] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void SetFuelTankHealth(Entity entity, const float health)
		{
			if (health <= 1000 && health >= -999)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							return veh.SetFuelTankHealth(health, RakNet::UNASSIGNED_RAKNET_GUID);
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetFuelTankHealth] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
			}
			else
			{
				LOG_WARNING("[%sSetFuelTankHealth] Invalid health, must be between -999 - 1000.", ThisNamespace, entity.GetID());
				return;
			}
			LOG_WARNING("[%sSetFuelTankHealth] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void FixDeformation(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("FixDeformation", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sFixDeformation] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sFixDeformation] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		Entity GetOccupant(Entity entity, const int seat)
		{
			if (seat >= 0 && seat < 12)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							CEntity p(veh.GetOccupant(seat));

							Entity ent;
							ent.SetID(p.GetID());
							ent.SetType(p.GetType());

							return ent;
						}
					}
					break;
				default:
					LOG_WARNING("[%sGetOccupant] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
			}
			else
			{
				LOG_WARNING("[%sGetOccupant] Invalid seat, valid seats are 0-11.", ThisNamespace, entity.GetID());
				return Entity();
			}
			LOG_WARNING("[%sGetOccupant] Invalid Entity: %i", ThisNamespace, entity.GetID());
			return Entity();
		}

		const Entity * GetOccupants(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						std::vector<Entity> Seats;
						for (int s = 0; s < 12; s++)
						{
							CEntity p = veh.GetOccupant(s);

							Entity ent;
							ent.SetID(p.GetID());
							ent.SetType(p.GetType());

							Seats.push_back(ent);
						}
						return Seats.data();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetOccupants] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetOccupants] Invalid Entity: %i", ThisNamespace, entity.GetID());
			return std::vector<Entity>(0).data();
		}

		const bool GetTaxiLightState(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetTaxiLightState();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetTaxiLightState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetTaxiLightState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		void SetTaxiLightState(Entity entity, const bool state)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.SetTaxiLightState(state, RakNet::UNASSIGNED_RAKNET_GUID);
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetTaxiLightState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetTaxiLightState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		const bool GetIndicatorState(Entity entity, const int turnlight)
		{
			if (turnlight == 0 || turnlight == 1)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							if (turnlight == 0)
								return veh.GetRightIndicatorState();
							if (turnlight == 1)
								return veh.GetLeftIndicatorState();
						}
					}
					break;
				default:
					LOG_WARNING("[%sGetIndicatorState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
			}
			else
			{
				LOG_WARNING("[%sGetIndicatorState] invalid turnlight, 1 for left light, 0 for right light.", ThisNamespace, entity.GetID());
				return false;
			}
			LOG_WARNING("[%sGetIndicatorState[ Invalid Entity: %i", ThisNamespace, entity.GetID());
			return false;
		}

		void SetIndicatorState(Entity entity, const int turnlight, const bool state)
		{
			if (turnlight == 0 || turnlight == 1)
			{
				switch (entity.GetType())
				{
				case GrandM::EntityType::Vehicle:
					for (auto &veh : g_Vehicles)
					{
						if (veh == entity)
						{
							if (turnlight == 0)
								return veh.SetRightIndicatorState(state, RakNet::UNASSIGNED_RAKNET_GUID);
							if (turnlight == 1)
								return veh.SetLeftIndicatorState(state, RakNet::UNASSIGNED_RAKNET_GUID);
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetIndicatorState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
					break;
				}
			}
			else
			{
				LOG_WARNING("[%sSetIndicatorState] invalid turnlight, 1 for left light, 0 for right light.", ThisNamespace, entity.GetID());
				return;
			}
			LOG_WARNING("[%sSetIndicatorState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		const bool GetInteriorLightState(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetInteriorLightState();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetInteriorLightState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetInteriorLightState] Invalid Entity: %i", ThisNamespace, entity.GetID());
			return false;
		}

		void SetInteriorLightState(Entity entity, const bool state)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.SetInteriorLightState(state, RakNet::UNASSIGNED_RAKNET_GUID);
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetInteriorLightState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetInteriorLightState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		const bool GetSirenSoundState(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetSirenSoundState();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetSirenSoundState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetSirenSoundState] Invalid Entity: %i", ThisNamespace, entity.GetID());
			return false;
		}

		void SetSirenSoundState(Entity entity, const bool state)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.SetSirenSoundState(state, RakNet::UNASSIGNED_RAKNET_GUID);
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetSirenSoundState] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetSirenSoundState] Invalid Entity: %i", ThisNamespace, entity.GetID());
		}

		Entity GetTowedVehicle(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						CEntity v = veh.GetTowedVehicle();

						Entity ent;
						ent.SetID(v.GetID());
						ent.SetType(v.GetType());

						return ent;
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetTowedVehicle] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetTowedVehicle] Invalid Entity: %i", ThisNamespace, entity.GetID());
			return Entity();
		}

		void SetTowedVehicle(Entity towVehicle, Entity vehicle)
		{
			switch (towVehicle.GetType())
			{
			case GrandM::EntityType::Vehicle:
				switch (vehicle.GetType())
				{
				case GrandM::EntityType::Vehicle:

					for (auto &veh : g_Vehicles)
					{
						if (veh == towVehicle)
						{
							RakNet::BitStream bitstream;
							bitstream.Write(towVehicle.GetID());
							bitstream.Write(vehicle.GetID());
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("TowVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

							for (unsigned int v = 0; v < g_Vehicles.size(); v++)
							{
								if (g_Vehicles[v] == vehicle)
									g_Vehicles[v].SetAssignee(veh.GetAssignee());
							}

							return veh.SetTowedVehicle(vehicle.GetID());
						}
					}
					break;
				default:
					LOG_WARNING("[%sSetTowedVehicle] Entity (vehicle) %i is not of type Vehicle.", ThisNamespace, vehicle);
					break;
				}
				LOG_WARNING("[%sSetTowedVehicle] Invalid Entity: (vehicle) %i", ThisNamespace, vehicle);
				return;
				break;
			default:
				LOG_WARNING("[%sSetTowedVehicle] Entity (towVehicle) %i is not of type Vehicle.", ThisNamespace, towVehicle);
				break;
			}
			LOG_WARNING("[%sSetTowedVehicle] Invalid Entity: (towVehicle) %i", ThisNamespace, towVehicle);
		}

		const bool IsTowed(Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh.GetTowedVehicle() == entity)
					{
						return true;
					}
				}
				break;
			default:
				LOG_WARNING("[%sIsTowed] Entity %i is not of type Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sIsTowed] Invalid Entity: %i", ThisNamespace, entity.GetID());
			return false;
		}
	}
}