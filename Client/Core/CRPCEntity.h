#ifndef __CRPCENTITY_H__
#define __CRPCENTITY_H__

class CRPCEntity
{
public:
	static void Destroy(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetPosition(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetRotation(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetPhysicsAssignment(RakNet::BitStream *bitStream, RakNet::Packet *packet);

	static void PedComponent(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void PedHeadBlend(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void PedHeadOverlay(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void PedProp(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void PedFaceFeature(RakNet::BitStream *bitStream, RakNet::Packet *packet);

	static void SetViewDistance(RakNet::BitStream *bitStream, RakNet::Packet *packet);

	static void SetNametag(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetNametagVisible(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetHealthbarVisible(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetNametagDistance(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetNametagColor(RakNet::BitStream *bitStream, RakNet::Packet *packet);

	static void SetHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetArmour(RakNet::BitStream *bitStream, RakNet::Packet *packet);

	static void Revive(RakNet::BitStream *bitStream, RakNet::Packet *packet);

	static void SetDimension(RakNet::BitStream *bitStream, RakNet::Packet *packet);
};

#endif