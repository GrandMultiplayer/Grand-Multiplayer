#include <winsock2.h>
#include <Windows.h>
#include <vector>

#include "MemoryAccess.h"
#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "CVector3.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "Vehicle.h"

/*
* Offsets/patterns done by E66666666 for their manual transmission mod @ (https://github.com/E66666666/GTAVManualTransmission/releases)
* against b1180.2 and b877.1 and are okay.
*/
void Vehicles::Init()
{
	LOG_DEBUG("Vehicle offsets: ");

	mem::init();

	// Get Offsets
	uintptr_t addr = MemoryAccess::FindPattern("\x3A\x91\x00\x00\x00\x00\x74\x00\x84\xD2", "xx????x?xx");
	rocketBoostActiveOffset = addr == 0 ? 0 : *(int*)(addr + 2);
	Utility::GetLog()->Write(rocketBoostActiveOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Rocket Boost Active Offset: 0x%X", rocketBoostActiveOffset);

	addr = MemoryAccess::FindPattern("\x48\x8B\x47\x00\xF3\x44\x0F\x10\x9F\x00\x00\x00\x00", "xxx?xxxxx????");
	rocketBoostChargeOffset = addr == 0 ? 0 : *(int*)(addr + 9);
	Utility::GetLog()->Write(rocketBoostChargeOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Rocket Boost Charge Offset: 0x%X", rocketBoostChargeOffset);

	addr = MemoryAccess::FindPattern("\x74\x26\x0F\x57\xC9", "xxxxx");
	fuelLevelOffset = addr == 0 ? 0 : *(int*)(addr + 8);
	Utility::GetLog()->Write(fuelLevelOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Fuel Level Offset: 0x%X", fuelLevelOffset);

	addr = MemoryAccess::FindPattern("\x48\x8D\x8F\x00\x00\x00\x00\x4C\x8B\xC3\xF3\x0F\x11\x7C\x24", "xxx????xxxxxxxx");
	nextGearOffset = addr == 0 ? 0 : *(int*)(addr + 3);
	Utility::GetLog()->Write(nextGearOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Next Gear Offset: 0x%X", nextGearOffset);

	currentGearOffset = addr == 0 ? 0 : *(int*)(addr + 3) + 2;
	Utility::GetLog()->Write(currentGearOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Current Gear Offset: 0x%X", currentGearOffset);

	topGearOffset = addr == 0 ? 0 : *(int*)(addr + 3) + 6;
	Utility::GetLog()->Write(topGearOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Top Gear Offset: 0x%X", topGearOffset);

	gearRatiosOffset = addr == 0 ? 0 : *(int*)(addr + 3) + 8;
	Utility::GetLog()->Write(gearRatiosOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Gear Ratios Offset: 0x%X", gearRatiosOffset);

	driveForceOffset = addr == 0 ? 0 : *(int*)(addr + 3) + 0x28;
	Utility::GetLog()->Write(driveForceOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Drive Force Offset: 0x%X", driveForceOffset);

	initialDriveMaxFlatVelOffset = addr == 0 ? 0 : *(int*)(addr + 3) + 0x2C;
	Utility::GetLog()->Write(initialDriveMaxFlatVelOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Initial Drive Max Flat Velocity Offset: 0x%X", initialDriveMaxFlatVelOffset);

	driveMaxFlatVelOffset = addr == 0 ? 0 : *(int*)(addr + 3) + 0x30;
	Utility::GetLog()->Write(driveMaxFlatVelOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Drive Max Flat Velocity Offset: 0x%X", driveMaxFlatVelOffset);

	addr = MemoryAccess::FindPattern("\x76\x03\x0F\x28\xF0\xF3\x44\x0F\x10\x93", "xxxxxxxxxx");
	currentRPMOffset = addr == 0 ? 0 : *(int*)(addr + 10);
	Utility::GetLog()->Write(currentRPMOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    RPM Offset: 0x%X", currentRPMOffset);

	clutchOffset = addr == 0 ? 0 : *(int*)(addr + 10) + 0xC;
	Utility::GetLog()->Write(clutchOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Clutch Offset: 0x%X", clutchOffset);

	throttleOffset = addr == 0 ? 0 : *(int*)(addr + 10) + 0x10;
	Utility::GetLog()->Write(throttleOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Throttle Offset: 0x%X", throttleOffset);

	addr = MemoryAccess::FindPattern("\xF3\x0F\x10\x9F\xD4\x08\x00\x00\x0F\x2F\xDF\x73\x0A", "xxxx????xxxxx");
	turboOffset = addr == 0 ? 0 : *(int*)(addr + 4);
	Utility::GetLog()->Write(turboOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Turbo Offset: 0x%X", turboOffset);

	addr = MemoryAccess::FindPattern("\x3C\x03\x0F\x85\x00\x00\x00\x00\x48\x8B\x41\x20\x48\x8B\x88", "xxxx????xxxxxxx");
	handlingOffset = addr == 0 ? 0 : *(int*)(addr + 0x16);
	Utility::GetLog()->Write(handlingOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Handling Offset: 0x%X", handlingOffset);

	addr = MemoryAccess::FindPattern("\x74\x0A\xF3\x0F\x11\xB3\x1C\x09\x00\x00\xEB\x25", "xxxxxx????xx");
	steeringAngleInputOffset = addr == 0 ? 0 : *(int*)(addr + 6);
	Utility::GetLog()->Write(steeringAngleInputOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Steering Input Offset: 0x%X", steeringAngleInputOffset);

	steeringAngleOffset = addr == 0 ? 0 : *(int*)(addr + 6) + 8;
	Utility::GetLog()->Write(steeringAngleOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Steering Angle Offset: 0x%X", steeringAngleOffset);

	throttlePOffset = addr == 0 ? 0 : *(int*)(addr + 6) + 0x10;
	Utility::GetLog()->Write(throttlePOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    ThrottleP Offset: 0x%X", throttlePOffset);

	brakePOffset = addr == 0 ? 0 : *(int*)(addr + 6) + 0x14;
	Utility::GetLog()->Write(brakePOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    BrakeP Offset: 0x%X", brakePOffset);

	addr = MemoryAccess::FindPattern("\x44\x88\xA3\x00\x00\x00\x00\x45\x8A\xF4", "xxx????xxx");
	handbrakeOffset = addr == 0 ? 0 : *(int*)(addr + 3);
	Utility::GetLog()->Write(handbrakeOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Handbrake Offset: 0x%X", handbrakeOffset);

	addr = MemoryAccess::FindPattern("\x0F\x29\x7C\x24\x30\x0F\x85\xE3\x00\x00\x00\xF3\x0F\x10\xB9\x68\x09\x00\x00", "xx???xx????xxxx????");
	dirtLevelOffset = addr == 0 ? 0 : *(int*)(addr + 0xF);
	Utility::GetLog()->Write(dirtLevelOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Dirt Level Offset: 0x%X", dirtLevelOffset);

	addr = MemoryAccess::FindPattern("\xF3\x0F\x11\x9B\xDC\x09\x00\x00\x0F\x84\xB1\x00\x00\x00", "xxxx????xxx???");
	engineTempOffset = addr == 0 ? 0 : *(int*)(addr + 4);
	Utility::GetLog()->Write(engineTempOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Engine Temperature Offset: 0x%X", engineTempOffset);

	addr = MemoryAccess::FindPattern("\xF3\x0F\x10\x8F\x10\x0A\x00\x00\xF3\x0F\x59\x05\x5E\x30\x8D\x00", "xxxx????xxxx????");
	dashSpeedOffset = addr == 0 ? 0 : *(int*)(addr + 4);
	Utility::GetLog()->Write(dashSpeedOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Dashboard Speed Offset: 0x%X", dashSpeedOffset);

	addr = MemoryAccess::FindPattern("\x8B\x83\x38\x0B\x00\x00\x83\xE8\x08\x83\xF8\x02", "xx????xx?xxx");
	modelTypeOffset = addr == 0 ? 0 : *(int*)(addr + 2);
	Utility::GetLog()->Write(modelTypeOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Model Type Offset: 0x%X", modelTypeOffset);

	addr = MemoryAccess::FindPattern("\x3B\xB7\x48\x0B\x00\x00\x7D\x0D", "xx????xx");
	wheelsPtrOffset = addr == 0 ? 0 : *(int*)(addr + 2) - 8;
	Utility::GetLog()->Write(wheelsPtrOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Wheels Pointer Offset: 0x%X", wheelsPtrOffset);

	numWheelsOffset = addr == 0 ? 0 : *(int*)(addr + 2);
	Utility::GetLog()->Write(numWheelsOffset == 0 ? Utility::eLogType::LogTypeWarning : Utility::eLogType::LogTypeDebug, "    Wheel Count Offset: 0x%X", numWheelsOffset);
}

BYTE *Vehicles::GetAddress(Vehicle handle)
{
	return reinterpret_cast<BYTE *>(mem::GetAddressOfEntity(handle));
}

bool Vehicles::GetRocketBoostActive(Vehicle handle)
{
	if (rocketBoostActiveOffset == 0) return false;
	return *reinterpret_cast<bool *>(GetAddress(handle) + rocketBoostActiveOffset);
}

void Vehicles::SetRocketBoostActive(Vehicle handle, bool val)
{
	if (rocketBoostActiveOffset == 0) return;
	*reinterpret_cast<bool *>(GetAddress(handle) + rocketBoostActiveOffset) = val;
}

float Vehicles::GetRocketBoostCharge(Vehicle handle)
{
	if (rocketBoostChargeOffset == 0) return 0.0f;
	return *reinterpret_cast<float *>(GetAddress(handle) + rocketBoostChargeOffset);
}

void Vehicles::SetRocketBoostCharge(Vehicle handle, float value)
{
	if (rocketBoostChargeOffset == 0) return;
	*reinterpret_cast<float *>(GetAddress(handle) + rocketBoostChargeOffset) = value;
}

float Vehicles::GetFuelLevel(Vehicle handle)
{
	if (fuelLevelOffset == 0) return 0.0f;
	return *reinterpret_cast<float *>(GetAddress(handle) + fuelLevelOffset);
}

void Vehicles::SetFuelLevel(Vehicle handle, float value)
{
	if (fuelLevelOffset == 0) return;
	*reinterpret_cast<float *>(GetAddress(handle) + fuelLevelOffset) = value;
}

uint16_t Vehicles::GetGearNext(Vehicle handle)
{
	if (nextGearOffset == 0) return 0;
	return *reinterpret_cast<const uint16_t *>(GetAddress(handle) + nextGearOffset);
}

void Vehicles::SetGearNext(Vehicle handle, uint16_t value)
{
	if (nextGearOffset == 0) return;
	*reinterpret_cast<uint16_t *>(GetAddress(handle) + nextGearOffset) = value;
}

uint16_t Vehicles::GetGearCurr(Vehicle handle)
{
	if (currentGearOffset == 0) return 0;
	return *reinterpret_cast<const uint16_t *>(GetAddress(handle) + currentGearOffset);
}

void Vehicles::SetGearCurr(Vehicle handle, uint16_t value)
{
	if (currentGearOffset == 0) return;
	*reinterpret_cast<uint16_t *>(GetAddress(handle) + currentGearOffset) = value;
}

unsigned char Vehicles::GetTopGear(Vehicle handle)
{
	if (topGearOffset == 0) return 0;
	return *reinterpret_cast<const unsigned char *>(GetAddress(handle) + topGearOffset);
}

std::vector<float> Vehicles::GetGearRatios(Vehicle handle)
{
	auto address = GetAddress(handle);
	std::vector<float> ratios;
	for (int gearOffset = 0; gearOffset <= 7; gearOffset++)
	{
		ratios.push_back(*reinterpret_cast<float *>(address + gearRatiosOffset + gearOffset * sizeof(float)));
	}
	return ratios;
}

float Vehicles::GetDriveForce(Vehicle handle)
{
	if (driveForceOffset == 0) return 0.0f;
	return *reinterpret_cast<float *>(GetAddress(handle) + driveForceOffset);
}

float Vehicles::GetInitialDriveMaxFlatVel(Vehicle handle)
{
	if (initialDriveMaxFlatVelOffset == 0) return 0.0f;
	return *reinterpret_cast<float *>(GetAddress(handle) + initialDriveMaxFlatVelOffset);
}

float Vehicles::GetDriveMaxFlatVel(Vehicle handle)
{
	if (driveMaxFlatVelOffset == 0) return 0.0f;
	return *reinterpret_cast<float *>(GetAddress(handle) + driveMaxFlatVelOffset);
}

float Vehicles::GetCurrentRPM(Vehicle handle)
{
	if (currentRPMOffset == 0) return 0.0f;
	return *reinterpret_cast<const float *>(GetAddress(handle) + currentRPMOffset);
}

void Vehicles::SetCurrentRPM(Vehicle handle, float value)
{
	if (currentRPMOffset == 0) return;
	*reinterpret_cast<float *>(GetAddress(handle) + currentRPMOffset) = value;
}

float Vehicles::GetClutch(Vehicle handle)
{
	if (clutchOffset == 0) return 0.0f;
	auto address = GetAddress(handle);
	return address == nullptr ? 0 : *reinterpret_cast<const float *>(address + clutchOffset);
}

void Vehicles::SetClutch(Vehicle handle, float value)
{
	if (clutchOffset == 0) return;
	auto address = GetAddress(handle);
	*reinterpret_cast<float *>(address + clutchOffset) = value;
}

float Vehicles::GetThrottle(Vehicle handle)
{
	if (throttleOffset == 0) return 0.0f;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + throttleOffset);
}

void Vehicles::SetThrottle(Vehicle handle, float value)
{
	if (throttleOffset == 0) return;
	auto address = GetAddress(handle);
	*reinterpret_cast<float *>(address + throttleOffset) = value;
}

float Vehicles::GetTurbo(Vehicle handle)
{
	if (turboOffset == 0) return 0.0f;
	auto address = GetAddress(handle);
	return address == nullptr ? 0 : *reinterpret_cast<const float *>(address + turboOffset);
}

void Vehicles::SetTurbo(Vehicle handle, float value)
{
	if (turboOffset == 0) return;
	auto address = GetAddress(handle);
	*reinterpret_cast<float *>(address + turboOffset) = value;
}

uint64_t Vehicles::GetHandlingPtr(Vehicle handle)
{
	if (handlingOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<uint64_t *>(address + handlingOffset);
}

float Vehicles::GetSteeringInputAngle(Vehicle handle)
{
	if (steeringAngleInputOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + steeringAngleInputOffset);
}

void Vehicles::SetSteeringInputAngle(Vehicle handle, float value)
{
	if (steeringAngleInputOffset == 0) return;
	auto address = GetAddress(handle);
	*reinterpret_cast<float *>(address + steeringAngleInputOffset) = value;
}

float Vehicles::GetSteeringAngle(Vehicle handle)
{
	if (steeringAngleOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + steeringAngleOffset);
}

void Vehicles::SetSteeringAngle(Vehicle handle, float value)
{
	if (steeringAngleOffset == 0) return;
	auto address = GetAddress(handle);
	*reinterpret_cast<float *>(address + steeringAngleOffset) = value;
}

float Vehicles::GetThrottleP(Vehicle handle)
{
	if (throttlePOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + throttlePOffset);
}

void Vehicles::SetThrottleP(Vehicle handle, float value)
{
	if (throttlePOffset == 0) return;
	auto address = GetAddress(handle);
	*reinterpret_cast<float *>(address + throttlePOffset) = value;
}

float Vehicles::GetBrakeP(Vehicle handle)
{
	if (brakePOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + brakePOffset);
}

void Vehicles::SetBrakeP(Vehicle handle, float value)
{
	if (brakePOffset == 0) return;
	auto address = GetAddress(handle);
	*reinterpret_cast<float *>(address + brakePOffset) = value;
}

bool Vehicles::GetHandbrake(Vehicle handle)
{
	if (handbrakeOffset == 0) return false;
	auto address = GetAddress(handle);
	return *reinterpret_cast<bool *>(address + handbrakeOffset);
}

float Vehicles::GetDirtLevel(Vehicle handle)
{
	if (dirtLevelOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + dirtLevelOffset);
}

float Vehicles::GetEngineTemp(Vehicle handle)
{
	if (engineTempOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + engineTempOffset);
}

float Vehicles::GetDashSpeed(Vehicle handle)
{
	if (dashSpeedOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<float *>(address + dashSpeedOffset);
}

void Vehicles::SetDashSpeed(Vehicle handle, float value)
{
	//if (dashSpeedOffset == 0) return;
	//auto address = GetAddress(handle);
	//*reinterpret_cast<float *>(address + dashSpeedOffset) = value;
}

int Vehicles::GetModelType(Vehicle handle)
{
	if (modelTypeOffset == 0) return 0;
	auto address = GetAddress(handle);
	return *reinterpret_cast<int *>(address + modelTypeOffset);
}

const float Vehicles::GetSeatDistance(Vehicle vehicle, CVector3 playerPos, std::string seatBone)
{
	Vector3 tPos = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(vehicle, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(vehicle, (char*)seatBone.c_str()));

	CVector3 seatpos = { tPos.x, tPos.y, tPos.z };

	return CVector3::Distance(playerPos, seatpos);
}