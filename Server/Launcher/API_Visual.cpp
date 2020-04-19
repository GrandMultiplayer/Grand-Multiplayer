// System

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Common.h"
#include "Server.h"
#include "CPlayer.h"
#include "ModelCache.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "API_Visual.h"

namespace API
{
	namespace Visual
	{
		const char * ThisNamespace = "API::Visual::";

		void ShowNotification(const char * message, const char * pic, const int icontype, const int color, const char * sender, const char * subject)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakString(message));
			bitstream.Write(RakNet::RakString(pic));
			bitstream.Write(icontype);
			bitstream.Write(color);
			bitstream.Write(RakNet::RakString(sender));
			bitstream.Write(RakNet::RakString(subject));

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowMessageAboveMap", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
		}

		void ShowNotificationToPlayer(Objects::Entity entity, const char * message, const char * pic, const int icontype, const int color, const char * sender, const char * subject)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakString(message));
			bitstream.Write(RakNet::RakString(pic));
			bitstream.Write(icontype);
			bitstream.Write(color);
			bitstream.Write(RakNet::RakString(sender));
			bitstream.Write(RakNet::RakString(subject));

			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowMessageAboveMap", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
					return;
				}
			}

			LOG_WARNING("[%sShowNotificationToPlayer] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		void ShowSubtitle(const char * message, const int duration, const bool shownow)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakString(message));
			bitstream.Write(duration);
			bitstream.Write(shownow);

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowSubtitle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
		}

		void ShowSubtitleToPlayer(Objects::Entity entity, const char * message, const int duration, const bool shownow)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakString(message));
			bitstream.Write(duration);
			bitstream.Write(shownow);

			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowSubtitle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
					return;
				}
			}

			LOG_WARNING("[%sShowSubtitleToPlayer] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		void SendChatMessage(const wchar_t * message)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakWString(message));

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SendMessage", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
		}

		void SendChatMessageToPlayer(Objects::Entity entity, const wchar_t * message)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakWString(message));

			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SendMessage", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
					return;
				}
			}

			LOG_WARNING("[%sSendChatMessageToPlayer] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		void ShowCursor(Objects::Entity entity, bool show)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(show);

			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ShowCursor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
					return;
				}
			}

			LOG_WARNING("[%sShowCursor] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		const bool IsHudHidden(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return !g_Players[i].IsHudDisplayed();
					}
				}
				break;
			default:
				LOG_WARNING("[%sIsHudHidden] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sIsHudHidden] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		void DisplayHud(Objects::Entity entity, bool show)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(show);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DisplayHud", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

						return g_Players[i].DisplayHud(show);
					}
				}
				break;
			default:
				LOG_WARNING("[%sDisplayHud] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sDisplayHud] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		const bool IsRadarHidden(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return !g_Players[i].IsMinimapDisplayed();
					}
				}
				break;
			default:
				LOG_WARNING("[%sIsRadarHidden] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sIsRadarHidden] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		void DisplayRadar(Objects::Entity entity, bool show)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(show);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DisplayMinimap", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

						return g_Players[i].DisplayMinimap(show);
					}
				}
				break;
			default:
				LOG_WARNING("[%sDisplayRadar] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sDisplayRadar] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		const bool IsCashHidden(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return !g_Players[i].IsCashDisplayed();
					}
				}
				break;
			default:
				LOG_WARNING("[%sIsCashHidden] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sIsCashHidden] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		void DisplayCash(Objects::Entity entity, bool show)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(!show);

						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DisplayCash", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

						return g_Players[i].DisplayCash(show);
					}
				}
				break;
			default:
				LOG_WARNING("[%sDisplayCash] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sDisplayCash] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		const bool IsAmmoHidden(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						return !g_Players[i].IsAmmoDisplayed();
					}
				}
				break;
			default:
				LOG_WARNING("[%sIsAmmoHidden] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sIsAmmoHidden] Invalid Entity %i", ThisNamespace, entity.GetID());
		}

		void DisplayAmmo(Objects::Entity entity, bool show)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Player:
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i] == entity)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(show);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DisplayAmmo", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);

						return g_Players[i].DisplayAmmo(show);
					}
				}
				break;
			default:
				LOG_WARNING("[%sDisplayAmmo] Entity %i is not a player.", ThisNamespace, entity.GetID());
				break;
			}
			LOG_WARNING("[%sDisplayAmmo] Invalid Entity %i", ThisNamespace, entity.GetID());
		}
	}
}
