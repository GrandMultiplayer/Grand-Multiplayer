// RakNet
#include <RakNetTypes.h>
#include <BitStream.h>

// Self
#include "RPCManager.h"

//GrandM
#include "Server.h"

#include "SharedUtility.h"

#include "RPCEntity.h"
#include "RPCPlayer.h"
#include "RPCVehicle.h"

#include "SyncManager.h"

namespace RPC
{
	namespace Manager
	{
		RPCMessage rpcmessages[] = {
		#pragma region Entity
			{ "SetPhysicsAssignment", RPC::Entity::SetPhysicsAssignment },
			{ "OnEntityDamage", RPC::Entity::OnEntityDamage },
			{ "OnProjectileImpact", RPC::Entity::OnProjectileImpact },
			{ "SetPedHeadBlend", RPC::Entity::SetPedHeadBlend },
			{ "SetPedProp", RPC::Entity::SetPedProp },
		#pragma endregion
		#pragma region Checkpoint
			{ "OnPlayerEnterCheckpoint", RPC::Entity::OnEntityEnterCheckpoint },
			{ "OnPlayerExitCheckpoint", RPC::Entity::OnEntityExitCheckpoint },
		#pragma endregion
		#pragma region Cef
			{ "OnCefFinishLoad", RPC::Entity::OnCefFinishLoad },
			{ "OnCefSendData", RPC::Entity::OnCefSendData },
		#pragma endregion
		#pragma region Player
			{ "InvalidWeapon", RPC::Player::InvalidWeapon },
			{ "UpdateWeapon", RPC::Player::UpdateWeapon },
			{ "OnPlayerShot", RPC::Player::OnPlayerShot },
			{ "OnPlayerAim", RPC::Player::OnPlayerAim },
			{ "OnPlayerPickup", RPC::Player::OnPlayerPickup },
		#pragma endregion
		#pragma region Vehicle
			{ "SetExtra", RPC::Vehicle::SetExtra },
			{ "VehicleInited", RPC::Vehicle::Inited },
			{ "TowVehicle", RPC::Vehicle::TowVehicle },
			{ "DeployCargobobRope", RPC::Vehicle::DeployCargobobRope }
		#pragma endregion
		};

		void RegisterRPCMessages()
		{
			for (unsigned int i = 0; i < SizeOfArray(rpcmessages); i++)
			{
				Server::GetServer()->GetNetworkManager()->GetRPC().RegisterSlot(rpcmessages[i].name, rpcmessages[i].functionPointer, 0);
			}

			SyncManager::Init();
		}

		void UnregisterRPCMessages()
		{
			for (unsigned int i = 0; i < SizeOfArray(rpcmessages); i++)
			{
				Server::GetServer()->GetNetworkManager()->GetRPC().UnregisterSlot(rpcmessages[i].name);
			}
		}
	}
}