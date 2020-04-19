#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "CVector3.h"
#include "APIStructs.h"
#include "CCheckpoint.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCCheckpoint.h"

void CRPCCheckpoint::Create(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCCheckpoint::Create.");

	int entity, reserved;
	CVector3 position;
	float radius;
	Color color;

	bitStream->Read(entity);
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);
	bitStream->Read(radius);
	bitStream->Read(color.Red);
	bitStream->Read(color.Green);
	bitStream->Read(color.Blue);
	bitStream->Read(color.Alpha);
	bitStream->Read(reserved);


	CCheckpoint newCp(entity, position, radius, color, reserved);
	g_Checkpoints.push_back(newCp);
}

void CRPCCheckpoint::Show(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCCheckpoint::Show.");

	int entity;

	bitStream->Read(entity);

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++) 
	{
		if (g_Checkpoints[i].GetID() == entity) 
		{
			return g_Checkpoints[i].Show();
		}
	}
}

void CRPCCheckpoint::Hide(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCCheckpoint::Hide.");

	int entity;

	bitStream->Read(entity);

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++) 
	{
		if (g_Checkpoints[i].GetID() == entity) 
		{
			g_Checkpoints[i].SetTriggered(false);
			return g_Checkpoints[i].Hide();
		}
	}
}

void CRPCCheckpoint::SetHeight(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCCheckpoint::SetHeight.");

	int entity;
	float nearHeight, farHeight;

	bitStream->Read(entity);
	bitStream->Read(nearHeight);
	bitStream->Read(farHeight);

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++) 
	{
		if (g_Checkpoints[i].GetID() == entity) 
		{
			return g_Checkpoints[i].SetHeight(nearHeight, farHeight);
		}
	}
}

void CRPCCheckpoint::PointTo(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCCheckpoint::PointTo.");

	int entity;
	CVector3 pointTo;

	bitStream->Read(entity);
	bitStream->Read(pointTo.x);
	bitStream->Read(pointTo.y);
	bitStream->Read(pointTo.z);

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++) 
	{
		if (g_Checkpoints[i].GetID() == entity) 
		{
			return g_Checkpoints[i].PointTo(pointTo);
		}
	}
}

void CRPCCheckpoint::SetType(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCCheckpoint::SetType.");

	int entity, type;

	bitStream->Read(entity);
	bitStream->Read(type);

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++) 
	{
		if (g_Checkpoints[i].GetID() == entity) 
		{
			return g_Checkpoints[i].SetType(type);
		}
	}
}

void CRPCCheckpoint::SetColor(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCCheckpoint::SetColor.");

	int entity;
	Color color;

	bitStream->Read(entity);
	bitStream->Read(color.Red);
	bitStream->Read(color.Green);
	bitStream->Read(color.Blue);
	bitStream->Read(color.Alpha);

	for (unsigned int i = 0; i < g_Checkpoints.size(); i++) 
	{
		if (g_Checkpoints[i].GetID() == entity) 
		{
			return g_Checkpoints[i].SetColor(color);
		}
	}
}