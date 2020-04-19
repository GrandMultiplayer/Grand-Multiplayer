// Self
#include "API_Object.h"

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"

// GrandM
#include "Common.h"
#include "Server.h"
#include "CObject.h"
#include "ModelCache.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace Object
	{
		const char * ThisNamespace = "API::Object::";

		Objects::Entity CreateObject(const uint32_t model, CVector3 position, CVector3 rotation, bool dynamic)
		{
			CObject object(model, position, rotation, dynamic);

			Objects::Entity ent;
			ent.SetID(object.GetID());
			ent.SetType(object.GetType());

			g_Objects.push_back(object);

			RakNet::BitStream bitstream;
			bitstream.Write(object.GetID());
			bitstream.Write(model);
			bitstream.Write(position.x);
			bitstream.Write(position.y);
			bitstream.Write(position.z);
			bitstream.Write(rotation.x);
			bitstream.Write(rotation.y);
			bitstream.Write(rotation.z);
			bitstream.Write(dynamic);

			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("CreateObject", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);

			ModelCache::AddModel(model);

			return ent;
		}

		const int GetTextureVariation(Objects::Entity entity)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						return g_Objects[i].GetTextureVariantion();
					}
				}
				break;
			default:
				LOG_WARNING("[%sGetTextureVariation] Entity %i is not of type Object.", ThisNamespace, entity.GetID());
				return -1;
				break;
			}
		}

		void SetTextureVariation(Objects::Entity entity, const int textureindex)
		{
			switch (entity.GetType())
			{
			case GrandM::EntityType::Object:
				for (unsigned int i = 0; i < g_Objects.size(); i++)
				{
					if (g_Objects[i] == entity)
					{
						g_Objects[i].SetTextureVariation(textureindex);

						RakNet::BitStream bitstream;
						bitstream.Write(entity.GetID());
						bitstream.Write(textureindex);
						Server::GetServer()->GetNetworkManager()->GetRPC().Signal("SetTextureVariation", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
						return;
					}
				}
				break;
			default:
				LOG_WARNING("[%sSetTextureVariation] Entity %i is not of type Object.", ThisNamespace, entity.GetID());
				break;
			}
		}
	}
}
