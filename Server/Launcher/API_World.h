#ifndef __APIWORLD_H__
#define __APIWORLD_H__
#pragma once

#include "Common.h"
#include "API_Objects.h"

namespace API
{
	namespace World
	{
		DLL_PUBLIC_C void GetTime(int *hour, int *minute, int *second);
		DLL_PUBLIC_C void SetTime(const int hour, const int minute, const int second);

		DLL_PUBLIC_C const char * GetWeather();
		DLL_PUBLIC_C void SetWeather(const char * weather);

		DLL_PUBLIC_C void LoadIPL(const char * ipl);
		DLL_PUBLIC_C void LoadIPLForPlayer(Objects::Entity entity, const char * ipl);

		DLL_PUBLIC_C void UnloadIPL(const char * ipl);
		DLL_PUBLIC_C void UnloadIPLForPlayer(Objects::Entity entity, const char * ipl);

		DLL_PUBLIC_C void SetBlackout(const bool state);
		DLL_PUBLIC_C void SetBlackoutForPlayer(Objects::Entity entity, const bool state);
		DLL_PUBLIC_C const bool IsBlackout();

		DLL_PUBLIC_C void SetGroundSnow(const bool state);
		DLL_PUBLIC_C void SetGroundSnowForPlayer(Objects::Entity entity, const bool state);
		DLL_PUBLIC_C const bool IsGroundSnow();
	}
}
#endif