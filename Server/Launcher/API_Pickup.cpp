// Self
#include "API_Pickup.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Server.h"
#include "CPickup.h"
#include "CPlayer.h"
#include "ModelCache.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace Pickup
	{
		const char * ThisNamespace = "API::Pickup::";

		Objects::Entity CreatePickup(const CVector3 position, const uint32_t model)
		{
			CPickup pickup(position);

			Objects::Entity ent;
			ent.SetID(pickup.GetID());
			ent.SetType(pickup.GetType());

			g_Pickups.push_back(pickup);

			RakNet::BitStream bitstream;
			bitstream.Write(pickup.GetID());
			bitstream.Write(position.x);
			bitstream.Write(position.y);
			bitstream.Write(position.z);
			bitstream.Write(model);

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreatePickup", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

			ModelCache::AddModel(model);

			return ent;
		}

		void ShowPickup(Objects::Entity entity, Objects::Entity playerEntity)
		{
			switch (playerEntity.GetType())
			{
			case GrandM::EntityType::Player:
				if (playerEntity.GetID() != -1)
				{
					for (unsigned int p = 0; p < g_Players.size(); p++)
					{
						if (g_Players[p] == playerEntity)
						{
							switch (entity.GetType())
							{
							case GrandM::EntityType::Pickup:
								for (unsigned int i = 0; i < g_Pickups.size(); i++)
								{
									if (g_Pickups[i] == entity)
									{
										RakNet::BitStream bitstream;
										bitstream.Write(g_Pickups[i].GetID());
										bitstream.Write(true);
										Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[p].GetGUID(), false, false);
										return;
									}
								}
								break;
							default:
								LOG_WARNING("[%sShowCheckpoint], entity %i is not of type Pickup.", ThisNamespace, entity.GetID());
								break;
							}

							return;
						}
					}
				}
				else
				{
					for (unsigned int i = 0; i < g_Pickups.size(); i++)
					{
						if (g_Pickups[i] == entity)
						{
							g_Pickups[i].SetVisible(true);

							RakNet::BitStream bitstream;
							bitstream.Write(g_Pickups[i].GetID());
							bitstream.Write(true);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
							return;
						}
					}
				}

				break;
			default:
				LOG_WARNING("[%sShowCheckpoint], playerEntity %i is not of type Player.", ThisNamespace, playerEntity);
				break;
			}
			LOG_WARNING("[%sShowCheckpoint], entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		void HidePickup(Objects::Entity entity, Objects::Entity playerEntity)
		{
			switch (playerEntity.GetType())
			{
			case GrandM::EntityType::Player:
				if (playerEntity.GetID() != -1)
				{
					for (unsigned int p = 0; p < g_Players.size(); p++)
					{
						if (g_Players[p] == playerEntity)
						{
							switch (entity.GetType())
							{
							case GrandM::EntityType::Pickup:
								for (unsigned int i = 0; i < g_Pickups.size(); i++)
								{
									if (g_Pickups[i] == entity)
									{
										RakNet::BitStream bitstream;
										bitstream.Write(g_Pickups[i].GetID());
										bitstream.Write(false);
										Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[p].GetGUID(), false, false);
										return;
									}
								}
								break;
							default:
								LOG_WARNING("[%sHide], entity %i is not of type Pickup.", ThisNamespace, entity.GetID());
								break;
							}

							return;
						}
					}
				}
				else
				{
					for (unsigned int i = 0; i < g_Pickups.size(); i++)
					{
						if (g_Pickups[i] == entity)
						{
							g_Pickups[i].SetVisible(false);

							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(false);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

							return;
						}
					}
				}
				break;
			default:
				LOG_WARNING("[%sHidePickup], playerEntity %i is not of type Player.", ThisNamespace, playerEntity);
				break;
			}
			LOG_WARNING("[%sHidePickup], playerEntity %i or entity %i is invalid.", ThisNamespace, playerEntity, entity);
		}

		const uint32_t GetPickupModel(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetModel();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetPickupModel], entity %i is not of type Pickup.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetPickupModel], entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		void SetPickupModel(Objects::Entity entity, const uint32_t model)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetModel(model);

						RakNet::BitStream bitstream;
						bitstream.Write(g_Pickups[i].GetID());
						bitstream.Write(model);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupModel", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetPickupModel], entity %i is not of type Pickup.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetPickupModel], entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		const int GetPickupRespawnDelay(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetRespawnDelay();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetPickupRespawnDelay], entity %i is not of type Pickup.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sGetPickupRespawnDelay], entity %i is invalid.", ThisNamespace, entity.GetID());
		}

		void SetPickupRespawnDelay(Objects::Entity entity, const int time)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetRespawnDelay(time);

						RakNet::BitStream bitstream;
						bitstream.Write(g_Pickups[i].GetID());
						bitstream.Write(time);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetPickupRespawnDelay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetPickupRespawnDelay], entity %i is not of type Pickup.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sSetPickupRespawnDelay], entity %i is invalid.", ThisNamespace, entity.GetID());
		}
	}
}