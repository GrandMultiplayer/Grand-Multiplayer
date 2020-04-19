#ifndef __CNPC_H__
#define __CNPC_H__
// Baseclass
#include "CEntity.h"

// RakNet
#include <RakNetTypes.h>

#include "Ped.h"
#include "CBlip.h"

class CNPC : public CEntity
{
private:
	CBlip blip;

	struct Data
	{
		struct Model
		{
			uint32_t hash;

			GrandM::Model::Component	components[12];
			GrandM::Model::HeadBlend	headBlend;
			GrandM::Model::HeadOverlay	headOverlays[13];
			GrandM::Model::Prop			props[3];
			GrandM::Model::FaceFeature	faceFeatures[20];
		} model;

		struct Weapon
		{
			unsigned long weapon;
			bool reloading;
		} weapon;

		struct Vehicle
		{
			int entityId;
			int seat;
		} vehicle;

		float forwardSpeed;

		int armour = 100;
	} data;

	struct Network
	{
		std::chrono::time_point<std::chrono::system_clock> lastSyncSent;
		bool synchronized;
	} network;

public:
	CNPC() = delete;
	CNPC(const int entity, const uint32_t model, const CVector3 position, const CVector3 rotation);
	~CNPC();

	// Copy constructor
	CNPC(const CNPC & e);

	// Copy Assign 
	CNPC &operator=(const CNPC &e);

	bool CreateNpc();
	void Destroy();
	void Delete();

	void Pulse();
	void Update(RakNet::Packet *packet);

	void ShowNametag();

	bool IsCreated() { return game.created; }

	int GetVehicle() { return data.vehicle.entityId; }
	int GetVehicleSeat() { return data.vehicle.seat; }

	const uint32_t GetModel() { return data.model.hash; }
	void SetModel(const uint32_t model) { data.model.hash = model; }

	GrandM::Model::Component GetModelComponent(const int index) { return data.model.components[index]; }
	void SetModelComponent(const int index, const int drawableid, const int textureid, const int paletteid);

	GrandM::Model::HeadBlend GetModelHeadBlend() { return data.model.headBlend; }
	void SetModelHeadBlend(const int shapeFirst, const float shapeMix, const int shapeSecond, const int shapeThird, const int skinFirst, const float skinMix, const int skinSecond, const int skinThird, const float thirdMix);

	GrandM::Model::HeadOverlay GetModelHeadOverlay(const int index) { return data.model.headOverlays[index]; }
	void			SetModelHeadOverlay(const int index, const int type, const int colorType, const int colorID, const int secondColorID, const float opacity);

	GrandM::Model::Prop	GetModelProp(const int index) { return data.model.props[index]; }
	void			SetModelProp(const int index, const int drawableid, const int textureid);

	float			GetModelFaceFeature(const int index) { return data.model.faceFeatures[index].scale; }
	void			SetModelFaceFeature(const int index, const float scale);

	void			SetNametag(const std::string nametag) { tag.text = nametag; }
	void			SetNametagVisible(const bool visible) { tag.textVisible = visible; }
	void			SetHealthbarVisible(const bool visible) { tag.healthVisible = visible; }
	void			SetNametagDistance(const float distance) { tag.distance = distance; }
	void			SetNametagColor(const Color color) { tag.textColor = color; }

	const int		GetArmour() { return data.armour; }
	void			SetArmour(const int armour);

	RakNet::RakNetGUID GetAssignee() { return game.physicsAssignee; }
	void SetAssignee(RakNet::RakNetGUID assignee) { game.physicsAssignee = assignee; }

	bool			GetSynchronized() { return network.synchronized; }

	void			Revive();

	CBlip*			Blip() { return &blip; }
};
extern std::vector<CNPC> g_Npcs;

#endif