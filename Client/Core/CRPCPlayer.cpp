#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "Common.h"
#include "CVector3.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CLocalPlayer.h"

#include "CefMainContext.h"
#include "CStreamer.h"
#include "GamePed.h"
#include "CGui.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCPlayer.h"

void CRPCPlayer::PlayerModel(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::PlayerModel.");

	uint32_t model;

	bitStream->Read(model);

	GTAV::GamePed::SetPedModel(CLocalPlayer::GetPed(), model);
}

void CRPCPlayer::SetControllable(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::SetControllable.");

	bool disablecontrols, frozen;

	bitStream->Read(disablecontrols);
	bitStream->Read(frozen);

	CLocalPlayer::SetControllable(!disablecontrols);
	CLocalPlayer::SetFrozen(frozen);

	ENTITY::FREEZE_ENTITY_POSITION(CLocalPlayer::GetPed(), frozen);
}

void CRPCPlayer::Kick(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::Kick.");

	RakNet::RakString reason;
	bitStream->Read(reason);

	CGui::ShowConnectionError("You were Kicked", reason.C_String());

	CGui::GiveFocus(true);
	CLocalPlayer::SetControllable(false);
}

void CRPCPlayer::WrongVersion(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::WrongVersion.");

	RakNet::RakString serverVersion;
	bitStream->Read(serverVersion);

	std::ostringstream reason;
	reason << "<p>Your version does not match the servers:</p><p>Your version: " << VERSION << "</p><p>Server version: " << serverVersion.C_String() << "</p>";

	CGui::ShowConnectionError("Client Version Mismatch", reason.str());

	CGui::GiveFocus(true);
	CLocalPlayer::SetControllable(false);
}

void CRPCPlayer::PutInVehicle(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::PutInVehicle.");

	int entity = -1, seat = -1;

	bitStream->Read(entity);
	bitStream->Read(seat);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			while (!g_Vehicles[i].IsCreated())
			{
				CVector3 pos;
				if (CLocalPlayer::IsScriptedCameraActive())
				{
					Vector3 camPos = CAM::GET_CAM_COORD(CLocalPlayer::GetScriptedCamera());
					pos = { camPos.x, camPos.y, camPos.z };
				}

				CStreamer::StreamVehiclesIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
				WAIT(10);
			}

			GTAV::GamePed::PutPedInVehicle(CLocalPlayer::GetPed(), g_Vehicles[i].GetEntity(), seat - 1);
			
			return;
		}
	}
}

void CRPCPlayer::GiveWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::GiveWeapon.");

	RakNet::RakString weapon;
	int ammo;

	bitStream->Read(weapon);
	bitStream->Read(ammo);

	CLocalPlayer::GiveWeapon(weapon.C_String(), ammo);
}

void CRPCPlayer::RemoveWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::RemoveWeapon.");

	RakNet::RakString weapon;

	bitStream->Read(weapon);

	CLocalPlayer::RemoveWeapon(weapon.C_String());
}

void CRPCPlayer::OnPlayerShot(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::OnPlayerShot.");

	RakNet::RakString weapon;
	int ammo, clipAmmo, entity;

	CVector3 Position, Aim;

	bitStream->Read(entity);
	bitStream->Read(weapon);
	bitStream->Read(ammo);
	bitStream->Read(clipAmmo);

	bitStream->Read(Aim.x);
	bitStream->Read(Aim.y);
	bitStream->Read(Aim.z);

	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetID() == entity)
		{
			return g_Players[i].TaskShoot(weapon.C_String(), ammo, Aim);
		}
	}
}

void CRPCPlayer::OnPlayerAim(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::OnPlayerAim.");

	int entity;
	bool aiming;

	bitStream->Read(entity);
	bitStream->Read(aiming);

	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetID() == entity) 
		{
			g_Players[i].SetAiming(aiming);
			return;
		}
	}
}

void CRPCPlayer::SetWeaponAmmo(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::SetWeaponAmmo.");

	RakNet::RakString weapon;
	int ammo, clipAmmo;

	bitStream->Read(weapon);
	bitStream->Read(ammo);
	bitStream->Read(clipAmmo);

	CLocalPlayer::SetWeaponAmmo(weapon.C_String(), ammo, clipAmmo);
}

void CRPCPlayer::EquipWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCPlayer::EquipWeapon.");

	RakNet::RakString weapon;

	bitStream->Read(weapon);

	CLocalPlayer::EquipWeapon(weapon.C_String());
}