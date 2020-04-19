#include <include/base/cef_logging.h>
#include <include/cef_cookie.h>

#include "MainMessageLoopExternalPump.h"
#include "ClientSwitches.h"
#include "ClientAppBrowser.h"

ClientAppBrowser::ClientAppBrowser()
{
	CreateDelegates(delegates_);
}

void ClientAppBrowser::OnBeforeCommandLineProcessing(
	const CefString& process_type,
	CefRefPtr<CefCommandLine> command_line)
{
	// Pass additional command-line flags to the browser process.
	if (process_type.empty())
	{
		// Pass additional command-line flags when off-screen rendering is enabled.
		if (command_line->HasSwitch(switches::kOffScreenRenderingEnabled))
		{
			// If the PDF extension is enabled then cc Surfaces must be disabled for
			// PDFs to render correctly.
			// See https://bitbucket.org/chromiumembedded/cef/issues/1689 for details.
			if (!command_line->HasSwitch("disable-extensions") &&
				!command_line->HasSwitch("disable-pdf-extension"))
			{
				command_line->AppendSwitch("disable-surfaces");
			}

			// Use software rendering and compositing (disable GPU) for increased FPS
			// and decreased CPU usage. This will also disable WebGL so remove these
			// switches if you need that capability.
			// See https://bitbucket.org/chromiumembedded/cef/issues/1257 for details.
			if (!command_line->HasSwitch(switches::kEnableGPU))
			{
				command_line->AppendSwitch("disable-gpu");
				command_line->AppendSwitch("disable-gpu-compositing");
			}
		}

		if (command_line->HasSwitch(switches::kUseViews) &&
			!command_line->HasSwitch("top-chrome-md"))
		{
			// Use non-material mode on all platforms by default. Among other things
			// this causes menu buttons to show hover state. See usage of
			// MaterialDesignController::IsModeMaterial() in Chromium code.
			command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
		}

		if (!command_line->HasSwitch(switches::kCachePath) &&
			!command_line->HasSwitch("disable-gpu-shader-disk-cache"))
		{
			// Don't create a "GPUCache" directory when cache-path is unspecified.
			command_line->AppendSwitch("disable-gpu-shader-disk-cache");
		}

		DelegateSet::iterator it = delegates_.begin();
		for (; it != delegates_.end(); ++it)
			(*it)->OnBeforeCommandLineProcessing(this, command_line);
	}
}

#include "include/cef_crash_util.h"
#include "include/cef_command_line.h"
#include "include/cef_file_util.h"
namespace
{
	class ClientBrowserDelegate : public ClientAppBrowser::Delegate
	{
	public:
		ClientBrowserDelegate() {}

		void OnContextInitialized(CefRefPtr<ClientAppBrowser> app) OVERRIDE
		{
			if (CefCrashReportingEnabled())
			{
				// Set some crash keys for testing purposes. Keys must be defined in the
				// "crash_reporter.cfg" file. See cef_crash_util.h for details.
				CefSetCrashKeyValue("testkey_small1", "value1_small_browser");
				CefSetCrashKeyValue("testkey_small2", "value2_small_browser");
				CefSetCrashKeyValue("testkey_medium1", "value1_medium_browser");
				CefSetCrashKeyValue("testkey_medium2", "value2_medium_browser");
				CefSetCrashKeyValue("testkey_large1", "value1_large_browser");
				CefSetCrashKeyValue("testkey_large2", "value2_large_browser");
			}

			const std::string& crl_sets_path =
				CefCommandLine::GetGlobalCommandLine()->GetSwitchValue(
					switches::kCRLSetsPath);
			if (!crl_sets_path.empty())
			{
				// Load the CRLSets file from the specified path.
				CefLoadCRLSetsFile(crl_sets_path);
			}
		}

	private:
		DISALLOW_COPY_AND_ASSIGN(ClientBrowserDelegate);
		IMPLEMENT_REFCOUNTING(ClientBrowserDelegate);
	};
}

void ClientAppBrowser::CreateDelegates(DelegateSet& delegates)
{
	delegates.insert(new ClientBrowserDelegate);
}

CefRefPtr<CefPrintHandler> ClientAppBrowser::CreatePrintHandler()
{
	return NULL;
}

void ClientAppBrowser::OnContextInitialized()
{
	// Register cookieable schemes with the global cookie manager.
	CefRefPtr<CefCookieManager> manager =
		CefCookieManager::GetGlobalManager(NULL);
	DCHECK(manager.get());
	manager->SetSupportedSchemes(cookieable_schemes_, NULL);

	print_handler_ = CreatePrintHandler();

	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnContextInitialized(this);
}

void ClientAppBrowser::OnBeforeChildProcessLaunch(
	CefRefPtr<CefCommandLine> command_line)
{
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnBeforeChildProcessLaunch(this, command_line);
}

void ClientAppBrowser::OnRenderProcessThreadCreated(
	CefRefPtr<CefListValue> extra_info)
{
	DelegateSet::iterator it = delegates_.begin();
	for (; it != delegates_.end(); ++it)
		(*it)->OnRenderProcessThreadCreated(this, extra_info);
}

void ClientAppBrowser::OnScheduleMessagePumpWork(int64 delay)
{
	// Only used when `--external-message-pump` is passed via the command-line.
	MainMessageLoopExternalPump* message_pump =
		MainMessageLoopExternalPump::Get();
	if (message_pump)
		message_pump->OnScheduleMessagePumpWork(delay);
}