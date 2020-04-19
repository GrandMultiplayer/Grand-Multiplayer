// Vendor.RakNet
//#include <RakNet/RakPeerInterface.h>
//#include <RakNet/BitStream.h>
//#include <RakNet/MessageIdentifiers.h>
//#include <RakNet/RPC4Plugin.h>
//#include <RakNet/DirectoryDeltaTransfer.h>
//#include <RakNet/FileListTransfer.h>
//#include <RakNet/IncrementalReadInterface.h>
//#include <RakNet/FileListTransferCBInterface.h>

// GrandM
#include "SharedUtility.h"
#include "CNetworkManager.h"

// RPC Handlers
#include "CRPCVisual.h"
#include "CRPCEntity.h"
#include "CRPCWorld.h"
#include "CRPCPlayer.h"
#include "CRPCObject.h"
#include "CRPCNpc.h"
#include "CRPCCheckpoint.h"
#include "CRPCVehicle.h"
#include "CRPCNetwork.h"
#include "CRPCCef.h"
#include "CRPCBlip.h"
#include "CRPCPickup.h"
#include "CRPCCamera.h"

// Self
#include "CRPCManager.h"

CRPCManager::RPCMessage rpcmessages[] = {
#pragma region Visual
	{ "ShowMessageAboveMap", CRPCVisual::ShowMessageAboveMap },
	{ "ShowSubtitle", CRPCVisual::ShowSubtitle },
	{ "SendMessage", CRPCVisual::SendChatMessage },
	{ "ShowCursor", CRPCVisual::ShowCursor},
	{ "DisplayHud", CRPCVisual::DisplayHud},
	{ "DisplayMinimap", CRPCVisual::DisplayMinimap },
	{ "DisplayCash", CRPCVisual::DisplayCash },
	{ "DisplayAmmo", CRPCVisual::DisplayAmmo },
#pragma endregion
#pragma region World
	{ "SetTime", CRPCWorld::SetTime },
	{ "SetWeather", CRPCWorld::SetWeather },
	{ "LoadIPL", CRPCWorld::LoadIPL },
	{ "UnloadIPL", CRPCWorld::UnloadIPL },
	{ "LoadStringModel", CRPCWorld::LoadStringModel },
	{ "LoadHashModel", CRPCWorld::LoadHashModel },
	{ "SetGroundSnow", CRPCWorld::SetGroundSnow },
	{ "SetBlackout", CRPCWorld::SetBlackout },
#pragma endregion
#pragma region Entity
	{ "DestroyEntity", CRPCEntity::Destroy },
	{ "SetPosition", CRPCEntity::SetPosition },
	{ "SetRotation", CRPCEntity::SetRotation },
	{ "SetPhysicsAssignment", CRPCEntity::SetPhysicsAssignment },
	{ "PedComponent", CRPCEntity::PedComponent },
	{ "PedHeadBlend", CRPCEntity::PedHeadBlend },
	{ "PedHeadOverlay", CRPCEntity::PedHeadOverlay },
	{ "PedProp", CRPCEntity::PedProp },
	{ "PedFaceFeature", CRPCEntity::PedFaceFeature },
	{ "SetViewDistance", CRPCEntity::SetViewDistance },
	{ "SetNametag", CRPCEntity::SetNametag },
	{ "SetNametagVisible", CRPCEntity::SetNametagVisible },
	{ "SetHealthbarVisible", CRPCEntity::SetHealthbarVisible },
	{ "SetNametagDistance", CRPCEntity::SetNametagDistance },
	{ "SetNametagColor", CRPCEntity::SetNametagColor },
	{ "SetHealth", CRPCEntity::SetHealth },
	{ "SetArmour", CRPCEntity::SetArmour },
	{ "Revive", CRPCEntity::Revive },
	{ "SetDimension", CRPCEntity::SetDimension },
#pragma endregion
#pragma region Player
	{ "PlayerModel", CRPCPlayer::PlayerModel },
	{ "SetControllable", CRPCPlayer::SetControllable },
	{ "Kick", CRPCPlayer::Kick },
	{ "WrongVersion", CRPCPlayer::WrongVersion },
	{ "PutInVehicle", CRPCPlayer::PutInVehicle },
	{ "GiveWeapon", CRPCPlayer::GiveWeapon },
	{ "RemoveWeapon", CRPCPlayer::RemoveWeapon },
	{ "OnPlayerShot", CRPCPlayer::OnPlayerShot },
	{ "OnPlayerAim", CRPCPlayer::OnPlayerAim },
	{ "SetWeaponAmmo", CRPCPlayer::SetWeaponAmmo },
	{ "EquipWeapon", CRPCPlayer::EquipWeapon },
#pragma endregion
#pragma region Object
	{ "CreateObject", CRPCObject::Create },
	{ "SetTextureVariation", CRPCObject::SetTextureVariation },
#pragma endregion
#pragma region Npc
	{ "CreateNPC", CRPCNpc::Create },
#pragma endregion
#pragma region Checkpoint
	{ "CreateCheckpoint", CRPCCheckpoint::Create  },
	{ "ShowCheckpoint", CRPCCheckpoint::Show },
	{ "HideCheckpoint", CRPCCheckpoint::Hide },
	{ "SetCheckpointHeight", CRPCCheckpoint::SetHeight },
	{ "CheckpointPointTo", CRPCCheckpoint::PointTo },
	{ "SetCheckpointType", CRPCCheckpoint::SetType },
	{ "SetCheckpointColor", CRPCCheckpoint::SetColor },
#pragma endregion
#pragma region Vehicle
	{ "InitVehicle", CRPCVehicle::InitVehicle },
	{ "SetStandardColor", CRPCVehicle::SetStandardColor},
	{ "SetCustomColor", CRPCVehicle::SetCustomColor },
	{ "SetNumberPlate", CRPCVehicle::SetNumberPlate },
	{ "SetMod", CRPCVehicle::SetMod },
	{ "SetDoorsLockState", CRPCVehicle::SetDoorsLockState },
	{ "SetNumberPlateStyle", CRPCVehicle::SetNumberPlateStyle },
	{ "SetExtra", CRPCVehicle::SetExtra },
	{ "FixDeformation", CRPCVehicle::FixDeformation },
	{ "TowVehicle", CRPCVehicle::TowVehicle },
	{ "DeployCargobobRope", CRPCVehicle::DeployCargobobRope },
#pragma endregion
#pragma region Network
	{ "FinishedSync", CRPCNetwork::FinishedSync },
#pragma endregion
#pragma region CEF
	{ "LoadURL", CRPCCef::LoadURL },
	{ "ExecuteJavaScript", CRPCCef::ExecuteJavaScript },
#pragma endregion
#pragma region Blip
	{ "CreateBlipE", CRPCBlip::CreateE },
	{ "CreateBlipPos", CRPCBlip::CreateP },
	{ "SetBlipColor", CRPCBlip::SetColor },
	{ "SetBlipSprite", CRPCBlip::SetSprite },
	{ "ShowBlip", CRPCBlip::Show },
	{ "HideBlip", CRPCBlip::Hide },
	{ "SetBlipText", CRPCBlip::SetText },
	{ "SetBlipShortRange", CRPCBlip::SetBlipShortRange },
#pragma endregion
#pragma region Pickup
	{ "CreatePickup", CRPCPickup::Create },
	{ "SetPickupVisible", CRPCPickup::SetPickupVisible },
	{ "SetPickupModel", CRPCPickup::SetPickupModel },
	{ "SetPickupRespawnDelay", CRPCPickup::SetPickupRespawnDelay },
#pragma endregion
#pragma region Camera
	{ "SetCameraPosition", CRPCCamera::SetCameraPosition },
	{ "SetCameraRotation", CRPCCamera::SetCameraRotation },
	{ "ResetCamera", CRPCCamera::ResetCamera },
	{ "FirstPersonOnly", CRPCCamera::FirstPersonOnly },
	{ "ThirdPersonOnly", CRPCCamera::ThirdPersonOnly },
	{ "AttachCamera", CRPCCamera::AttachCamera },
	{ "DetachCamera", CRPCCamera::DetachCamera }
#pragma endregion
};

void CRPCManager::RegisterRPCMessages()
{
	for (int i = 0; i < SizeOfArray(rpcmessages); i++)
	{
		CNetworkManager::GetRPC().RegisterSlot(rpcmessages[i].name, rpcmessages[i].functionPointer, 0);
	}
}

void CRPCManager::UnregisterRPCMessages()
{
	for (int i = 0; i < SizeOfArray(rpcmessages); i++)
	{
		CNetworkManager::GetRPC().UnregisterSlot(rpcmessages[i].name);
	}
}
