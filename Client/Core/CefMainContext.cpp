// NEEDS TO BE FIRST ELSE WE GET WINSOCK ERRORS
#include <winsock2.h>

#include "CefMainContext.h"

//#include <Windows.h>

// DirectX // DirectInput
#include <d3d11.h>
//#include <d3dcompiler.h>
//#define DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>

// Vendor.CEF
//#include <include/cef_app.h>
//#include <include/cef_client.h>
//#include <include/cef_parser.h>
//#include <include/cef_web_plugin.h>

#include "OffscreenClient.h"
#include "DirectXHook.h"
#include "CefTexture.h"
#include "CefInput.h"
#include "CGui.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

HHOOK CefMainContext::msgHook;

CefRefPtr<OffscreenClient> CefMainContext::offscreenClient;
CefSettings CefMainContext::settings;
CefWindowInfo CefMainContext::window_info;
CefBrowserSettings CefMainContext::browserSettings;

namespace
{
	CefMainContext* g_maincontext = NULL;
}

CefMainContext* CefMainContext::Get()
{
	DCHECK(g_maincontext);
	return g_maincontext;
}

CefMainContext::CefMainContext() :
	initialized_(false),
	shutdown_(false)
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));

	std::string filePath = buffer;
	filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size());

	mainUrl = std::string(filePath + "/ui/index.html");
	//CefBrowserHost::CreateBrowser(window_info, offscreenClient.get(), CefString(std::string(filePath + "/ui/index.html")), browserSettings, nullptr);

	DCHECK(!g_maincontext);
	g_maincontext = this;
}

CefMainContext::CefMainContext(CefRefPtr<CefCommandLine> command_line, bool terminate_when_all_windows_closed) : 
	initialized_(false),
	shutdown_(false)
{
	// Set the main URL.
	char buffer[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));

	std::string filePath = buffer;
	filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size());

	mainUrl = std::string(filePath + "/ui/index.html");

	Vector2 resolution = CGui::GetResolution();

	window_info.width = resolution.x;
	window_info.height = resolution.y;
	window_info.SetAsWindowless(FindWindowA(NULL, "Grand Theft Auto V"));

	browserSettings.windowless_frame_rate = 30; // 30 is default
	browserSettings.javascript = STATE_ENABLED;
	browserSettings.web_security = STATE_ENABLED/*STATE_DISABLED*/;

	offscreenClient = new OffscreenClient;

	DCHECK(!g_maincontext);
	g_maincontext = this;
}

CefMainContext::~CefMainContext()
{
	// The context must either not have been initialized, or it must have also been shut down.
	DCHECK(!initialized_ || shutdown_);

	g_maincontext = NULL;
}

void CefMainContext::PopulateSettings(CefSettings* settings)
{
	std::string modualFolder = Utility::GetOurModuleFolder();

	std::string cachePath = modualFolder + "cache\\cef\\";
	CreateDirectoryA(cachePath.c_str(), nullptr);

	settings->windowless_rendering_enabled = true;
	settings->multi_threaded_message_loop = false;
	settings->remote_debugging_port = 22327;
	settings->pack_loading_disabled = false;
	settings->log_severity = LOGSEVERITY_INFO/*LOGSEVERITY_WARNING*/;
	//settings->no_sandbox = true;

	CefString(&settings->resources_dir_path).FromString(modualFolder);

	std::string locals = modualFolder + "locales";
	CefString(&settings->locales_dir_path).FromString(locals);

	std::string ceflog = modualFolder + "cef_debug.log";
	CefString(&settings->log_file).FromString(ceflog);

	CefString(&settings->locale).FromASCII("en-US");
	CefString(&settings->cache_path).FromString(cachePath);

	// Specify the path for the sub-process executable.
	std::string cefproc = modualFolder + "SubProcess.exe";
	CefString(&settings->browser_subprocess_path).FromASCII(cefproc.c_str());
}

LRESULT CALLBACK MsgHookRet(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPRETSTRUCT* msg = (CWPRETSTRUCT*)lParam;

	switch (msg->message)
	{
	case WM_SIZE:
		if (g_D3DHook.GetDevice() != NULL && wParam != SIZE_MINIMIZED)
		{
			CefTexture::CleanupRenderTarget();
			g_D3DHook.GetSwapChain()->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			CefTexture::CreateRenderTarget();
		}
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			break;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

bool CefMainContext::Initialize(const CefMainArgs& args, const CefSettings& settings, CefRefPtr<CefApp> application, void* windows_sandbox_info)
{
	DCHECK(!initialized_);
	DCHECK(!shutdown_);

	msgHook = SetWindowsHookExA(WH_CALLWNDPROCRET, MsgHookRet, GetModuleHandle(NULL), 0);

	CefEnableHighDPISupport();

	// settings.cache_path / may want to set-up to prevent memory hogging
	if (!CefInitialize(args, settings, application, windows_sandbox_info))
	{
		LOG_ERROR("[CefMainContext] Cef Initialize Failed.");
		return false;
	}

	offscreenClient = new OffscreenClient;

	Vector2 resolution = CGui::GetResolution();

	window_info.width = resolution.x;
	window_info.height = resolution.y;
	window_info.SetAsWindowless(FindWindowA(NULL, "Grand Theft Auto V"));

	browserSettings.windowless_frame_rate = 30; // 30 is default
	browserSettings.javascript = STATE_ENABLED;
	browserSettings.web_security = STATE_ENABLED/*STATE_DISABLED*/;

	CefInput::InstallHook();
	//CefBrowserHost::CreateBrowser(window_info, offscreenClient.get(), CefString(this->GetMainURL()), browserSettings, nullptr);

	initialized_ = true;
	LOG_DEBUG("[CefRenderer] Initialized.");
	return true;
}

void CefMainContext::Shutdown()
{
	//DCHECK(thread_checker_.CalledOnValidThread());
	DCHECK(initialized_);
	DCHECK(!shutdown_);

	//root_window_manager_.reset();

	CefShutdown();

	shutdown_ = true;
}

void CefMainContext::OnTick()
{
	CefRunMessageLoop();
}