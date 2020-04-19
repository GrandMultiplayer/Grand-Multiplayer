#ifndef __CRPCPICKUP_H__
#define __CRPCPICKUP_H__

class CRPCPickup
{
public:
	static void Create(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetPickupVisible(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetPickupModel(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetPickupRespawnDelay(RakNet::BitStream *bitStream, RakNet::Packet *packet);
};

#endif