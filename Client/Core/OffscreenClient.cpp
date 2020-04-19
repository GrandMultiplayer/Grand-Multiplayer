// NEEDS TO BE FIRST ELSE WE GET WINSOCK ERRORS
#include <winsock2.h>

#include "OffscreenClient.h"

//#include <Windows.h>
//#include <mutex>
//#include <list>

// Vendor.CEF
//#include <include/cef_app.h>
//#include <include/cef_client.h>
//#include <include/cef_render_handler.h>
//#include <include/cef_sandbox_win.h>
//#include <include/wrapper/cef_helpers.h>

// Vendor.RakNet
#include <MessageIdentifiers.h>

// Hooking
#include "hook/enums.h"
#include "hook/natives.h"

// GrandM
#include "CefMainContext.h"
#include "Functions.h"
#include "CefTexture.h"
#include "CLocalPlayer.h"
#include "CGui.h"
#include "CConfig.h"
#include "CNetworkManager.h"

// Discord
#include "discord_rpc.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

OffscreenClient::OffscreenClient()
{
}

OffscreenClient::~OffscreenClient()
{
}

void OffscreenClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (!m_browser.get())
	{
		m_browser = browser;
	}
	else
	{
		LOG_WARNING("[OffscreenClient] Another browser was attempted to be created");
	}
}

void OffscreenClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	//AutoLock lock_scope(this);
	m_browser = NULL;
}

bool OffscreenClient::DoClose(CefRefPtr<CefBrowser> browser)
{
	LOG_WARNING("[OffscreenClient] DoClose");
	return false;
}

void OffscreenClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{
	//LOG_DEBUG("[OffscreenClient::OnPaint] painting type %i at %i x %i", type, width, height);

	if (type != PaintElementType::PET_VIEW)
		return;

	//LOG_DEBUG("[CEF::OnPaint] %i x %i", width, height);
	//LOG_DEBUG("[CEF::OnPaint] %i x %i", dirtyRects[0].width, dirtyRects[0].height);

	//RECT rect;
	//GetWindowRect(GetDesktopWindow(), &rect);

	//LOG_DEBUG("[CEF::OnPaint] %i x %i", rect.right, rect.bottom);
	/*
	if ((width == rect.right && height == rect.bottom) || (width == 1914 && height == 1052))
	{
		width -= 1;
		height -= 1;
	}
	else if (width == 1123 && height == 622)
	{
		width = 1122;
		height = 622;
	}*/

	std::lock_guard<std::mutex> lock(CefTexture::paintMutex);
	CefTexture::drawData.push_back(std::make_unique<DrawData>(width, height, (const unsigned *)buffer, dirtyRects));
	//CefTexture::drawData.push_back(std::make_unique<DrawData>(rect.right, rect.bottom, (const unsigned *)buffer, dirtyRects));

	// Stops CEF eating all the ram because of game freezes by only ever allowing at max 60 frames to be stored.
	// CEF currently runs at 30fps
	if (CefTexture::drawData.size() > 60)
		CefTexture::drawData.pop_front();
}

bool OffscreenClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	RECT dRect;
	GetClientRect(FindWindowA(NULL, "Grand Theft Auto V"), &dRect);

	float w = dRect.right - dRect.left;
	float h = dRect.bottom - dRect.top;

	RECT iRect, wRect;
	GetWindowRect(GetDesktopWindow(), &iRect);
	GetWindowRect(FindWindowA(NULL, "Grand Theft Auto V"), &wRect);

	if(w <= (iRect.right - iRect.left) || h <= (iRect.bottom - iRect.top))
	{
		if ((iRect.right - iRect.left) == (wRect.right - wRect.left))
			w = (iRect.right - iRect.left);

		if ((iRect.bottom - iRect.top) == (wRect.bottom - wRect.top))
			h = (iRect.bottom - iRect.top);
	}

	//LOG_DEBUG("CEF::GetViewRect %i x %i", w, h);

	rect.Set(0, 0, w, h);
	//rect.Set(0, 0, 2560, 1440);
	return true;
}

bool OffscreenClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	// Check the message name.
	const std::string& message_name = message->GetName();
	const CefRefPtr<CefListValue> args = message->GetArgumentList();

	if (message_name == "GrandM.PageLoaded")
	{
		RakNet::BitStream bitstream;
		CNetworkManager::GetRPC().Signal("OnCefFinishLoad", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
	}
	else if (message_name == "GrandM.SendData")
	{
		RakNet::BitStream bitstream;
		bitstream.Write(RakNet::RakString(args->GetString(0).ToString().c_str()));
		CNetworkManager::GetRPC().Signal("OnCefSendData", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
	}
	else if (message_name == "GrandM.SendChatMessage")
	{
		RakNet::BitStream bitstream;
		bitstream.Write((unsigned char)ID_CHAT_MESSAGE);
		bitstream.Write(RakNet::RakWString(args->GetString(0).ToWString().c_str()));
		CNetworkManager::GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false);
	}
	else if (message_name == "GrandM.ShowCursor")
	{
		CGui::GiveFocus(args->GetBool(0));
		CLocalPlayer::SetControllable(!args->GetBool(0));
	}
	else if (message_name == "GrandM.SetOption")
	{
		if (args->GetString(0).ToString().compare("name") == 0)
			CConfig::SetName(args->GetString(1).ToString());
		else if (args->GetString(0).ToString().compare("ip") == 0)
			CConfig::SetIp(args->GetString(1).ToString());
		else if (args->GetString(0).ToString().compare("debugui") == 0)
			CConfig::SetDebugUI(args->GetBool(1));
		else if (args->GetString(0).ToString().compare("editor") == 0)
			CConfig::SetEditor(args->GetBool(1));
		else if (args->GetString(0).ToString().compare("chatvisible") == 0)
			CConfig::SetChatVisible(args->GetBool(1));
		else if (args->GetString(0).ToString().compare("port") == 0)
			CConfig::SetConPort(args->GetInt(1));
		else  if (args->GetString(0).ToString().compare("chatsize") == 0)
			CConfig::SetChatSize(args->GetInt(1));
		else  if (args->GetString(0).ToString().compare("chatmessagecount") == 0)
			CConfig::SetVisibleMessageCount(args->GetInt(1));

		CConfig::Save();
	}
	else if (message_name == "GrandM.GetOption")
	{
		// SOMEHOW WE NEED TO SEND THIS BACK??? OR USE JS TO SET THE VALUES PROBS EASYIER

		CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

		if (frame != nullptr && frame->IsValid())
		{
			// TEMP SETUP... NO ONE OTHER THAN THE CLIENT FOR NOW SHOULD BE USING THIS AS IT DOES NOT WORK AS INTENDED
			if (args->GetString(0).ToString().compare("name") == 0)
			{
				std::string code = "$('#s-nickname').val(" + CConfig::GetName() + "));";

				frame->ExecuteJavaScript(CefString(code.c_str()), frame->GetURL(), 0);
			}
			else if (args->GetString(0).ToString().compare("ip") == 0)
			{
				std::string code = "$('#s-nickname').val(" + CConfig::GetName() + "));";

				frame->ExecuteJavaScript(CefString(code.c_str()), frame->GetURL(), 0);
			}
			else if (args->GetString(0).ToString().compare("debugui") == 0)
				CConfig::SetDebugUI(args->GetBool(1));
			else if (args->GetString(0).ToString().compare("editor") == 0)
				CConfig::SetEditor(args->GetBool(1));
			else if (args->GetString(0).ToString().compare("chatvisible") == 0)
				CConfig::SetChatVisible(args->GetBool(1));
			else if (args->GetString(0).ToString().compare("port") == 0)
				CConfig::SetConPort(args->GetInt(1));
			else  if (args->GetString(0).ToString().compare("chatsize") == 0)
				CConfig::SetChatSize(args->GetInt(1));
			else  if (args->GetString(0).ToString().compare("chatmessagecount") == 0)
				CConfig::SetVisibleMessageCount(args->GetInt(1));


			
			
		}

		"$('#s-nickname').val(GrandM.GetOption('name'));"
		""
		"$('#s-ip').val(GrandM.GetOption('ip'));"
		""
		"$('#dc_ip').val(GrandM.GetOption('ip'));"
		""
		"$('#s-port').val(GrandM.GetOption('port'));"
		""
		"$('#dc_port').val(GrandM.GetOption('port'));"
		""
		"$('#s-chat').prop('checked', GrandM.GetOption('chatvisible')).change();"
		""
		"$('#s-debug').prop('checked', GrandM.GetOption('debugui')).change();"
		""
		"$('#s-editor').prop('checked', GrandM.GetOption('editor')).change();"
		""
		"slider.setValue(GrandM.GetOption('chatmessagecount'), true, true);"
		""
		"$('#s-chatsize').selectpicker('val', GrandM.GetOption('chatsize'));";
	}
	else if (message_name == "GrandM.Connect")
	{
		CNetworkManager::Connect(args->GetString(0).ToString().c_str(), args->GetString(1).ToString().c_str(), args->GetInt(2));
	}
	else if (message_name == "GrandM.Disconnect")
	{
		CNetworkManager::triggerDisconnect = true;
	}
	else if (message_name == "GrandM.Retry")
	{
		CNetworkManager::Connect(CNetworkManager::GetLastIP().c_str(), CNetworkManager::GetLastPass().c_str(), CNetworkManager::GetLastPort());
	}
	else if (message_name == "GrandM.ShowGameSettings")
	{
		UI::SET_FRONTEND_ACTIVE(false);
		UI::ACTIVATE_FRONTEND_MENU($("FE_MENU_VERSION_SP_PAUSE"), false, -1);

		CGui::ShowMainMenu(false, true);
		CGui::GiveFocus(false);
	}
	else if (message_name == "GrandM.RockstarEditor")
	{
		RENDERING::_ACTIVATE_ROCKSTAR_EDITOR();
	}
	else if (message_name == "GrandM.Ping")
	{
		CNetworkManager::GetInterface()->Ping(args->GetString(0).ToString().c_str(), args->GetInt(1), false);
	}
	else if (message_name == "GrandM.Quit")
	{
		Discord_Shutdown();
		CNetworkManager::Stop();
		CefShutdown();
		exit(EXIT_SUCCESS);
		return true;
	}

	return false;
}