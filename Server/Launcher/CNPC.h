#ifndef __NPC_H__
#define __NPC_H__
#pragma once
#include <vector>
#include <stdint.h>

#include "CEntity.h"
#include "CVitals.h"
#include "CBlip.h"

#include "Ped.h"
#include "Weapon.h"
#include "GUI.h"
#include "APIStructs.h"

class CNPC : public CEntity, public CVitals
{
protected:
	const int getID() override { return identity.entityId; }
	const int getType() override { return identity.type; }
private:
	struct Data
	{
		struct Model
		{
			uint32_t hash;

			GrandM::Model::Component components[12];
			GrandM::Model::HeadBlend headBlend;
			GrandM::Model::HeadOverlay headOverlays[13];
			GrandM::Model::Prop props[3];
			GrandM::Model::FaceFeature faceFeatures[20];
		} Model;

		GrandM::Weapon::WeaponData WeaponData;

		struct VehicleData
		{
			int id = -1;
			int seat = -1;
		} Vehicle;

		float forwardSpeed;

	} data;

	CBlip	blip;

	struct Network
	{
		std::chrono::time_point<std::chrono::system_clock> lastSync;

		// The Assigned user for physics sync
		RakNet::RakNetGUID physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	} network;

public:
	CNPC() = delete;
	CNPC(const uint32_t model, const CVector3 position, const CVector3 rotation);
	~CNPC();

	// Copy constructor
	CNPC(const CNPC & e);

	// Copy Assign 
	CNPC &operator=(const CNPC &e);

	void Pulse();
	void Update(RakNet::Packet *packet);
	void SyncData(RakNet::RakNetGUID requester);

	RakNet::RakNetGUID GetAssignee() { return network.physicsAssignee; }
	void SetAssignee(RakNet::RakNetGUID assignee);

	int GetVehicle() { return data.Vehicle.id; }
	int GetVehicleSeat() { return data.Vehicle.seat; }

	const uint32_t GetModel() { return data.Model.hash; }
	void SetModel(const uint32_t model) { data.Model.hash = model; }

	GrandM::Model::Component GetModelComponent(const int index) { return data.Model.components[index]; }
	void SetModelComponent(const int index, const GrandM::Model::Component component) { data.Model.components[index] = component; }

	GrandM::Model::HeadBlend GetModelHeadBlend() { return data.Model.headBlend; }
	void SetModelHeadBlend(GrandM::Model::HeadBlend headblend) { data.Model.headBlend = headblend; }

	GrandM::Model::HeadOverlay GetModelHeadOverlay(const int index) { return data.Model.headOverlays[index]; }
	void SetModelHeadOverlay(const int index, GrandM::Model::HeadOverlay headOverlay) { data.Model.headOverlays[index] = headOverlay; }

	GrandM::Model::Prop GetModelProp(const int index) { return data.Model.props[index]; }
	void SetModelProp(const int index, GrandM::Model::Prop prop) { data.Model.props[index] = prop; }

	float GetModelFaceFeature(const int index) { return data.Model.faceFeatures[index].scale; }
	void SetModelFaceFeature(const int index, const float scale) { data.Model.faceFeatures[index].scale = scale; }

	void Revive();

	CBlip* GetBlip() { return &blip; }
};
extern std::vector<CNPC> g_Npcs;
#endif