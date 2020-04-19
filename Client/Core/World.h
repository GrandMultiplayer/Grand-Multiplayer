#ifndef __GTAV_WORLD_H__
#define __GTAV_WORLD_H__
#include "CVector3.h"

namespace GTAV
{
	namespace World
	{
		bool WorldToScreenRel(CVector3 worldCoords, Vector2 screenCoords);
		Vector3 ScreenRelToWorld(Vector3 camPos, Vector3 camRot, Vector2 coord);
		Vector3 RotationToDirection(Vector3 rotation);
		Vector3 DirectionToRotation(Vector3 direction);

		void RevealFullMap();
		void LiftPlayerWorldLimits();

		void SetGroundSnow(const bool state);
	}
}

#endif