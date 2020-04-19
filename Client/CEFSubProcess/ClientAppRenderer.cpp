#include "ClientAppRenderer.h"

#include "include/base/cef_logging.h"

#include "CefHandlerV8.h"

CefRefPtr<CefBrowser> ClientAppRenderer::m_browser = nullptr;

ClientAppRenderer::ClientAppRenderer()
{
	CreateDelegates(delegates_);
}

void ClientAppRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnRenderThreadCreated(this, extra_info);
}

void ClientAppRenderer::OnWebKitInitialized()
{
	std::string app_code =
		"var GrandM;"
		"if (!GrandM)"
		"    GrandM = {};"
		"(function() {"
		"    GrandM.PageLoaded = function() {"
		"        native function PageLoaded();"
		"        return PageLoaded();"
		"    };"
		""
		"	GrandM.SendData = function(data) {"
		"        native function SendData(data);"
		"        return SendData(data);"
		"    };"
		""
		"	GrandM.SendChatMessage = function(data) {"
		"        native function SendChatMessage(data);"
		"        return SendChatMessage(data);"
		"    };"
		""
		"	GrandM.ShowCursor = function(data) {"
		"        native function ShowCursor(data);"
		"        return ShowCursor(data);"
		"    };"
		""
		"	GrandM.SetOption = function(option, value) {"
		"        native function SetOption(option, value);"
		"        return SetOption(option, value);"
		"    };"
		""
		"	GrandM.GetOption = function(data) {"
		"        native function GetOption(data);"
		"        return GetOption(data);"
		"    };"
		""
		"	GrandM.Connect = function(ip, port, pass) {"
		"        native function Connect(ip, port, pass);"
		"        return Connect(ip, port, pass);"
		"    };"
		""
		"	GrandM.Disconnect = function() {"
		"        native function Disconnect();"
		"        return Disconnect();"
		"    };"
		""
		"	GrandM.Retry = function() {"
		"        native function Retry();"
		"        return Retry();"
		"    };"
		""
		"	GrandM.Quit = function() {"
		"        native function Quit();"
		"        return Quit();"
		"    };"
		""
		"	GrandM.ShowGameSettings = function() {"
		"        native function ShowGameSettings();"
		"        return ShowGameSettings();"
		"    };"
		""
		"	GrandM.RockstarEditor = function() {"
		"        native function RockstarEditor();"
		"        return RockstarEditor();"
		"    };"
		""
		"	GrandM.Ping = function(ip, port) {"
		"        native function Ping(ip, port);"
		"        return Ping(ip, port);"
		"    };"
		""
		//crash
		/*"	app.setMessageCallback('binding_test', function(name, args) {"
		"		document.getElementById('result').value = \"Response: \" + args[0];"
		"	}); "
		""*/
		"})();;";

	CefRegisterExtension(CefString("v8/app"), CefString(app_code), new CefHandlerV8());

	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnWebKitInitialized(this);
}

void ClientAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
	m_browser = browser;

	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnBrowserCreated(this, browser);
}

void ClientAppRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnBrowserDestroyed(this, browser);
}

CefRefPtr<CefLoadHandler> ClientAppRenderer::GetLoadHandler()
{
	CefRefPtr<CefLoadHandler> load_handler;
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end() && !load_handler.get(); ++it)
		load_handler = (*it)->GetLoadHandler(this);

	return load_handler;
}

void ClientAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnContextCreated(this, browser, frame, context);
}

void ClientAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnContextReleased(this, browser, frame, context);
}

void ClientAppRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, 
	CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{
	LOG(ERROR) << "Renderer Recived uncaught exception in frame" << frame->GetName().ToString() << " with exception of " << exception->GetMessageW().ToString();

	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
	{
		(*it)->OnUncaughtException(this, browser, frame, context, exception,
			stackTrace);
	}
}

void ClientAppRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnFocusedNodeChanged(this, browser, frame, node);
}

bool ClientAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	DCHECK_EQ(source_process, PID_BROWSER);

	bool handled = false;

	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end() && !handled; ++it)
	{
		handled =
			(*it)->OnProcessMessageReceived(this, browser, source_process, message);
	}

	return handled;
}

void ClientAppRenderer::CreateDelegates(DelegateSet& delegates)
{
	//renderer::CreateDelegates(delegates);
	//performance_test::CreateDelegates(delegates);
}