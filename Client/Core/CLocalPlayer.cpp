// RakNet
#include <BitStream.h>
#include <PacketPriority.h>
#include <RakNetTypes.h>
#include <MessageIdentifiers.h>

#include "Memory.h"
#include "hook/enums.h"
#include "hook/natives.h"
#include <SharedUtility.h>

#include "Functions.h"
#include "APIStructs.h"
#include "Ped.h"
#include "Tasks.h"
#include "CGui.h"
#include "Common.h"
#include "World.h"
#include "CVehicle.h"
#include "GamePed.h"
#include "CWorld.h"
#include "CPlayer.h"
#include "CObject.h"
#include "CNPC.h"
#include "CNetworkManager.h"
#include "Animations.h"

#include "NetworkCallbacks.h"
#include "CMain.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CLocalPlayer.h"

CLocalPlayer::Identity CLocalPlayer::identity;
CLocalPlayer::World CLocalPlayer::world;

GrandM::GUI::Tag CLocalPlayer::tag;

Transform CLocalPlayer::transform;

bool CLocalPlayer::dead = false;
int	CLocalPlayer::lastAttacker = -1;

CLocalPlayer::Game CLocalPlayer::game;
CLocalPlayer::Info CLocalPlayer::info;
CLocalPlayer::Data CLocalPlayer::data;
CLocalPlayer::PlayerControl CLocalPlayer::control;
CLocalPlayer::Sync CLocalPlayer::sync;
CLocalPlayer::PlayerNetwork CLocalPlayer::network;
//CLocalPlayer::Data::WeaponData CLocalPlayer::game.weaponData;

std::vector<int> CLocalPlayer::activeTasks;

void CLocalPlayer::Init()
{
	LOG_DEBUG("[CLocalPlayer] Initializing...");

	if (CUTSCENE::IS_CUTSCENE_ACTIVE())
	{
		CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
		CUTSCENE::REMOVE_CUTSCENE();
	}

	if (PLAYER::_IS_PLAYER_CAM_CONTROL_DISABLED())
		PLAYER::SET_PLAYER_CONTROL(CLocalPlayer::GetPlayer(), true, 256);

	game.player = PLAYER::PLAYER_ID();
	game.ped = PLAYER::GET_PLAYER_PED(game.player);
	game.vehicleEntering = 0;

	identity.entityId = -1;

	data.vehicle.entityID = -1;
	data.vehicle.seat = -1;
	
	GAMEPLAY::_SET_UNK_MAP_FLAG(2);
	GAMEPLAY::_SET_UNK_MAP_FLAG(0);

	ENTITY::FREEZE_ENTITY_POSITION(CLocalPlayer::GetPed(), false);

	GAMEPLAY::_RESET_LOCALPLAYER_STATE();

	UI::_SET_HUD_COLOUR(9, 93, 182, 229, 255);

	PED::SET_PED_COMBAT_ATTRIBUTES(game.ped, 13, 0);
	PED::SET_PED_COMBAT_ATTRIBUTES(game.ped, 14, 0);
	PED::SET_PED_COMBAT_ATTRIBUTES(game.ped, 8, 0);
	PED::SET_PED_COMBAT_ATTRIBUTES(game.ped, 9, 0);
	PED::SET_PED_ALERTNESS(game.ped, 0);
	PED::SET_PED_COMBAT_ABILITY(game.ped, 0);
	PED::SET_PED_COMBAT_RANGE(game.ped, 0);
	PED::SET_PED_SEEING_RANGE(game.ped, 0.0);
	PED::SET_PED_HEARING_RANGE(game.ped, 0.0);

	WEAPON::REMOVE_ALL_PED_WEAPONS(game.ped, false);
	PLAYER::DISABLE_PLAYER_VEHICLE_REWARDS(game.player);
	WEAPON::SET_PED_DROPS_WEAPONS_WHEN_DEAD(game.ped, false);

	ENTITY::SET_ENTITY_HEALTH(game.ped, 100);
	PED::SET_PED_ARMOUR(game.ped, 0);

	PLAYER::ENABLE_SPECIAL_ABILITY(game.player, false);

	STATS::STAT_SET_INT($("SP0_SPECIAL_ABILITY_UNLOCKED"), 0, false);
	STATS::STAT_SET_INT($("SP0_STAMINA"), 100, false);
	STATS::STAT_SET_INT($("SP0_STEALTH_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP0_LUNG_CAPACITY"), 100, false);
	STATS::STAT_SET_INT($("SP0_FLYING_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP0_SHOOTING_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP0_STRENGTH"), 100, false);
	STATS::STAT_SET_INT($("SP0_WHEELIE_ABILITY"), 100, false);

	STATS::STAT_SET_INT($("SP1_SPECIAL_ABILITY_UNLOCKED"), 0, false);
	STATS::STAT_SET_INT($("SP1_STAMINA"), 100, false);
	STATS::STAT_SET_INT($("SP1_STEALTH_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP1_LUNG_CAPACITY"), 100, false);
	STATS::STAT_SET_INT($("SP1_FLYING_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP1_SHOOTING_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP1_STRENGTH"), 100, false);
	STATS::STAT_SET_INT($("SP1_WHEELIE_ABILITY"), 100, false);

	STATS::STAT_SET_INT($("SP2_SPECIAL_ABILITY_UNLOCKED"), 0, false);
	STATS::STAT_SET_INT($("SP2_STAMINA"), 100, false);
	STATS::STAT_SET_INT($("SP2_STEALTH_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP2_LUNG_CAPACITY"), 100, false);
	STATS::STAT_SET_INT($("SP2_FLYING_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP2_SHOOTING_ABILITY"), 100, false);
	STATS::STAT_SET_INT($("SP2_STRENGTH"), 100, false);
	STATS::STAT_SET_INT($("SP2_WHEELIE_ABILITY"), 100, false);

	STATS::STAT_SET_INT($("MP0_SCRIPT_INCREASE_STAM"), 100, false);
	STATS::STAT_SET_INT($("MP0_SCRIPT_INCREASE_STRN"), 100, false);
	STATS::STAT_SET_INT($("MP0_SCRIPT_INCREASE_LUNG"), 100, false);
	STATS::STAT_SET_INT($("MP0_SCRIPT_INCREASE_DRIV"), 100, false);
	STATS::STAT_SET_INT($("MP0_SCRIPT_INCREASE_FLY"), 100, false);
	STATS::STAT_SET_INT($("MP0_SCRIPT_INCREASE_SHO"), 100, false);
	STATS::STAT_SET_INT($("MP0_SCRIPT_INCREASE_STL"), 100, false);
	STATS::STAT_SET_INT($("MP0_NO_MORE_TUTORIALS"), 1, false);

	PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(game.player, 0.0f);

	/* 48 89 5C 24 08 57 48 83 EC 20 33 C0 8B FA 48 8B D9 83 FA 01 75 ? 38 05 ? ? ? ? 0F 45 F8 */
	/*
	auto slowMo = Memory::pattern("48 89 5C 24 08 57 48 83 EC 20 33 C0 8B FA 48 8B D9 83 FA 01 75 ? 38 05 ? ? ? ? 0F 45 F8");

	char * location = slowMo.count(1).get(0).get<char>(2);
	*(unsigned short *)location = 0xC031; // xor eax, eax
	*(unsigned char *)(location + 2) = 0xC3; // ret
	*/

	//LOG_DEBUG("[CLocalPlayer] Disabling Weapon & Radio Wheel scaleform slowmotion.");
	//char* wheelslowmo = Memory::pattern("32 C0 F3 0F 11 09").count(6).get(0).get<char>(2);
	//Memory::nop(wheelslowmo, 6);

	// DEFAULT_SCRIPTED_CAMERA | DEFAULT_ANIMATED_CAMERA
	game.camera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", true);

	UI::SET_ABILITY_BAR_VALUE(0,0);
}

void CLocalPlayer::Destroy()
{
	LOG_DEBUG("[CLocalPlayer] Destroyed.");
	game.player = NULL;
	game.ped = 0;
	game.vehicleEntering = 0;

	data.vehicle.entityID = -1;
	data.vehicle.seat = -1;
}

void CLocalPlayer::Pulse()
{
	Hud();
	CGui::Debug::UpdateDebug();

	uint64_t now = getticktime();
	if ((sync.lastSyncSent + (1000 / 10)) <= now)
	{
		if (CNetworkManager::g_ConnectionState != CONSTATE_COND)
			return;

		if(!ENTITY::DOES_ENTITY_EXIST(game.ped))
			game.ped = PLAYER::GET_PLAYER_PED(game.player);

		if (game.thirdPersonOnly)
		{
			CAM::_DISABLE_FIRST_PERSON_CAM_THIS_FRAME();
			CAM::_DISABLE_VEHICLE_FIRST_PERSON_CAM_THIS_FRAME();

			if (CAM::IS_FOLLOW_VEHICLE_CAM_ACTIVE())
				if (CAM::GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() == 4)
					CAM::SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(0);
			else
				if (CAM::GET_FOLLOW_PED_CAM_VIEW_MODE() == 4)
					CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(0);
		}
		else if (game.firstPersonOnly)
		{
			CONTROLS::DISABLE_CONTROL_ACTION(2, ControlNextCamera, 1);

			if (CAM::IS_FOLLOW_VEHICLE_CAM_ACTIVE())
				if (CAM::GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() != 4)
					CAM::SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(4);
				else
					if (CAM::GET_FOLLOW_PED_CAM_VIEW_MODE() != 4)
						CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(4);
		}

		// Update the local players Data
		Vector3 Coordinates = ENTITY::GET_ENTITY_COORDS(game.ped, ENTITY::IS_ENTITY_DEAD(game.ped, 0));
		Vector3 Rotation = ENTITY::GET_ENTITY_ROTATION(game.ped, 2);
		Vector3 Velocity = ENTITY::GET_ENTITY_VELOCITY(game.ped);
			
		data.model.hash = ENTITY::GET_ENTITY_MODEL(game.ped);
		data.model.type = PED::GET_PED_TYPE(game.ped);

		Hash weapon;
		WEAPON::GET_CURRENT_PED_WEAPON(game.ped, &weapon, true);
		data.weaponData.heldWeapon = weapon;

		data.forwardSpeed = ENTITY::GET_ENTITY_SPEED(game.ped);
		transform.Position = CVector3(Coordinates.x, Coordinates.y, Coordinates.z);
		transform.Rotation = CVector3(Rotation.x, Rotation.y, Rotation.z);
		transform.Velocity = CVector3(Velocity.x, Velocity.y, Velocity.z);

		VehicleChecks();

		data.vehicle.entityID = GTAV::GamePed::GetVehicleObject(game.ped).GetID();
		data.vehicle.seat = GTAV::GamePed::GetVehicleSeat(game.ped);

		Hash model;
		if(data.vehicle.entityID != -1)
			model = ENTITY::GET_ENTITY_MODEL(GTAV::GamePed::GetVehicle(game.ped));

		// Hide Ability Bar when not in special vehicle. Lectro | Vindicator | Ruiner2 | Voltic2 | Opressor , all are specials [DISABLES RADIO TOO SO NO]
		/*if (Game.DirectorMode == true && (model == 640818791 || model == -1353081087 || model == 941494461 || model == 989294410 || model == 884483972))
		{
			Game.DirectorMode = false;
			UI::_SET_DIRECTOR_MODE(false);
		}
		else if(Game.DirectorMode == false && (Data.Vehicle.VehicleID == -1 || model != 640818791 || model != -1353081087 || model != 941494461 || model != 989294410 || model != 884483972))
		{
			Game.DirectorMode = true;
			UI::_SET_DIRECTOR_MODE(true);
		}*/
		// END


		//Task Loop
		std::vector<int> tempTasks;
		for (int i = 500; i > -1; i--)
		{
			if (BRAIN::GET_IS_TASK_ACTIVE(game.ped, i))
			{
				tempTasks.push_back(i);
			}
		}
		activeTasks = tempTasks;
		//End Task Loop

		data.task = GTAV::GamePed::GetPedTask(game.ped);

		WeaponChecks();

/*
				- START Attack Detection -
*/

		// #Melee temp block controls of attacking for melee untill 
		// i come up with a better detection method for melee
		// this blocks the control thus not detected swing
		if (control.Attacking && GTAV::Tasks::IsTaskActive(game.ped, GTAV::eTask::MELEE_COMBAT))
			CONTROLS::DISABLE_CONTROL_ACTION(2, ControlAttack, 1);
		else if (control.Attacking && !GTAV::Tasks::IsTaskActive(game.ped, GTAV::eTask::MELEE_COMBAT))
			control.Attacking = false;

/*
				- END Attack Detection -
*/

/*
				- START MELEE SYSTEM -
*/

		data.weaponData.isInMeleeCombat = PED::IS_PED_IN_MELEE_COMBAT(game.ped);
		data.weaponData.melee.target = GTAV::GamePed::GetPlayerMeleeTarget(game.player);

		//if (!data.weaponData.melee.target.IsNull())
		//{

		//}
		//else // No target so just swing
		//{

		//}

/*
				- END MELEE SYSTEM -
*/

		Vector3 aimPos = GTAV::World::ScreenRelToWorld(CAM::GET_GAMEPLAY_CAM_COORD(), CAM::GET_GAMEPLAY_CAM_ROT(0), Vector2(0, 0));

		

		/* Health Check for 'world' damage */
		const int curHealth = ENTITY::GET_ENTITY_HEALTH(game.ped);

		if (curHealth < data.health.health.Get())
		{
			GrandM::Network::Callback::OnEntityDamage(identity.entityId, (data.health.health.Get() - curHealth), -1, "World");
			data.health.health.Set(curHealth, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, 0);
		}


		// Send to server
		RakNet::BitStream bitstream;
		bitstream.Reset();
		bitstream.Write((unsigned char)ID_PACKET_MOVEMENT);
		// Player Entity Type & Id Data
		bitstream.Write(0); // type = 0 for player
		bitstream.Write(identity.entityId);
		// Player Position Data
		bitstream.Write(transform.Position.x);
		bitstream.Write(transform.Position.y);
		bitstream.Write(transform.Position.z);
		// Player Rotation Data
		bitstream.Write(transform.Rotation.x);
		bitstream.Write(transform.Rotation.y);
		bitstream.Write(transform.Rotation.z);
		// Player Velocity Data
		bitstream.Write(transform.Velocity.x);
		bitstream.Write(transform.Velocity.y);
		bitstream.Write(transform.Velocity.z);
		// Player Forward Speed Data
		bitstream.Write(data.forwardSpeed);
		CNetworkManager::GetInterface()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, CNetworkManager::GetSystemAddress(), false);



		bitstream.Reset();
		bitstream.Write((unsigned char)ID_PACKET_PLAYER);
		bitstream.Write(data.vehicle.entityID);
		bitstream.Write(data.vehicle.seat);

		bitstream.Write(data.task);

		bitstream.Write(data.weaponData.heldWeapon);
		bitstream.Write(data.weaponData.reloading);

		bitstream.Write(data.weaponData.isInMeleeCombat);
		bitstream.Write(data.weaponData.melee.target.GetType());
		bitstream.Write(data.weaponData.melee.target.GetID());

		// Aiming
		bitstream.Write(aimPos.x);
		bitstream.Write(aimPos.y);
		bitstream.Write(aimPos.z);

		// Script Camera
		bitstream.Write(game.cameraActive);
		if (game.cameraActive)
		{
			Vector3 camPos = CAM::GET_CAM_COORD(CLocalPlayer::GetScriptedCamera());
			CVector3 pos = { camPos.x, camPos.y, camPos.z };

			bitstream.Write(pos.x);
			bitstream.Write(pos.y);
			bitstream.Write(pos.z);
		}

		CNetworkManager::GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, RELIABLE_SEQUENCED, 0, CNetworkManager::GetSystemAddress(), false);

		sync.lastSyncSent = getticktime();
	}

	RakNet::BitStream bitstream;

	if (BRAIN::GET_IS_TASK_ACTIVE(game.ped, 290) != data.weaponData.aiming)
	{
		data.weaponData.aiming = !data.weaponData.aiming;
		
		bitstream.Reset();
		bitstream.Write(data.weaponData.aiming);
		CNetworkManager::GetRPC().Signal("OnPlayerAim", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
	}
}

void CLocalPlayer::VehicleChecks()
{
	const int vehicleId = GTAV::GamePed::GetVehicleObject(game.ped).GetID();

	// Single BitStream define we just reset each time we need it fresh
	RakNet::BitStream bitstream;

	// Check if player is exiting a vehicle
	if (data.vehicle.entityID > -1 && !control.controlVehicleExit)
	{
		control.controlVehicleExit = true;

		bitstream.Reset();
		bitstream.Write(data.vehicle.entityID);
		CNetworkManager::GetRPC().Signal("OnPlayerExitingVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
	}
	else if (control.controlVehicleExit)
	{
		control.controlVehicleExit = false;
	}

	// Check the Vehicle the player is attempting to enter
	/*if (PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(Game.Ped))
	{
		Vehicle enteringVehicle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(Game.Ped);
		if (enteringVehicle != Game.VehicleEntering)
		{
			for (int i = 0; i < g_Vehicles.size(); i++)
			{
				if (enteringVehicle == g_Vehicles[i].GetEntity())
				{
					if (g_Vehicles[i].GetOccupant(0) != -1)
					{
						AI::CLEAR_PED_TASKS_IMMEDIATELY(Game.Ped);
					}
					else
					{
						Game.VehicleEntering = enteringVehicle;

						//bitstream.Reset();
						//bitstream.Write(g_Vehicles[i].GetId());
						//CNetworkManager::GetRPC().Signal("OnPlayerEnteringVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
					}
					break;
				}
			}
		}
	}*/


	// Check the Vehicle the Player is in
	if (data.vehicle.entityID != vehicleId && GTAV::GamePed::GetVehicleSeat(game.ped) == 0 && GTAV::GamePed::GetVehicleObject(game.ped).GetID() != -1)
	{
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (vehicleId == g_Vehicles[i].GetID())
			{
				if (g_Vehicles[i].GetAssignee() != CNetworkManager::GetInterface()->GetMyGUID() && g_Vehicles[i].GetOccupant(0) == info.myEntityID)
				{
					g_Vehicles[i].SetAssignee(CNetworkManager::GetInterface()->GetMyGUID());

					bitstream.Reset();
					bitstream.Write(g_Vehicles[i].GetID());
					bitstream.Write(g_Vehicles[i].GetType());
					CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
				}
				else if (g_Vehicles[i].GetOccupant(0) != -1)
				{
					BRAIN::TASK_STAND_STILL(game.ped, 1);
					//AI::CLEAR_PED_TASKS_IMMEDIATELY(Game.Ped);
				}
				break;
			}
		}
	}

	if (vehicleId != -1)
	{
		const bool horn = PLAYER::IS_PLAYER_PRESSING_HORN(game.player);
		if (data.pressingHorn != horn)
		{
			data.pressingHorn = horn;

			bitstream.Reset();
			bitstream.Write(vehicleId);
			bitstream.Write(horn);
			CNetworkManager::GetRPC().Signal("StartVehicleHorn", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
		}
	}
}

Ped	CLocalPlayer::GetPed()
{
	if (!ENTITY::DOES_ENTITY_EXIST(game.ped))
		return game.ped = PLAYER::GET_PLAYER_PED(game.player);

	return game.ped;
}

void CLocalPlayer::GiveWeapon(const std::string weapon, int ammo)
{
	Hash wHash = $(weapon);
	
	if (WEAPON::IS_WEAPON_VALID(wHash))
	{
		int maxAmmo, clipMaxAmmo;
		WEAPON::GET_MAX_AMMO(game.ped, wHash, &maxAmmo);
		clipMaxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(game.ped, wHash, 0);

		if (ammo > maxAmmo)
			ammo = maxAmmo;

		
		for (unsigned int i = 0; i < data.weaponData.weapons.size(); i++)
		{
			if (std::strcmp(weapon.c_str(), data.weaponData.weapons[i].name.c_str()) == 0)
			{
				if (data.weaponData.weapons[i].ammo + ammo > maxAmmo)
				{
					ammo = maxAmmo;
				}
				else 
				{
					ammo += data.weaponData.weapons[i].ammo;
				}

				data.weaponData.weapons[i].ammo = ammo;

				RakNet::BitStream bitstream;
				bitstream.Write(RakNet::RakString(weapon.c_str()));
				bitstream.Write(data.weaponData.weapons[i].ammo);
				bitstream.Write(data.weaponData.weapons[i].clipAmmo);
				CNetworkManager::GetRPC().Signal("UpdateWeapon", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
					
				WEAPON::GIVE_WEAPON_TO_PED(game.ped, wHash, ammo, false, false);
				WEAPON::SET_PED_AMMO(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].ammo, 0);
				WEAPON::SET_AMMO_IN_CLIP(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].clipAmmo);
				return;
			}
		}
		
		Data::WeaponData::Weapon newWeapon;

		newWeapon.name = weapon;
		newWeapon.hash = wHash;
		newWeapon.ammo = ammo - clipMaxAmmo;
		newWeapon.clipAmmo = clipMaxAmmo;

		RakNet::BitStream bitstream;
		bitstream.Write(RakNet::RakString(weapon.c_str()));
		bitstream.Write(newWeapon.ammo);
		bitstream.Write(newWeapon.clipAmmo);
		CNetworkManager::GetRPC().Signal("UpdateWeapon", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

		WEAPON::GIVE_WEAPON_TO_PED(game.ped, wHash, ammo, false, false);
		WEAPON::SET_PED_AMMO(game.ped, newWeapon.hash, newWeapon.ammo, 0);
		WEAPON::SET_AMMO_IN_CLIP(game.ped, newWeapon.hash, newWeapon.clipAmmo);

		data.weaponData.weapons.push_back(newWeapon);
	}
	else
	{
		RakNet::BitStream bitstream;
		bitstream.Write(RakNet::RakString(weapon.c_str()));
		CNetworkManager::GetRPC().Signal("InvalidWeapon", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
	}
}

void CLocalPlayer::RemoveWeapon(const std::string weapon) 
{
	for (unsigned int i = 0; i <  data.weaponData.weapons.size(); i++)
	{
		if (std::strcmp(weapon.c_str(), data.weaponData.weapons[i].name.c_str()) == 0)
		{
			data.weaponData.weapons.erase(data.weaponData.weapons.begin() + i);
			data.weaponData.weapons.shrink_to_fit();

			Hash wHash = $(weapon);
			WEAPON::REMOVE_WEAPON_FROM_PED(game.ped, wHash);
			return;
		}
	}
}

/// the anti-cheat & shooting/attacking needs to be seperate
void CLocalPlayer::WeaponChecks() 
{
	Hash weapon = WEAPON::GET_SELECTED_PED_WEAPON(game.ped);
	int wepClass = GTAV::Weapon::GetWeaponClass(weapon);



	if (wepClass == 0)
	{
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, ControlAttack) && CONTROLS::IS_CONTROL_ENABLED(2, ControlAttack) && (PED::IS_PED_IN_MELEE_COMBAT(game.ped) || GTAV::Tasks::IsTaskActive(game.ped, GTAV::eTask::MELEE_COMBAT)))
		{
			/*
			if shooting and not aim prevented and not task melee combat
			or
			isinmeleecombat or task melee combat and control attack pressed
			*/
			OnPlayerShot("weapon_unarmed", 0, 0);
			control.Attacking = true;
		}
	}
	else
	{
		bool success = false;
		for (unsigned int i = 0; i < data.weaponData.weapons.size(); i++)
		{
			if (data.weaponData.weapons[i].hash == weapon)
			{
				if (wepClass == 1 || wepClass == 2)
				{
					if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, ControlAttack) && (PED::IS_PED_IN_MELEE_COMBAT(game.ped) || GTAV::Tasks::IsTaskActive(game.ped, GTAV::eTask::MELEE_COMBAT)))
					{
						OnPlayerShot(data.weaponData.weapons[i].name, 0, 0);
						control.Attacking = true;
					}
				}
				else
				{
					// Fetch the current amount of ammo in the clip
					int ammoInClip;
					WEAPON::GET_AMMO_IN_CLIP(game.ped, weapon, &ammoInClip);

					// Check if the ammount in the clip is less than what we have stored and that its not 0 and that what we have stored is atleast one.
					if (ammoInClip < data.weaponData.weapons[i].clipAmmo && !(ammoInClip == 0 && data.weaponData.weapons[i].clipAmmo != 1))
					{
						data.weaponData.weapons[i].clipAmmo = ammoInClip;

						data.justShot = i;
						OnPlayerShot(data.weaponData.weapons[i].name, data.weaponData.weapons[i].ammo, data.weaponData.weapons[i].clipAmmo);
					}

					if (ammoInClip != data.weaponData.weapons[i].clipAmmo)
					{
						data.weaponData.weapons[i].clipAmmo = ammoInClip;
						if (data.weaponData.weapons[i].clipAmmo > 0)
						{
							WEAPON::SET_PED_AMMO(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].ammo, 0);
							WEAPON::SET_AMMO_IN_CLIP(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].clipAmmo);
						}
					}
				}

				success = true;
				break;
			}
		}

		OnProjectileImpact();

		if (!success)
			UpdateWeapons(weapon);
	}
}

void CLocalPlayer::UpdateWeapons(Hash weapon)
{
	// This would allow someone to cheat guns when in a vehicle but once they get out they would be stripped from the player.
	if (data.vehicle.entityID != -1)
		return;

	WEAPON::REMOVE_ALL_PED_WEAPONS(game.ped, true);
	
	for (unsigned int i = 0; i <  data.weaponData.weapons.size(); i++)
	{
		WEAPON::GIVE_WEAPON_TO_PED(game.ped, data.weaponData.weapons[i].hash, 0, false, false);

		WEAPON::SET_PED_AMMO(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].ammo, 0);
		WEAPON::SET_AMMO_IN_CLIP(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].clipAmmo);
	}
}

void CLocalPlayer::OnPlayerShot(const std::string weapon, const int ammo, const int clipAmmo) 
{
	Vector3 aimPos = GTAV::World::ScreenRelToWorld(CAM::GET_GAMEPLAY_CAM_COORD(), CAM::GET_GAMEPLAY_CAM_ROT(0), Vector2(0, 0));

	RakNet::BitStream bitstream;
	bitstream.Write(RakNet::RakString(weapon.c_str()));
	bitstream.Write(ammo);
	bitstream.Write(clipAmmo);

	bitstream.Write(aimPos.x);
	bitstream.Write(aimPos.y);
	bitstream.Write(aimPos.z);

	CNetworkManager::GetRPC().Signal("OnPlayerShot", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
}

void CLocalPlayer::MainMenu()
{
	data.health.health.Set(200);
	data.health.maxHealth = 200;
	
	PED::REVIVE_INJURED_PED(game.ped);
	PED::RESURRECT_PED(game.ped);
	GAMEPLAY::_RESET_LOCALPLAYER_STATE();
	PLAYER::RESET_PLAYER_ARREST_STATE(game.ped);
	ENTITY::SET_ENTITY_HEALTH(game.ped, data.health.maxHealth);
	PED::RESET_PED_VISIBLE_DAMAGE(game.ped);

	CAM::SET_CAM_COORD(game.camera, 0, 0, 0);
	CAM::DETACH_CAM(game.camera);
	CAM::SET_CAM_ACTIVE(game.camera, false);
	CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 1, 0, 0);
	
	CWorld::SetTime(7, 0, 0);
	
	//ENTITY::SET_ENTITY_COORDS_NO_OFFSET(CLocalPlayer::GetPed(), 464.f, 5621.f, 786.f, false, false, false);
	//STREAMING::LOAD_SCENE(464.f, 5621.f, 786.f);
	STREAMING::LOAD_ALL_OBJECTS_NOW();

	//ENTITY::FREEZE_ENTITY_POSITION(CLocalPlayer::GetPed(), true);
	//ENTITY::SET_ENTITY_VISIBLE(CLocalPlayer::GetPed(), false, 0); // No longer really need this as the idle screen is hidden by GUI now
	//ENTITY::SET_ENTITY_ALPHA(CLocalPlayer::GetPed(), 0, false); // No longer really need this as the idle screen is hidden by GUI now
	
	data.weaponData.weapons.clear();
	WEAPON::REMOVE_ALL_PED_WEAPONS(CLocalPlayer::GetPed(), false);

	UI::DISPLAY_RADAR(false);
	UI::DISPLAY_HUD(false);

	CGui::SetConnected(false);
	CGui::ShowMainMenu(true, true);
	CGui::ClearChat();
	CGui::ClearFrame();

	CAM::DO_SCREEN_FADE_IN(5000);
}

void CLocalPlayer::SetWeaponAmmo(const std::string weapon, const int ammo, const int clipAmmo)
{
	for (unsigned int i = 0; i <data.weaponData.weapons.size(); i++)
	{
		if (std::strcmp(weapon.c_str(), data.weaponData.weapons[i].name.c_str()) == 0)
		{
			data.weaponData.weapons[i].ammo = ammo;
			data.weaponData.weapons[i].clipAmmo = clipAmmo;

			WEAPON::SET_PED_AMMO(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].ammo, 0);
			WEAPON::SET_AMMO_IN_CLIP(game.ped, data.weaponData.weapons[i].hash, data.weaponData.weapons[i].clipAmmo);
			return;
		}
	}
}

void CLocalPlayer::EquipWeapon(const std::string weapon)
{
	for (unsigned int i = 0; i < data.weaponData.weapons.size(); i++)
	{
		if (std::strcmp(weapon.c_str(), data.weaponData.weapons[i].name.c_str()) == 0)
		{
			WEAPON::SET_CURRENT_PED_WEAPON(game.ped, data.weaponData.weapons[i].hash, true);
			return;
		}
	}
}

void CLocalPlayer::OnProjectileImpact()
{
	if (data.justShot == -1)
		return;

	Vector3 bulletPos = Vector3{ 0,0,0 };
	if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(game.ped, &bulletPos)) // right now the returned pos is broken as I need to patch the returned vector
	{
		//std::cout << "Impact @ " << bulletPos.x << ", " << bulletPos.y << ", " << bulletPos.z << std::endl;

		// Players check
		if (!g_Players.empty())
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].IsCreated())
				{
					if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(g_Players[i].GetPed(), data.weaponData.weapons[data.justShot].hash, 0))
					{
						int bone;
						PED::GET_PED_LAST_DAMAGE_BONE(g_Players[i].GetPed(), &bone);
						PED::CLEAR_PED_LAST_DAMAGE_BONE(g_Players[i].GetPed());

						
						// Send OnProjectileImpact
						RakNet::BitStream sData;
						sData.Write(identity.entityId);
						sData.Write(RakNet::RakString(data.weaponData.weapons[data.justShot].name.c_str()));
						sData.Write(bulletPos.x);
						sData.Write(bulletPos.y);
						sData.Write(bulletPos.z);
						sData.Write(g_Players[i].GetID());
						sData.Write(bone);
						CNetworkManager::GetRPC().Signal("OnProjectileImpact", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);


						int damageDealt = (g_Players[i].GetHealth() - ENTITY::GET_ENTITY_HEALTH(g_Players[i].GetPed())) + (g_Players[i].GetArmour() - PED::GET_PED_ARMOUR(g_Players[i].GetPed()));
						ENTITY::SET_ENTITY_HEALTH(g_Players[i].GetPed(), g_Players[i].GetHealth());
						PED::SET_PED_ARMOUR(g_Players[i].GetPed(), g_Players[i].GetArmour());
						
						if (damageDealt != 0) 
							GrandM::Network::Callback::OnEntityDamage(g_Players[i].GetID(), damageDealt, info.myEntityID, data.weaponData.weapons[data.justShot].name);

						data.justShot = -1;

						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Players[i].GetPed());
						return;
					}
					else
					{
						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Players[i].GetPed());
					}
				}
			}
		}

		// Vehicles check
		if (!g_Vehicles.empty())
		{
			for (unsigned int i = 0; i < g_Vehicles.size(); i++)
			{
				if (g_Vehicles[i].IsCreated())
				{
					if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(g_Vehicles[i].GetEntity(), data.weaponData.weapons[data.justShot].hash, 0))
					{
						// Send OnProjectileImpact
						RakNet::BitStream sData;
						sData.Write(identity.entityId);
						sData.Write(RakNet::RakString(data.weaponData.weapons[data.justShot].name.c_str()));
						sData.Write(bulletPos.x);
						sData.Write(bulletPos.y);
						sData.Write(bulletPos.z);
						sData.Write(g_Vehicles[i].GetID());
						sData.Write(-1);
						CNetworkManager::GetRPC().Signal("OnProjectileImpact", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

						//int damageDealt = g_Vehicles[i].vehicleEngineHealth - VEHICLE::GET_VEHICLE_ENGINE_HEALTH(g_Vehicles[i].GetEntity());
						//VEHICLE::SET_VEHICLE_ENGINE_HEALTH(g_Vehicles[i].GetEntity(), g_Vehicles[i].vehicleEngineHealth);

						//int fuelDamageDealt = g_Vehicles[i].vehicleFuelTankHealth - VEHICLE::GET_VEHICLE_PETROL_TANK_HEALTH(g_Vehicles[i].GetEntity());
						//VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(g_Vehicles[i].GetEntity(), g_Vehicles[i].vehicleFuelTankHealth);
						int damageDealt = g_Vehicles[i].GetHealth() - ENTITY::GET_ENTITY_HEALTH(g_Vehicles[i].GetEntity());
						ENTITY::SET_ENTITY_HEALTH(g_Vehicles[i].GetEntity(), g_Vehicles[i].GetHealth());

						if (damageDealt != 0)
							GrandM::Network::Callback::OnEntityDamage(g_Vehicles[i].GetID(), damageDealt, info.myEntityID, data.weaponData.weapons[data.justShot].name);

						data.justShot = -1;

						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Vehicles[i].GetEntity());
						return;

					}
					else
					{
						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Vehicles[i].GetEntity());
					}
				}
			}
		}

		// Object check
		if (!g_Objects.empty())
		{
			for (unsigned int i = 0; i < g_Objects.size(); i++)
			{
				if (g_Objects[i].IsCreated())
				{
					if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(g_Objects[i].GetEntity(), data.weaponData.weapons[data.justShot].hash, 0))
					{
						// Send OnProjectileImpact
						RakNet::BitStream sData;
						sData.Write(identity.entityId);
						sData.Write(RakNet::RakString(data.weaponData.weapons[data.justShot].name.c_str()));
						sData.Write(bulletPos.x);
						sData.Write(bulletPos.y);
						sData.Write(bulletPos.z);
						sData.Write(g_Objects[i].GetID());
						sData.Write(-1);
						CNetworkManager::GetRPC().Signal("OnProjectileImpact", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
						
						data.justShot = -1;

						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Objects[i].GetEntity());
						return;
					}
					else
					{
						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Objects[i].GetEntity());
					}
				}
			}
		}

		// NPC check
		if (!g_Npcs.empty())
		{
			for (unsigned int i = 0; i < g_Npcs.size(); i++)
			{
				if (g_Npcs[i].IsCreated())
				{
					if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(g_Npcs[i].GetEntity(), data.weaponData.weapons[data.justShot].hash, 0))
					{
						int bone;
						PED::GET_PED_LAST_DAMAGE_BONE(g_Npcs[i].GetEntity(), &bone);
						PED::CLEAR_PED_LAST_DAMAGE_BONE(g_Npcs[i].GetEntity());

						// Send OnProjectileImpact
						RakNet::BitStream sData;
						sData.Write(identity.entityId);
						sData.Write(RakNet::RakString(data.weaponData.weapons[data.justShot].name.c_str()));
						sData.Write(bulletPos.x);
						sData.Write(bulletPos.y);
						sData.Write(bulletPos.z);
						sData.Write(g_Npcs[i].GetID());
						sData.Write(bone);
						CNetworkManager::GetRPC().Signal("OnProjectileImpact", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

						int damageDealt = (g_Npcs[i].GetHealth() - ENTITY::GET_ENTITY_HEALTH(g_Npcs[i].GetEntity())) + (g_Npcs[i].GetArmour() - PED::GET_PED_ARMOUR(g_Npcs[i].GetEntity()));
						ENTITY::SET_ENTITY_HEALTH(g_Npcs[i].GetEntity(), g_Npcs[i].GetHealth());
						PED::SET_PED_ARMOUR(g_Npcs[i].GetEntity(), g_Npcs[i].GetArmour());

						if (damageDealt != 0)
							GrandM::Network::Callback::OnEntityDamage(g_Npcs[i].GetID(), damageDealt, info.myEntityID, data.weaponData.weapons[data.justShot].name);

						data.justShot = -1;

						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Npcs[i].GetEntity());
						return;
					}
					else
					{
						WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(g_Npcs[i].GetEntity());
					}
				}
			}
		}

		// Send OnProjectileImpact
		RakNet::BitStream sData;
		sData.Write(identity.entityId);
		sData.Write(RakNet::RakString(data.weaponData.weapons[data.justShot].name.c_str()));
		sData.Write(bulletPos.x);
		sData.Write(bulletPos.y);
		sData.Write(bulletPos.z);
		sData.Write(-1);
		sData.Write(-1);
		CNetworkManager::GetRPC().Signal("OnProjectileImpact", &sData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

		data.justShot = -1;
	}
}

void CLocalPlayer::Hud()
{
	if (!game.hud.displayAmmo)
	{
		UI::DISPLAY_AMMO_THIS_FRAME(false);
	}
}

void CLocalPlayer::SetScriptedCameraActive(const bool state)
{
	game.cameraActive = state;

	if (state)
	{
		CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, 1, 0, 0);
	}
	else
	{
		CAM::SET_CAM_COORD(game.camera, 0, 0, 0);
		CAM::DETACH_CAM(game.camera);
		CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 1, 0, 0);
	}
	
	CAM::SET_CAM_ACTIVE(game.camera, state);
}

void CLocalPlayer::SetDimension(const int dim)
{
	world.dim = dim; 

	CWorld::Destroy();
}