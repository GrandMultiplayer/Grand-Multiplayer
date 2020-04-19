#ifndef __RPCENTITY_H__
#define __RPCENTITY_H__
#include <BitStream.h>

namespace RPC
{
	namespace Entity
	{
		void SetPhysicsAssignment(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void OnEntityEnterCheckpoint(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnEntityExitCheckpoint(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnCefFinishLoad(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnCefSendData(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnEntityDamage(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnProjectileImpact(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void SetPedHeadBlend(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void SetPedHeadoverlay(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void SetPedProp(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	}
}
#endif