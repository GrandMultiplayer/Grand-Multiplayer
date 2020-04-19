#ifndef __PLAYER_H__
#define __PLAYER_H__
#pragma once
#include <stdint.h>
#include <vector>
#include <string>

#include "CEntity.h" // The base class
#include "CVitals.h"

#include "APIStructs.h"
#include "Ped.h"
#include "Weapon.h"
#include "GUI.h"

#include "CBlip.h"

#include "SyncValue.h"

class CPlayer : public CEntity, public CVitals
{
protected:
	const int getID() override { return identity.entityId; }
	const int getType() override { return identity.type; }
private:
	struct Data
	{
		std::string userName;
		std::string secretKey;

		struct ModelData
		{
			uint32_t hash = 0xDC59940D;

			GrandM::Model::Component	components[12];
			GrandM::Model::HeadBlend	headBlend;
			GrandM::Model::HeadOverlay	headOverlays[13];
			GrandM::Model::Prop			props[3];
			GrandM::Model::FaceFeature	faceFeatures[20];
		} Model;

		GrandM::Weapon::WeaponData		WeaponData;

		GrandM::GUI::GameHud			gameHud;

		SyncInt armour = SyncInt(100, "armour");

		int		aiTask;

		bool	hasControl = true;
		bool	frozen = false;

		struct VehicleData
		{
			int			id = -1;
			int			seat = -1;
		} Vehicle;

		struct ScriptedCameraData
		{
			bool		active = false;
			CVector3	position;
		} ScriptedCamera;
	} data;

	CBlip blip;

	struct Network
	{
		RakNet::RakNetGUID guid;
		RakNet::SystemAddress systemAddress;

		std::chrono::time_point<std::chrono::system_clock> lastSync;
	} network;

public:
	// De/Constuctors
	CPlayer() = delete;
	CPlayer(std::string name, std::string secretKey, RakNet::RakNetGUID guid, RakNet::SystemAddress systemAddress);
	~CPlayer();

	// Copy constructor
	CPlayer(const CPlayer & e);

	// Copy Assign 
	CPlayer &operator=(const CPlayer &e);

	/// <summary>
	/// Equal
	/// </summary>
	/// <param name="e">Entity to compare with</param>
	/// <returns>true if the same else false</returns>
	const bool operator==(CEntity &e) { return (identity.entityId == e.GetID()); }

	/// <summary>
	/// Equal Entity
	/// </summary>
	/// <param name="e">Entity to compare with</param>
	/// <returns>true if the same else false</returns>
	const bool operator==(Objects::Entity &e) { return (identity.entityId == e.GetID()); };

	// Network sorta
	void Pulse();
	void Update(RakNet::Packet *packet);
	void RequestData(RakNet::RakNetGUID requester);

	RakNet::RakNetGUID GetGUID() { return network.guid; }
	RakNet::SystemAddress GetIp() { return network.systemAddress; }

	// Other
	const std::string GetUsername() { return data.userName; }
	void SetUsername(std::string name) { data.userName = name; }

	const std::string GetSecretKey() { return data.secretKey; }

	CEntity GetVehicle();
	const int GetVehicleSeat() { return data.Vehicle.seat; }

	const uint32_t GetModel() { return data.Model.hash; }
	void SetModel(const uint32_t model) { data.Model.hash = model; }

	GrandM::Model::Component GetModelComponent(const int index) { return data.Model.components[index]; }
	void SetModelComponent(const int index, GrandM::Model::Component component) { data.Model.components[index] = component; }

	GrandM::Model::HeadBlend GetModelHeadBlend() { return data.Model.headBlend; }
	void SetModelHeadBlend(GrandM::Model::HeadBlend headBlend) { data.Model.headBlend = headBlend; }

	GrandM::Model::HeadOverlay GetModelHeadOverlay(const int index) { return data.Model.headOverlays[index]; }
	void SetModelHeadOverlay(const int index, GrandM::Model::HeadOverlay headOverlay) { data.Model.headOverlays[index] = headOverlay; }

	GrandM::Model::Prop	GetModelProp(const int index) { return data.Model.props[index]; }
	void SetModelProp(const int index, GrandM::Model::Prop prop) { data.Model.props[index] = prop; }

	const float GetModelFaceFeature(const int index) { return data.Model.faceFeatures[index].scale; }
	void SetModelFaceFeature(const int index, const float scale) { data.Model.faceFeatures[index].scale = scale; }

	const bool HasControl() { return data.hasControl; }
	void SetControllable(const bool controllable) { data.hasControl = controllable; }

	void GiveWeapon(const std::string weapon, const int ammo);
	void RemoveWeapon(const std::string weapon);
	const bool UpdateWeapon(const std::string weapon, const int ammo, const int clipAmmo);

	std::vector<GrandM::Weapon::Weapon> GetWeapons() { return data.WeaponData.weapons; };

	const int GetWeaponAmmo(const std::string weapon);
	const int GetWeaponClipAmmo(const std::string weapon);

	void SetAiming(const bool aiming) { data.WeaponData.aiming = aiming; }
	const bool IsAiming() { return data.WeaponData.aiming; }

	const bool IsFrozen() { return data.frozen; }
	void SetFrozen(const bool frozen) { data.frozen = frozen; }

	void Revive();

	const bool IsHudDisplayed() { return data.gameHud.displayHud; }
	void DisplayHud(const bool state) { data.gameHud.displayHud = state; }

	const bool IsMinimapDisplayed() { return data.gameHud.displayMinimap; }
	void DisplayMinimap(const bool state) { data.gameHud.displayMinimap = state; }

	const bool IsCashDisplayed() { return data.gameHud.displayCash; }
	void DisplayCash(const bool state) { data.gameHud.displayCash = state; }

	const bool IsAmmoDisplayed() { return data.gameHud.displayAmmo; }
	void DisplayAmmo(const bool state) { data.gameHud.displayAmmo = state; }

	const bool IsScriptedCameraActive() { return data.ScriptedCamera.active; }
	const CVector3 GetScriptCameraPosition() { return data.ScriptedCamera.position; }

	CBlip* GetBlip() { return &blip; }
};
extern std::vector<CPlayer> g_Players;
#endif
