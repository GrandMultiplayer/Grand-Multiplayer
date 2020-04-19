#include "ClientApp.h"

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/wrapper/cef_helpers.h>
#include <include/cef_command_line.h>

#include "../Core/ClientHandler.h"

namespace
{
	// These flags must match the Chromium values.
	const char kProcessType[] = "type";
	const char kRendererProcess[] = "renderer";
}

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

//			LOG(ERROR) << "GrandM Sub << res " << window->GetSize().width;
			//LOG(ERROR) << "GrandM Sub << res %i", s;

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

ClientApp::ClientApp() {}

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

void ClientApp::RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar, std::vector<CefString>& cookiable_schemes)
{
	//scheme_test::RegisterCustomSchemes(registrar, cookiable_schemes);
}

void ClientApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
	RegisterCustomSchemes(registrar, cookieable_schemes_);
}