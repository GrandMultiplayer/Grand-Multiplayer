// Self
#include "API_Camera.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Common.h"
#include "Server.h"
#include "CPlayer.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace Camera
	{
		const char * ThisNamespace = "API::Camera::";

		void SetCameraPosition(Objects::Entity entity, const CVector3 Position)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(Position.x);
						bitstream.Write(Position.y);
						bitstream.Write(Position.z);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCameraPosition", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetCameraPosition] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void SetCameraRotation(Objects::Entity entity, const CVector3 Rotation)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(Rotation.x);
						bitstream.Write(Rotation.y);
						bitstream.Write(Rotation.z);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCameraRotation", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetCameraRotation] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void ResetCamera(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ResetCamera", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sResetCamera] %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void FirstPersonOnly(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("FirstPersonOnly", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sFirstPersonOnly] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void ThirdPersonOnly(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ThirdPersonOnly", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sThirdPersonOnly] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
		}

		void AttachCamera(Objects::Entity player, Objects::Entity entity, const CVector3 offset)
		{
			switch (player.GetType())
			{
			case GrandM::EntityType::Player:
				switch (entity.GetType())
				{
				case GrandM::EntityType::Player:
				case GrandM::EntityType::NPC:
				case GrandM::EntityType::Vehicle:
				case GrandM::EntityType::Object:
					for (unsigned int i = 0; i < g_Players.size(); i++)
					{
						if (g_Players[i] == player)
						{
							RakNet::BitStream bitstream;
							bitstream.Write(entity.GetID());
							bitstream.Write(entity.GetType());
							bitstream.Write(offset.x);
							bitstream.Write(offset.y);
							bitstream.Write(offset.z);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("AttachCamera", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
							return;
						}
					}
					break;
				default:
					LOG_WARNING("[%sAttachCamera] Entity %i is not of type Player, NPC, Vehicle or Object.", ThisNamespace, entity.GetID());
					break;
				}
				break;
			default:
				LOG_WARNING("[%sAttachCamera] Entity 'player' %i is not of type Player.", ThisNamespace, player);
				break;
			}
		}

		void DetachCamera(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DetachCamera", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sDetachCamera] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
		}

		const bool IsCameraActive(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return g_Players[i].IsScriptedCameraActive();
					}
				}
				break;
			default:
				LOG_WARNING("[%sIsCameraActive] Entity %i is not of type Player.", ThisNamespace, entity.GetID());
				break;
			}
		}
	}
}