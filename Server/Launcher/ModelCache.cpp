// Self
#include "ModelCache.h"

// System

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Server.h"

std::vector<uint32_t> ModelCache::hashModels;

void ModelCache::LoadModels(RakNet::RakNetGUID user)
{
	for (unsigned int i = 0; i < ModelCache::hashModels.size(); i++)
	{
		RakNet::BitStream bitstream;
		bitstream.Write(hashModels[i]);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("LoadHashModel", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
	}
}

void ModelCache::AddModel(const uint32_t model)
{
	for (unsigned int i = 0; i < ModelCache::hashModels.size(); i++)
	{
		if (hashModels[i] == model)
			return;
	}

	hashModels.push_back(model);
}