#ifndef __CLIENTAPP_H__
#define __CLIENTAPP_H__

#include <vector>

#include "include/cef_app.h"

class ClientApp : public CefApp, public CefRenderProcessHandler
{
public:
	ClientApp();

	enum ProcessType
	{
		BrowserProcess,
		RendererProcess,
		ZygoteProcess,
		OtherProcess,
	};

	// Determine the process type based on command-line arguments.
	static ProcessType GetProcessType(CefRefPtr<CefCommandLine> command_line);

protected:
	// Schemes that will be registered with the global cookie manager.
	std::vector<CefString> cookieable_schemes_;

private:
	// Registers custom schemes.
	static void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar,
		std::vector<CefString>& cookiable_schemes);

	// CefApp methods.
	void OnRegisterCustomSchemes(
		CefRawPtr<CefSchemeRegistrar> registrar) OVERRIDE;

	// CefApp methods.
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE
	{
		return this;
	}

	DISALLOW_COPY_AND_ASSIGN(ClientApp);
};

#endif