#ifndef __APICAMERA_H__
#define __APICAMERA_H__
#pragma once

#include "API_Objects.h"
#include "CVector3.h"

namespace API
{
	namespace Camera
	{
		DLL_PUBLIC_C void SetCameraPosition(Objects::Entity player, const CVector3 Position);
		DLL_PUBLIC_C void SetCameraRotation(Objects::Entity player, const CVector3 Rotation);
		DLL_PUBLIC_C void ResetCamera(Objects::Entity player);

		DLL_PUBLIC_C void FirstPersonOnly(Objects::Entity player);
		DLL_PUBLIC_C void ThirdPersonOnly(Objects::Entity player);

		DLL_PUBLIC_C void AttachCamera(Objects::Entity player, Objects::Entity entity, const CVector3 offset);
		DLL_PUBLIC_C void DetachCamera(Objects::Entity player);

		DLL_PUBLIC_C const bool IsCameraActive(Objects::Entity player);
	}
}
#endif