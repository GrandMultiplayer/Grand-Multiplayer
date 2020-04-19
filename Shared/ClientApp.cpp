#include <include/cef_client.h>
#include "include/cef_command_line.h"

//#include "../Client/Core/CefScheme.h"
#include "ClientApp.h"


// These flags must match the Chromium values.
const char kProcessType[] = "type";
const char kRendererProcess[] = "renderer";

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

void ClientApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) 
{
	RegisterCustomSchemes(registrar, cookieable_schemes_);
}

void ClientApp::RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar, std::vector<CefString>& cookiable_schemes)
{
	
}