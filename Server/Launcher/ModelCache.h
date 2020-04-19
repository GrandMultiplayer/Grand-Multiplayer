#ifndef __MODELCACHE_H__
#define __MODELCACHE_H__
#pragma once
#include <vector>
#include <stdint.h>

#include "RakNetTypes.h"

class ModelCache
{
public:
	static std::vector<uint32_t> hashModels;

	static void LoadModels(RakNet::RakNetGUID user);

	static void AddModel(const uint32_t model);
};

#endif