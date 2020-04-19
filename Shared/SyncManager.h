#ifndef __SYNCMANAGER_H__
#define __SYNCMANAGER_H__

#include <RakPeerInterface.h>
#include <DirectoryDeltaTransfer.h>

#include "VehicleSyncManager.h"

namespace SyncManager
{
	typedef struct
	{
		const char *name;
		void(*functionPointer) (RakNet::BitStream *userData, RakNet::Packet *packet);
	} Sync;

	void Init();
	void EntityHealth(RakNet::BitStream *bitStream, RakNet::Packet *packet);
};

#endif // !__SYNCMANAGER_H__