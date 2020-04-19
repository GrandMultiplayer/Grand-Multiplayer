#ifndef __CLOCALPLAYER_H__
#define __CLOCALPLAYER_H__
#include <string>
#include <vector>

#include "hook/types.h"
#include "hook/natives.h"

#include "CVector3.h"
#include "Transform.h"
#include "APIStructs.h"
#include "GUI.h"

#include "CEntity.h"
#include "SyncValue.h"

static class CLocalPlayer
{
private:
	static struct Identity
	{
		int	entityId = -1;
	} identity;

	static struct World
	{
		float streamDistance;
		int dim; // Dimension
	} world;

	static GrandM::GUI::Tag tag;

	static Transform transform;

	//static int	health;
	static bool dead;
	static int	lastAttacker;

	static struct Game
	{
		Player player;
		Ped ped;

		Vehicle vehicleEntering;

		Cam camera;
		bool cameraActive = false;

		bool firstPersonOnly = false;
		bool thirdPersonOnly = false;

		struct GameHud
		{
			bool displayAmmo = true;
		} hud;
	} game;

	static struct Info
	{
		int myEntityID;
		std::string	Name;
	} info;

	static struct Data
	{
		struct Model
		{
			Hash hash;
			int type;
		} model;

		struct WeaponData
		{
			long			heldWeapon;

			bool			reloading = false;

			bool			aiming = false;

			bool			isInMeleeCombat = false;

			struct Weapon
			{
				std::string		name;
				long			hash;

				int				clipAmmo = 0;
				int				ammo = 0;

				bool			equiped = false;

				bool			lazer = false;

				// Tints = 0:Normal - 1:Green - 2:Gold - 3:Pink - 4:Army - 5:LSPD  6:Orange - 7:Platinum [think there is more neeed to test, like antique for example]
				int				tint = 0;

				// GIVE_WEAPON_COMPONENT_TO_PED | GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT 
				// Compontents, Needs some research
			};

			std::vector<Weapon> weapons;


			struct MeleeData
			{
				bool inMeleeCombat = false;
				bool swingFinished = true;
				bool isAnimPlaying = false;

				std::string meleeDic;
				std::string meleeAnim;

				float animationEnd = 0.f;
				unsigned long lastMelee;

				CEntity target = CEntity();
			} melee;

		} weaponData;
		unsigned int justShot = -1;

		struct Vehicle
		{
			int entityID;
			int seat;
		} vehicle;

		struct PlayerHealth
		{
			SyncInt health = SyncInt(200, "EntityHealth");
			int maxHealth = 200;
		} health;

		int task;

		float forwardSpeed;

		bool canControl = true;
		bool frozen = false;

		bool pressingHorn = false;
	} data;

	static struct PlayerControl
	{
		bool controlVehicleExit;
		bool Attacking = false;
	} control;

	static struct Sync
	{
		uint64_t lastSyncSent;
	} sync;

	static struct PlayerNetwork
	{
		std::string	secretKey;
	} network;

public:
	static std::vector<int> activeTasks;

	static void Init();
	static void Destroy();

	static void Pulse();
	static void VehicleChecks();

	static Player GetPlayer() { return game.player; }

	static Ped	GetPed();
	static void SetPed(Ped ped) { game.ped = ped; }

	static void SetID(int entityid) { identity.entityId = entityid; }
	static const int GetID() { return identity.entityId; }

	static const std::string GetSecretKey() { return network.secretKey; }
	static void SetSecretKey(std::string secretkey) { network.secretKey = secretkey; }

	static bool IsInAnyVehicle() { return (bool)PED::IS_PED_IN_ANY_VEHICLE(game.ped, FALSE); }

	static CVector3 GetPosition() { return transform.Position; }
	static Vector3 GetPositionEx() { Vector3 Pos = { transform.Position.x, transform.Position.y, transform.Position.z }; return Pos; }

	static CVector3 GetRotation() { return transform.Rotation; }

	static CVector3 GetVelocity() { return transform.Velocity; }

	static float GetForwardSpeed() { return data.forwardSpeed; }

	static int GetVehicleID() { return data.vehicle.entityID; }
	static int GetSeat() { return data.vehicle.seat; }

	static BOOL IsPlaying() { return PLAYER::IS_PLAYER_PLAYING(game.player); }

	static const bool IsControllable() { return data.canControl; }
	static void SetControllable(const bool controlable) 
	{ 
		data.canControl = controlable;
	}

	static const bool IsFrozen() { return data.frozen; }
	static void SetFrozen(const bool frozen) { data.frozen = frozen; }

	static const int GetTask() { return data.task; }

	static void DisplayAmmo(const bool state) { game.hud.displayAmmo = state; }

	static void GiveWeapon(const std::string weapon, int ammo);
	static void RemoveWeapon(const std::string weapon);

	static void WeaponChecks();
	static void UpdateWeapons(Hash weapon);
	static void OnPlayerShot(const std::string weapon, const int ammo, const int clipAmmo);
	static void SetWeaponAmmo(const std::string weapon, const int ammo, const int clipAmmo);
	static void EquipWeapon(const std::string weapon);
	static void OnProjectileImpact();

	static void MainMenu();

	static void Hud();

	static void SetHealth(const int health, RakNet::RakNetGUID sender) { data.health.health.Set(health, sender, identity.entityId, 0); }
	static const int GetHealth() { return data.health.health.Get(); }
	static void SetMaxHealth(const int health) { data.health.maxHealth = health; }
	static const int GetMaxHealth() { return data.health.maxHealth; }

	static Cam GetScriptedCamera() { return game.camera; }

	static void SetScriptedCameraActive(const bool state);
	static const bool IsScriptedCameraActive() { return game.cameraActive; }

	static const bool IsFirstPersonOnly() { return game.firstPersonOnly; }
	static void SetFirstPersonOnly() { game.firstPersonOnly = true; game.thirdPersonOnly = false; }

	static const bool IsThirdPersonOnly() { return game.thirdPersonOnly; }
	static void SetThirdPersonOnly() { game.thirdPersonOnly = true; game.firstPersonOnly = false; }

	static void SetDimension(const int dim);
	static const int GetDimension() { return world.dim; }
};
#endif