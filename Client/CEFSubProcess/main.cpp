#include <windows.h>

// Vendor.CEF
#include "include/base/cef_logging.h"

#include "ClientAppRenderer.h"

// Program entry-point function.
int RunMain(HINSTANCE hInstance, int nCmdShow) 
{
	CefEnableHighDPISupport();

	// Structure for passing command-line arguments.
	// The definition of this structure is platform-specific.
	CefMainArgs args(hInstance);

	// Parse command-line arguments.
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
	command_line->InitFromString(::GetCommandLineW());

	// Create a ClientApp of the correct type.
	CefRefPtr<CefApp> app;
	ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
	if (process_type == ClientApp::BrowserProcess)
	{
		//app = new ClientAppBrowser();
	}
	else if (process_type == ClientApp::RendererProcess)
	{
		app = new ClientAppRenderer();
	}
	else if (process_type == ClientApp::OtherProcess)
	{
		app = new ClientAppRenderer();
		//app = new ClientAppOther();
	}

	// Execute the sub-process logic. This will block until the sub-process should exit.
	return CefExecuteProcess(args, app.get(), NULL);
}

// Program entry point function.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	return RunMain(hInstance, nCmdShow);
}