// Self
#include "API_Player.h"

//Standard
#include <vector>

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
	namespace Player
	{
		const char * ThisNamespace = "API::Player::";

		const uint32_t GetPlayerModel(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetModel();
					}
				}
			default:
				LOG_WARNING("[%sGetPlayerModel] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetPlayerModel] Entity %i is invalid.", ThisNamespace, entity.GetID());

			return NULL;
		}

		void SetPlayerModel(Objects::Entity entity, const uint32_t model)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(model);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PlayerModel", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

						ModelCache::AddModel(model);

						return g_Players[i].SetModel(model);
					}
				}
			default:
				LOG_WARNING("[%sSetPlayerModel] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetPlayerModel] Entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		const char * GetUsername(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetUsername().c_str();
					}
				}
			default:
				LOG_WARNING("[%sGetUsername] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetUsername] Entity %i is invalid.", ThisNamespace, entity.GetID());

			return NULL;
		}

		void SetUsername(Objects::Entity entity, const char * name)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].SetUsername(name);
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetUsername] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetUsername] Entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		const bool IsControllable(Objects::Entity entity)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					return g_Players[i].HasControl();
				}
			}

			LOG_WARNING("[%sIsControllable] Entity %i is invalid.", ThisNamespace, entity.GetID());
			return false;
		}

		void SetControllable(Objects::Entity entity, bool disablecontrols, bool frozen)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					RakNet::BitStream bitstream;
					bitstream.Write(disablecontrols);
					bitstream.Write(frozen);
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetControllable", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

					g_Players[i].SetFrozen(frozen);
					return g_Players[i].SetControllable(disablecontrols);
				}
			}

			LOG_WARNING("[%sSetControllable] Entity %i is invalid.", ThisNamespace, entity.GetID());
		}


		const char * GetIP(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetIp().ToString();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetIP] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetIP] Entity %i is invalid.", ThisNamespace, entity.GetID());
			return NULL;
		}

		const char * GetSecretKey(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetSecretKey().c_str();
					}
				}
				break;
			default:
				LOG_WARNING("%sGetSecretKey Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("%sGetSecretKey Entity %i is invalid.", ThisNamespace, entity.GetID());

			return NULL;
		}

		void Kick(Objects::Entity entity, const char * reason)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(RakNet::RakString(reason));
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("Kick", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);


						Server::GetServer()->GetNetworkManager()->GetInterface()->CloseConnection(g_Players[i].GetGUID(), true);
						Server::GetServer()->GetNetworkManager()->PlayerLeft(g_Players[i].GetGUID(), 2);

						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sKick] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sKick] Entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		void PutInVehicle(Objects::Entity playerEntity, Objects::Entity vehicleEntity, const int seat)
		{
			switch (vehicleEntity.GetType())
			{
			case GrandM::EntityType::Vehicle:
				switch (playerEntity.GetType())
				{
				case GrandM::EntityType::Player:
					for (unsigned int i = 0; i < g_Players.size(); i++)
					{
						if (g_Players[i] == playerEntity)
						{
							RakNet::BitStream bitstream;
							bitstream.Write(vehicleEntity.GetID());
							bitstream.Write(seat);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PutInVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

							for (unsigned int v = 0; v < g_Vehicles.size(); v++)
							{
								if (g_Vehicles[v] == vehicleEntity)
								{
									g_Vehicles[v].SetAssignee(g_Players[i].GetGUID());
									break;
								}
							}
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sPutInVehicle] Entity %i is not of type Player.", ThisNamespace, playerEntity);
					break;
				}
				break;
			default:
				LOG_WARNING("[%sPutInVehicle] Entity %i is not of type Vehicle.", ThisNamespace, vehicleEntity);
				return;
			}

			LOG_WARNING("[%sPutInVehicle] Invalid entities.", ThisNamespace, playerEntity);
		}

		Objects::Entity GetVehicle(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						Objects::Entity ent;
						ent.SetType(GrandM::EntityType::Player);
						ent.SetID(g_Players[i].GetID());

						return ent;
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetVehicle] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetVehicle] Player Entity %i is invalid.", ThisNamespace, entity.GetID());

			return Objects::Entity();
		}

		const int GetSeat(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetVehicleSeat();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetSeat] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetSeat] Player Entity %i is invalid.", ThisNamespace, entity.GetID());

			return -1;
		}

		void GiveWeapon(Objects::Entity entity, const char * weapon, const int ammo)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(RakNet::RakString(weapon));
						bitstream.Write(ammo);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("GiveWeapon", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

						g_Players[i].GiveWeapon(weapon, ammo);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sGiveWeapon] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGiveWeapon] Player Entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		void RemoveWeapon(Objects::Entity entity, const char * weapon)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(RakNet::RakString(weapon));
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("RemoveWeapon", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

						g_Players[i].RemoveWeapon(weapon);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sRemoveWeapon] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sRemoveWeapon] Player Entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		void GetWeaponAmmo(Objects::Entity entity, const char * weapon, int* ammo, int* clipAmmo)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						*ammo = g_Players[i].GetWeaponAmmo(weapon);
						*clipAmmo = g_Players[i].GetWeaponAmmo(weapon);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetWeaponAmmo] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetWeaponAmmo] Player Entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		void SetWeaponAmmo(Objects::Entity entity, const char * weapon, const int ammo, const int clipAmmo)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].UpdateWeapon(weapon, ammo, clipAmmo);

						RakNet::BitStream bitstream;
						bitstream.Write(RakNet::RakString(weapon));
						bitstream.Write(ammo);
						bitstream.Write(clipAmmo);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetWeaponAmmo", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%SetWeaponAmmo] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%SetWeaponAmmo] Player Entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		char ** GetWeapons(Objects::Entity entity)
		{
			std::vector<char *> weapons;

			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return weapons.data();
					}
				}
				break;
			default:
				LOG_WARNING("[%GetWeapons] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%GetWeapons] Player Entity %i is invalid.", ThisNamespace, entity.GetID());

			return weapons.data();
		}

		void EquipWeapon(Objects::Entity entity, const char * weapon)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(RakNet::RakString(weapon));
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("EquipWeapon", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%EquipWeapon] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%EquiptWeapon] Player Entity %i is invalid.", ThisNamespace, entity.GetID());
		}
	}
}