

// GrandM
#include "Server.h"
#include "CVehicle.h"

// Self
#include "RPCVehicle.h"

namespace RPC
{
	namespace Vehicle
	{
		void Inited(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int id = -1;

			bitStream->Read(id);

			if (!g_Vehicles.empty())
			{
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetID() == id)
					{
						g_Vehicles[i].SetNeedsInit(false);
						return;
					}
				}
			}
		}

		void SetExtra(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int id, extra;
			bool state;

			bitStream->Read(id);
			bitStream->Read(extra);
			bitStream->Read(state);

			if (!g_Vehicles.empty())
			{
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetID() == id)
					{
						g_Vehicles[i].SetupExtra(extra, state);
						return;
					}
				}
			}
		}

		void TowVehicle(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int id, towedId;

			bitStream->Read(id);
			bitStream->Read(towedId);

			if (!g_Vehicles.empty())
			{
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetID() == id)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(id);
						bitstream.Write(towedId);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("TowVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

						return g_Vehicles[i].SetTowedVehicle(towedId);
					}
				}
			}
		}

		void DeployCargobobRope(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int id, ropeType;

			bitStream->Read(id);
			bitStream->Read(ropeType);

			if (!g_Vehicles.empty())
			{
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetID() == id)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(id);
						bitstream.Write(ropeType);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DeployCargobobRope", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);

						return g_Vehicles[i].SetCargobobRopeType(ropeType);
					}
				}
			}
		}
	}
}