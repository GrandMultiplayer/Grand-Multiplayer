#ifndef __CLIENTAPP_H__
#define __CLIENTAPP_H__
#pragma once

#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/views/cef_browser_view.h>

class ClientApp : public CefApp, public CefLoadHandler, public CefBrowserProcessHandler, public CefContextMenuHandler
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

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;

	IMPLEMENT_REFCOUNTING(ClientApp);
};
#endif