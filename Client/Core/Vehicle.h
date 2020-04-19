#ifndef __VEHICLES_H__
#define __VEHICLES_H__
#include "CVector3.h"

struct WheelDimensions
{
	float TyreRadius;
	float RimRadius;
	float TyreWidth;
};

class Vehicles
{
public:
	void Init();
	BYTE *GetAddress(Vehicle handle);

	/*
	* Vehicle struct
	*/
	bool GetRocketBoostActive(Vehicle handle);
	void SetRocketBoostActive(Vehicle handle, bool val);

	float GetRocketBoostCharge(Vehicle handle);
	void SetRocketBoostCharge(Vehicle handle, float value);

	float GetFuelLevel(Vehicle handle);
	void SetFuelLevel(Vehicle handle, float value);

	uint16_t GetGearNext(Vehicle handle);
	void SetGearNext(Vehicle handle, uint16_t value);

	uint16_t GetGearCurr(Vehicle handle);
	void SetGearCurr(Vehicle handle, uint16_t value);

	unsigned char GetTopGear(Vehicle handle);
	// SetTopGear not logical

	// Divide GetDriveMaxFlatVel by the values in this thing to get the top
	// speed for the gear.
	std::vector<float> GetGearRatios(Vehicle handle);
	float GetDriveForce(Vehicle handle);
	float GetInitialDriveMaxFlatVel(Vehicle handle);
	float GetDriveMaxFlatVel(Vehicle handle);

	float GetCurrentRPM(Vehicle handle);
	void SetCurrentRPM(Vehicle handle, float value);

	float GetClutch(Vehicle handle);
	void SetClutch(Vehicle handle, float value);

	float GetThrottle(Vehicle handle);
	void SetThrottle(Vehicle handle, float value);

	float GetTurbo(Vehicle handle);
	void SetTurbo(Vehicle handle, float value);

	uint64_t GetHandlingPtr(Vehicle handle);

	// Steering input angle, steering lock independent
	float GetSteeringInputAngle(Vehicle handle);
	void SetSteeringInputAngle(Vehicle handle, float value);

	// Wheel angle, steering lock dependent
	float GetSteeringAngle(Vehicle handle);
	void SetSteeringAngle(Vehicle handle, float value);

	float GetThrottleP(Vehicle handle);
	void SetThrottleP(Vehicle handle, float value);

	float GetBrakeP(Vehicle handle);
	void SetBrakeP(Vehicle handle, float value);

	bool GetHandbrake(Vehicle handle);
	// SetHandbrake not implemented

	float GetDirtLevel(Vehicle handle);
	// No set impl.

	float GetEngineTemp(Vehicle handle);
	// No set impl.

	float GetDashSpeed(Vehicle handle);
	void SetDashSpeed(Vehicle handle, float value); //??

	int GetModelType(Vehicle handle);

	static const float GetSeatDistance(Vehicle vehicle, CVector3 PlayerPos, std::string seatBone);

private:
	int rocketBoostActiveOffset = 0;
	int rocketBoostChargeOffset = 0;
	int fuelLevelOffset = 0;
	int nextGearOffset = 0;
	int currentGearOffset = 0;
	int topGearOffset = 0;
	int gearRatiosOffset = 0;
	int driveForceOffset = 0;
	int initialDriveMaxFlatVelOffset = 0;
	int driveMaxFlatVelOffset = 0;
	int currentRPMOffset = 0;
	int clutchOffset = 0;
	int throttleOffset = 0;
	int turboOffset = 0;
	int handlingOffset = 0;
	int steeringAngleInputOffset = 0;
	int steeringAngleOffset = 0;
	int throttlePOffset = 0;
	int brakePOffset = 0;
	int handbrakeOffset = 0;
	int dirtLevelOffset = 0;
	int engineTempOffset = 0;
	int dashSpeedOffset = 0;
	int wheelsPtrOffset = 0;
	int numWheelsOffset = 0;
	// 0 = car, 1 = plane, 2 = trailer, 3 = quad, 6 = amphibious car, 7 = amphibious quad, 8 = heli, 11 = motorcycle, 12 = bicycle, 14 = train, 
	int modelTypeOffset = 0;
};
extern Vehicles vehExt;
#endif
