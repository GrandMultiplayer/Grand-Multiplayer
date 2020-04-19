#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "CCore.h"
#include "CEntity.h"
#include "CPlayer.h"
#include "CObject.h"
#include "CNPC.h"
#include "CCheckpoint.h"
#include "CVehicle.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCBlip.h"

void CRPCBlip::CreateE(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::CreateE");

	int entity = -1, type = -1;

	bitStream->Read(entity);
	bitStream->Read(type);

	if (entity == -1)
	{
		LOG_WARNING("[CRPCBlip::CreateE] Invalid entity recived as -1");
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				CBlip* blip = g_Players[i].Blip();

				if(!blip->HasBlip())
					blip->Create(entity);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				CBlip* blip = g_Npcs[i].Blip();

				if (!blip->HasBlip())
					blip->Create(entity);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				CBlip* blip = g_Objects[i].Blip();

				if (!blip->HasBlip())
					blip->Create(entity);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				CBlip* blip = g_Checkpoints[i].Blip();
				blip->Create(entity);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				CBlip* blip = g_Vehicles[i].Blip();

				if (!blip->HasBlip())
					blip->Create(entity);

				blip = nullptr;
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCBlip::CreateE: Entity %i  is not a valid Entity Type.", entity );
		break;
	}
}

void CRPCBlip::CreateP(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::CreateP.");

	int entity = -1;
	CVector3 position;

	bitStream->Read(entity);
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(position.z);

	CBlip newBlip(entity, position);
	g_Blips.push_back(newBlip);
}

void CRPCBlip::SetColor(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::SetColor.");

	int entity = -1, color = -1, type = -1;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(color);

	if (entity == -1)
	{
		LOG_WARNING("[CRPCBlip::SetColor] Invalid entity is -1");
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				CBlip* blip = g_Players[i].Blip();
				blip->SetColor(color);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				CBlip* blip = g_Npcs[i].Blip();
				blip->SetColor(color);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				CBlip* blip = g_Objects[i].Blip();
				blip->SetColor(color);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				CBlip* blip = g_Checkpoints[i].Blip();
				blip->SetColor(color);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				CBlip* blip = g_Vehicles[i].Blip();
				blip->SetColor(color);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetColor(color);
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCBlip::SetColor - Invalid Entity %i.", entity);

		return;
		break;
	}
}

void CRPCBlip::Show(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::Show.");

	int entity = -1, type = -1;

	bitStream->Read(entity);
	bitStream->Read(type);

	if (entity == -1)
	{
		LOG_WARNING("[CRPCBlip::Show] Invalid entity recived as -1");
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				CBlip* blip = g_Players[i].Blip();
				blip->Show(g_Players[i].GetPed());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				CBlip* blip = g_Npcs[i].Blip();
				blip->Show(g_Npcs[i].GetEntity());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				CBlip* blip = g_Objects[i].Blip();
				blip->Show(g_Objects[i].GetEntity());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				CBlip* blip = g_Checkpoints[i].Blip();
				blip->Show(g_Checkpoints[i].GetCheckpoint());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				CBlip* blip = g_Vehicles[i].Blip();
				blip->Show(g_Vehicles[i].GetEntity());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].Show();
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCBlip::Show - Invalid Entity: %i.", entity);
		break;
	}
}

void CRPCBlip::Hide(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::Hide.");

	int entity = -1, type = -1;

	bitStream->Read(entity);
	bitStream->Read(type);

	if (entity == -1)
	{
		LOG_WARNING("[CRPCBlip::Hide] Invalid entity recived as -1");
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				CBlip* blip = g_Players[i].Blip();
				blip->Hide();

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				CBlip* blip = g_Npcs[i].Blip();
				blip->Hide();

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				CBlip* blip = g_Objects[i].Blip();
				blip->Hide();

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				CBlip* blip = g_Checkpoints[i].Blip();
				blip->Hide();

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				CBlip* blip = g_Vehicles[i].Blip();
				blip->Hide();

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].Hide();
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCBlip::Hide - Invalid Entity: %i.", entity);
		break;
	}
}

void CRPCBlip::SetSprite(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::SetSprite.");

	int entity = -1, sprite = -1, type = -1;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(sprite);

	if (entity == -1)
	{
		LOG_WARNING("[CRPCBlip::SetSprite] Invalid entity recived as -1");
		return;
	}

	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				CBlip* blip = g_Players[i].Blip();
				blip->SetSprite(sprite);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				CBlip* blip = g_Npcs[i].Blip();
				blip->SetSprite(sprite);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				CBlip* blip = g_Objects[i].Blip();
				blip->SetSprite(sprite);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				CBlip* blip = g_Checkpoints[i].Blip();
				blip->SetSprite(sprite);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				CBlip* blip = g_Vehicles[i].Blip();
				blip->SetSprite(sprite);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetSprite(sprite);
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCBlip::SetSprite - Invalid Entity: %i.", entity);
		break;
	}
}

void CRPCBlip::SetText(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::SetText.");

	int entity = -1, type = -1;
	RakNet::RakString text;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(text);

	if (entity == -1)
	{
		LOG_WARNING("[CRPCBlip::SetText] Invalid entity recived as -1");
		return;
	}
	
	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				CBlip* blip = g_Players[i].Blip();
				blip->SetText(text.C_String());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				CBlip* blip = g_Npcs[i].Blip();
				blip->SetText(text.C_String());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				CBlip* blip = g_Objects[i].Blip();
				blip->SetText(text.C_String());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				CBlip* blip = g_Checkpoints[i].Blip();
				blip->SetText(text.C_String());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				CBlip* blip = g_Vehicles[i].Blip();
				blip->SetText(text.C_String());

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetText(text.C_String());
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCBlip::SetText - Invalid Entity: %i.", entity);
		break;
	}
}

void CRPCBlip::SetBlipShortRange(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCBlip::SetBlipShortRange.");

	int entity = -1, type = -1;
	bool state;

	bitStream->Read(entity);
	bitStream->Read(type);
	bitStream->Read(state);

	if (entity == -1)
	{
		LOG_WARNING("[CRPCBlip::SetBlipShortRange] Invalid entity recived as -1");
		return;
	}
	
	switch (type)
	{
	case CEntity::ePlayer:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetID() == entity)
			{
				CBlip* blip = g_Players[i].Blip();
				blip->SetShortRange(state);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eNPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetID() == entity)
			{
				CBlip* blip = g_Npcs[i].Blip();
				blip->SetShortRange(state);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eObject:
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			if (g_Objects[i].GetID() == entity)
			{
				CBlip* blip = g_Objects[i].Blip();
				blip->SetShortRange(state);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eCheckpoint:
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			if (g_Checkpoints[i].GetID() == entity)
			{
				CBlip* blip = g_Checkpoints[i].Blip();
				blip->SetShortRange(state);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eVehicle:
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() == entity)
			{
				CBlip* blip = g_Vehicles[i].Blip();
				blip->SetShortRange(state);

				blip = nullptr;
				return;
			}
		}
		break;
	case CEntity::eBlip:
		for (unsigned int i = 0; i < g_Blips.size(); i++)
		{
			if (g_Blips[i].GetID() == entity)
			{
				g_Blips[i].SetShortRange(state);
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("CRPCBlip::SetBlipShortRange - Invalid Entity: %i.", entity);
		break;
	}
}