#ifndef __CLIENTAPP_H__
#define __CLIENTAPP_H__
#pragma once

#include <set>

#include "include/cef_app.h"
#include "include/cef_menu_model.h"
#include "include/cef_menu_model_delegate.h"

class ClientApp : public CefApp
{
public:
	ClientApp();

	enum ProcessType {
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
	static void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar, std::vector<CefString>& cookiable_schemes);

	// CefApp methods.
	void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) OVERRIDE;

	DISALLOW_COPY_AND_ASSIGN(ClientApp);
};
#endif