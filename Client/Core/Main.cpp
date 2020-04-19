#include <winsock2.h>
#include <Windows.h>

// GameHook
#include "hook\enums.h"
#include "hook\types.h"
#include "hook\main.h"

// Discord
#include "discord_rpc.h"

#include "CMain.h"
// Vendor.CEF
//#include "include/base/cef_scoped_ptr.h"
//#include "include/cef_command_line.h"
//#include "include/cef_sandbox_win.h"
//#include <include/cef_app.h>
//#include <include/cef_client.h>

#include "CefMainContext.h"
#include "ClientApp.h"
#include "OffscreenClient.h"
#include "DirectXHook.h"
#include "Keyboard.h"
#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

DWORD WINAPI PreThread(LPVOID sHModule)
{
	HMODULE hModule = static_cast<HMODULE>(sHModule);

	CMain::InitializeGame(hModule);

	return 0;
}

DWORD WINAPI CEFThread(LPVOID sHModule)
{
	LOG_DEBUG("[CEF Thread] Started.");

	HMODULE hModule = static_cast<HMODULE>(sHModule);

	while (g_D3DHook.GetDevice() == NULL)
		Sleep(100);

	//msgHook = SetWindowsHookExA(WH_CALLWNDPROCRET, MsgHookRet, GetModuleHandle(NULL), 0);

	// Parse command-line arguments.
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
	command_line->InitFromString(::GetCommandLineW());

	// Structure for passing command-line arguments.
	// The definition of this structure is platform-specific.
	CefMainArgs args(hModule);

	CefRefPtr<ClientApp> app(new ClientApp);

	// Execute the secondary process, if any.
	int exit_code = CefExecuteProcess(args, app, nullptr);
	if (exit_code >= 0)
		return exit_code;

	// Create the main context object.
	scoped_ptr<CefMainContext> context(new CefMainContext(command_line, true));

	CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	// Populate the settings based on command line arguments.
	context->PopulateSettings(&settings);

	// Initialize CEF.
	LOG_DEBUG("[CEF Thread] Initializing.");
	context->Initialize(args, settings, app, nullptr);
	
	CefBrowserHost::CreateBrowser(CefMainContext::window_info, CefMainContext::getOFC(), CefString(context->GetMainURL()), CefMainContext::browserSettings, nullptr);

	LOG_DEBUG("[CEF Thread] tick.");
	context->OnTick();

	// Shut down CEF.
	LOG_DEBUG("[CEF Thread] shutdown.");
	context->Shutdown();

	// Release objects in reverse order of creation.
	//message_loop.reset();
	context.reset();

	return 0;
}

DWORD WINAPI clientStart(LPVOID sHModule)
{
	HMODULE hModule = static_cast<HMODULE>(sHModule);
	
	HWND windowHandle = NULL;
	while (windowHandle == NULL)
	{
		windowHandle = FindWindowA("grcWindow", NULL);
		Sleep(100);
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PreThread, (LPVOID)hModule, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CEFThread, (LPVOID)hModule, NULL, NULL);

	keyboardHandlerRegister(OnKeyboardMessage);
	//scriptRegister(hModule, CMain::Start);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientStart, (LPVOID)hModule, NULL, NULL);
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
		LOG_DEBUG("[Core] End.");

		Discord_Shutdown();
		CNetworkManager::Stop();
		CefShutdown();
		scriptUnregister(hModule);
		FreeConsole();
	}
		break;
	}

	return TRUE;
}