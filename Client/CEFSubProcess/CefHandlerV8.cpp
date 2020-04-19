#include "CefHandlerV8.h"

// Vendor.CEF
#include <include/cef_app.h>
#include <include/base/cef_logging.h>

#include "ClientAppRenderer.h"

bool CefHandlerV8::Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception)
{
	if (name == "PageLoaded")
	{
		// Create the message object.
		CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.PageLoaded");

		// Retrieve the argument list object.
		CefRefPtr<CefListValue> args = msg->GetArgumentList();

		// Send the process message to the render process.
		// Use PID_BROWSER instead when sending a message to the browser process.
		ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);

		return true;
	}
	else if (name == "SendData")
	{
		if ((arguments.size() == 1) && arguments[0]->IsString())
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.SendData");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			args->SetString(0, arguments[0]->GetStringValue().ToString().c_str());

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);
			
			return true;
		}
	}
	else if (name == "SendChatMessage")
	{
		if ((arguments.size() == 1) && arguments[0]->IsString())
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.SendChatMessage");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			args->SetString(0, arguments[0]->GetStringValue());

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);
			
			return true;
		}
	}
	else if (name == "ShowCursor")
	{
		if ((arguments.size() == 1) && arguments[0]->IsBool()) 
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.ShowCursor");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			args->SetBool(0, arguments[0]->GetBoolValue());

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);

			return true;
		}
	}
	else if (name == "SetOption")
	{
		if ((arguments.size() == 2) && arguments[0]->IsString())
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.SetOption");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			args->SetString(0, arguments[0]->GetStringValue().c_str());

			if (arguments[1]->IsString())
			{
				args->SetString(1, arguments[1]->GetStringValue().c_str());
			}
			else if (arguments[1]->IsBool())
			{
				args->SetBool(1, arguments[1]->GetBoolValue());
			}
			else if (arguments[1]->IsInt())
			{
				args->SetInt(1, arguments[1]->GetIntValue());
			}

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);

			return true;
		}
	}
	else if (name == "GetOption")
	{
		if ((arguments.size() == 1) && arguments[0]->IsString())
		{
			/*if (arguments[0]->GetStringValue().compare("name") == 0)
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
			}*/

			return true;
		}
	}
	else if (name == "Connect")
	{
		if ((arguments.size() == 3) && arguments[1]->IsInt())
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.Connect");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			args->SetString(0, arguments[0]->GetStringValue().ToString().c_str());
			args->SetString(1, arguments[2]->GetStringValue().ToString().c_str());
			args->SetInt(2, arguments[1]->GetIntValue());

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);
			
			return true;
		}
	}
	else if (name == "Disconnect")
	{
		if ((arguments.size() == 0))
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.Disconnect");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);

			return true;
		}
	}
	else if (name == "Retry")
	{
		if ((arguments.size() == 0))
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.Retry");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);

			return true;
		}
	}
	else if (name == "Quit")
	{
		if ((arguments.size() == 0))
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.Quit");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_BROWSER, msg);
			
			return true;
		}
	}
	else if (name == "ShowGameSettings")
	{
		if ((arguments.size() == 0))
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.ShowGameSettings");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);

			return true;
		}
	}
	else if (name == "RockstarEditor")
	{
		if ((arguments.size() == 0))
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.RockstarEditor");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);

			return true;
		}
	}
	else if (name == "Ping")
	{
		if ((arguments.size() == 2))
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("GrandM.Ping");

			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			args->SetString(0, arguments[0]->GetStringValue().ToString().c_str());
			args->SetInt(1, arguments[1]->GetIntValue());

			ClientAppRenderer::GetBrowser()->SendProcessMessage(PID_RENDERER, msg);
		}
	}

	return false;
}
