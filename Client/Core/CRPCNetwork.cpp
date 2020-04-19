// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

#include "hook/enums.h"
#include "hook/types.h"

#include "Common.h"
#include "APIStructs.h"
#include "GUI.h"
#include "CGui.h"
#include "CNetworkManager.h"

#include "CLocalPlayer.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCNetwork.h"

void CRPCNetwork::FinishedSync(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("[RPC] Network>>FinishedSync.");

	int entity;
	RakNet::RakString serverName, serverWebsite;

	bitStream->Read(entity);
	bitStream->Read(serverName);
	bitStream->Read(serverWebsite);

	CLocalPlayer::SetID(entity);

	CGui::ShowChat(true, true);

	// No Longer Needed
	ENTITY::FREEZE_ENTITY_POSITION(CLocalPlayer::GetPed(), false);
	ENTITY::SET_ENTITY_VISIBLE(CLocalPlayer::GetPed(), true, 0);
	ENTITY::SET_ENTITY_ALPHA(CLocalPlayer::GetPed(), 0, false);
	ENTITY::RESET_ENTITY_ALPHA(CLocalPlayer::GetPed());

	UI::DISPLAY_RADAR(true);
	UI::DISPLAY_HUD(true);
	UI::DISPLAY_CASH(true);

	// Set our state to connected
	CNetworkManager::g_ConnectionState = CNetworkManager::Connected;

	CLocalPlayer::SetControllable(true);
	CGui::GiveFocus(false);
}