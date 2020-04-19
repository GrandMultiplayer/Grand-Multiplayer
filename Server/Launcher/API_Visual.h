#ifndef __APIVISUAL_H__
#define __APIVISUAL_H__
#pragma once

#include "Common.h"
#include "API_Objects.h"

namespace API
{
	namespace Visual
	{
		DLL_PUBLIC_C void ShowNotification(const char * message, const char * pic, const int icontype, const int color, const char * sender, const char * subject);
		DLL_PUBLIC_C void ShowNotificationToPlayer(Objects::Entity entity, const char * message, const char * pic, const int icontype, const int color, const char * sender, const char * subject);

		DLL_PUBLIC_C void ShowSubtitle(const char * message, const int duration, const bool shownow);
		DLL_PUBLIC_C void ShowSubtitleToPlayer(Objects::Entity entity, const char * message, const int duration, const bool shownow);

		DLL_PUBLIC_C void SendChatMessage(const wchar_t * message);
		DLL_PUBLIC_C void SendChatMessageToPlayer(Objects::Entity entity, const wchar_t * message);

		DLL_PUBLIC_C void ShowCursor(Objects::Entity entity, bool show);

		DLL_PUBLIC_C const bool IsHudHidden(Objects::Entity entity);
		DLL_PUBLIC_C void DisplayHud(Objects::Entity entity, bool show);

		DLL_PUBLIC_C const bool IsRadarHidden(Objects::Entity entity);
		DLL_PUBLIC_C void DisplayRadar(Objects::Entity entity, bool show);

		DLL_PUBLIC_C const bool IsCashHidden(Objects::Entity entity);
		DLL_PUBLIC_C void DisplayCash(Objects::Entity entity, bool show);

		DLL_PUBLIC_C const bool IsAmmoHidden(Objects::Entity entity);
		DLL_PUBLIC_C void DisplayAmmo(Objects::Entity entity, bool show);
	}
}
#endif