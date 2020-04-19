#include "CVector3.h"
#include "CMaths.h"

#include "hook/enums.h"
#include "hook/natives.h"
#include "Memory.h"
#include "MemoryAccess.h"

// Self
#include "World.h"

uint8_t original[20] = { 0 };
size_t num = sizeof(original);

namespace GTAV
{
	namespace World
	{
		bool WorldToScreenRel(CVector3 worldCoords, Vector2 screenCoords)
		{
			float screenX, screenY;
			if (!GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(worldCoords.x, worldCoords.y, worldCoords.z, &screenX, &screenY))
			{
				screenCoords = Vector2();
				return false;
			}
			screenCoords = Vector2((screenX - 0.5f) * 2, (screenY - 0.5f) * 2);
			return true;
		}

		Vector3 ScreenRelToWorld(Vector3 camPos, Vector3 camRot, Vector2 coord)
		{
			Vector3 camForward = RotationToDirection(camRot);

			Vector3 rotUp = camRot;
			rotUp.x += 10;

			Vector3 rotDown = camRot;
			rotDown.x += -10;

			Vector3 rotLeft = camRot;
			rotLeft.z += -10;

			Vector3 rotRight = camRot;
			rotRight.z += 10;

			Vector3 camRight = RotationToDirection(rotRight);
			camRight.x = RotationToDirection(rotLeft).x;
			camRight.y = RotationToDirection(rotLeft).y;
			camRight.z = RotationToDirection(rotLeft).z;

			Vector3 camUp = RotationToDirection(rotUp);
			camUp.x = RotationToDirection(rotDown).x;
			camUp.y = RotationToDirection(rotDown).y;
			camUp.z = RotationToDirection(rotDown).z;

			double rollRad = -Math::ConvertDegreesToRadians(camRot.y);

			Vector3 camRightRoll;
			camRightRoll.x = camRight.x * (float)cos(rollRad) - camUp.x * (float)sin(rollRad);
			camRightRoll.y = camRight.y * (float)cos(rollRad) - camUp.y * (float)sin(rollRad);
			camRightRoll.z = camRight.z * (float)cos(rollRad) - camUp.z * (float)sin(rollRad);

			Vector3 camUpRoll;
			camUpRoll.x = camRight.x * (float)sin(rollRad) + camUp.x * (float)cos(rollRad);
			camUpRoll.y = camRight.y * (float)sin(rollRad) + camUp.y * (float)cos(rollRad);
			camUpRoll.z = camRight.z * (float)sin(rollRad) + camUp.z * (float)cos(rollRad);

			CVector3 point3D;
			point3D.x = camPos.x + camForward.x * 10.0f + camRightRoll.x + camUpRoll.x;
			point3D.y = camPos.y + camForward.y * 10.0f + camRightRoll.y + camUpRoll.y;
			point3D.z = camPos.z + camForward.z * 10.0f + camRightRoll.z + camUpRoll.z;

			Vector2 point2D;
			if (!WorldToScreenRel(point3D, point2D))
			{
				Vector3 rel;
				rel.x = camPos.x + camForward.x * 10.0f;
				rel.y = camPos.y + camForward.y * 10.0f;
				rel.z = camPos.z + camForward.z * 10.0f;
				return rel;
			}

			CVector3 point3DZero;
			point3DZero.x = camPos.x + camForward.x * 10.0f;
			point3DZero.y = camPos.y + camForward.y * 10.0f;
			point3DZero.z = camPos.z + camForward.z * 10.0f;

			Vector2 point2DZero;
			if (!WorldToScreenRel(point3DZero, point2DZero))
			{
				Vector3 rel;
				rel.x = camPos.x + camForward.x * 10.0f;
				rel.y = camPos.y + camForward.y * 10.0f;
				rel.z = camPos.z + camForward.z * 10.0f;
				return rel;
			}

			const double eps = 0.001;
			if (abs(point2D.x - point2DZero.x) < eps || abs(point2D.y - point2DZero.y) < eps)
			{
				Vector3 rel;
				rel.x = camPos.x + camForward.x * 10.0f;
				rel.y = camPos.y + camForward.y * 10.0f;
				rel.z = camPos.z + camForward.z * 10.0f;
				return rel;
			}
			float scaleX = (coord.x - point2DZero.x) / (point2D.x - point2DZero.x);
			float scaleY = (coord.y - point2DZero.y) / (point2D.y - point2DZero.y);

			Vector3 point3Dret;
			point3Dret.x = camPos.x + camForward.x * 10.0f + camRightRoll.x * scaleX + camUpRoll.x * scaleY;
			point3Dret.y = camPos.y + camForward.y * 10.0f + camRightRoll.y * scaleX + camUpRoll.y * scaleY;
			point3Dret.z = camPos.z + camForward.z * 10.0f + camRightRoll.z * scaleX + camUpRoll.z * scaleY;

			return point3Dret;
		}

		Vector3 RotationToDirection(Vector3 rotation)
		{
			float z = Math::ConvertDegreesToRadians(rotation.z);
			float x = Math::ConvertDegreesToRadians(rotation.x);
			float num = abs(cos(x));

			Vector3 Direction;
			Direction.x = (float)(-sin(z) * num);
			Direction.y = (float)(cos(z) * num);
			Direction.z = (float)sin(x);

			return Direction;
		}

		Vector3 DirectionToRotation(Vector3 direction)
		{
			float x = atan2(direction.z, direction.y);
			float y = 0;
			float z = -atan2(direction.x, direction.y);

			Vector3 rot;
			rot.x = Math::ConvertRadiansToDegrees(x);
			rot.y = Math::ConvertRadiansToDegrees(y);
			rot.z = Math::ConvertRadiansToDegrees(z);

			return rot;
		}

		void RevealFullMap()
		{
			// GTA:Online Full Map
			auto checkMultiplayerDrawMapFrame = Memory::pattern("40 8A 35 ? ? ? ? 84 C0 74 05 45 84 FF");
			Memory::nop(checkMultiplayerDrawMapFrame.count(1).get(0).get<void>(0), 7);
		}

		void LiftPlayerWorldLimits()
		{
			static auto explosions = MemoryAccess::FindPattern("\x48\x8B\x03\x0F\x28\xBB\x00\x00\x00\x00\xF3\x0F\x10\x35\x00\x00\x00\x00\x48\x8B\xCB", "xxxxxx????xxxx????xxx");

			if (explosions)
			{
				DWORD old;
				size_t size = 0x9A;

				VirtualProtect((void *)explosions, size, PAGE_EXECUTE_READWRITE, &old);
				memset((void *)explosions, 0x90, size);
				VirtualProtect((void *)explosions, size, old, &old);
			}



			static auto mapboundries = MemoryAccess::FindPattern("\x45\x33\xC0\x44\x38\x05\x00\x00\x00\x00\x74\x4A\x48\x8B\x81\x00\x00\x00\x00\x41\x8B\xD0", "xxxxxx????xxxxx????xxx");

			if (mapboundries)
			{
				float *maxWorldLimits = (float *)(mapboundries + *(int *)(mapboundries + 0x26) + 0x2B);
				float *minWorldLimits = (float *)(mapboundries + *(int *)(mapboundries + 0x31) + 0x35);

				maxWorldLimits[0] = FLT_MAX;
				maxWorldLimits[1] = FLT_MAX;
				maxWorldLimits[2] = FLT_MAX;

				minWorldLimits[0] = -FLT_MAX;
				minWorldLimits[1] = -FLT_MAX;
				minWorldLimits[2] = -FLT_MAX;
			}
		}

		void SetGroundSnow(const bool state)
		{
			static auto addrSnow = MemoryAccess::FindPattern("\x74\x25\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00", "xxx????x????");

			static bool bInitialized = false;

			if (!bInitialized)
			{
				// Unprotect Memory
				VirtualProtect((void*)addrSnow, num, PAGE_EXECUTE_READWRITE, nullptr);

				// Copy original Memory
				memcpy(&original, (void*)addrSnow, num);

				bInitialized = true;
			}

			if (state)
			{
				// NOP
				memset((void*)addrSnow, 0x90, num);
			}
			else
			{
				// Reset original memory
				memcpy((void*)addrSnow, &original, num);
			}


			static auto VAR_FeetSnowTracks_call = MemoryAccess::FindPattern("\x80\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x37", "xx?????xxxxx");
			static auto VAR_FeetSnowTracks = VAR_FeetSnowTracks_call + (*(int32_t *)(VAR_FeetSnowTracks_call + 2)) + 7;

			static auto VAR_VehicleSnowTracks_call = MemoryAccess::FindPattern("\x40\x38\x3D\x00\x00\x00\x00\x48\x8B\x42\x20", "xxx????xxxx");
			static auto VAR_VehicleSnowTracks = VAR_VehicleSnowTracks_call + (*(int32_t *)(VAR_VehicleSnowTracks_call + 3)) + 7;

			VirtualProtect((void*)VAR_FeetSnowTracks, 1, PAGE_EXECUTE_READWRITE, nullptr);
			VirtualProtect((void*)VAR_VehicleSnowTracks, 1, PAGE_EXECUTE_READWRITE, nullptr);

			// Enable/Disable Vehicle/Foot Snow tracks
			*(uint8_t *)VAR_FeetSnowTracks = state;
			*(uint8_t *)VAR_VehicleSnowTracks = state;

			// Switch for big/small tracks
			static auto vehicleTrackTypes = MemoryAccess::FindPattern("\xB9\x00\x00\x00\x00\x84\xC0\x44\x0F\x44\xF1", "x????xxxxxx");

			VirtualProtect((void*)vehicleTrackTypes, 1, PAGE_EXECUTE_READWRITE, nullptr);
			*(uint8_t *)(vehicleTrackTypes + 1) = state ? 0x13 : 0x14;

			static auto pedTrackTypes = MemoryAccess::FindPattern("\xB9\x00\x00\x00\x00\x84\xC0\x0F\x44\xD9\x48\x8B\x4F\x30", "x????xxxxxxxxx");
			VirtualProtect((void*)pedTrackTypes, 1, PAGE_EXECUTE_READWRITE, nullptr);
			*(uint8_t *)(pedTrackTypes + 1) = state ? 0x13 : 0x14;
		}
	}
}