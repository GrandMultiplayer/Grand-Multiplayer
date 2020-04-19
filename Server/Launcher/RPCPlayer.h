#ifndef __RPCPLAYER_H__
#define __RPCPLAYER_H__
#include <BitStream.h>

namespace RPC
{
	namespace Player
	{
		void InvalidWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void UpdateWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnPlayerShot(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnPlayerAim(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void OnPlayerPickup(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	}
}

#endif // __RPCPLAYER_H__