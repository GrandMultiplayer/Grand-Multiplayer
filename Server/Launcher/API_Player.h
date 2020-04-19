#ifndef __APIPLAYER_H__
#define __APIPLAYER_H__
#pragma once
#include <stdint.h>

#include "Common.h"
#include "API_Objects.h"

namespace API
{
	namespace Player
	{
		DLL_PUBLIC_C const uint32_t GetPlayerModel(Objects::Entity entity);
		DLL_PUBLIC_C void SetPlayerModel(Objects::Entity entity, const uint32_t model);

		DLL_PUBLIC_C const char * GetUsername(Objects::Entity entity);
		DLL_PUBLIC_C void SetUsername(Objects::Entity entity, const char * name);

		DLL_PUBLIC_C const bool IsControllable(Objects::Entity entity);
		DLL_PUBLIC_C void SetControllable(Objects::Entity entity, bool disablecontrols, bool frozen = false);

		DLL_PUBLIC_C const char * GetIP(Objects::Entity entity);

		DLL_PUBLIC_C const char * GetSecretKey(Objects::Entity entity);

		DLL_PUBLIC_C void Kick(Objects::Entity entity, const char * reason);

		DLL_PUBLIC_C void PutInVehicle(Objects::Entity playerEntity, Objects::Entity vehicleEntity, const int seat);
		DLL_PUBLIC_C Objects::Entity GetVehicle(Objects::Entity entity);
		DLL_PUBLIC_C const int GetSeat(Objects::Entity entity);

		DLL_PUBLIC_C void GiveWeapon(Objects::Entity entity, const char * weapon, const int ammo = 0);
		DLL_PUBLIC_C void RemoveWeapon(Objects::Entity entity, const char * weapon);

		DLL_PUBLIC_C void GetWeaponAmmo(Objects::Entity entity, const char * weapon, int* ammo, int* clipAmmo);
		DLL_PUBLIC_C void SetWeaponAmmo(Objects::Entity entity, const char * weapon, const int ammo, const int clipAmmo);

		DLL_PUBLIC_C char ** GetWeapons(Objects::Entity entity);

		DLL_PUBLIC_C void EquipWeapon(Objects::Entity entity, const char * weapon);
	}
}

#endif