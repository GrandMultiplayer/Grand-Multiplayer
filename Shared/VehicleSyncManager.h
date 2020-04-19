#ifndef __VEHICLESYNCMANAGER_H__
#define __VEHICLESYNCMANAGER_H__

namespace SyncManager
{
	namespace Vehicle
	{
		void EngineState(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void EngineHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void FuelHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void HeadlightState(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void HighbeamState(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void RoofState(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void TaxiLight(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void LeftIndicator(RakNet::BitStream *bitStream, RakNet::Packet *packet);
		void RightIndicator(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void InteriorLight(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void Horn(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void SirenSound(RakNet::BitStream *bitStream, RakNet::Packet *packet);

		void BoostActive(RakNet::BitStream *bitStream, RakNet::Packet *packet);
	};
};

#endif // !__SYNCMANAGER_H__