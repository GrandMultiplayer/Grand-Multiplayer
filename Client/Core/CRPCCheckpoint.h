#ifndef __CRPCCHECKPOINT_H__
#define __CRPCCHECKPOINT_H__

class CRPCCheckpoint 
{
public:
	static void Create(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void Show(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void Hide(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetHeight(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void PointTo(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetType(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetColor(RakNet::BitStream *bitStream, RakNet::Packet *packet);
};

#endif