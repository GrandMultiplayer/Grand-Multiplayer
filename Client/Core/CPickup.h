#ifndef __CPICKUP_H__
#define __CPICKUP_H__
#include "CEntity.h" // The base class

#include "CBlip.h"

class CPickup : public CEntity
{
private:
	CBlip blip;

	struct Data
	{
		uint32_t model;

		bool visible = false;

		struct Trigger
		{
			bool triggered = false;

			int respawnDelay = 30000; // Time in MS

			unsigned long triggeredAt;
		} trigger;


		unsigned long lastCheck;
	} data;


public:
	CPickup() = delete;
	CPickup(const int id);
	~CPickup();

	// Copy constructor
	CPickup(const CPickup & e);

	// Copy Assign 
	CPickup &operator=(const CPickup &e);

	CBlip* GetBlip() { return &blip; }

	const bool IsCreated() { return game.created; }

	const bool Spawn();
	void Delete();

	void Destroy();

	void Pulse();

	const uint32_t GetModel() { return data.model; }
	void SetModel(const uint32_t model);

	const bool IsVisible() { return data.visible; }
	void SetVisible(const bool state) { data.visible = state; }

	void SetPosition(const CVector3 position);

	void SetRespawnDelay(const int time) { data.trigger.respawnDelay = time; }
};
extern std::vector<CPickup> g_Pickups;
#endif