// System

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Common.h"
#include "Server.h"
#include "CBlip.h"
#include "CPlayer.h"
#include "CNPC.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CCheckpoint.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "API_Blip.h"

namespace API
{
	namespace Blip
	{
		const char * ThisNamespace = "API::Blip";

		void CreateBlip(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						CBlip* blip = g_Players[i].GetBlip();
						blip->CreateE(g_Players[i].GetID(), GrandM::EntityType::Player);

						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);

						blip = nullptr;

						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						CBlip* blip = g_Npcs[i].GetBlip();
						blip->CreateE(g_Npcs[i].GetID(), GrandM::EntityType::NPC);

						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);

						blip = nullptr;

						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						CBlip* blip = g_Objects[i].GetBlip();
						blip->CreateE(g_Objects[i].GetID(), GrandM::EntityType::Object);

						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);

						blip = nullptr;

						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						LOG_PRINT("[%s::CreateBlip_] Type Checkpoint Entities Current do not work for blip (Soon ™).", ThisNamespace);
						CBlip* blip = g_Checkpoints[i].GetBlip();
						blip->CreateE(g_Checkpoints[i].GetID(), GrandM::EntityType::Checkpoint);

						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);

						blip = nullptr;

						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						CBlip* blip = g_Vehicles[i].GetBlip();
						blip->CreateE(g_Vehicles[i].GetID(), GrandM::EntityType::Vehicle);

						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);

						blip = nullptr;

						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::CreateBlip_] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		Objects::Entity CreateBlip_(const CVector3 position)
		{
			CBlip blip(position);

			Objects::Entity ent;
			ent.SetID(blip.GetID());
			ent.SetType(blip.GetType());

			g_Blips.push_back(blip);

			RakNet::BitStream bitstream;
			bitstream.Write(blip.GetID());
			bitstream.Write(position.x);
			bitstream.Write(position.y);
			bitstream.Write(position.z);

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipPos", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

			return ent;
		}

		void ShowBlip(Objects::Entity entity, Objects::Entity player)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
			case GrandM::EntityType::Vehicle:
			case GrandM::EntityType::Object:
			case GrandM::EntityType::Checkpoint:
			case GrandM::EntityType::Blip:
			case GrandM::EntityType::NPC:
			{
				switch (player.GetType())
				{
				case GrandM::EntityType::Player:
				{
					for (unsigned int p = 0; p < g_Players.size(); p++)
					{
						if (g_Players[p] == player)
						{
							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(entity.GetType());
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowBlip", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[p].GetGUID(), false, false);
							return;
						}
					}
					break;
				}
				default:
					LOG_WARNING("[%s::ShowBlip] Entity 'player' is invalid, must be of type player.", ThisNamespace);
					break;
				}

				break;
			}
			default:
				break;
			}
		}

		void HideBlip(Objects::Entity entity, Objects::Entity player)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
			case GrandM::EntityType::Vehicle:
			case GrandM::EntityType::Object:
			case GrandM::EntityType::Checkpoint:
			case GrandM::EntityType::Blip:
			case GrandM::EntityType::NPC:
			{
				switch (player.GetType())
				{
				case GrandM::EntityType::Player:
				{
					for (unsigned int p = 0; p < g_Players.size(); p++)
					{
						if (g_Players[p] == player)
						{
							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(entity.GetType());
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("HideBlip", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[p].GetGUID(), false, false);
							return;
						}
					}
					break;
				}
				default:
					LOG_WARNING("[%s::HideBlip] Entity 'player' is invalid, must be of type player.", ThisNamespace);
					return;
					break;
				}
				break;
			}
			default:
				break;
			}
		}

		void ShowBlipToAll(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].GetBlip()->Show();

						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].GetBlip()->Show();
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].GetBlip()->Show();
						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].GetBlip()->Show();
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						g_Vehicles[i].GetBlip()->Show();
						return;
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						g_Blips[i].Show();
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::ShowBlipToAll] Entity %i is not a valid entity.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void HideBlipFromAll(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].GetBlip()->Hide();
						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].GetBlip()->Hide();
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].GetBlip()->Hide();
						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].GetBlip()->Hide();
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						g_Vehicles[i].GetBlip()->Hide();
						return;
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						g_Blips[i].Hide();
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::HideBlipFromAll] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		const int GetBlipColor(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetBlip()->GetColor();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetBlip()->GetColor();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return  g_Objects[i].GetBlip()->GetColor();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetBlip()->GetColor();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						return g_Vehicles[i].GetBlip()->GetColor();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].GetColor();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetBlipColor] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void SetBlipColor(Objects::Entity entity, const int color)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].GetBlip()->SetColor(color);
						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].GetBlip()->SetColor(color);
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].GetBlip()->SetColor(color);
						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].GetBlip()->SetColor(color);
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						CBlip* blip = g_Vehicles[i].GetBlip();
						blip->SetColor(color);

						blip = nullptr;
						return;
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						g_Blips[i].SetColor(color);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetBlipColor] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		const int GetSprite(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetBlip()->GetSprite();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetBlip()->GetSprite();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetBlip()->GetSprite();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetBlip()->GetSprite();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						return g_Vehicles[i].GetBlip()->GetSprite();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].GetSprite();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetSprite] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void SetSprite(Objects::Entity entity, const int sprite)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].GetBlip()->SetSprite(sprite);
						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].GetBlip()->SetSprite(sprite);
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].GetBlip()->SetSprite(sprite);
						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].GetBlip()->SetSprite(sprite);
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						g_Vehicles[i].GetBlip()->SetSprite(sprite);
						return;
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						g_Blips[i].SetSprite(sprite);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetSprite] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		const char * GetBlipText(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetBlip()->GetText().c_str();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetBlip()->GetText().c_str();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetBlip()->GetText().c_str();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetBlip()->GetText().c_str();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						return g_Vehicles[i].GetBlip()->GetText().c_str();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].GetText().c_str();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetBlipText] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void SetBlipText(Objects::Entity entity, const char * text)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].GetBlip()->SetText(text);
						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].GetBlip()->SetText(text);
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].GetBlip()->SetText(text);
						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].GetBlip()->SetText(text);
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						g_Vehicles[i].GetBlip()->SetText(text);
						return;
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						g_Blips[i].SetText(text);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetBlipText] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		const bool IsShortRanged(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetBlip()->IsShortRanged();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetBlip()->IsShortRanged();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetBlip()->IsShortRanged();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetBlip()->IsShortRanged();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						return g_Vehicles[i].GetBlip()->IsShortRanged();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].IsShortRanged();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::IsShortRanged] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void SetShortRanged(Objects::Entity entity, const bool state)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].GetBlip()->SetShortRanged(state);
						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].GetBlip()->SetShortRanged(state);
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].GetBlip()->SetShortRanged(state);
						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].GetBlip()->SetShortRanged(state);
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i] == entity)
					{
						g_Vehicles[i].GetBlip()->SetShortRanged(state);
						return;
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						g_Blips[i].SetShortRanged(state);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetShortRanged] Entity %i is not Entity Type.", ThisNamespace, entity.GetID());
				break;
			}
		}
	}
}