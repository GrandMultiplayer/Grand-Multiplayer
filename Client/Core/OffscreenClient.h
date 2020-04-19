#ifndef __OFFSCREENCLIENT_H__
#define __OFFSCREENCLIENT_H__
#pragma once

#include <include/cef_client.h>
#include <include/cef_life_span_handler.h>
#include <include/cef_render_handler.h>
#include <include/cef_render_process_handler.h>
#include <include/wrapper/cef_helpers.h>

class OffscreenClient : public CefClient, public CefLifeSpanHandler, public CefRenderHandler, public CefRenderProcessHandler
{
public:
	OffscreenClient();
	~OffscreenClient();

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
	{
		return this;
	}

	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE
	{
		return this;
	}

	virtual void OnPaint(CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		const void* buffer,
		int width,
		int height) OVERRIDE;

	// CefLifeSpanHandler methods
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }

	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;

	// private:
	// The child browser window
	CefRefPtr<CefBrowser> m_browser;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(OffscreenClient);
	// Include the default locking implementation.
	IMPLEMENT_LOCKING(OffscreenClient);

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;
};
#endif