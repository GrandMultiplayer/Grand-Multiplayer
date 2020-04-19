#ifndef __CCORE_H__
#define __CCORE_H__
#include "hook/types.h"

struct VehicleSeats
{
	Hash	hash;
	int		TurretSeat;
};


class CCore
{
private:
	static unsigned long LastCleanUp;
	static unsigned long LastUnlock;

public:
	static bool Initialized;

	static bool Initialize();
	static void OnGameTick();
	static void PulseEntities();

	static void CleanUp();
	static void CleanUpTick();

	static void PreventCheat();
	static void KeyCheck();
};

#endif