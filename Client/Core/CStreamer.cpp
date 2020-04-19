#include <winsock2.h>
#include <Windows.h>
#include <vector>

// RakNet
#include <RakNetTypes.h>

#include "hook/enums.h"
#include "hook\types.h"
#include "hook\natives.h"

#include "Common.h"

#include "CVector3.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"
#include "CPickup.h"
#include "CLocalPlayer.h"
#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CStreamer.h"

#define _VERBOSE_LOGING
//#define _LOCALTESTING_

std::vector<CStreamer::streamedObject> CStreamer::streamed;

float CStreamer::MaxRange = 1000.0f;

int CStreamer::MaxPlayers = 40;		// The max visable players to be shown
int CStreamer::PlayerCount = 0;	// The current count of visable players
float CStreamer::PlayerRange = 500.0f;	// Should the 'MaxPlayers' be reached it'll store the range it reached at that point here for the streamingout to check.

int CStreamer::MaxVehicles = 50;
int CStreamer::VehicleCount = 0;
float CStreamer::VehicleRange = 500.0f;

int CStreamer::MaxObjects = 500;
int CStreamer::ObjectCount = 0;
float CStreamer::ObjectRange = 500.0f;

int CStreamer::MaxNpcs = 20;
int CStreamer::NpcCount = 0;
float CStreamer::NpcRange = 500.0f;

int CStreamer::MaxPickups = 20;
int CStreamer::PickupCount = 0;
float CStreamer::PickupRange = 500.0f;

void CStreamer::Start()
{
	CStreamer streamer;
	streamer.Pulse();
}

void CStreamer::Pulse()
{
	CVector3 pos;
	if (CLocalPlayer::IsScriptedCameraActive())
	{
		Vector3 camPos = CAM::GET_CAM_COORD(CLocalPlayer::GetScriptedCamera());
		pos = { camPos.x, camPos.y, camPos.z };
	}

	StreamOut(CLocalPlayer::IsScriptedCameraActive(), pos);

	/*
		We stream objects in first so that npcs, players, vehicle spawn ontop of it when spawned
		otherwise vehicles and npcs would fall threw befor the object existed.

		Also vehicles befor players to garentee a player is put into the vehicle if they are in it.
	*/
	for (float curDis = 50.0f; curDis < MaxRange; curDis += 50.0f)
	{
		StreamObjectsIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, curDis);
		StreamVehiclesIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, curDis);
		StreamPlayersIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, curDis);
		StreamNpcIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, curDis);
		StreamPickupIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, curDis);
		//StreamOtherIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, curDis);
	}
}

void CStreamer::StreamObjectsIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance)
{
	if (CNetworkManager::g_ConnectionState != CONSTATE_COND)
		return;

	for (unsigned int o = 0; o < g_Objects.size(); o++)
	{
		if (g_Objects[o].GetViewDistance() < distance)
		{
			if (ObjectCount >= MaxObjects)
				return;

			if (!g_Objects[o].IsCreated() && ((scriptedCameraState && CVector3::Distance2D(camPosition, g_Objects[o].GetPosition()) < distance)
				|| (CVector3::Distance2D(playerPosition, g_Objects[o].GetPosition()) < distance)))
			{
#ifdef _VERBOSE_LOGING
				LOG_DEBUG("<Streamer> Attempting to stream in Object [%i]", g_Objects[o].GetID());
#endif

				if (g_Objects[o].CreateObject())
				{
					ObjectCount++;

					if (scriptedCameraState && CVector3::Distance2D(camPosition, g_Objects[o].GetPosition()) < distance)
						ObjectRange = CVector3::Distance2D(camPosition, g_Objects[o].GetPosition());
					else
						ObjectRange = CVector3::Distance2D(playerPosition, g_Objects[o].GetPosition());

					//do we neeed this? we could just loop threw each pool and check if its created...
					streamedObject newSObj;
					newSObj.entity = g_Objects[o].GetID();
					newSObj.type = CEntity::Type::eObject;

					streamed.push_back(newSObj);
				}
			}
		}
	}
}

void CStreamer::StreamVehiclesIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance)
{
	if (CNetworkManager::g_ConnectionState != CONSTATE_COND)
		return;

	for (unsigned int v = 0; v < g_Vehicles.size(); v++)
	{
		if (VehicleCount >= MaxVehicles)
			return;

		if (!g_Vehicles[v].Exists() &&
			((scriptedCameraState && CVector3::Distance2D(camPosition, g_Vehicles[v].GetPosition()) < distance)
			|| (CVector3::Distance2D(playerPosition, g_Vehicles[v].GetPosition()) < distance)))
		{
#ifdef _VERBOSE_LOGING
			LOG_DEBUG("<Streamer> Attempting to stream in Vehicle [%i]", g_Vehicles[v].GetID());
#endif

			if (g_Vehicles[v].CreateVehicle())
			{
				VehicleCount++;

				if (scriptedCameraState && CVector3::Distance2D(camPosition, g_Vehicles[v].GetPosition()) < distance)
					VehicleRange = CVector3::Distance2D(camPosition, g_Vehicles[v].GetPosition());
				else
					VehicleRange = CVector3::Distance2D(playerPosition, g_Vehicles[v].GetPosition());

				//do we neeed this? we could just loop threw each pool and check if its created...
				streamedObject newSObj;
				newSObj.entity = g_Vehicles[v].GetID();
				newSObj.type = CEntity::Type::eVehicle;

				streamed.push_back(newSObj);
			}
		}
	}
}

void CStreamer::StreamPlayersIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance)
{
	if (CNetworkManager::g_ConnectionState != CONSTATE_COND)
		return;

	for (unsigned int p = 0; p < g_Players.size(); p++)
	{
		if (PlayerCount >= MaxPlayers)
			return;

#ifndef _LOCALTESTING_
		if (g_Players[p].GetID() != CLocalPlayer::GetID())
		{
#endif
			if (!g_Players[p].IsCreated() && ((scriptedCameraState && CVector3::Distance2D(camPosition, g_Players[p].GetPosition()) < distance)
				|| (CVector3::Distance2D(playerPosition, g_Players[p].GetPosition()) < distance)))
			{
#ifdef _VERBOSE_LOGING
				LOG_DEBUG("<Streamer> Attempting to stream in Player [%i]", g_Players[p].GetID());
#endif

				if (g_Players[p].CreatePed())
				{
					PlayerCount++;

					if (scriptedCameraState && CVector3::Distance2D(camPosition, g_Players[p].GetPosition()) < distance)
						PlayerRange = CVector3::Distance2D(camPosition, g_Players[p].GetPosition());
					else
						PlayerRange = CVector3::Distance2D(playerPosition, g_Players[p].GetPosition());

					//do we neeed this? we could just loop threw each pool and check if its created...
					streamedObject newSObj;
					newSObj.entity = g_Players[p].GetID();
					newSObj.type = CEntity::Type::ePlayer;

					streamed.push_back(newSObj);
				}
			}
#ifndef _LOCALTESTING_
		}
#endif
	}
}

void CStreamer::StreamNpcIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance)
{
	if (CNetworkManager::g_ConnectionState != CONSTATE_COND)
		return;

	for (unsigned int n = 0; n < g_Npcs.size(); n++)
	{
		if (NpcCount >= MaxNpcs)
			return;

		if (!g_Npcs[n].IsCreated() && ((scriptedCameraState && CVector3::Distance2D(camPosition, g_Npcs[n].GetPosition()) < distance)
			|| (CVector3::Distance2D(playerPosition, g_Npcs[n].GetPosition()) < distance)))
		{
#ifdef _VERBOSE_LOGING
			LOG_DEBUG("<Streamer> Attempting to stream in NPC [%i]", g_Npcs[n].GetID());
#endif

			if (g_Npcs[n].CreateNpc())
			{
				NpcCount++;

				if (scriptedCameraState && CVector3::Distance2D(camPosition, g_Npcs[n].GetPosition()) < distance)
					NpcRange = CVector3::Distance2D(camPosition, g_Npcs[n].GetPosition());
				else
					NpcRange = CVector3::Distance2D(playerPosition, g_Npcs[n].GetPosition());

				//do we neeed this? we could just loop threw each pool and check if its created...
				streamedObject newSObj;
				newSObj.entity = g_Npcs[n].GetID();
				newSObj.type = CEntity::Type::eNPC;

				streamed.push_back(newSObj);
			}
		}
	}
}

void CStreamer::StreamPickupIn(const CVector3 playerPosition, const bool scriptedCameraState, const CVector3 camPosition, const float distance)
{
	if (CNetworkManager::g_ConnectionState != CONSTATE_COND)
		return;

	for (unsigned int p = 0; p < g_Pickups.size(); p++)
	{
		if (PickupCount >= MaxPickups)
			return;

		if (!g_Pickups[p].IsCreated() && ((scriptedCameraState && CVector3::Distance2D(camPosition, g_Pickups[p].GetPosition()) < distance)
			|| (CVector3::Distance2D(playerPosition, g_Pickups[p].GetPosition()) < distance)))
		{
#ifdef _VERBOSE_LOGING
			LOG_DEBUG("<Streamer> Attempting to stream in Pickup [%i]", g_Pickups[p].GetID());
#endif

			if (g_Pickups[p].Spawn())
			{
				PickupCount++;

				if (scriptedCameraState && CVector3::Distance2D(camPosition, g_Pickups[p].GetPosition()) < distance)
					PickupRange = CVector3::Distance2D(camPosition, g_Pickups[p].GetPosition());
				else
					PickupRange = CVector3::Distance2D(playerPosition, g_Pickups[p].GetPosition());

				//do we neeed this? we could just loop threw each pool and check if its created...
				streamedObject newSObj;
				newSObj.entity = g_Pickups[p].GetID();
				newSObj.type = CEntity::Type::ePickup;

				streamed.push_back(newSObj);
			}
		}
	}
}


void CStreamer::StreamOut(const bool scriptedCameraState, const CVector3 camPosition)
{
	if (CNetworkManager::g_ConnectionState != CONSTATE_COND)
	{
		if (!streamed.empty())
		{
			for (unsigned int i = 0; i < streamed.size(); i++)
			{
				streamed.erase(streamed.begin() + i);
			}
			streamed.shrink_to_fit();
		}

		return;
	}
	/*
		The steaming out needs some more work as tings will get streamed out regardless of their set view distance.
	*/
	const CVector3 playerPosition = CLocalPlayer::GetPosition();

	for (unsigned int i = 0; i < streamed.size(); i++)
	{
		switch (streamed[i].type)
		{
		case CEntity::Type::ePlayer:
		{
			for (unsigned int x = 0; x < g_Players.size(); x++)
			{
				if (g_Players[x].GetID() == streamed[i].entity)
				{
					if (g_Players[x].IsCreated())
					{
						float camDistance = CVector3::Distance2D(camPosition, g_Players[x].GetPosition());
						float localDistance = CVector3::Distance2D(playerPosition, g_Players[x].GetPosition());

						bool outCameraRange = true, outLocalRange = true;

						if (scriptedCameraState)
							if (camDistance < (PlayerRange + 10.0f) || (camDistance < g_Players[x].GetViewDistance() && PlayerCount < MaxPlayers))
								outCameraRange = false;

						if (localDistance < (PlayerRange + 10.0f) || (localDistance < g_Players[x].GetViewDistance() && PlayerCount < MaxPlayers))
							outLocalRange = false;

						if (outCameraRange && outLocalRange)
						{
#ifdef _VERBOSE_LOGING
							LOG_DEBUG("<Streamer> Streaming Out player %i.", streamed[i].entity);
#endif

							g_Players[x].Delete();

							streamed.erase(streamed.begin() + i);
							PlayerCount--;
						}
					}
					else
					{
						streamed.erase(streamed.begin() + i);
						PlayerCount--;
					}

					break;
				}
			}
			break;
		}
		case CEntity::Type::eVehicle:
		{
			for (unsigned int x = 0; x < g_Vehicles.size(); x++)
			{
				if (g_Vehicles[x].GetID() == streamed[i].entity)
				{
					if (g_Vehicles[x].Exists())
					{
						float camDistance = CVector3::Distance2D(camPosition, g_Vehicles[x].GetPosition());
						float localDistance = CVector3::Distance2D(playerPosition, g_Vehicles[x].GetPosition());

						bool outCameraRange = true, outLocalRange = true;

						if (scriptedCameraState)
							if (camDistance < (VehicleRange + 10.0f) || (camDistance < g_Vehicles[x].GetViewDistance() && VehicleCount < MaxVehicles))
								outCameraRange = false;

						if (localDistance < (VehicleRange + 10.0f) || (localDistance < g_Vehicles[x].GetViewDistance() && VehicleCount < MaxVehicles))
							outLocalRange = false;

						if (outCameraRange && outLocalRange)
						{
#ifdef _VERBOSE_LOGING
							LOG_DEBUG("<Streamer> Streaming Out vehicle %i.", streamed[i].entity);
#endif

							g_Vehicles[x].Delete();

							streamed.erase(streamed.begin() + i);
							VehicleCount--;
						}
					}
					else
					{
						streamed.erase(streamed.begin() + i);
						VehicleCount--;
					}

					break;
				}
			}
			break;
		}
		case CEntity::Type::eObject:
		{
			for (unsigned int x = 0; x < g_Objects.size(); x++)
			{
				if (g_Objects[x].GetID() == streamed[i].entity)
				{
					if (g_Objects[x].IsCreated())
					{
						float camDistance = CVector3::Distance2D(camPosition, g_Objects[x].GetPosition());
						float localDistance = CVector3::Distance2D(playerPosition, g_Objects[x].GetPosition());

						bool outCameraRange = true, outLocalRange = true;

						if (scriptedCameraState)
							if (camDistance < (ObjectRange + 10.0f) || (camDistance < g_Objects[x].GetViewDistance() && ObjectCount < MaxObjects))
								outCameraRange = false;

						if (localDistance < (ObjectRange + 10.0f) || (localDistance < g_Objects[x].GetViewDistance() && ObjectCount < MaxObjects))
							outLocalRange = false;

						if (outCameraRange && outLocalRange)
						{
#ifdef _VERBOSE_LOGING
							LOG_DEBUG("<Streamer> Streaming Out object %i.", streamed[i].entity);
#endif

							g_Objects[x].Delete();

							streamed.erase(streamed.begin() + i);
							ObjectCount--;
						}
					}
					else
					{
						streamed.erase(streamed.begin() + i);
						ObjectCount--;
					}

					break;
				}
			}
			break;
		}
		case CEntity::Type::eNPC:
		{
			for (unsigned int x = 0; x < g_Npcs.size(); x++)
			{
				if (g_Npcs[x].GetID() == streamed[i].entity)
				{
					if (g_Npcs[x].IsCreated())
					{
						float camDistance = CVector3::Distance2D(camPosition, g_Npcs[x].GetPosition());
						float localDistance = CVector3::Distance2D(playerPosition, g_Npcs[x].GetPosition());

						bool outCameraRange = true, outLocalRange = true;

						if (scriptedCameraState)
							if (camDistance < (NpcRange + 10.0f) || (camDistance < g_Npcs[x].GetViewDistance() && NpcCount < MaxNpcs))
								outCameraRange = false;

						if (localDistance < (NpcRange + 10.0f) || (localDistance < g_Npcs[x].GetViewDistance() && NpcCount < MaxNpcs))
							outLocalRange = false;

						if (outCameraRange && outLocalRange)
						{
#ifdef _VERBOSE_LOGING
							LOG_DEBUG("<Streamer> Streaming Out npc %i.", streamed[i].entity);
#endif

							g_Npcs[x].Delete();

							streamed.erase(streamed.begin() + i);
							NpcCount--;
						}
					}
					else
					{
						streamed.erase(streamed.begin() + i);
						NpcCount--;
					}

					break;
				}
			}
			break;
		}
		case CEntity::Type::ePickup:
		{
			for (unsigned int x = 0; x < g_Pickups.size(); x++)
			{
				if (g_Pickups[x].GetID() == streamed[i].entity)
				{
					if (g_Pickups[x].IsCreated())
					{
						float camDistance = CVector3::Distance2D(camPosition, g_Pickups[x].GetPosition());
						float localDistance = CVector3::Distance2D(playerPosition, g_Pickups[x].GetPosition());

						bool outCameraRange = true, outLocalRange = true;

						if (scriptedCameraState)
							if (camDistance < (PickupRange + 10.0f) || (camDistance < g_Pickups[x].GetViewDistance() && PickupCount < MaxPickups))
								outCameraRange = false;

						if (localDistance < (PickupRange + 10.0f) || (localDistance < g_Pickups[x].GetViewDistance() && PickupCount < MaxPickups))
							outLocalRange = false;

						if (outCameraRange && outLocalRange)
						{
#ifdef _VERBOSE_LOGING
							LOG_DEBUG("<Streamer> Streaming Out pickup %i.", streamed[i].entity);
#endif

							g_Pickups[x].Delete();

							streamed.erase(streamed.begin() + i);
							PickupCount--;
						}
					}
					else
					{
						streamed.erase(streamed.begin() + i);
						PickupCount--;
					}

					break;
				}
			}
			break;
		}
		default:
			LOG_DEBUG("<Streamer> Found Invalid entity %i with unknown type to stream out.", streamed[i].entity);
			streamed.erase(streamed.begin() + i);
			break;
		}
	}

	streamed.shrink_to_fit();
}

void CStreamer::ForceStreamOut()
{
#ifdef _VERBOSE_LOGING
	LOG_DEBUG("<Streamer> ForceStreamOut Called.");
#endif

	if (!streamed.empty())
	{
		for (unsigned int i = 0; i < streamed.size(); i++)
		{
			switch (streamed[i].type)
			{
			case CEntity::Type::ePlayer:
				for (unsigned int index = 0; index < g_Players.size(); index++)
				{
					if (g_Players[index].GetID() == streamed[i].entity)
					{
						if (g_Players[index].IsCreated())
							g_Players[index].Delete();

						streamed.erase(streamed.begin() + i);
						PlayerCount--;
						break;
					}
				}
				break;
			case CEntity::Type::eVehicle:
				for (unsigned int index = 0; index < g_Vehicles.size(); index++)
				{
					if (g_Vehicles[index].GetID() == streamed[i].entity)
					{
						if (g_Vehicles[index].IsCreated())
							g_Vehicles[index].Delete();

						streamed.erase(streamed.begin() + i);
						VehicleCount--;
						break;
					}
				}
				break;
			case CEntity::Type::eObject:
				for (unsigned int index = 0; index < g_Objects.size(); index++)
				{
					if (g_Objects[index].GetID() == streamed[i].entity)
					{
						if (g_Objects[index].IsCreated())
							g_Objects[index].Delete();

						streamed.erase(streamed.begin() + i);
						ObjectCount--;
						break;
					}
				}
				break;
			case CEntity::Type::eNPC:
				for (unsigned int index = 0; index < g_Npcs.size(); index++)
				{
					if (g_Npcs[index].GetID() == streamed[i].entity)
					{
						if (g_Npcs[index].IsCreated())
							g_Npcs[index].Delete();

						streamed.erase(streamed.begin() + i);
						NpcCount--;
						break;
					}
				}
			case CEntity::Type::ePickup:
				for (unsigned int index = 0; index < g_Pickups.size(); index++)
				{
					if (g_Pickups[index].GetID() == streamed[i].entity)
					{
						if (g_Pickups[index].IsCreated())
							g_Pickups[index].Delete();

						streamed.erase(streamed.begin() + i);
						PickupCount--;
						break;
					}
				}
				break;
			default:
				LOG_DEBUG("[CStreamer] Found Invalid entity %i when forced to stream out.", streamed[i].entity);
				streamed.erase(streamed.begin() + i);
				break;
			}
		}

		streamed.shrink_to_fit();

		PlayerCount = 0;
		VehicleCount = 0;
		ObjectCount = 0;
		NpcCount = 0;
		PickupCount = 0;
	}
}

void CStreamer::Reset()
{
	if (!streamed.empty())
	{
		for (int i = 0; i < streamed.size(); i++)
		{
			streamed.erase(streamed.begin() + i);
		}

		streamed.shrink_to_fit();

		PlayerCount = 0;
		VehicleCount = 0;
		ObjectCount = 0;
		NpcCount = 0;
		PickupCount = 0;
	}
}