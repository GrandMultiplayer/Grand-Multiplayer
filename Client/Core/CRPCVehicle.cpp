// RakNet
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "APIStructs.h"
#include "CVector3.h"
#include "CVehicle.h"
#include "CNetworkManager.h"
#include "CStreamer.h"
#include "CLocalPlayer.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "CRPCVehicle.h"

void CRPCVehicle::InitVehicle(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity = -1;
	uint32_t model;
	CVector3 position, rotation;
	bool needsInit = false;

	bitStream->Read(entity);
	bitStream->Read(model);
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);
	bitStream->Read(rotation.x);
	bitStream->Read(rotation.y);
	bitStream->Read(rotation.z);
	bitStream->Read(needsInit);

	if (entity == -1)
	{
		LOG_ERROR("CRPCVehicle::InitVehicle | Recived invalid vehicle {E:%i} with model %i.", entity, model);
		return;
	}

	CVehicle newVehicle(entity);
	newVehicle.SetPosition(position);
	newVehicle.SetRotation(rotation);
	newVehicle.SetModel(model);
	newVehicle.SetNeedsInit(needsInit);
	g_Vehicles.push_back(newVehicle);

	if (CNetworkManager::g_ConnectionState == CNetworkManager::ConenctionState::Connected)
	{
		CVector3 pos;
		if (CLocalPlayer::IsScriptedCameraActive())
		{
			Vector3 camPos = CAM::GET_CAM_COORD(CLocalPlayer::GetScriptedCamera());
			pos = { camPos.x, camPos.y, camPos.z };
		}

		CStreamer::StreamVehiclesIn(CLocalPlayer::GetPosition(), CLocalPlayer::IsScriptedCameraActive(), pos, 50.0f);
	}
}

void CRPCVehicle::SetStandardColor(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::SetStandardColor.");

	int entity, layer, paintType, color;

	bitStream->Read(entity);
	bitStream->Read(layer);
	bitStream->Read(paintType);
	bitStream->Read(color);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			g_Vehicles[i].SetColor(layer, paintType, color);
			return;
		}
	}
}

void CRPCVehicle::SetCustomColor(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::SetCustomColor.");

	int entity, layer;
	Color color;

	bitStream->Read(entity);
	bitStream->Read(layer);
	bitStream->Read(color.Red);
	bitStream->Read(color.Green);
	bitStream->Read(color.Blue);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			g_Vehicles[i].SetColor(layer, color);
			return;
		}
	}
}

void CRPCVehicle::SetNumberPlate(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::SetNumberPlate.");

	int entity;
	RakNet::RakString plate;

	bitStream->Read(entity);
	bitStream->Read(plate);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			g_Vehicles[i].SetNumberPlate(plate.C_String());
			return;
		}
	}
}

void CRPCVehicle::SetMod(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::SetMod.");

	int entity, modType, modIndex;

	bitStream->Read(entity);
	bitStream->Read(modType);
	bitStream->Read(modIndex);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			g_Vehicles[i].SetMod(modType, modIndex);
			return;
		}
	}
}

//void CRPCVehicle::SetEngineState(RakNet::BitStream *bitStream, RakNet::Packet *packet)
//{
//	LOG_DEBUG("CRPCVehicle::SetEngineState.");
//
//	int entity;
//	bool state, instant, forced;
//
//	bitStream->Read(entity);
//	bitStream->Read(state);
//	bitStream->Read(instant);
//	bitStream->Read(forced);
//
//	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
//	{
//		if (g_Vehicles[i].GetID() == entity)
//		{
//			g_Vehicles[i].SetEngineState(state, instant, forced);
//			return;
//		}
//	}
//}

void CRPCVehicle::SetDoorsLockState(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::SetDoorsLockState.");

	int entity, state;

	bitStream->Read(entity);
	bitStream->Read(state);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			g_Vehicles[i].SetDoorsLockState(state);
			return;
		}
	}
}

void CRPCVehicle::SetNumberPlateStyle(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::SetNumberPlateStyle.");

	int entity, state;

	bitStream->Read(entity);
	bitStream->Read(state);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			g_Vehicles[i].SetNumberPlateStyle(state);
			return;
		}
	}
}

void CRPCVehicle::SetExtra(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::SetExtra.");

	int entity, extra;
	bool toggle;

	bitStream->Read(entity);
	bitStream->Read(extra);
	bitStream->Read(toggle);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			g_Vehicles[i].SetExtra(extra, toggle);
			return;
		}
	}
}

void CRPCVehicle::FixDeformation(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVehicle::FixDeformation.");

	int entity;

	bitStream->Read(entity);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			return g_Vehicles[i].FixDeformation();
		}
	}
}

void CRPCVehicle::TowVehicle(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, towedVehicle;

	bitStream->Read(entity);
	bitStream->Read(towedVehicle);

	LOG_DEBUG("CRPCVehicle::TowVehicle [ %i , %i ]", entity, towedVehicle);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			return g_Vehicles[i].SetTowedVehicle(towedVehicle);
		}
	}
}

void CRPCVehicle::DeployCargobobRope(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	int entity, ropeType;

	bitStream->Read(entity);
	bitStream->Read(ropeType);

	LOG_DEBUG("CRPCVehicle::DeployCargobobRope [ %i , %i ]", entity, ropeType);

	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetID() == entity)
		{
			return g_Vehicles[i].SetCargobobRopeType(ropeType);
		}
	}
}