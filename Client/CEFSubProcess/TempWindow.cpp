#include "TempWindow.h"

#include <windows.h>

#include "include/base/cef_logging.h"

namespace
{
	const wchar_t kWndClass[] = L"Client_TempWindow";

	// Create the temp window.
	HWND CreateTempWindow()
	{
		HINSTANCE hInstance = ::GetModuleHandle(NULL);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = kWndClass;
		RegisterClassEx(&wc);

		// Create a 1x1 pixel hidden window.
		return CreateWindow(kWndClass, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0,
			1, 1, NULL, NULL, hInstance, NULL);
	}

	TempWindow* g_temp_window = NULL;
}

TempWindow::TempWindow() : hwnd_(NULL)
{
	DCHECK(!g_temp_window);
	g_temp_window = this;

	hwnd_ = CreateTempWindow();
	CHECK(hwnd_);
}

TempWindow::~TempWindow()
{
	g_temp_window = NULL;
	DCHECK(hwnd_);
	DestroyWindow(hwnd_);
}

CefWindowHandle TempWindow::GetWindowHandle()
{
	DCHECK(g_temp_window);
	return g_temp_window->hwnd_;
}