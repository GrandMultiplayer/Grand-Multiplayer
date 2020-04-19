#ifndef __APIBLIP_H__
#define __APIBLIP_H__
#pragma once

#include "Common.h"
#include "API_Objects.h"
#include "CVector3.h"

namespace API
{
	namespace Blip
	{
		// Blip thats tied to an entity
		DLL_PUBLIC_C void CreateBlip(Objects::Entity entity);

		// Blip thats at a world position
		DLL_PUBLIC_C Objects::Entity CreateBlip_(const CVector3 position);

		DLL_PUBLIC_C void ShowBlip(Objects::Entity entity, Objects::Entity player);
		DLL_PUBLIC_C void HideBlip(Objects::Entity entity, Objects::Entity player);

		DLL_PUBLIC_C void ShowBlipToAll(Objects::Entity entity);
		DLL_PUBLIC_C void HideBlipFromAll(Objects::Entity entity);

		DLL_PUBLIC_C const int GetBlipColor(Objects::Entity entity);
		DLL_PUBLIC_C void SetBlipColor(Objects::Entity entity, const int color);

		DLL_PUBLIC_C const int GetSprite(Objects::Entity entity);
		DLL_PUBLIC_C void SetSprite(Objects::Entity entity, const int sprite);

		DLL_PUBLIC_C const char * GetBlipText(Objects::Entity entity);
		DLL_PUBLIC_C void SetBlipText(Objects::Entity entity, const char * text);

		DLL_PUBLIC_C const bool IsShortRanged(Objects::Entity entity);
		DLL_PUBLIC_C void SetShortRanged(Objects::Entity entity, const bool state);
	}
}
#endif