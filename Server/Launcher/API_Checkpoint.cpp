// Self
#include "API_Checkpoint.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Server.h"
#include "CPlayer.h"
#include "CCheckpoint.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace Checkpoint
	{
		const char * ThisNamespace = "API::Checkpoint";

		Objects::Entity CreateCheckpoint(const CVector3 position, const float radius, const Color color, const int reserved)
		{
			CCheckpoint checkpoint(position, radius, color, reserved);

			Objects::Entity ent;
			ent.SetID(checkpoint.GetID());
			ent.SetType(checkpoint.GetType());

			g_Checkpoints.push_back(checkpoint);

			RakNet::BitStream bitstream;
			bitstream.Write(checkpoint.GetID());
			bitstream.Write(position.x);
			bitstream.Write(position.y);
			bitstream.Write(position.z);
			bitstream.Write(radius);
			bitstream.Write(color.Red);
			bitstream.Write(color.Green);
			bitstream.Write(color.Blue);
			bitstream.Write(color.Alpha);
			bitstream.Write(reserved);

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

			return ent;
		}

		void ShowCheckpoint(Objects::Entity checkpointentity, Objects::Entity playerentity)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				if (playerentity.GetID() != -1)
				{
					switch (playerentity.GetType())
					{
					case GrandM::EntityType::Player:
						for (unsigned int i = 0; i < g_Players.size(); i++)
						{
							if (g_Players[i] == playerentity)
							{
								RakNet::BitStream bitstream;
								bitstream.Write(checkpointentity.GetID());
								Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
								return;
							}
						}
						break;
					default:
						LOG_WARNING("[%s::ShowCheckpoint] The 'playerentity' is not a player.", ThisNamespace);
						break;
					}
				}
				else
				{
					RakNet::BitStream bitstream;
					bitstream.Write(checkpointentity.GetID());
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
				}
				break;
			default:
				LOG_WARNING("[%s::ShowCheckpoint] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		void HideCheckpoint(Objects::Entity checkpointentity, Objects::Entity playerentity)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				if (playerentity.GetID() != -1)
				{
					switch (playerentity.GetType())
					{
					case GrandM::EntityType::Player:
						for (unsigned int i = 0; i < g_Players.size(); i++)
						{
							if (g_Players[i] == playerentity)
							{
								RakNet::BitStream bitstream;
								bitstream.Write(checkpointentity.GetID());
								Server::GetServer()->GetNetworkManager()->GetRPC().Signal("HideCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
								return;
							}
						}
						break;
					default:
						LOG_WARNING("[%s::HideCheckpoint] The 'playerentity' is not a player.", ThisNamespace);
						break;
					}
				}
				else
				{
					RakNet::BitStream bitstream;
					bitstream.Write(checkpointentity.GetID());
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("HideCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
				}
				break;
			default:
				LOG_WARNING("[%s::HideCheckpoint] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		const float GetNearHeight(Objects::Entity checkpointentity)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						return g_Checkpoints[i].GetNearHeight();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetNearHeight] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		void SetNearHeight(Objects::Entity checkpointentity, const float height)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						g_Checkpoints[i].SetNearHeight(height);

						RakNet::BitStream bitstream;
						bitstream.Write(checkpointentity.GetID());
						bitstream.Write(height);
						bitstream.Write(g_Checkpoints[i].GetFarHeight());

						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCheckpointHeight", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetNearHeight] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		const float GetFarHeight(Objects::Entity checkpointentity)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						return g_Checkpoints[i].GetFarHeight();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetFarHeight] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		void SetFarHeight(Objects::Entity checkpointentity, const float height)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						g_Checkpoints[i].SetFarHeight(height);

						RakNet::BitStream bitstream;
						bitstream.Write(checkpointentity.GetID());
						bitstream.Write(g_Checkpoints[i].GetNearHeight());
						bitstream.Write(height);

						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCheckpointHeight", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetFarHeight] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		const CVector3 GetPointTo(Objects::Entity checkpointentity)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						return g_Checkpoints[i].GetPointTo();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetPointTo] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		void PointTo(Objects::Entity checkpointentity, const CVector3 pointToPos)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						g_Checkpoints[i].PointTo(pointToPos);

						RakNet::BitStream bitstream;
						bitstream.Write(checkpointentity.GetID());
						bitstream.Write(pointToPos.x);
						bitstream.Write(pointToPos.y);
						bitstream.Write(pointToPos.z);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CheckpointPointTo", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::PointTo] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		const int GetCheckpointType(Objects::Entity checkpointentity)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						return g_Checkpoints[i].GetType();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetCheckpointType] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		void SetCheckpointType(Objects::Entity checkpointentity, const int type)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						g_Checkpoints[i].SetType(type);

						RakNet::BitStream bitstream;
						bitstream.Write(checkpointentity.GetID());
						bitstream.Write(type);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCheckpointType", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetCheckpointType] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		const Color GetCheckpointColor(Objects::Entity checkpointentity)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						return g_Checkpoints[i].GetColor();
					}
				}
				break;
			default:
				LOG_WARNING("[%s::GetCheckpointColor] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}

		void SetCheckpointColor(Objects::Entity checkpointentity, const Color color)
		{
			switch (checkpointentity.GetType())
			{
			case GrandM::EntityType::Checkpoint:
				for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
				{
					if (g_Checkpoints[i] == checkpointentity)
					{
						g_Checkpoints[i].SetColor(color);

						RakNet::BitStream bitstream;
						bitstream.Write(checkpointentity.GetID());
						bitstream.Write(color.Red);
						bitstream.Write(color.Green);
						bitstream.Write(color.Blue);
						bitstream.Write(color.Alpha);

						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetCheckpointColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%s::SetCheckpointColor] The 'checkpointentity' is not a checkpoint.", ThisNamespace);
				break;
			}
		}
	}
}