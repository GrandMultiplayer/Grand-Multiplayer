#ifndef __CRPCPLAYER_H__
#define __CRPCPLAYER_H__

class CRPCPlayer
{
public:
	static void PlayerModel(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetControllable(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void Kick(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void WrongVersion(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void PutInVehicle(RakNet::BitStream *bitStream, RakNet::Packet *packet);

	static void GiveWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void RemoveWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void OnPlayerShot(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void OnPlayerAim(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void SetWeaponAmmo(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	static void EquipWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet);
};

#endif