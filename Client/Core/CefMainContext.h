#ifndef __CEFRENDERER_H__
#define __CEFRENDERER_H__
#pragma once

#include <include/base/cef_ref_counted.h>
#include <include/base/cef_scoped_ptr.h>
#include <include/internal/cef_types_wrappers.h>
#include <include/cef_command_line.h>
#include <include/cef_app.h>

#include "OffscreenClient.h"

class CefMainContext
{
private:
	static HHOOK msgHook;

	static CefRefPtr<OffscreenClient> offscreenClient;
	static CefSettings settings;

	std::string mainUrl;

	// Track context state. Accessing these variables from multiple threads is
	// safe because only a single thread will exist at the time that they're set
	// (during context initialization and shutdown).
	bool initialized_;
	bool shutdown_;

public:
	static CefBrowserSettings browserSettings;
	static CefWindowInfo window_info;

	CefMainContext();

	CefMainContext(CefRefPtr<CefCommandLine> command_line, bool terminate_when_all_windows_closed);

	~CefMainContext();

	std::string GetMainURL() { return mainUrl; }

	bool Initialize(const CefMainArgs& args, const CefSettings& settings, CefRefPtr<CefApp> application, void* windows_sandbox_info);

	static CefMainContext* Get();

	void PopulateSettings(CefSettings* settings);

	// Shut down CEF and associated context state. This method must be called on
	// the same thread that created this object.
	void Shutdown();

	void OnTick();

	static CefRefPtr<CefBrowser> getBrowser() { return offscreenClient->GetBrowser(); }

	static OffscreenClient* getOFC() { return offscreenClient; }

	const bool IsInitialized() { return initialized_; }

private:
	DISALLOW_COPY_AND_ASSIGN(CefMainContext);
};
#endif