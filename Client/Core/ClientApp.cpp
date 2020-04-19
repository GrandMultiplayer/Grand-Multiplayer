#include "ClientApp.h"

#include <winsock2.h>
#include <Windows.h>

#include <string>

// Vendor.CEF
//#include <include/cef_browser.h>
//#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/wrapper/cef_helpers.h>
//#include <include/cef_command_line.h>

// GrandM CEF Impl
#include "ClientHandler.h"
//#include "CefHandlerV8.h"

#include "CGui.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace
{

	// When using the Views framework this object provides the delegate
	// implementation for the CefWindow that hosts the Views-based browser.
	class ClientWindowDelegate : public CefWindowDelegate
	{
	public:
		explicit ClientWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
			: browser_view_(browser_view)
		{
		}

		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE
		{
			// Add the browser view and show the window.
			window->AddChildView(browser_view_);
			window->Show();

			// Give keyboard focus to the browser view.
			browser_view_->RequestFocus();
		}

		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE
		{
			browser_view_ = NULL;
		}

		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE
		{
			// Allow the window to close if the browser says it's OK.
			CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
			if (browser)
				return browser->GetHost()->TryCloseBrowser();
			return true;
		}

	private:
		CefRefPtr<CefBrowserView> browser_view_;

		IMPLEMENT_REFCOUNTING(ClientWindowDelegate);
		DISALLOW_COPY_AND_ASSIGN(ClientWindowDelegate);
	};

}  // namespace

namespace
{
	// These flags must match the Chromium values.
	const char kProcessType[] = "type";
	const char kRendererProcess[] = "renderer";
}

ClientApp::ClientApp()
{
}

ClientApp::ProcessType ClientApp::GetProcessType(CefRefPtr<CefCommandLine> command_line)
{
	// The command-line flag won't be specified for the browser process.
	if (!command_line->HasSwitch(kProcessType))
		return BrowserProcess;

	const std::string& process_type = command_line->GetSwitchValue(kProcessType);
	if (process_type == kRendererProcess)
		return RendererProcess;

	return OtherProcess;
}

void ClientApp::OnContextInitialized()
{
	LOG_DEBUG("OnContextInitialized");

	CEF_REQUIRE_UI_THREAD();

	CefRefPtr<CefCommandLine> command_line =
		CefCommandLine::GetGlobalCommandLine();

#if defined(OS_WIN) || defined(OS_LINUX)
	// Create the browser using the Views framework if "--use-views" is specified
	// via the command-line. Otherwise, create the browser using the native
	// platform framework. The Views framework is currently only supported on
	// Windows and Linux.
	const bool use_views = command_line->HasSwitch("use-views");
#else
	const bool use_views = false;
#endif

	// SimpleHandler implements browser-level callbacks.
	CefRefPtr<ClientHandler> handler(new ClientHandler(use_views));

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	std::string url;

	// Check if a "--url=" value was provided via the command-line. If so, use
	// that instead of the default URL.
	url = command_line->GetSwitchValue("url");
	if (url.empty())
		url = "http://www.google.com";

	if (use_views)
	{
		// Create the BrowserView.
		CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
			handler, url, browser_settings, NULL, NULL);

		// Create the Window. It will show itself after creation.
		CefWindow::CreateTopLevelWindow(new ClientWindowDelegate(browser_view));
	}
	else
	{
		Vector2 resolution = CGui::GetResolution();

		LOG_DEBUG("[ClientApp] Res %i %i.", resolution.x, resolution.y);

		// Information used when creating the native window.
		CefWindowInfo window_info;
		window_info.width = resolution.x;
		window_info.height = resolution.y;
		window_info.SetAsWindowless(FindWindowA(NULL, "Grand Theft Auto V"));

#if defined(OS_WIN)
		// On Windows we need to specify certain flags that will be passed to
		// CreateWindowEx().
		//window_info.SetAsPopup(NULL, "cefsimple");
#endif

		// Create the first browser window.
		CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, NULL);
	}
}

void ClientApp::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	CEF_REQUIRE_UI_THREAD();
	model->Clear();
}