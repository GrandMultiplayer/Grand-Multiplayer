#ifndef __H_CSTREAMER__
#define __H_CSTREAMER__
#include "CEntity.h"

class CStreamer
{
private:
	struct streamedObject
	{
		int entity = -1;
		CEntity::Type type = CEntity::Type::eNill;
	};

	static std::vector<streamedObject> streamed;

	static float MaxRange;

	static int MaxPlayers;		// The max visable players to be shown
	static int PlayerCount;	// The current count of visable players
	static float PlayerRange;	// Should the 'MaxPlayers' be reached it'll store the range it reached at that point here for the streamingout to check.

	static int MaxVehicles;
	static int VehicleCount;
	static float VehicleRange;

	static int MaxObjects;
	static int ObjectCount;
	static float ObjectRange;

	static int MaxNpcs;
	static int NpcCount;
	static float NpcRange;

	static int MaxPickups;
	static int PickupCount;
	static float PickupRange;
public:
	static void Start();
	static void Pulse();
	static void StreamObjectsIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance);
	static void StreamVehiclesIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance);
	static void StreamPlayersIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance);
	static void StreamNpcIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance);
	static void StreamPickupIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance);
	//static void StreamOtherIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance);
	static void StreamOut(const bool scriptedCameraState, const CVector3 camPosition);
	static void ForceStreamOut();
	static void Reset();

	static const int GetPlayerCount() { return PlayerCount; }
	static const int GetVehicleCount() { return VehicleCount; }
	static const int GetObjectCount() { return ObjectCount; }
	static const int GetNpcCount() { return NpcCount; }
	static const int GetPickupCount() { return PickupCount; }
};
#endif