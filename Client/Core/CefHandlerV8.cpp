// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

// Vendor.CEF
#include <include/cef_app.h>


#include "APIStructs.h"
#include "GUI.h"
#include "CGui.h"
#include "CNetworkManager.h"

#include "CLocalPlayer.h"
#include "CConfig.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CefHandlerV8.h"

CefHandlerV8::CefHandlerV8(CefRefPtr<CefApp> app)
{
	this->app = app;
}

bool CefHandlerV8::Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception)
{
	if (name == "PageLoaded")
	{
		/*RakNet::BitStream bitstream;
		CNetworkManager::GetRPC().Signal("OnCefFinishLoad", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);*/
		return true;
	}
	else if (name == "SendData")
	{
		if ((arguments.size() == 1) && arguments[0]->IsString())
		{
			/*std::string text = arguments[0]->GetStringValue();

			RakNet::BitStream bitstream;
			bitstream.Write(RakNet::RakString(text.c_str()));
			CNetworkManager::GetRPC().Signal("OnCefSendData", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);*/
			return true;
		}
	}
	else if (name == "SendChatMessage")
	{
		if ((arguments.size() == 1) && arguments[0]->IsString())
		{
			/*std::string text = arguments[0]->GetStringValue();

			RakNet::BitStream bitstream;
			bitstream.Write((unsigned char)ID_CHAT_MESSAGE);
			bitstream.Write(RakNet::RakString(text.c_str()));
			CNetworkManager::GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false);*/
			return true;
		}
	}
	else if (name == "ShowCursor")
	{
		if ((arguments.size() == 1) && arguments[0]->IsBool()) 
		{
			/*CGui::GiveFocus(arguments[0]->GetBoolValue());
			CLocalPlayer::SetControllable(!arguments[0]->GetBoolValue());*/
			return true;
		}
	}
	else if (name == "SetOption")
	{
		if ((arguments.size() == 2) && arguments[0]->IsString())
		{
			/*if (arguments[1]->IsString())
			{
				if(arguments[0]->GetStringValue().compare("name") == 0)
					CConfig::SetName(arguments[1]->GetStringValue().ToString());
				else if (arguments[0]->GetStringValue().compare("ip") == 0)
					CConfig::SetIp(arguments[1]->GetStringValue().ToString());
			}
			else if (arguments[1]->IsBool())
			{
				if (arguments[0]->GetStringValue().compare("debugui") == 0)
					CConfig::SetDebugUI(arguments[1]->GetBoolValue());
				else if (arguments[0]->GetStringValue().compare("editor") == 0)
					CConfig::SetEditor(arguments[1]->GetBoolValue());
				else if (arguments[0]->GetStringValue().compare("chatvisible") == 0)
					CConfig::SetChatVisible(arguments[1]->GetBoolValue());
			}
			else if (arguments[1]->IsInt())
			{
				if (arguments[0]->GetStringValue().compare("port") == 0)
					CConfig::SetConPort(arguments[1]->GetIntValue());
				else  if (arguments[0]->GetStringValue().compare("chatsize") == 0)
					CConfig::SetChatSize(arguments[1]->GetIntValue());
				else  if (arguments[0]->GetStringValue().compare("chatmessagecount") == 0)
					CConfig::SetVisibleMessageCount(arguments[1]->GetIntValue());
			}

			CConfig::Save();
			*/
			return true;
		}
	}
	else if (name == "GetOption")
	{
		if ((arguments.size() == 1) && arguments[0]->IsString())
		{
			if (arguments[0]->GetStringValue().compare("name") == 0)
			{
				retval = CefV8Value::CreateString(CefString(CConfig::GetName().c_str()));
			}
			else if (arguments[0]->GetStringValue().compare("ip") == 0)
			{
				retval = CefV8Value::CreateString(CefString(CConfig::GetIp().c_str()));
			}
			else if (arguments[0]->GetStringValue().compare("port") == 0)
			{
				retval = CefV8Value::CreateInt(CConfig::GetPort());
			}
			else if (arguments[0]->GetStringValue().compare("debugui") == 0)
			{
				retval = CefV8Value::CreateBool(CConfig::IsDebugUI());
			}
			else if (arguments[0]->GetStringValue().compare("debuglog") == 0)
			{
				retval = CefV8Value::CreateBool(CConfig::IsDebugLog());
			}
			else if (arguments[0]->GetStringValue().compare("editor") == 0)
			{
				retval = CefV8Value::CreateBool(CConfig::GetEditor());
			}
			else if (arguments[0]->GetStringValue().compare("chatvisible") == 0)
			{
				retval = CefV8Value::CreateBool(CConfig::IsChatVisible());
			}
			else if (arguments[0]->GetStringValue().compare("chatsize") == 0)
			{
				retval = CefV8Value::CreateInt(CConfig::GetChatSize());
			}
			else if (arguments[0]->GetStringValue().compare("chatmessagecount") == 0)
			{
				retval = CefV8Value::CreateInt(CConfig::GetVisibleMessageCount());
			}

			return true;
		}
	}
	else if (name == "Connect")
	{
		if ((arguments.size() == 3) && arguments[1]->IsInt())
		{
			/*CNetworkManager::Connect(arguments[0]->GetStringValue().ToString().c_str(), arguments[2]->GetStringValue().ToString().c_str(), arguments[1]->GetIntValue());*/
			return true;
		}
	}
	else if (name == "Disconnect")
	{
		if ((arguments.size() == 0))
		{
			// We can't flat out call a disconnect from another thread as scripthook functions can only be called via the main tread
			// Thus we need to trigger the main thread to call a disconnect.
			//CNetworkManager::Disconnect();

			//CNetworkManager::triggerDisconnect = true;

			return true;
		}
	}
	else if (name == "Retry")
	{
//		CNetworkManager::Connect(CNetworkManager::GetLastIP().c_str(), CNetworkManager::GetLastPass().c_str(), CNetworkManager::GetLastPort());
		return true;
	}
	else if (name == "Quit")
	{
		if ((arguments.size() == 0))
		{
			//CNetworkManager::Stop();
			//exit(EXIT_SUCCESS);
			//LOG_ERROR("[V8] Grand.Quit");
			return true;
		}
	}
	else if (name == "ShowGameSettings")
	{
		if ((arguments.size() == 0))
		{
			/*UI::SET_FRONTEND_ACTIVE(false);
			UI::ACTIVATE_FRONTEND_MENU($("FE_MENU_VERSION_SP_PAUSE"), false, -1);
			
			CGui::ShowMainMenu(false, true);
			CGui::GiveFocus(false);*/

			return true;
		}
	}
	else if (name == "RockstarEditor")
	{
		if ((arguments.size() == 0))
		{
			//RENDERING::_ACTIVATE_ROCKSTAR_EDITOR();

			return true;
		}
	}
	else if (name == "Ping")
	{
		if ((arguments.size() == 2))
		{
			//CNetworkManager::GetInterface()->Ping(arguments[0]->GetStringValue().ToString().c_str(), arguments[1]->GetIntValue(), false);
		}
	}

	return false;
}
