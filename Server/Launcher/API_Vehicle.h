#ifndef __APIVEHICLE_H__
#define __APIVEHICLE_H__
#pragma once
#include <stdint.h>

#include "Common.h"
#include "API_Objects.h"
#include "APIStructs.h"
#include "CVector3.h"

using Objects::Entity;

namespace API
{
	namespace Vehicle
	{
//#pragma comment(linker, "/export:API::Vehicle::Create=Create")
		DLL_PUBLIC_C Entity CreateVehicle(const uint32_t model, const CVector3 position, const CVector3 rotation);
		
		DLL_PUBLIC_C const int GetVehicleColor(Entity entity, const int layer);
		DLL_PUBLIC_C const int GetVehicleColorType(Entity entity, const int layer);
		DLL_PUBLIC_C void SetVehicleColor(Entity entity, const int layer, const int painttype, const int color);

		DLL_PUBLIC_C const Color GetVehicleRGBColor(Entity entity, const int layer);
		DLL_PUBLIC_C void SetVehicleRGBColor(Entity entity, const int layer, const Color color);

		DLL_PUBLIC_C void SetNumberPlate(Entity entity, const char * plate);
		DLL_PUBLIC_C const char * GetNumberPlate(Objects::Entity entity);

		DLL_PUBLIC_C const int GetMod(Entity entity, const int modType);
		DLL_PUBLIC_C void SetMod(Entity entity, const int modType, const int modIndex);

		DLL_PUBLIC_C const bool GetEngineState(Entity entity);
		DLL_PUBLIC_C void SetEngineState(Entity entity, const bool state, const bool instant);

		DLL_PUBLIC_C const int GetDoorsLockState(Entity entity);
		DLL_PUBLIC_C void SetDoorsLockState(Entity entity, const int state);
		DLL_PUBLIC_C void SetDoorsLockStateForPlayer(Entity entity, const int state, Entity player);

		DLL_PUBLIC_C const uint32_t GetVehicleModel(Entity entity);

		//DLL_PUBLIC static void SetLivery(const int entity, const int livery);

		DLL_PUBLIC_C const int GetNumberPlateStyle(Entity entity);
		DLL_PUBLIC_C void SetNumberPlateStyle(Entity entity, const int style);

		DLL_PUBLIC_C const bool GetExtra(Entity entity, const int extra);
		DLL_PUBLIC_C void SetExtra(Entity entity, const int extra, const bool toggle);

		DLL_PUBLIC_C const float GetEngineHealth(Entity entity);
		DLL_PUBLIC_C void SetEngineHealth(Entity entity, const float health);

		DLL_PUBLIC_C const float GetFuelTankHealth(Entity entity);
		DLL_PUBLIC_C void SetFuelTankHealth(Entity entity, const float health);

		DLL_PUBLIC_C void FixDeformation(Entity entity);

		DLL_PUBLIC_C Entity GetOccupant(Entity entity, const int seat);
		DLL_PUBLIC_C const Entity * GetOccupants(Entity entity);

		DLL_PUBLIC_C const bool GetTaxiLightState(Entity entity);
		DLL_PUBLIC_C void SetTaxiLightState(Entity entity, const bool state);

		DLL_PUBLIC_C const bool GetIndicatorState(Entity entity, const int turnlight);
		DLL_PUBLIC_C void SetIndicatorState(Entity entity, const int turnlight, const bool state);

		DLL_PUBLIC_C const bool GetInteriorLightState(Entity entity);
		DLL_PUBLIC_C void SetInteriorLightState(Entity entity, const bool state);

		DLL_PUBLIC_C const bool GetSirenSoundState(Entity entity);
		DLL_PUBLIC_C void SetSirenSoundState(Entity entity, const bool state);

		DLL_PUBLIC_C Entity GetTowedVehicle(Entity entity);
		DLL_PUBLIC_C void SetTowedVehicle(Entity towVehicle, Entity vehicle);
		DLL_PUBLIC_C const bool IsTowed(Entity entity);

	}
}
#endif