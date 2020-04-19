#include <iostream>

// RakNet
#include "RakNet/RakNetTypes.h"

#include "Server.h"

#include "SharedUtility.h"

#include "SyncManager.h"


namespace SyncManager
{
	Sync syncmessages[] = {
		{ "cHealth", test },
	};

	void Init()
	{
		for (unsigned int i = 0; i < SizeOfArray(syncmessages); i++)
		{
			Server::GetServer()->GetNetworkManager()->GetRPC().RegisterSlot(syncmessages[i].name, syncmessages[i].functionPointer, 0);
		}
	}

	void test(RakNet::BitStream *bitStream, RakNet::Packet *packet)
	{
		std::cout << "test" << std::endl;
	}
}