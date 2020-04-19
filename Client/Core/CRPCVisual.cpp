#include <iostream>
#include <sstream>

// RakNet
#include <BitStream.h>

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>

#include "hook/enums.h"
#include "hook/types.h"

#include "APIStructs.h"
#include "GUI.h"
#include "CVisual.h"
#include "CefMainContext.h"
#include "CLocalPlayer.h"
#include "CGui.h"

#include "hook/natives.h"

  // Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

  // Self
#include "CRPCVisual.h"

void CRPCVisual::ShowMessageAboveMap(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVisual::ShowMessageAboveMap.");

	int iconType, color;
	RakNet::RakString message, pic, sender, subject;

	bitStream->Read(message);
	bitStream->Read(pic);
	bitStream->Read(iconType);
	bitStream->Read(color);
	bitStream->Read(sender);
	bitStream->Read(subject);

	CVisual::ShowMessageAboveMap(message.C_String(), pic.C_String(), iconType, color, sender.C_String(), subject.C_String());
}

void CRPCVisual::ShowSubtitle(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCVisual::ShowSubtitle.");

	RakNet::RakString message;
	int duration;
	bool shownow;

	bitStream->Read(message);
	bitStream->Read(duration);
	bitStream->Read(shownow);

	CVisual::ShowSubtitle(message.C_String(), duration, shownow);
}

void CRPCVisual::SendChatMessage(RakNet::BitStream *bitStream, RakNet::Packet *packet)
{
	LOG_DEBUG("CRPCVisual::SendChatMessage.");

	RakNet::RakString message;

	bitStream->Read(message);

	CGui::ShowChatMessage(message.C_String());
}

void CRPCVisual::ShowCursor(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCVisual::ShowCursor.");

	bool show;

	bitStream->Read(show);

	CGui::GiveFocus(show);
}

void CRPCVisual::DisplayHud(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCVisual::DisplayHud.");

	bool show;

	bitStream->Read(show);

	UI::DISPLAY_HUD(show);
}

void CRPCVisual::DisplayMinimap(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCVisual::DisplayMinimap.");

	bool show;

	bitStream->Read(show);

	UI::DISPLAY_RADAR(show);
}

void CRPCVisual::DisplayCash(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCVisual::DisplayCash.");

	bool show;

	bitStream->Read(show);

	UI::DISPLAY_CASH(show);
}

void CRPCVisual::DisplayAmmo(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCVisual::DisplayAmmo.");

	bool state;

	bitStream->Read(state);

	CLocalPlayer::DisplayAmmo(state);
}