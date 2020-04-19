// Self
#include "CPlayer.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

// GrandM
#include "SharedUtility.h"

#include "Server.h"
#include "CVehicle.h"

// API
#include "CAPI.h"
#include "API_Callback_Entity.h"

CPlayer::CPlayer(std::string name, std::string secretKey, RakNet::RakNetGUID guid, RakNet::SystemAddress systemAddress)
{
	// Assign current count as entity id
	identity.entityId = CEntity::count;
	// Increment the count for the next entity id
	CEntity::count++;

	identity.type = GrandM::EntityType::Player;

	data.userName = name;
	data.secretKey = secretKey;

	network.guid = guid;
	network.systemAddress = systemAddress;

	network.lastSync = std::chrono::system_clock::now();
}

CPlayer::~CPlayer()
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)ID_PLAYER_LEFT);
	bitstream.Write(identity.entityId);
	Server::GetServer()->GetNetworkManager()->GetInterface()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

CPlayer::CPlayer(const CPlayer & e)
{
	//CEntity
	identity = e.identity;
	world = e.world;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	sync = e.sync;

	//CPlayer
	data = e.data;
	blip = e.blip;
	network = e.network;
}

CPlayer & CPlayer::operator=(const CPlayer &e)
{
	//CEntity
	identity = e.identity;
	world = e.world;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	sync = e.sync;

	//CPlayer
	data = e.data;
	blip = e.blip;
	network = e.network;

	return (*this);
}

void CPlayer::Pulse()
{
	if (std::chrono::duration_cast<std::chrono::milliseconds>(network.lastSync.time_since_epoch()).count() + (1000 / Server::GetServer()->GetSyncRate())
		<= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() && identity.entityId != -1)
	{
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)ID_PACKET_PLAYER);

		bitstream.Write(identity.entityId);
		bitstream.Write(RakNet::RakString(data.userName.c_str()));

		bitstream.Write(data.Model.hash);

		bitstream.Write(data.Vehicle.id);
		bitstream.Write(data.Vehicle.seat);

		bitstream.Write(data.aiTask);

		bitstream.Write(data.WeaponData.heldWeapon);
		bitstream.Write(data.WeaponData.reloading);

		bitstream.Write(data.WeaponData.melee.inMeleeCombat);
		bitstream.Write(data.WeaponData.melee.targetId);
		bitstream.Write(data.WeaponData.melee.targetType);

		bitstream.Write(data.WeaponData.aimPosition.x);
		bitstream.Write(data.WeaponData.aimPosition.y);
		bitstream.Write(data.WeaponData.aimPosition.z);

		if (!g_Players.empty())
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetDimension() == world.dim)
				{
					if ((g_Players[i].IsScriptedCameraActive() && CVector3::Distance2D(g_Players[i].GetScriptCameraPosition(), transform.Position) <= 1000.0f)
						|| (CVector3::Distance2D(g_Players[i].GetPosition(), transform.Position) <= 1000.0f))
					{
						// We shouldn't send to the player that is this entity.
						Server::GetServer()->GetNetworkManager()->GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, g_Players[i].GetGUID(), false);
					}
				}
			}
		}

		network.lastSync = std::chrono::system_clock::now();
	}

	// Dead Check
	//if (health.Get() <= 0 && !dead)
	//{
	//	dead = true;

	//	for (unsigned int i = 0; i < g_ApiModules.size(); i++)
	//	{
	//		void *Instance = g_ApiModules[i].GetInstance();
	//		API::Callback::Entity::OnEntityDeath(Instance, *this, GetLastAttacker());
	//	}
	//}
}

void CPlayer::Update(RakNet::Packet *packet)
{
	if (network.guid != packet->guid || network.systemAddress != packet->systemAddress)
	{
		packet->deleteData;
		return;
	}
	
	int lastVehicle = data.Vehicle.id;
	int lastSeat = data.Vehicle.seat;

	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);

	bitstream.Read(data.Vehicle.id);
	bitstream.Read(data.Vehicle.seat);

	bitstream.Read(data.aiTask);

	bitstream.Read(data.WeaponData.heldWeapon);
	bitstream.Read(data.WeaponData.reloading);

	bitstream.Read(data.WeaponData.melee.inMeleeCombat);
	bitstream.Read(data.WeaponData.melee.targetId);
	bitstream.Read(data.WeaponData.melee.targetType);

	bitstream.Read(data.WeaponData.aimPosition.x);
	bitstream.Read(data.WeaponData.aimPosition.y);
	bitstream.Read(data.WeaponData.aimPosition.z);

	bitstream.Read(data.ScriptedCamera.active);

	if (data.ScriptedCamera.active)
	{
		bitstream.Read(data.ScriptedCamera.position.x);
		bitstream.Read(data.ScriptedCamera.position.y);
		bitstream.Read(data.ScriptedCamera.position.z);
	}

	packet->deleteData;
	
	if (lastVehicle != data.Vehicle.id)
	{
		bool doneOne, doneTwo = false;
		for (int i = 0; i < g_Vehicles.size(); i++)
		{
			if (data.Vehicle.id != -1)
			{
				if (g_Vehicles[i].GetID() == data.Vehicle.id && g_Vehicles[i].GetOccupant(data.Vehicle.seat).GetID() != identity.entityId)
				{
					g_Vehicles[i].SetOccupant(data.Vehicle.seat, identity.entityId);

					//OnPlayerEnterVehicle(player,vehicle,seat);
					doneOne = true;
				}
			}

			if (lastSeat != -1 && lastVehicle != -1)
			{
				if (g_Vehicles[i].GetID() == lastVehicle)
				{
					g_Vehicles[i].SetOccupant(lastSeat, -1);
					doneTwo = true;
				}
			}

			if (doneOne && doneTwo)
				break;
		}
	}
	else if (lastSeat != data.Vehicle.seat)
	{
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == data.Vehicle.id && g_Vehicles[i].GetOccupant(lastSeat).GetID() == identity.entityId)
			{
				g_Vehicles[i].SetOccupant(lastSeat, -1);

				//OnPlayerEnterVehicle(player,vehicle,seat);

				break;
			}
		}
	}
}

void CPlayer::RequestData(RakNet::RakNetGUID requester)
{
	RakNet::BitStream bitstream;

#pragma region Blip
	if (blip.HasBlip())
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipE", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

		blip.SyncData(requester);
	}
#pragma endregion

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(RakNet::RakString(tag.text.c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametag", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.textVisible);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.healthVisible);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetHealthbarVisible", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.distance);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagDistance", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(tag.textColor.Red);
	bitstream.Write(tag.textColor.Green);
	bitstream.Write(tag.textColor.Blue);
	bitstream.Write(tag.textColor.Alpha);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetNametagColor", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(data.Model.headBlend.shapeFirst);
	bitstream.Write(data.Model.headBlend.shapeMix);
	bitstream.Write(data.Model.headBlend.shapeSecond);
	bitstream.Write(data.Model.headBlend.shapeThird);
	bitstream.Write(data.Model.headBlend.skinFirst);
	bitstream.Write(data.Model.headBlend.skinMix);
	bitstream.Write(data.Model.headBlend.skinSecond);
	bitstream.Write(data.Model.headBlend.skinThird);
	bitstream.Write(data.Model.headBlend.thirdMix);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadBlend", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);

	for (int i = 0; i < SizeOfArray(data.Model.components); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.components[i].drawableid);
		bitstream.Write(data.Model.components[i].paletteid);
		bitstream.Write(data.Model.components[i].textureid);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedComponent", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	for (int i = 0; i < SizeOfArray(data.Model.faceFeatures); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.faceFeatures[i].scale);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedFaceFeature", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	for (int i = 0; i < SizeOfArray(data.Model.headOverlays); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.headOverlays[i].index);
		bitstream.Write(data.Model.headOverlays[i].colorType);
		bitstream.Write(data.Model.headOverlays[i].colorID);
		bitstream.Write(data.Model.headOverlays[i].secondColorID);
		bitstream.Write(data.Model.headOverlays[i].opacity);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedHeadOverlay", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	for (int i = 0; i < SizeOfArray(data.Model.props); i++)
	{
		bitstream.Reset();
		bitstream.Write(identity.entityId);
		bitstream.Write(identity.type);
		bitstream.Write(i);
		bitstream.Write(data.Model.props[i].drawableid);
		bitstream.Write(data.Model.props[i].textureid);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("PedProp", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
	}

	bitstream.Reset();
	bitstream.Write(identity.entityId);
	bitstream.Write(identity.type);
	bitstream.Write(health);
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetHealth", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, requester, false, false);
}

CEntity CPlayer::GetVehicle()
{
	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == data.Vehicle.id)
		{
			return (CEntity)g_Vehicles[i];
		}
	}
	return CEntity();
}

void CPlayer::GiveWeapon(const std::string weapon, const int ammo)
{
	if (!data.WeaponData.weapons.empty())
	{
		for (int i = 0; i < data.WeaponData.weapons.size(); i++)
		{
			if (weapon.compare(data.WeaponData.weapons[i].name.c_str()) == 0)
			{
				data.WeaponData.weapons[i].ammo += ammo;
				return;
			}
		}
	}

	GrandM::Weapon::Weapon newWeapon;

	newWeapon.name = weapon;
	newWeapon.ammo = ammo;
	data.WeaponData.weapons.push_back(newWeapon);
}

void CPlayer::RemoveWeapon(const std::string weapon)
{
	if (!data.WeaponData.weapons.empty())
	{
		for (int i = 0; i < data.WeaponData.weapons.size(); i++)
		{
			if (weapon.compare(data.WeaponData.weapons[i].name.c_str()) == 0)
			{
				data.WeaponData.weapons.erase(data.WeaponData.weapons.begin() + i);
				data.WeaponData.weapons.shrink_to_fit();
				return;
			}
		}
	}
}

const bool CPlayer::UpdateWeapon(const std::string weapon, const int ammo, const int clipAmmo)
{
	if (weapon.compare("weapon_unarmed") == 0)
		return true;

	if (!data.WeaponData.weapons.empty())
	{
		for (unsigned int i = 0; i < data.WeaponData.weapons.size(); i++)
		{
			if (weapon.compare(data.WeaponData.weapons[i].name.c_str()) == 0)
			{
				data.WeaponData.weapons[i].ammo = ammo;
				data.WeaponData.weapons[i].clipAmmo = clipAmmo;

				return true;
			}
		}
	}

	return false;
}

const int CPlayer::GetWeaponAmmo(const std::string weapon)
{
	if (!data.WeaponData.weapons.empty())
	{
		for (unsigned int i = 0; i < data.WeaponData.weapons.size(); i++)
		{
			if (weapon.compare(data.WeaponData.weapons[i].name.c_str()) == 0)
			{
				return data.WeaponData.weapons[i].ammo;
			}
		}
	}

	return -1;
}

const int CPlayer::GetWeaponClipAmmo(const std::string weapon)
{
	if (!data.WeaponData.weapons.empty())
	{
		for (unsigned int i = 0; i < data.WeaponData.weapons.size(); i++)
		{
			if (weapon.compare(data.WeaponData.weapons[i].name.c_str()) == 0)
			{
				return data.WeaponData.weapons[i].ammo;
			}
		}
	}

	return -1;
}

void CPlayer::Revive()
{
	dead = false;
	health.Set(200);
}
