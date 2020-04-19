// Self
#include "GamePed.h"

// RakNet
#include <BitStream.h>
#include <PacketPriority.h>
#include <RakNetTypes.h>
#include <MessageIdentifiers.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "Functions.h"
#include "APIStructs.h"
#include "CVehicle.h"
#include "CLocalPlayer.h"
#include "CPlayer.h"
#include "CNetworkManager.h"

namespace GTAV
{
	namespace GamePed
	{
		//Forward Declar
		int GetVehicleSeat(Ped ped);

		Vehicle GetVehicle(Ped ped)
		{
			return PED::GET_VEHICLE_PED_IS_IN(ped, false);
		}

		Hash GetPedSelectedWeapon(Ped ped)
		{
			return WEAPON::GET_SELECTED_PED_WEAPON(ped);
		}

		int GetPedHealth(Ped ped)
		{
			return ENTITY::GET_ENTITY_HEALTH(ped);
		}

		void SetPedHealth(Ped ped, int health)
		{
			ENTITY::SET_ENTITY_HEALTH(ped, health);
		}

		int GetPedArmour(Ped ped)
		{
			return PED::GET_PED_ARMOUR(ped);
		}

		void SetPedArmour(Ped ped, int armour)
		{
			PED::SET_PED_ARMOUR(ped, armour);
		}

		void PutPedInVehicle(Ped ped, Vehicle vehicle, int seat)
		{
			PED::SET_PED_INTO_VEHICLE(ped, vehicle, seat);
		}

		void SetPedModel(Ped ped, const uint32_t model)
		{
			Hash weapon = GetPedSelectedWeapon(ped);

			int health = GetPedHealth(ped);
			int armour = GetPedArmour(ped);

			Vehicle vehicle = GetVehicle(ped);
			int seat = GetVehicleSeat(ped);

			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
			{
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model))
					WAIT(0);

				PLAYER::SET_PLAYER_MODEL(CLocalPlayer::GetPlayer(), model);

				if (CLocalPlayer::GetPed() == ped && !ENTITY::DOES_ENTITY_EXIST(ped))
				{
					ped = CLocalPlayer::GetPed();
				}

				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ped);

				RakNet::BitStream bitstream;

				headBlendData headData;
				PED::_GET_PED_HEAD_BLEND_DATA(CLocalPlayer::GetPed(), &headData);

				bitstream.Reset();
				bitstream.Write(CLocalPlayer::GetID());
				bitstream.Write(headData.shapeFirst);
				bitstream.Write(headData.shapeMix);
				bitstream.Write(headData.shapeSecond);
				bitstream.Write(headData.shapeThird);
				bitstream.Write(headData.skinFirst);
				bitstream.Write(headData.skinMix);
				bitstream.Write(headData.skinSecond);
				bitstream.Write(headData.skinThird);
				bitstream.Write(headData.thirdMix);
				CNetworkManager::GetRPC().Signal("SetPedHeadBlend", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

				// Not ENTIERLY sure for _GET_PED_HEAD_OVERLAY_VALUE even works since headoverlays are not a single int...
				/*for (int i = 0; i < 13; i++)
				{
					bitstream.Reset();
					bitstream.Write(CLocalPlayer::GetId());
					bitstream.Write(i);
					bitstream.Write(PED::_GET_PED_HEAD_OVERLAY_VALUE(CLocalPlayer::GetPed(), i));
					CNetworkManager::GetRPC().Signal("SetPedHeadoverlay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
				}*/

				for (int i = 0; i < 3; i++)
				{
					bitstream.Reset();
					bitstream.Write(CLocalPlayer::GetID());
					bitstream.Write(i);
					bitstream.Write(PED::GET_PED_PROP_INDEX(CLocalPlayer::GetPed(), i));
					bitstream.Write(PED::GET_PED_TEXTURE_VARIATION(CLocalPlayer::GetPed(), i));
					CNetworkManager::GetRPC().Signal("SetPedProp", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
				}

				if (vehicle > -1)
					PutPedInVehicle(ped, vehicle, seat);

				SetPedHealth(ped, health);
				SetPedArmour(ped, armour);

				//SetPedHealthRegenRate(regenRate);
				//UpdateWeapons(weapon);
			}
		}

		void SetPedComponentVariation(Ped ped, const int componentid, const int drawableid, const int textureid, const int paletteid)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
				PED::SET_PED_COMPONENT_VARIATION(ped, componentid, drawableid, textureid, paletteid);
		}

		void SetPedHeadBlend(Ped ped, const int shapeFirst, const int shapeSecond, const int shapeThird, const int skinFirst, const int skinSecond, const int skinThird, const float shapeMix, const float skinMix, const float thirdMix)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
				PED::SET_PED_HEAD_BLEND_DATA(ped, shapeFirst, shapeSecond, shapeThird, skinFirst, skinSecond, skinThird, shapeMix, skinMix, thirdMix, 0);
		}

		void SetPedHeadOverlayColor(Ped ped, const int overlayid, const int index, const int colorType, const int colorid, const int secondColorid, const float opacity)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			{
				PED::SET_PED_HEAD_OVERLAY(ped, overlayid, index, opacity);
				PED::_SET_PED_HEAD_OVERLAY_COLOR(ped, overlayid, colorType, colorid, secondColorid);
			}
		}

		void SetPedProp(Ped ped, const int componentid, const int drawableid, const int textureid)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
				PED::SET_PED_PROP_INDEX(ped, componentid, drawableid, textureid, true);
		}

		void SetPedFaceFeature(Ped ped, const int index, const float scale)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
				PED::_SET_PED_FACE_FEATURE(ped, index, scale);
		}

		int GetPedTask(Ped ped)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			{
				for (int i = 500; i > -1; i--)
				{
					if (BRAIN::GET_IS_TASK_ACTIVE(ped, i) && i != 38 && i != 35)
					{
						return i;
					}
				}
			}

			return -1;
		}

		const int GetVehicleIndex(Ped ped)
		{
			Vehicle t_CurrentVehicle = GetVehicle(ped);

			if (t_CurrentVehicle > 0)
			{
				for (int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetEntity() == t_CurrentVehicle)
					{
						return i;
					}
				}
			}
			return -1;
		}

		CEntity GetVehicleObject(Ped ped)
		{
			Vehicle t_CurrentVehicle = GetVehicle(ped);

			if (t_CurrentVehicle > 0)
			{
				for (int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetEntity() == t_CurrentVehicle)
					{
						return g_Vehicles[i];
					}
				}
			}

			return CEntity();
		}

		int GetVehicleSeat(Ped ped)
		{
			Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);

			if (GetVehicle(ped))
			{
				for (int i = -1; i < 10; i++)
				{
					if (ped == VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, i, 0))
					{
						return i + 1;  // makes it so 0 is driver and 1+ passengers
					}
				}
			}
			return -1;

			// Non-Native (need to convert this)
			/*if (!getGameVersion() == 32 || getGameVersion() == 33) // 1.36 / 877
			{
			if (IsInVehicle(ped))
			{
			if (*(int *)(getScriptHandleBaseAddress(ped) + 0x158A) > 1000)
			*(int *)(getScriptHandleBaseAddress(ped) + 0x158A) = *(int *)(getScriptHandleBaseAddress(ped) + 0x158A) - 262144;

			return *(int *)(getScriptHandleBaseAddress(ped) + 0x158A);
			}
			else
			return -1;
			}
			else // Latest
			{
			if (IsInVehicle(ped))
			{
			if (*(int *)(getScriptHandleBaseAddress(ped) + 0x159A) > 1000)
			*(int *)(getScriptHandleBaseAddress(ped) + 0x159A) = *(int *)(getScriptHandleBaseAddress(ped) + 0x159A) - 262144;

			return *(int *)(getScriptHandleBaseAddress(ped) + 0x159A);
			}
			else
			return -1;
			}*/
		}

		/*bool IsInVehicle(Ped ped)
		{
		if (Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false) > 0)
		{
		return true;
		}
		return false;
		}*/

		CEntity GetPlayerMeleeTarget(Player player) 
		{
			Entity target;
			if (PLAYER::GET_PLAYER_TARGET_ENTITY(player, &target))
			{
				for (int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Players[i].GetEntity() == target)
					{
						return g_Players[i];
					}
				}
			}

			return CEntity();
		}

		CEntity GetPedMeleeTarget(Ped ped)
		{
			Entity target = PED::GET_MELEE_TARGET_FOR_PED(ped);
			if (target > 0)
			{
				for (int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Players[i].GetEntity() == target)
					{
						return g_Players[i];
					}
				}
			}

			return CEntity();
		}
	}
}