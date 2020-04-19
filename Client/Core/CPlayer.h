#ifndef __CPLAYER_H__
#define __CPLAYER_H__
#include "CEntity.h" // The base class
#include <RakNetTypes.h>

#include "APIStructs.h"
#include "Ped.h"
#include "Weapon.h"
#include "GUI.h"

#include "CBlip.h"

class CPlayer : public CEntity
{
private:
	struct Grand
	{
		CBlip blip;

		Object aimingObject;	// For Aiming while moving
		Object followObject;	// For Aiming while moving
	} grand;

	struct Player
	{
		std::string	name;
	} player;

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

		struct WeaponData
		{
			Hash hash;

			struct MeleeData
			{
				bool inMeleeCombat = false;
				bool swingFinished = true;
				bool isAnimPlaying = false;

				std::string meleeDic;
				std::string meleeAnim;

				float animationEnd = 0.f;
				unsigned long lastMelee;

				int targetId = -1;
				int targetType = -1;
			} melee;

			bool reloading;
			unsigned long lastReload;

			bool aiming = false;
			CVector3 aimPosition;
			int aimMoveSpeed = -1;

			bool justShot = false;
		} weapon;

		struct Vehicle
		{
			int entityID = -1;
			int seat = -1;
		} vehicle;

		struct FootAnim
		{
			std::string animation;
			std::string dictionary;
		} onFootAnim;

		float forwardSpeed;
		int task;
		bool jumping;

		int armour = 100;
	} data;

public:
	CPlayer() = delete;
	CPlayer(const int id, std::string Name);
	~CPlayer();

	// Copy constructor
	CPlayer(const CPlayer & e);

	// Copy Assign 
	CPlayer &operator=(const CPlayer &e);

	bool CreatePed();
	void RequestData();
	void Destroy();
	void Delete();

	void Pulse();
	//void UpdateMovement(RakNet::Packet *packet);

	void Update(RakNet::Packet *packet);

	void Interpolate();

	void SetTargetPosition();
	void SetTargetRotation();

	void UpdateTargetAnimations();
	void UpdateTargetData();
	void UpdatePlayerModel();

	void ShowNametag();

	bool IsTargetAnimal();
	bool GetAnimation(char *dict, char *anim);

	std::string GetUsername() { return player.name; }

	Ped	GetPed() { return game.entity; }
	bool IsCreated() { return game.created; }

	CVector3 GetPosition() { return transform.Position; }
	CVector3 GetRotation() { return transform.Rotation; }

	void SetModelComponent(const int componentid, const int drawableid, const int textureid, const int paletteid);
	void SetModelHeadBlend(const int shapeFirst, const int shapeSecond, const int shapeThird, const int skinFirst, const int skinSecond, const int skinThird, const float shapeMix, const float skinMix, const float thirdMix);
	void SetModelHeadOverlay(const int overlayid, const int index, const int colorType, const int colorid, const int secondColorid, const float opacity);
	void SetModelProp(const int componentid, const int drawableid, const int textureid);
	void SetModelFaceFeature(const int index, const float scale);

	void TaskShoot(const std::string weapon, const int ammo, const CVector3 aim);
	void SetAiming(const bool aiming) { data.weapon.aiming = aiming; }

	const int GetArmour() { return data.armour; }
	void SetArmour(const int armour);

	void Revive();

	CBlip* Blip() { return &grand.blip; }
};
extern std::vector<CPlayer> g_Players;

#endif