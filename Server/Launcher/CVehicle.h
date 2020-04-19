#ifndef __CVEHICLE_H__
#define __CVEHICLE_H__
#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <stdint.h>

#include "CEntity.h"
#include "CVitals.h"

#include "VehicleStructs.h"

#include "CBlip.h"

class CVehicle : public CEntity, public CVitals
{
protected:
	const int getID() override { return identity.entityId; }
	const int getType() override { return identity.type; }
private:
	struct Data
	{
		struct Model
		{
			uint32_t hash = 0xbc993509;

			GrandM::VehicleData::Paint Colors[2];
			GrandM::VehicleData::Mod Mods[49];
			GrandM::VehicleData::Extra Extras[15];
		} model;

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

		SyncBool taxiLight = SyncBool(false, "VehicleTaxiLight");

		SyncBool leftIndicator = SyncBool(false, "VehicleLeftIndicator");
		SyncBool rightIndicator = SyncBool(false, "VehicleRightIndicator");

		SyncBool interiorLight = SyncBool(false, "VehicleInteriorLight");

		SyncBool horn = SyncBool(false, "VehicleHorn");

		SyncBool sirenSound = SyncBool(false, "VehicleSirenSound");

		SyncBool boostActive = SyncBool(false, "VehicleBoostActive");

		// Vehicle Data
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

		int towedVehicle = -1;
		int cargobobRopeType = -1;

		struct Plate
		{
			std::string	text = "GrandM";
			int style = 0;
		} plate;

		int doorsLockState = 0;

		float heading;

		bool needsInit = true;

		int occupants[12];
	} data;

	CBlip	blip;

	struct Network
	{
		// The Assigned user for physics sync
		RakNet::RakNetGUID physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	} network;

public:
	CVehicle() = delete;

	CVehicle(uint32_t model, CVector3 position, CVector3 rotation);
	~CVehicle();

	// Copy constructor
	CVehicle(const CVehicle & e);

	// Copy Assign 
	CVehicle &operator=(const CVehicle &e);

	const bool operator==(Objects::Entity &e)
	{
		return identity.entityId == e.GetID();
	}

	void PulseMovement() override;
	//void Update(RakNet::Packet *packet);

	void UpdateVehicleData(RakNet::Packet *packet);
	void PulseVehicleData();

	void SyncData(RakNet::RakNetGUID requester);

	const bool NeedsInit() { return data.needsInit; }
	void SetNeedsInit(const bool state) { data.needsInit = state; }

	RakNet::RakNetGUID GetAssignee() { return network.physicsAssignee; }
	void SetAssignee(RakNet::RakNetGUID assignee);

	const uint32_t GetModel() { return data.model.hash; }
	
	CEntity GetOccupant(int seat);
	void SetOccupant(int seat, int player) { data.occupants[seat] = player; }

	const int GetColor(const int layer);
	const int GetColorType(const int layer);
	const Color GetCustomColor(const int layer);
	void SetColor(const int layer, const int painttype, const int color);
	void SetColor(const int layer, const Color color);

	std::string GetNumberPlate() { return data.plate.text; }
	void SetNumberPlate(const std::string plate) { data.plate.text = plate; }
	const int GetNumberPlateStyle() { return data.plate.style; }
	void SetNumberPlateStyle(const int style) { data.plate.style = style; }

	const int GetMod(const int modType) { return data.model.Mods[modType].index; }
	void SetMod(const int modType, const int modIndex) { data.model.Mods[modType].index = modIndex; data.model.Mods[modType].Edited = true; }
	// Extras 0 = ON & 1 = OFF, Which is why i invert the input
	const bool GetExtra(const int extra) { return !data.model.Extras[extra].enabled; }
	void SetExtra(const int extra, const bool toggle) { data.model.Extras[extra].enabled = !toggle; data.model.Extras[extra].edited = true; }
	void SetupExtra(const int extra, const bool toggle) { if (data.model.Extras[extra].edited) { data.model.Extras[extra].enabled = !toggle; data.model.Extras[extra].edited = true; } }

	const bool GetEngineState() { return data.engine.engineState.Get(); }
	void SetEngineState(const bool state, const RakNet::RakNetGUID sender) { data.engine.engineState.Set(state, sender, identity.entityId, identity.type); }
	void SetForcedEngineState(const bool state) { data.engine.forcedEngineState = state; }

	const float GetEngineHealth() { return data.engine.health.Get(); }
	void SetEngineHealth(const float health, const RakNet::RakNetGUID sender) { data.engine.health.Set(health, sender, identity.entityId, identity.type); }
	const float GetFuelTankHealth() { return data.engine.fuelHealth.Get(); }
	void SetFuelTankHealth(const float health, const RakNet::RakNetGUID sender) { data.engine.fuelHealth.Set(health, sender, identity.entityId, identity.type); }

	const int GetDoorsLockState() { return data.doorsLockState; }
	void SetDoorsLockState(const int state) { data.doorsLockState = state; }

	const bool GetTaxiLightState() { return data.taxiLight.Get(); }
	void SetTaxiLightState(const bool state, const RakNet::RakNetGUID sender) { data.taxiLight.Set(state, sender, identity.entityId, identity.type); }

	const bool GetLeftIndicatorState() { return data.leftIndicator.Get(); }
	void SetLeftIndicatorState(const bool state, const RakNet::RakNetGUID sender) { data.leftIndicator.Set(state, sender, identity.entityId, identity.type); }
	const bool GetRightIndicatorState() { return data.rightIndicator.Get(); }
	void SetRightIndicatorState(const bool state, const RakNet::RakNetGUID sender) { data.rightIndicator.Set(state, sender, identity.entityId, identity.type); }

	const bool GetInteriorLightState() { return data.interiorLight.Get(); }
	void SetInteriorLightState(const bool state, const RakNet::RakNetGUID sender) { data.interiorLight.Set(state, sender, identity.entityId, identity.type); }

	void SetHeadlightState(const bool state, const RakNet::RakNetGUID sender) { data.headlightState.Set(state, sender, identity.entityId, identity.type); }
	void SetHighbeamState(const bool state, const RakNet::RakNetGUID sender) { data.highbeamState.Set(state, sender, identity.entityId, identity.type); }

	const bool GetSirenSoundState() { return data.sirenSound.Get(); }
	void SetSirenSoundState(const bool state, const RakNet::RakNetGUID sender) { data.sirenSound.Set(state, sender, identity.entityId, identity.type); }

	void SetHornState(const bool state, const RakNet::RakNetGUID sender) { data.horn.Set(state, sender, identity.entityId, identity.type); }

	void SetRoofState(const int state, const RakNet::RakNetGUID sender) { data.roofState.Set(state, sender, identity.entityId, identity.type); }

	void SetBoostState(const int state, const RakNet::RakNetGUID sender) { data.boostActive.Set(state, sender, identity.entityId, identity.type); }

	CEntity GetTowedVehicle();
	void SetTowedVehicle(const int entity) { data.towedVehicle = entity; }

	const int GetCargobobRopeType() { return data.cargobobRopeType; }
	void SetCargobobRopeType(const int type) { data.cargobobRopeType = type; }

	CBlip* GetBlip() { return &blip; }
};
extern std::vector<CVehicle> g_Vehicles;
#endif