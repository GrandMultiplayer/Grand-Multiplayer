#ifndef __RPCVEHICLE_H__
#define __RPCVEHICLE_H__
#include <BitStream.h>

namespace RPC
{
	namespace Vehicle
	{
		void Inited(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void SetExtra(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void TowVehicle(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void DeployCargobobRope(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	}
}

#endif