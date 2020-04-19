// System
#include <vector>

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"

// GrandM
#include "Server.h"
#include "CWorld.h"

#include "CVector3.h"
#include "APIStructs.h"

#include "CVehicle.h"
#include "CObject.h"
#include "CCheckpoint.h"
#include "CBlip.h"
#include "CNPC.h"
#include "CPickup.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "NetworkSync.h"

namespace NetworkSync
{
	void SyncPlayersLocalData(RakNet::RakNetGUID guid, int id)
	{
		/*RakNet::BitStream bitstream;
		bitstream.Write(id);
		server->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);*/
	}

	void SyncServerWorld(RakNet::RakNetGUID user)
	{
		/*
		* NOTE:
		*		Should the WORLD have dims too?
		*/
		RakNet::BitStream bitstream;

		// Sync Time
		bitstream.Reset();
		bitstream.Write(CWorld::GetTime().hour);
		bitstream.Write(CWorld::GetTime().minute);
		bitstream.Write(CWorld::GetTime().second);
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetTime", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

		// Sync Weather
		bitstream.Reset();
		bitstream.Write(RakNet::RakString(CWorld::GetWeather().Weather.c_str()));
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetWeather", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

		// Sync Ground Snow
		bitstream.Reset();
		bitstream.Write(CWorld::IsGroundSnow());
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetGroundSnow", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

		// Sync IPLs
		if (!CWorld::g_IPLs.empty())
		{
			for (unsigned int i = 0; i < CWorld::g_IPLs.size(); i++)
			{
				if (CWorld::g_IPLs[i].enabled)
				{
					RakNet::BitStream bitstream;
					bitstream.Write(RakNet::RakString(CWorld::g_IPLs[i].ipl.c_str()));
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("LoadIPL", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
				}
				else
				{
					RakNet::BitStream bitstream;
					bitstream.Write(RakNet::RakString(CWorld::g_IPLs[i].ipl.c_str()));
					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("UnloadIPL", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
				}
			}
		}

		// Sync Blackout
		bitstream.Reset();
		bitstream.Write(CWorld::IsBlackout());
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetBlackout", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

		// Default World is 0, This may change as the mod develops but not at the moment.... 
		// IE: Setting the players world befor they sync in onplayerconnecting, although since the player isn't even technicaly
		// an entity yet that would be odd, we will see.
		SyncEntities(user, 0);
	}


	void SyncEntities(RakNet::RakNetGUID user, int dim)
	{
		/*
		* NOTE:
		*		Should the WORLD have dims too?
		*/
		RakNet::BitStream bitstream;

		// Init vehicles
		if (!g_Vehicles.empty())
		{
			for (unsigned int i = 0; i < g_Vehicles.size(); i++)
			{
				LOG_DEBUG("Checking Vehicle %i : %i", g_Vehicles[i].GetID(), g_Vehicles[i].GetDimension());

				if (g_Vehicles[i].GetDimension() == dim)
				{
					LOG_DEBUG("Sending Vehicle %i : %i", g_Vehicles[i].GetID(), g_Vehicles[i].GetModel());

					bitstream.Reset();
					CVector3 Position = g_Vehicles[i].GetPosition();
					CVector3 Rotation = g_Vehicles[i].GetRotation();

					bitstream.Write(g_Vehicles[i].GetID());
					bitstream.Write(g_Vehicles[i].GetModel());
					bitstream.Write(Position.x);
					bitstream.Write(Position.y);
					bitstream.Write(Position.z);
					bitstream.Write(Rotation.x);
					bitstream.Write(Rotation.y);
					bitstream.Write(Rotation.z);
					bitstream.Write(g_Vehicles[i].NeedsInit());

					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("InitVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

					g_Vehicles[i].SyncData(user);
				}
			}
		}

		// Sync Objects
		if (!g_Objects.empty())
		{
			for (unsigned int o = 0; o < g_Objects.size(); o++)
			{
				if (g_Objects[o].GetDimension() == dim)
				{
					bitstream.Reset();
					bitstream.Write(g_Objects[o].GetID());
					bitstream.Write(g_Objects[o].GetModel());

					CVector3 Position = g_Objects[o].GetPosition();
					CVector3 Rotation = g_Objects[o].GetRotation();

					bitstream.Write(Position.x);
					bitstream.Write(Position.y);
					bitstream.Write(Position.z);
					bitstream.Write(Rotation.x);
					bitstream.Write(Rotation.y);
					bitstream.Write(Rotation.z);
					bitstream.Write(g_Objects[o].IsDynamic());

					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateObject", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

					g_Objects[o].SyncData(user);
				}
			}
		}

		// Sync NPCs
		if (!g_Npcs.empty())
		{
			for (unsigned int o = 0; o < g_Npcs.size(); o++)
			{
				if (g_Npcs[o].GetDimension() == dim)
				{
					bitstream.Reset();
					bitstream.Write(g_Npcs[o].GetID());
					bitstream.Write(g_Npcs[o].GetModel());

					CVector3 Position = g_Npcs[o].GetPosition();
					CVector3 Rotation = g_Npcs[o].GetRotation();

					bitstream.Write(Position.x);
					bitstream.Write(Position.y);
					bitstream.Write(Position.z);
					bitstream.Write(Rotation.x);
					bitstream.Write(Rotation.y);
					bitstream.Write(Rotation.z);

					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateNPC", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

					g_Npcs[o].SyncData(user);
				}
			}
		}

		// Sync Checkpoints
		if (!g_Checkpoints.empty())
		{
			for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
			{
				if (g_Checkpoints[i].GetDimension() == dim)
				{
					bitstream.Reset();
					bitstream.Write(g_Checkpoints[i].GetID());

					const CVector3 Position = g_Checkpoints[i].GetPosition();
					const CVector3 PointTo = g_Checkpoints[i].GetPointTo();
					const Color Col = g_Checkpoints[i].GetColor();

					bitstream.Write(Position.x);
					bitstream.Write(Position.y);
					bitstream.Write(Position.z);
					/*bitstream.Write(PointTo.x);
					bitstream.Write(PointTo.y);
					bitstream.Write(PointTo.z);*/
					//bitstream.Write(g_Checkpoints[i].GetType());
					bitstream.Write(g_Checkpoints[i].GetRadius());
					bitstream.Write(Col.Red);
					bitstream.Write(Col.Green);
					bitstream.Write(Col.Blue);
					bitstream.Write(Col.Alpha);
					bitstream.Write(g_Checkpoints[i].GetReserved());

					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateCheckpoint", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

					g_Checkpoints[i].SyncData(user);
				}
			}
		}

		// Sync Blips
		if (!g_Blips.empty())
		{
			for (unsigned int i = 0; i < g_Blips.size(); i++)
			{
				if (g_Blips[i].GetDimension() == dim)
				{
					bitstream.Reset();
					bitstream.Write(g_Blips[i].GetID());

					const CVector3 Position = g_Blips[i].GetPosition();

					bitstream.Write(Position.x);
					bitstream.Write(Position.y);
					bitstream.Write(Position.z);

					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateBlipPos", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

					g_Blips[i].SyncData(user);
				}
			}
		}

		// Sync Pickups
		if (!g_Pickups.empty())
		{
			for (unsigned int i = 0; i < g_Pickups.size(); i++)
			{
				if (g_Pickups[i].GetDimension() == dim)
				{
					bitstream.Reset();
					bitstream.Write(g_Pickups[i].GetID());

					const CVector3 Position = g_Pickups[i].GetPosition();

					bitstream.Write(Position.x);
					bitstream.Write(Position.y);
					bitstream.Write(Position.z);
					bitstream.Write(g_Pickups[i].GetModel());

					Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreatePickup", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);

					g_Pickups[i].SyncData(user);
				}
			}
		}
	}
}