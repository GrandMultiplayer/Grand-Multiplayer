#ifndef __CRPCBlip_H__
#define __CRPCBlip_H__

class CRPCBlip
{
public:
	static void CreateE(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void CreateP(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetColor(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void Show(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void Hide(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetSprite(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetText(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetBlipShortRange(RakNet::BitStream *bitStream, RakNet::Packet *packet);
};

#endif