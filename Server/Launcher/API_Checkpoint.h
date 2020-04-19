#ifndef __APICHECKPOINT_H__
#define __APICHECKPOINT_H__
#pragma once

#include "Common.h"
#include "API_Objects.h"
#include "APIStructs.h"
#include "CVector3.h"

namespace API
{
	namespace Checkpoint
	{
		DLL_PUBLIC_C Objects::Entity CreateCheckpoint(const CVector3 position, const float radius, const Color color, const int reserved);
		DLL_PUBLIC_C void ShowCheckpoint(Objects::Entity checkpointentity, Objects::Entity playerentity);
		DLL_PUBLIC_C void HideCheckpoint(Objects::Entity checkpointentity, Objects::Entity playerentity);
		DLL_PUBLIC_C const float GetNearHeight(Objects::Entity checkpointentity);
		DLL_PUBLIC_C void SetNearHeight(Objects::Entity checkpointentity, const float height);
		DLL_PUBLIC_C const float GetFarHeight(Objects::Entity checkpointentity);
		DLL_PUBLIC_C void SetFarHeight(Objects::Entity checkpointentity, const float height);
		DLL_PUBLIC_C const CVector3 GetPointTo(Objects::Entity checkpointentity);
		DLL_PUBLIC_C void PointTo(Objects::Entity checkpointentity, const CVector3 pointToPos);
		DLL_PUBLIC_C const int GetCheckpointType(Objects::Entity checkpointentity);
		DLL_PUBLIC_C void SetCheckpointType(Objects::Entity checkpointentity, const int type);
		DLL_PUBLIC_C const Color GetCheckpointColor(Objects::Entity checkpointentity);
		DLL_PUBLIC_C void SetCheckpointColor(Objects::Entity checkpointentity, const Color color);
	}
}
#endif