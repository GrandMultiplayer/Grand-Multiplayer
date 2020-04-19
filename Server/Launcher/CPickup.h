#ifndef __PICKUP_H__
#define __PICKUP_H__
#pragma once
#include <vector>
#include <stdint.h>

#include "CEntity.h"
#include "CBlip.h"

class CPickup : public CEntity
{
private:
	struct Data
	{
		uint32_t model = 0xE5121369;
		bool visible = false;

		int	respawnDelay = 30000; // Time in MS
	} data;

	CBlip	blip;

public:
	CPickup() = delete;
	explicit CPickup(const CVector3 position);
	~CPickup();

	// Copy constructor
	CPickup(const CPickup & e);

	// Copy Assign 
	CPickup &operator=(const CPickup &e);

	void SyncData(RakNet::RakNetGUID user);

	const uint32_t GetModel() { return data.model; }
	void SetModel(const uint32_t model) { data.model = model; }

	const bool IsVIsible() { return data.visible; }
	void SetVisible(const bool state) { data.visible = state; }

	const int GetRespawnDelay() { return data.respawnDelay; }
	void SetRespawnDelay(const int time) { data.respawnDelay = time; }

	CBlip* GetBlip() { return &blip; }
};
extern std::vector<CPickup> g_Pickups;
#endif