// GrandM
// Self
#include "RPCEntity.h"

//#include "Common.h"
#include "Server.h"
#include "CPlayer.h"
#include "CNPC.h"
#include "CVehicle.h"
#include "CObject.h"

// API
#include "CAPI.h"
#include "API_Entity.h"
#include "API_Callback_Checkpoint.h"
#include "API_Callback_Cef.h"
#include "API_Callback_Entity.h"

#include "Log.h"

namespace RPC
{
	namespace Entity
	{
		void SetPhysicsAssignment(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int entityid = -1, type = -1;
			RakNet::RakNetGUID owner = RakNet::UNASSIGNED_RAKNET_GUID;

			bitStream->Read(entityid);
			bitStream->Read(type);
			bitStream->Read(owner);

			if (entityid == -1 || type == -1)
				return;

			switch (type)
			{
			case GrandM::EntityType::Vehicle:
			{
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetID() == entityid)
					{
						g_Vehicles[i].SetAssignee(owner);
						break;
					}
				}
				break;
			}
			case GrandM::EntityType::Object:
			{
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i].GetID() == entityid)
					{
						g_Objects[i].SetAssignee(owner);
						break;
					}
				}
				break;
			}
			case GrandM::EntityType::NPC:
			{
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i].GetID() == entityid)
					{
						g_Npcs[i].SetAssignee(owner);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void OnEntityEnterCheckpoint(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int checkpointid, entityid;

			bitStream->Read(checkpointid);
			bitStream->Read(entityid);

			CEntity* checkpoint = CEntity::GetEntityFromID(checkpointid);
			CEntity* entity = CEntity::GetEntityFromID(entityid);

			if (!checkpoint->IsNull() && !entity->IsNull())
			{
				for (unsigned int i = 0; i < g_ApiModules.size(); i++)
				{
					void *Instance = g_ApiModules[i].GetInstance();
					API::Checkpoints::OnEntityEnterCheckpoint(Instance, *checkpoint, *entity);
				}
			}

			entity = checkpoint = nullptr;
		}

		void OnEntityExitCheckpoint(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int checkpointid, entityid;

			bitStream->Read(checkpointid);
			bitStream->Read(entityid);

			CEntity* checkpoint = CEntity::GetEntityFromID(checkpointid);
			CEntity* entity = CEntity::GetEntityFromID(entityid);

			if (!checkpoint->IsNull() && !entity->IsNull())
			{
				for (unsigned int i = 0; i < g_ApiModules.size(); i++)
				{
					void *Instance = g_ApiModules[i].GetInstance();
					API::Checkpoints::OnEntityExitCheckpoint(Instance, *checkpoint, *entity);
				}
			}

			entity = checkpoint = nullptr;
		}

		void OnCefFinishLoad(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			for (unsigned int p = 0; p < g_Players.size(); p++)
			{
				if (g_Players[p].GetGUID() == packet->guid)
				{
					for (int i = 0; i < g_ApiModules.size(); i++)
					{
						void *Instance = g_ApiModules[i].GetInstance();
						API::Cef::OnCefFinishLoad(Instance, g_Players[p]);
					}
					return;
				}
			}
		}

		void OnCefSendData(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			RakNet::RakString data;
			bitStream->Read(data);

			for (unsigned int p = 0; p < g_Players.size(); p++)
			{
				if (g_Players[p].GetGUID() == packet->guid)
				{
					for (unsigned int i = 0; i < g_ApiModules.size(); i++)
					{
						void *Instance = g_ApiModules[i].GetInstance();
						API::Cef::OnCefSendData(Instance, g_Players[p], data.C_String());
					}
					return;
				}
			}
		}

		void OnEntityDamage(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int entityid = -1, damage = 0, attackerid = -1;
			RakNet::RakString weapon;

			bitStream->Read(entityid);
			bitStream->Read(damage);
			bitStream->Read(attackerid);
			bitStream->Read(weapon);

			LOG_PRINT("RPC::Entity::OnEntityDamage <%i, %i, %i, %s>", entityid, damage, attackerid, weapon.C_String());

			CEntity* entity = CEntity::GetEntityFromID(entityid);
			CEntity* attacker = CEntity::GetEntityFromID(attackerid);

			if (!entity->IsNull() && !attacker->IsNull())
			{
				bool sendDamage = true;

				for (unsigned int i = 0; i < g_ApiModules.size(); i++)
				{
					void *Instance = g_ApiModules[i].GetInstance();

					if (!API::Callback::Entity::OnEntityDamage(Instance, *entity, damage, *attacker, weapon.C_String()))
						sendDamage = false;
				}

				// ##server#redo##001##
				// If a single plugin returns false then no damage is automaticly handled by the server.
				//if (sendDamage)
					//API::Entity::SetHealth(*entity, entity->GetHealth() - damage);

				entity->SetLastAttacker(attacker->GetID());
			}

			entity = attacker = nullptr;
		}

		void OnProjectileImpact(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int playerid, entityid, bone;
			RakNet::RakString weapon;
			CVector3 position;

			bitStream->Read(playerid);
			bitStream->Read(weapon);
			bitStream->Read(position.x);
			bitStream->Read(position.y);
			bitStream->Read(position.z);
			bitStream->Read(entityid);
			bitStream->Read(bone);

			CEntity* player = CEntity::GetEntityFromID(playerid);
			CEntity* entity = CEntity::GetEntityFromID(entityid);

			if (!player->IsNull() && !entity->IsNull())
			{
				for (unsigned int i = 0; i < g_ApiModules.size(); i++)
				{
					void *Instance = g_ApiModules[i].GetInstance();

					API::Callback::Entity::OnProjectileImpact(Instance, *player, weapon.C_String(), position, *entity, bone);
				}
			}

			entity = player = nullptr;
		}

		void SetPedHeadBlend(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int entityid;
			GrandM::Model::HeadBlend headblend;

			bitStream->Read(entityid);
			bitStream->Read(headblend.shapeFirst);
			bitStream->Read(headblend.shapeMix);
			bitStream->Read(headblend.shapeSecond);
			bitStream->Read(headblend.shapeThird);
			bitStream->Read(headblend.skinFirst);
			bitStream->Read(headblend.skinMix);
			bitStream->Read(headblend.skinSecond);
			bitStream->Read(headblend.skinThird);
			bitStream->Read(headblend.thirdMix);

			switch (CEntity::GetEntityTypeFromID(entityid))
			{
			case GrandM::EntityType::Player:
			{
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetID() == entityid)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(headblend.shapeFirst);
						bitstream.Write(headblend.shapeMix);
						bitstream.Write(headblend.shapeSecond);
						bitstream.Write(headblend.shapeThird);
						bitstream.Write(headblend.skinFirst);
						bitstream.Write(headblend.skinMix);
						bitstream.Write(headblend.skinSecond);
						bitstream.Write(headblend.skinThird);
						bitstream.Write(headblend.thirdMix);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadBlend", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

						g_Players[i].SetModelHeadBlend(headblend);
						break;
					}
				}
				break;
			}
			case GrandM::EntityType::NPC:
			{
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i].GetID() == entityid)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(headblend.shapeFirst);
						bitstream.Write(headblend.shapeMix);
						bitstream.Write(headblend.shapeSecond);
						bitstream.Write(headblend.shapeThird);
						bitstream.Write(headblend.skinFirst);
						bitstream.Write(headblend.skinMix);
						bitstream.Write(headblend.skinSecond);
						bitstream.Write(headblend.skinThird);
						bitstream.Write(headblend.thirdMix);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadBlend", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

						g_Npcs[i].SetModelHeadBlend(headblend);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void SetPedHeadoverlay(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int entityid, overlayid;
			GrandM::Model::HeadOverlay headoverlay;

			bitStream->Read(entityid);
			bitStream->Read(overlayid);
			bitStream->Read(headoverlay.index);
			bitStream->Read(headoverlay.colorType);
			bitStream->Read(headoverlay.colorID);
			bitStream->Read(headoverlay.secondColorID);
			bitStream->Read(headoverlay.opacity);

			CEntity* entity = CEntity::GetEntityFromID(entityid);

			switch (entity->GetType())
			{
			case GrandM::EntityType::Player:
			{
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == *entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(overlayid);
						bitstream.Write(headoverlay.index);
						bitstream.Write(headoverlay.colorType);
						bitstream.Write(headoverlay.colorID);
						bitstream.Write(headoverlay.secondColorID);
						bitstream.Write(headoverlay.opacity);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadOverlay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

						g_Players[i].SetModelHeadOverlay(overlayid, headoverlay);
						break;
					}
				}
				break;
			}
			case GrandM::EntityType::NPC:
			{
				for (unsigned int i = 0; i < g_Npcs.size(); i++)
				{
					if (g_Npcs[i].GetID() == entityid)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(g_Npcs[i].GetID());
						bitstream.Write(overlayid);
						bitstream.Write(headoverlay.index);
						bitstream.Write(headoverlay.colorType);
						bitstream.Write(headoverlay.colorID);
						bitstream.Write(headoverlay.secondColorID);
						bitstream.Write(headoverlay.opacity);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadOverlay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

						g_Npcs[i].SetModelHeadOverlay(overlayid, headoverlay);
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		void SetPedProp(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int entityid, componentid;
			GrandM::Model::Prop prop;

			bitStream->Read(entityid);
			bitStream->Read(componentid);
			bitStream->Read(prop.drawableid);
			bitStream->Read(prop.textureid);

			CEntity* entity = CEntity::GetEntityFromID(entityid);

			if (!entity->IsNull())
			{
				switch (entity->GetType())
				{
				case GrandM::EntityType::Player:
				{
					for (unsigned int i = 0; i < g_Players.size(); i++)
					{
						if (g_Players[i] == *entity)
						{
							RakNet::BitStream bitstream;
							bitstream.Write(g_Players[i].GetID());
							bitstream.Write(componentid);
							bitstream.Write(prop.drawableid);
							bitstream.Write(prop.textureid);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedProp", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

							g_Players[i].SetModelProp(componentid, prop);
							break;
						}
					}
					break;
				}
				case GrandM::EntityType::NPC:
				{
					for (unsigned int i = 0; i < g_Npcs.size(); i++)
					{
						if (g_Npcs[i].GetID() == entityid)
						{
							RakNet::BitStream bitstream;
							bitstream.Write(g_Npcs[i].GetID());
							bitstream.Write(componentid);
							bitstream.Write(prop.drawableid);
							bitstream.Write(prop.textureid);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedProp", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

							g_Npcs[i].SetModelProp(componentid, prop);
							break;
						}
					}
					break;
				}
				default:
					break;
				}
			}

			entity = nullptr;
		}
	}
}
