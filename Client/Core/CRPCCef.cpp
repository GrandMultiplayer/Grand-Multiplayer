#include <vector>
#include <iostream>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>

#include "CefMainContext.h"
#include "CGui.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CRPCCef.h"

void CRPCCef::LoadURL(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCCef::LoadURL.");

	RakNet::RakString frame, url, appcode;
	bool remote;

	bitStream->Read(frame);
	bitStream->Read(url);
	bitStream->Read(appcode);
	bitStream->Read(remote);

	CGui::LoadFrame(frame.C_String(), url.C_String(), remote);
}

void CRPCCef::ExecuteJavaScript(RakNet::BitStream * bitStream, RakNet::Packet * packet)
{
	LOG_DEBUG("CRPCCef::ExecuteJavaScript.");

	RakNet::RakString frame, call;

	bitStream->Read(frame);
	bitStream->Read(call);

	CefRefPtr<CefFrame> cefFrame = CefMainContext::getBrowser()->GetFrame(CefString(frame.C_String()));
	cefFrame->ExecuteJavaScript(CefString(call), cefFrame->GetURL(), 0);
}
