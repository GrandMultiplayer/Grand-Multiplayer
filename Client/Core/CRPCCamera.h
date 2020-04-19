#ifndef __CRPCCamera_H__
#define __CRPCCamera_H__

class CRPCCamera
{
public:
	static void SetCameraPosition(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetCameraRotation(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void ResetCamera(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void FirstPersonOnly(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void ThirdPersonOnly(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void AttachCamera(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void DetachCamera(RakNet::BitStream *bitStream, RakNet::Packet *packet);
};

#endif