#ifndef __CVEHICLE_H__
#define __CVEHICLE_H__
#pragma once

#include <vector>
#include <stdint.h>

#include "CEntity.h"
#include "hook/types.h"
#include "VehicleStructs.h"
#include "CBlip.h"
#include "Vehicle.h"

// RakNet
#include <RakNetTypes.h>

class CVehicle : public CEntity
{
private:
	CBlip blip;

	struct Data
	{
		struct Model
		{
			uint32_t hash = 0xbc993509;

			GrandM::VehicleData::Paint Colors[2];
			GrandM::VehicleData::Mod Mods[49];
			GrandM::VehicleData::Extra Extras[15];
		} model;

		int Class = -1;

		float forwardSpeed;

		struct Engine
		{
			SyncBool engineState = SyncBool(false, "VehicleEngineState");
			int	forcedEngineState = -1;

			SyncFloat health = SyncFloat(1000.0f, "VehicleEngineHealth");
			SyncFloat fuelHealth = SyncFloat(1000.0f, "VehicleFuelHealth");
		} engine;

		SyncBool headlightState = SyncBool(false, "VehicleHeadlightState");
		SyncBool highbeamState = SyncBool(false, "VehicleHighbeamState");

		SyncInt roofState = SyncInt(0, "VehicleRoofState");

		//
		uint16_t gear = 0;
		float rpm = .0f;

		float clutch = .0f;
		float turbo = .0f;
		float acceleration = .0f;
		float brake = .0f;

		float dashSpeed = .0f;
		float steeringAngle = .0f;
		float forwardWheelAngle = .0f;
		//

		SyncBool taxiLight = SyncBool(false, "VehicleTaxiLight");

		SyncBool leftIndicator = SyncBool(false, "VehicleLeftIndicator");
		SyncBool rightIndicator = SyncBool(false, "VehicleRightIndicator");

		SyncBool interiorLight = SyncBool(false, "VehicleInteriorLight");

		SyncBool horn = SyncBool(false, "VehicleHorn");

		SyncBool sirenSound = SyncBool(false, "VehicleSirenSound");

		bool hasRocketboost = false; // Just a somthing we use to store if a vehicle is a vehicle that has boost or not.
		SyncBool boostActive = SyncBool(false, "VehicleBoostActive");

		// Tow/Crane
		int towedVehicle = -1;
		int cargobobRopeType = -1;

		struct Plate
		{
			std::string	text = "GrandM";
			int style = 0;
		} plate;

		int doorsLockState = 0;

		float heading;
		bool needsInit = false;
	} data;

	struct VehicleSync
	{
		uint64_t lastSyncSent;
		uint64_t lastSyncReceived;
		uint64_t lastUpdateData;
	} vehicleSync;

	int occupants[12];

public:
	CVehicle() = delete;
	explicit CVehicle(const int id);
	~CVehicle();

	// Copy constructor
	CVehicle(const CVehicle & e);

	// Copy Assign 
	CVehicle &operator=(const CVehicle &e);

	// Move constructor.
	CVehicle(CVehicle&& copy);

	// Move assignment
	CVehicle& operator=(CVehicle&& copy);

	bool CreateVehicle();
	void Destroy();
	void Delete();

	void PulseVehicleData();
	void UpdateVehicleData(RakNet::Packet *packet);

	//void Interpolate();

	void SetTargetPosition();

	void SetTargetRotation();

	void SetData();

	static Vehicle getClosestVehicleFromPedPos(Ped ped, int maxDistance);

	int GetOccupant(int seat) { return occupants[seat]; }

	const uint32_t GetModel() { return data.model.hash; }
	void SetModel(const uint32_t model) { data.model.hash = model; }

	RakNet::RakNetGUID GetAssignee() { return game.physicsAssignee; }
	void SetAssignee(RakNet::RakNetGUID assignee) { game.physicsAssignee = assignee; }

	void SetColor(const int layer, const int painttype, const int color);
	void SetColor(const int layer, const Color color);
	void SetNumberPlate(const std::string plate);
	void SetMod(const int modType, const int modIndex);

	void SetEngineState(const bool state, const bool instant, const bool forced);
	void SetEngineState(const bool state, RakNet::RakNetGUID sender);

	void SetDoorsLockState(const int state);
	void SetNumberPlateStyle(const int style);
	void SetExtra(const int extra, const bool toggle);
	void SetEngineHealth(const float health, RakNet::RakNetGUID sender);
	void SetFuelTankHealth(const float health, RakNet::RakNetGUID sender);
	void FixDeformation();
	void SetTaxiLightState(const bool state, RakNet::RakNetGUID sender);
	void SetIndicatorState(const int turnlight, const bool state, RakNet::RakNetGUID sender);
	void SetInteriorLightState(const bool state, RakNet::RakNetGUID sender);
	void SetSirenSoundState(const bool state, RakNet::RakNetGUID sender);
	void SetNeedsInit(const bool state) { data.needsInit = state; }
	void SetHornState(const bool state, RakNet::RakNetGUID sender);
	void SetHeadlightState(const bool state, RakNet::RakNetGUID sender);
	void SetHighbeamState(const bool state, RakNet::RakNetGUID sender);
	void SetRoofState(const int state, RakNet::RakNetGUID sender);
	void SetBoostState(const bool state, RakNet::RakNetGUID sender);

	const int GetCargobobRopeType() { return data.cargobobRopeType; }
	void SetCargobobRopeType(const int type);

	void Assignment();

	const bool Exists();

	CBlip* Blip() { return &blip; }

	void SetTowedVehicle(const int entity);
};
extern std::vector<CVehicle> g_Vehicles;

#endif