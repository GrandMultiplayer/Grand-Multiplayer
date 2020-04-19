#ifndef __CLIENTAPPBROWSER_H__
#define __CLIENTAPPBROWSER_H__

#include <set>

#include "../Shared/ClientApp.h"

class ClientAppBrowser : public ClientApp, public CefBrowserProcessHandler
{
public:
	// Interface for browser delegates. All Delegates must be returned via
	// CreateDelegates. Do not perform work in the Delegate
	// constructor. See CefBrowserProcessHandler for documentation.
	class Delegate : public virtual CefBaseRefCounted
	{
	public:
		virtual void OnBeforeCommandLineProcessing(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefCommandLine> command_line) {}

		virtual void OnContextInitialized(CefRefPtr<ClientAppBrowser> app) {}

		virtual void OnBeforeChildProcessLaunch(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefCommandLine> command_line) {}

		virtual void OnRenderProcessThreadCreated(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefListValue> extra_info) {}
	};

	typedef std::set<CefRefPtr<Delegate>> DelegateSet;

	ClientAppBrowser();

private:
	// Creates all of the Delegate objects.
	static void CreateDelegates(DelegateSet& delegates);

	// Create the Linux print handler.
	static CefRefPtr<CefPrintHandler> CreatePrintHandler();

	// CefApp methods.
	void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	
	CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE
	{
		return this;
	}

	// CefBrowserProcessHandler methods.
	void OnContextInitialized() OVERRIDE;
	void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
	
	CefRefPtr<CefPrintHandler> GetPrintHandler() OVERRIDE
	{
		return print_handler_;
	}

	void OnScheduleMessagePumpWork(int64 delay) OVERRIDE;

	// Set of supported Delegates.
	DelegateSet delegates_;

	CefRefPtr<CefPrintHandler> print_handler_;

	IMPLEMENT_REFCOUNTING(ClientAppBrowser);
	DISALLOW_COPY_AND_ASSIGN(ClientAppBrowser);
};
#endif