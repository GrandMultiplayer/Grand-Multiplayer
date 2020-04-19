
// GrandM
#include "Server.h"
#include "CPlayer.h"

// API
#include "CAPI.h"
#include "API_Callback_Player.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "RPCPlayer.h"

namespace RPC
{
	namespace Player
	{
		void InvalidWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			RakNet::RakString weapon;

			bitStream->Read(weapon);

			if (!g_Players.empty())
			{
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetGUID() == packet->guid)
					{
						g_Players[i].RemoveWeapon(weapon.C_String());
						return;
					}
				}
			}
		}

		void UpdateWeapon(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			RakNet::RakString weapon;
			int ammo, clipAmmo;

			bitStream->Read(weapon);
			bitStream->Read(ammo);
			bitStream->Read(clipAmmo);

			if (!g_Players.empty())
			{
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetGUID() == packet->guid)
					{
						g_Players[i].UpdateWeapon(weapon.C_String(), ammo, clipAmmo);
						return;
					}
				}
			}
		}

		void OnPlayerShot(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			RakNet::RakString weapon;
			int ammo, clipAmmo;
			float xx;

			CVector3 Position, Aim;

			bitStream->Read(weapon);
			bitStream->Read(ammo);
			bitStream->Read(clipAmmo);

			bitStream->Read(Aim.x);
			bitStream->Read(Aim.y);
			bitStream->Read(Aim.z);

			//PRINT(ConsoleForeground::GREEN, false, "OPS %f", xx);

			if (!g_Players.empty())
			{
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetGUID() == packet->guid)
					{
						if (g_Players[i].UpdateWeapon(weapon.C_String(), ammo, clipAmmo))
						{
							for (unsigned int a = 0; a < g_ApiModules.size(); a++)
							{
								void *Instance = g_ApiModules[a].GetInstance();
								API::Players::OnPlayerShot(Instance, g_Players[i], weapon.C_String());
							}

							RakNet::BitStream bitstream;
							bitstream.Write(g_Players[i].GetID());
							bitstream.Write(weapon);
							bitstream.Write(ammo);
							bitstream.Write(clipAmmo);
							bitstream.Write(Aim.x);
							bitstream.Write(Aim.y);
							bitstream.Write(Aim.z);
							//Server::GetServer()->GetNetworkManager()->GetRPC().Signal("OnPlayerShot", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);
							Server::GetServer()->GetNetworkManager()->GetRPC().Signal("OnPlayerShot", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false, false);
						}
						return;
					}
				}
			}
		}

		void OnPlayerAim(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			bool aiming;

			bitStream->Read(aiming);

			if (!g_Players.empty())
			{
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetGUID() == packet->guid)
					{
						g_Players[i].SetAiming(aiming);

						RakNet::BitStream bitstream;
						bitstream.Write(g_Players[i].GetID());
						bitstream.Write(aiming);
						//Server::GetServer()->GetNetworkManager()->GetRPC().Signal("OnPlayerAim", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true, false);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("OnPlayerAim", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false, false);

						return;
					}
				}
			}
		}

		void OnPlayerPickup(RakNet::BitStream *bitStream, RakNet::Packet *packet)
		{
			int pickup, playerid;

			bitStream->Read(pickup);
			bitStream->Read(playerid);

			for (unsigned int p = 0; p < g_Players.size(); p++)
			{
				if (g_Players[p].GetID() == playerid)
				{
					for (unsigned int i = 0; i < g_ApiModules.size(); i++)
					{
						void *Instance = g_ApiModules[i].GetInstance();

						API::Players::OnPlayerPickup(Instance, g_Players[p], pickup);
					}
					return;
				}
			}
		}
	}
}
