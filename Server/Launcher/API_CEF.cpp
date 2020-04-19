// Self
#include "API_CEF.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Server.h"
#include "CPlayer.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace CEF
	{
		const char * ThisNamespace = "API::CEF::";

		void LoadURL(Objects::Entity entity, const char * frame, const char * url, const char * appcode, bool remote)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakString(frame));
			bitstream.Write(RakNet::RakString(url));
			bitstream.Write(RakNet::RakString(appcode));
			bitstream.Write(remote);

			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("LoadURL", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
					return;
				}
			}

			LOG_WARNING("[%sLoadURL] Invalid entity %i", ThisNamespace, entity.GetID());
			return;
		}

		void ExecuteJavaScript(Objects::Entity entity, const char * frame, const char * call)
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i] == entity)
				{
					RakNet::BitStream bitstream;
					bitstream.Write(RakNet::RakString(frame));
					bitstream.Write(RakNet::RakString(call));
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("ExecuteJavaScript", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_Players[i].GetGUID(), false, false);
					return;
				}
			}
			LOG_WARNING("[%sExecuteJavaScript] Invalid entity %i", ThisNamespace, entity.GetID());
		}
	}
}