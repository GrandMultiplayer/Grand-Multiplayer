#include <string>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/types.h"

// GrandM
#include "APIStructs.h"
#include "GUI.h"
#include "CLocalPlayer.h"
#include "CNetworkManager.h"

#include "NetworkCallbacks.h"

namespace GrandM
{
	namespace Network
	{
		namespace Callback
		{
			void OnProjectileImpact(std::string weapon, Vector3 impact, const int entity, const int bone)
			{
				RakNet::BitStream bitstream;
				bitstream.Write(CLocalPlayer::GetID());
				bitstream.Write(RakNet::RakString(weapon.c_str()));
				bitstream.Write(impact.x);
				bitstream.Write(impact.y);
				bitstream.Write(impact.z);
				bitstream.Write(entity);
				bitstream.Write(bone);

				CNetworkManager::GetRPC().Signal("OnProjectileImpact", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
			}

			void OnEntityDamage(const int entity, const int damage, const int attacker, std::string weapon)
			{
				RakNet::BitStream bitstream;
				bitstream.Write(entity);
				bitstream.Write(damage);
				bitstream.Write(attacker);
				bitstream.Write(RakNet::RakString(weapon.c_str()));

				CNetworkManager::GetRPC().Signal("OnEntityDamage", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
			}
		}
	}
}