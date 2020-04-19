// System

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Common.h"
#include "Server.h"
#include "CWorld.h"
#include "CPlayer.h"
#include "ModelCache.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "API_World.h"

namespace API
{
	namespace World
	{
		const char * ThisNamespace = "API::World::";

		void GetTime(int* hour, int* minute, int* second)
		{
			*hour = CWorld::GetTime().hour;
			*minute = CWorld::GetTime().minute;
			*second = CWorld::GetTime().second;
		}

		void SetTime(const int hour, const int minute, const int second)
		{
			CWorld::SetTime(hour, minute, second);
		}

		const char * GetWeather()
		{
			return CWorld::GetWeather().Weather.c_str();
		}

		void SetWeather(const char * weather)
		{
			CWorld::SetWeather(weather);
		}

		void LoadIPL(const char * ipl)
		{
			CWorld::LoadIPL(ipl);
		}

		void LoadIPLForPlayer(Objects::Entity entity, const char * ipl)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(RakNet::RakString(ipl));
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("LoadIPL", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						break;
					}
				}

				break;
			default:
				LOG_WARNING("[%sLoadIPLForPlayer] This function can only be used on players.", ThisNamespace);
				break;
			}
		}

		void UnloadIPL(const char * ipl)
		{
			CWorld::UnloadIPL(ipl);
		}

		void UnloadIPLForPlayer(Objects::Entity entity, const char * ipl)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(RakNet::RakString(ipl));
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("UnloadIPL", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						break;
					}
				}

				break;
			default:
				LOG_WARNING("[%sUnloadIPLForPlayer] This function can only be used on players.", ThisNamespace);
				break;
			}
		}


		void SetBlackout(const bool state)
		{
			CWorld::SetBlackout(state);
		}

		void SetBlackoutForPlayer(Objects::Entity entity, const bool state)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(state);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlackout", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						break;
					}
				}

				break;
			default:
				LOG_WARNING("[%sSetBlackoutForPlayer] This function can only be used on players.", ThisNamespace);
				break;
			}
		}

		const bool IsBlackout()
		{
			return CWorld::IsBlackout();
		}

		void SetGroundSnow(const bool state)
		{
			CWorld::SetGroundSnow(state);
		}

		void SetGroundSnowForPlayer(Objects::Entity entity, const bool state)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(state);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetGroundSnow", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
						break;
					}
				}

				break;
			default:
				LOG_WARNING("[%sSetGroundSnowForPlayer] This function can only be used on players.", ThisNamespace);
				break;
			}
		}

		const bool IsGroundSnow()
		{
			return CWorld::IsGroundSnow();
		}
	}
}