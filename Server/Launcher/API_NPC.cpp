// Self
#include "API_NPC.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Common.h"
#include "Server.h"
#include "CNPC.h"
#include "ModelCache.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace NPC
	{
		const char * ThisNamespace = "API::NPC::";

		Objects::Entity CreateNPC(const uint32_t model, const CVector3 position, const CVector3 rotation)
		{
			CNPC npc(model, position, rotation);

			Objects::Entity ent;
			ent.SetID(npc.GetID());
			ent.SetType(npc.GetType());

			g_Npcs.push_back(npc);

			RakNet::BitStream bitstream;
			bitstream.Write(npc.GetID());
			bitstream.Write(model);
			bitstream.Write(position.x);
			bitstream.Write(position.y);
			bitstream.Write(position.z);
			bitstream.Write(rotation.x);
			bitstream.Write(rotation.y);
			bitstream.Write(rotation.z);

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateNPC", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

			ModelCache::AddModel(model);

			return ent;
		}
	}
}