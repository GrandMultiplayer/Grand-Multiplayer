// Self
#include "API_Entity.h"

// Standard
#include <vector>

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Server.h"
#include "CPlayer.h"
#include "CCheckpoint.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"
#include "CPickup.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace Entity
	{
		const char * ThisNamespace = "API::Entity";

		void Destroy(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(g_Players[i].GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DestroyEntity", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Players.erase(g_Players.begin() + i);
						g_Players.shrink_to_fit();
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
			{
				int i = 0;
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(veh.GetID());
						bitstream.Write(veh.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DestroyEntity", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Vehicles.erase(g_Vehicles.begin() + i);
						g_Vehicles.shrink_to_fit();
						return;
					}

					i++;
				}
				break;
			}
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(g_Npcs[i].GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DestroyEntity", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Npcs.erase(g_Npcs.begin() + i);
						g_Npcs.shrink_to_fit();
						return;
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Blips[i].GetID());
						bitstream.Write(g_Blips[i].GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DestroyEntity", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Blips.erase(g_Blips.begin() + i);
						g_Blips.shrink_to_fit();
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Objects[i].GetID());
						bitstream.Write(g_Objects[i].GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DestroyEntity", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Objects.erase(g_Objects.begin() + i);
						g_Objects.shrink_to_fit();
						return;
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Checkpoints[i].GetID());
						bitstream.Write(g_Checkpoints[i].GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DestroyEntity", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Checkpoints.erase(g_Checkpoints.begin() + i);
						g_Checkpoints.shrink_to_fit();
						return;
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Pickups[i].GetID());
						bitstream.Write(g_Pickups[i].GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DestroyEntity", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Pickups.erase(g_Pickups.begin() + i);
						g_Pickups.shrink_to_fit();
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::Destroy] Tried to destroy an invalid entity.", ThisNamespace);
			}
		}

		const CVector3 GetPosition(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetPosition();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetPosition();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetPosition();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].GetPosition();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetPosition();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetPosition();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetPosition();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetPosition] Invalid entity.", ThisNamespace);
			}
		}

		void SetPosition(Objects::Entity entity, const CVector3 position)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].SetPosition(position);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.SetPosition(position);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].SetPosition(position);
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].SetPosition(position);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].SetPosition(position);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].SetPosition(position);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].SetPosition(position);
					}
				}
				break;
			default:
				break;
			}

			LOG_WARNING("[%s::SetPosition] Invalid entity.", ThisNamespace);
		}

		const CVector3 GetRotation(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetRotation();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetRotation();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetRotation();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].GetRotation();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetRotation();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetRotation();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetRotation();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetRotation] Invalid entity.", ThisNamespace);
			}
		}

		void SetRotation(Objects::Entity entity, const CVector3 rotation)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].SetRotation(rotation);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.SetRotation(rotation);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].SetRotation(rotation);
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].SetRotation(rotation);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].SetRotation(rotation);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].SetRotation(rotation);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].SetRotation(rotation);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetRotation] Invalid entity.", ThisNamespace);
			}
		}

		// NPC & Player
		const GrandM::Model::Component GetPedComponent(Objects::Entity entity, const int componentid)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetModelComponent(componentid);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetModelComponent(componentid);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetPedComponent] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				return GrandM::Model::Component();
				break;
			}
			LOG_WARNING("[%s::GetPedComponent] Invalid entity.", ThisNamespace);
		}

		void SetPedComponent(Objects::Entity entity, const int componentid, const GrandM::Model::Component component)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(GrandM::EntityType::Player);
						bitstream.Write(componentid);
						bitstream.Write(component.drawableid);
						bitstream.Write(component.paletteid);
						bitstream.Write(component.textureid);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedComponent", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Players[i].SetModelComponent(componentid, component);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(GrandM::EntityType::NPC);
						bitstream.Write(componentid);
						bitstream.Write(component.drawableid);
						bitstream.Write(component.paletteid);
						bitstream.Write(component.textureid);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedComponent", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Npcs[i].SetModelComponent(componentid, component);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetPedComponent] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::SetPedComponent] Invalid entity.", ThisNamespace);
		}

		const GrandM::Model::HeadBlend GetPedHeadBlend(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetModelHeadBlend();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetModelHeadBlend();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetPedHeadBlend] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::GetPedHeadBlend] Invalid entity.", ThisNamespace);
		}

		void SetPedHeadBlend(Objects::Entity entity, const GrandM::Model::HeadBlend headblend)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(GrandM::EntityType::Player);
						bitstream.Write(headblend.shapeFirst);
						bitstream.Write(headblend.shapeMix);
						bitstream.Write(headblend.shapeSecond);
						bitstream.Write(headblend.shapeThird);
						bitstream.Write(headblend.skinFirst);
						bitstream.Write(headblend.skinMix);
						bitstream.Write(headblend.skinSecond);
						bitstream.Write(headblend.skinThird);
						bitstream.Write(headblend.thirdMix);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadBlend", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Players[i].SetModelHeadBlend(headblend);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(GrandM::EntityType::NPC);
						bitstream.Write(headblend.shapeFirst);
						bitstream.Write(headblend.shapeMix);
						bitstream.Write(headblend.shapeSecond);
						bitstream.Write(headblend.shapeThird);
						bitstream.Write(headblend.skinFirst);
						bitstream.Write(headblend.skinMix);
						bitstream.Write(headblend.skinSecond);
						bitstream.Write(headblend.skinThird);
						bitstream.Write(headblend.thirdMix);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadBlend", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Npcs[i].SetModelHeadBlend(headblend);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetPedHeadBlend] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::SetPedHeadBlend] Invalid entity.", ThisNamespace);
		}

		const GrandM::Model::HeadOverlay GetPedHeadOverlay(Objects::Entity entity, const int index)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetModelHeadOverlay(index);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetModelHeadOverlay(index);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetPedHeadOverlay] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::GetPedHeadOverlay] Invalid entity.", ThisNamespace);
		}

		void SetPedHeadOverlay(Objects::Entity entity, const int index, const GrandM::Model::HeadOverlay overlay)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(GrandM::EntityType::Player);
						bitstream.Write(index);
						bitstream.Write(overlay.index);
						bitstream.Write(overlay.colorType);
						bitstream.Write(overlay.colorID);
						bitstream.Write(overlay.secondColorID);
						bitstream.Write(overlay.opacity);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadOverlay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Players[i].SetModelHeadOverlay(index, overlay);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(GrandM::EntityType::NPC);
						bitstream.Write(index);
						bitstream.Write(overlay.index);
						bitstream.Write(overlay.colorType);
						bitstream.Write(overlay.colorID);
						bitstream.Write(overlay.secondColorID);
						bitstream.Write(overlay.opacity);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadOverlay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Npcs[i].SetModelHeadOverlay(index, overlay);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetPedHeadOverlay] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::SetPedHeadOverlay] Invalid entity.", ThisNamespace);
		}

		const GrandM::Model::Prop GetPedProp(Objects::Entity entity, const int index)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return  g_Players[i].GetModelProp(index);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetModelProp(index);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetPedProp] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::GetPedProp] Invalid entity.", ThisNamespace);
		}

		void SetPedProp(Objects::Entity entity, const int index, const GrandM::Model::Prop prop)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(GrandM::EntityType::Player);
						bitstream.Write(index);
						bitstream.Write(prop.drawableid);
						bitstream.Write(prop.textureid);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedProp", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Players[i].SetModelProp(index, prop);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(GrandM::EntityType::NPC);
						bitstream.Write(index);
						bitstream.Write(prop.drawableid);
						bitstream.Write(prop.textureid);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedProp", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Npcs[i].SetModelProp(index, prop);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetPedProp] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::SetPedProp] Invalid entity.", ThisNamespace);
		}

		const float GetPedFaceFeature(Objects::Entity entity, const int feature)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetModelFaceFeature(feature);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetModelFaceFeature(feature);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetPedFaceFeature] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::GetPedFaceFeature] Invalid entity.", ThisNamespace);
		}

		void SetPedFaceFeature(Objects::Entity entity, const int index, const float scale)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(GrandM::EntityType::Player);
						bitstream.Write(index);
						bitstream.Write(scale);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedFaceFeature", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Players[i].SetModelFaceFeature(index, scale);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(GrandM::EntityType::NPC);
						bitstream.Write(index);
						bitstream.Write(scale);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedFaceFeature", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Npcs[i].SetModelFaceFeature(index, scale);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetPedFaceFeature] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::SetPedFaceFeature] Invalid entity.", ThisNamespace);
		}

		const float GetViewDistance(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetViewDistance();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetViewDistance();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetViewDistance();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].GetViewDistance();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetViewDistance();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetViewDistance();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetViewDistance();
					}
				}
				break;
			}
			LOG_WARNING("[%s::GetViewDistance] Invalid entity.", ThisNamespace);
		}

		void SetViewDistance(Objects::Entity entity, const float distance)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].SetViewDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetViewDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].SetViewDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetViewDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].SetViewDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetViewDistance(distance);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetViewDistance] Invalid entity.", ThisNamespace);
				break;
			}
		}

		Objects::Entity * GetEntities(const int type)
		{
			std::vector<Objects::Entity> Entities;

			Objects::Entity ent;

			switch (type)
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					ent.SetID(g_Players[i].GetID());
					ent.SetType(GrandM::EntityType::Player);

					Entities.push_back(ent);
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					ent.SetID(g_Npcs[i].GetID());
					ent.SetType(GrandM::EntityType::NPC);

					Entities.push_back(ent);
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					ent.SetID(veh.GetID());
					ent.SetType(GrandM::EntityType::Vehicle);

					Entities.push_back(ent);
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					ent.SetID(g_Objects[i].GetID());
					ent.SetType(GrandM::EntityType::Object);

					Entities.push_back(ent);
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					ent.SetID(g_Checkpoints[i].GetID());
					ent.SetType(GrandM::EntityType::Checkpoint);

					Entities.push_back(ent);
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					ent.SetID(g_Pickups[i].GetID());
					ent.SetType(GrandM::EntityType::Pickup);

					Entities.push_back(ent);
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					ent.SetID(g_Blips[i].GetID());
					ent.SetType(GrandM::EntityType::Blip);

					Entities.push_back(ent);
				}
				break;
			default:
				LOG_WARNING("[%s::GetEntities] Invalid type.", ThisNamespace);
				break;
			}
			return Entities.data();
		}

		const char * GetNametag(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetNametag().c_str();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetNametag().c_str();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetNametag().c_str();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetNametag().c_str();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetNametag().c_str();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetNametag().c_str();
					}
				}
				break;
			}

			LOG_WARNING("[%s::GetNametag] Invalid entity.", ThisNamespace);
		}

		void SetNametag(Objects::Entity entity, const char * nametag)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].SetNametag(nametag);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].SetNametag(nametag);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetNametag(nametag);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetNametag(nametag);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].SetNametag(nametag);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetNametag(nametag);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetNametag] Invalid entity.", ThisNamespace);
				break;
			}
		}

		const bool IsNametagVisible(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].IsNametagVisible();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].IsNametagVisible();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.IsNametagVisible();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].IsNametagVisible();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].IsNametagVisible();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].IsNametagVisible();
					}
				}
				break;
			}

			LOG_WARNING("[%s::IsNametagVisible] Invalid entity.", ThisNamespace);
		}

		void SetNametagVisible(Objects::Entity entity, const bool visible)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].SetNametagVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].SetNametagVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetNametagVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetNametagVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].SetNametagVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetNametagVisible(visible);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetNametagVisible] Invalid entity.", ThisNamespace);
				break;
			}
		}

		const float GetNametagDistance(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetNametagDistance();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetNametagDistance();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetNametagDistance();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetNametagDistance();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetNametagDistance();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetNametagDistance();
					}
				}
				break;
			}

			LOG_WARNING("[%s::GetNametagDistance] Invalid entity.", ThisNamespace);
		}

		void SetNametagDistance(Objects::Entity entity, const float distance)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].SetNametagDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].SetNametagDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetNametagDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetNametagDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].SetNametagDistance(distance);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetNametagDistance(distance);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetNametagDistance] Invalid entity.", ThisNamespace);
				break;
			}
		}

		const int GetHealth(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetHealth();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetHealth();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetHealth();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetHealth();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetHealth] Only Objects, Vehicles, NPCs and Players have health.", ThisNamespace);
				break;
			}

			LOG_WARNING("[%s::GetHealth] Invalid entity.", ThisNamespace);
		}

		void SetHealth(Objects::Entity entity, const int health)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].SetHealth(health, RakNet::UNASSIGNED_RAKNET_GUID);
						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].SetHealth(health, RakNet::UNASSIGNED_RAKNET_GUID);
						return;
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetHealth(health, RakNet::UNASSIGNED_RAKNET_GUID);
						return;
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetHealth(health, RakNet::UNASSIGNED_RAKNET_GUID);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("%sSetHealth Entity %s is not a Player, NPC, Object or Vehicle.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("%sSetHealth Entity %s invalid.", ThisNamespace, entity.GetID());
		}

		const int GetArmour(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetArmour();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetArmour();
					}
				}
				break;
			default:
				LOG_WARNING("%sGetArmour Entity %s is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("%sGetArmour Entity %s invalid.", ThisNamespace, entity.GetID());
		}

		void SetArmour(Objects::Entity entity, const int armour)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						bitstream.Write(armour);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetArmour", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Players[i].SetArmour(armour, RakNet::UNASSIGNED_RAKNET_GUID);
						return;
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						bitstream.Write(armour);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetArmour", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						g_Npcs[i].SetArmour(armour, RakNet::UNASSIGNED_RAKNET_GUID);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("%sSetArmour Entity %s is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("%sSetArmour Entity %s invalid.", ThisNamespace, entity.GetID());
		}

		const bool IsHealthbarVisible(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].IsHealthbarVisible();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].IsHealthbarVisible();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.IsHealthbarVisible();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].IsHealthbarVisible();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].IsHealthbarVisible();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].IsHealthbarVisible();
					}
				}
				break;
			}

			LOG_WARNING("[%s::IsHealthbarVisible] Invalid entity.", ThisNamespace);
		}

		void SetHealthbarVisible(Objects::Entity entity, const bool visible)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].SetHealthbarVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].SetHealthbarVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetHealthbarVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetHealthbarVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].SetHealthbarVisible(visible);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetHealthbarVisible(visible);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetHealthbarVisible] Invalid entity.", ThisNamespace);
				break;
			}
		}

		const Color GetNametagColor(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetNametagColor();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetNametagColor();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetNametagColor();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetNametagColor();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetNametagColor();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetNametagColor();
					}
				}
				break;
			}

			LOG_WARNING("[%s::GetNametagColor] Invalid entity.", ThisNamespace);
		}

		void SetNametagColor(Objects::Entity entity, const Color color)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						g_Players[i].SetNametagColor(color);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						g_Npcs[i].SetNametagColor(color);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						veh.SetNametagColor(color);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetNametagColor(color);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						g_Checkpoints[i].SetNametagColor(color);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						g_Pickups[i].SetNametagColor(color);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetNametagColor] Invalid entity.", ThisNamespace);
				break;
			}
		}

		void Revive(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("Revive", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Players[i].Revive();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(entity.GetType());
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("Revive", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

						return g_Npcs[i].Revive();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::Revive] Entity %i is not a Player or NPC.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::Revive] Invalid entity.", ThisNamespace);
		}

		const bool IsAlive(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return !g_Players[i].IsDead();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return !g_Npcs[i].IsDead();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return !veh.IsDead();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return !g_Objects[i].IsDead();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::IsAlive] Entity %i is not a Player, NPC, Vehicle or Object.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%s::IsAlive] Invalid entity.", ThisNamespace);
		}

		void SetDimension(Objects::Entity entity, const int dim)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].SetDimension(dim);
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].SetDimension(dim);
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.SetDimension(dim);
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].SetDimension(dim);
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].SetDimension(dim);
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].SetDimension(dim);
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].SetDimension(dim);
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetDimension]  %i is of an Unknown type....", ThisNamespace, entity.GetID());
				break;
			}

			LOG_WARNING("[%s::SetDimension] Invalid entity.", ThisNamespace);
		}

		const int GetDimension(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].GetDimension();
					}
				}
				break;
			case GrandM::EntityType::NPC:
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i] == entity)
					{
						return g_Npcs[i].GetDimension();
					}
				}
				break;
			case GrandM::EntityType::Vehicle:
				for (auto &veh : g_Vehicles)
				{
					if (veh == entity)
					{
						return veh.GetDimension();
					}
				}
				break;
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetDimension();
					}
				}
				break;
			case GrandM::EntityType::Pickup:
				for (unsigned int i = 0; i < g_Pickups.size(); i++)
				{
					if (g_Pickups[i] == entity)
					{
						return g_Pickups[i].GetDimension();
					}
				}
				break;
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == entity)
					{
						return g_Checkpoints[i].GetDimension();
					}
				}
				break;
			case GrandM::EntityType::Blip:
				for (unsigned int i = 0; i < g_Blips.size(); i++)
				{
					if (g_Blips[i] == entity)
					{
						return g_Blips[i].GetDimension();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetDimension]  %i is of an Unknown type....", ThisNamespace, entity.GetID());
				break;
			}

			LOG_WARNING("[%s::GetDimension] Invalid entity.", ThisNamespace);
		}
	}
}
