#ifndef __APIPICKUP_H__
#define __APIPICKUP_H__
#pragma once
#include <stdint.h>

#include "Common.h"
#include "CVector3.h"
#include "API_Objects.h"

namespace API
{
	namespace Pickup
	{
		DLL_PUBLIC_C Objects::Entity CreatePickup(const CVector3 position, const uint32_t model = 0xE5121369);

		DLL_PUBLIC_C void ShowPickup(Objects::Entity entity, Objects::Entity playerEntity);

		DLL_PUBLIC_C void HidePickup(Objects::Entity entity, Objects::Entity playerEntity);

		DLL_PUBLIC_C const uint32_t GetPickupModel(Objects::Entity entity);
		DLL_PUBLIC_C void SetPickupModel(Objects::Entity entity, const uint32_t model);

		DLL_PUBLIC_C const int GetPickupRespawnDelay(Objects::Entity entity);
		DLL_PUBLIC_C void SetPickupRespawnDelay(Objects::Entity entity, const int time);
	}
}
#endif