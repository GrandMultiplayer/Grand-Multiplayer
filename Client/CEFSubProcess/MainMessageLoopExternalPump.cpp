#include "MainMessageLoopExternalPump.h"

#include <climits>
#include <CommCtrl.h>

#include "include/cef_app.h"
#include "include/wrapper/cef_helpers.h"

#include "MainMessageLoop.h"
#include "Util.h"

namespace
{
	// Special timer delay placeholder value. Intentionally 32-bit for Windows and
	// OS X platform API compatibility.
	const int32 kTimerDelayPlaceholder = INT_MAX;

	// The maximum number of milliseconds we're willing to wait between calls to
	// DoWork().
	const int64 kMaxTimerDelay = 1000 / 30;  // 30fps

	MainMessageLoopExternalPump* g_external_message_pump = NULL;

}

MainMessageLoopExternalPump::MainMessageLoopExternalPump()
	: is_active_(false), reentrancy_detected_(false)
{
	DCHECK(!g_external_message_pump);
	g_external_message_pump = this;
}

MainMessageLoopExternalPump::~MainMessageLoopExternalPump()
{
	g_external_message_pump = NULL;
}

MainMessageLoopExternalPump* MainMessageLoopExternalPump::Get()
{
	return g_external_message_pump;
}

void MainMessageLoopExternalPump::OnScheduleWork(int64 delay_ms)
{
	REQUIRE_MAIN_THREAD();

	if (delay_ms == kTimerDelayPlaceholder && IsTimerPending())
	{
		// Don't set the maximum timer requested from DoWork() if a timer event is
		// currently pending.
		return;
	}

	KillTimer();

	if (delay_ms <= 0)
	{
		// Execute the work immediately.
		DoWork();
	}
	else
	{
		// Never wait longer than the maximum allowed time.
		if (delay_ms > kMaxTimerDelay)
			delay_ms = kMaxTimerDelay;

		// Results in call to OnTimerTimeout() after the specified delay.
		SetTimer(delay_ms);
	}
}

void MainMessageLoopExternalPump::OnTimerTimeout()
{
	REQUIRE_MAIN_THREAD();

	KillTimer();
	DoWork();
}

void MainMessageLoopExternalPump::DoWork()
{
	const bool was_reentrant = PerformMessageLoopWork();
	if (was_reentrant)
	{
		// Execute the remaining work as soon as possible.
		OnScheduleMessagePumpWork(0);
	}
	else if (!IsTimerPending())
	{
		// Schedule a timer event at the maximum allowed time. This may be dropped
		// in OnScheduleWork() if another timer event is already in-flight.
		OnScheduleMessagePumpWork(kTimerDelayPlaceholder);
	}
}

bool MainMessageLoopExternalPump::PerformMessageLoopWork()
{
	if (is_active_)
	{
		// When CefDoMessageLoopWork() is called there may be various callbacks
		// (such as paint and IPC messages) that result in additional calls to this
		// method. If re-entrancy is detected we must repost a request again to the
		// owner thread to ensure that the discarded call is executed in the future.
		reentrancy_detected_ = true;
		return false;
	}

	reentrancy_detected_ = false;

	is_active_ = true;
	CefDoMessageLoopWork();
	is_active_ = false;

	// |reentrancy_detected_| may have changed due to re-entrant calls to this
	// method.
	return reentrancy_detected_;
}

namespace
{

	// Message sent to get an additional time slice for pumping (processing) another
	// task (a series of such messages creates a continuous task pump).
	static const int kMsgHaveWork = WM_USER + 1;

	class MainMessageLoopExternalPumpWin : public MainMessageLoopExternalPump
	{
	public:
		MainMessageLoopExternalPumpWin();
		~MainMessageLoopExternalPumpWin();

		// MainMessageLoopStd methods:
		void Quit() OVERRIDE;
		int Run() OVERRIDE;

		// MainMessageLoopExternalPump methods:
		void OnScheduleMessagePumpWork(int64 delay_ms) OVERRIDE;

	protected:
		// MainMessageLoopExternalPump methods:
		void SetTimer(int64 delay_ms) OVERRIDE;
		void KillTimer() OVERRIDE;
		bool IsTimerPending() OVERRIDE { return timer_pending_; }

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd,
			UINT msg,
			WPARAM wparam,
			LPARAM lparam);

		// True if a timer event is currently pending.
		bool timer_pending_;

		// HWND owned by the thread that CefDoMessageLoopWork should be invoked on.
		HWND main_thread_target_;
	};

	MainMessageLoopExternalPumpWin::MainMessageLoopExternalPumpWin()
		: timer_pending_(false), main_thread_target_(NULL)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		const wchar_t* const kClassName = L"CEFMainTargetHWND";

		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = hInstance;
		wcex.lpszClassName = kClassName;
		RegisterClassEx(&wcex);

		// Create the message handling window.
		main_thread_target_ =
			CreateWindowW(kClassName, NULL, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0,
				HWND_MESSAGE, NULL, hInstance, NULL);
		DCHECK(main_thread_target_);
		SetUserDataPtr(main_thread_target_, this);
	}

	MainMessageLoopExternalPumpWin::~MainMessageLoopExternalPumpWin()
	{
		KillTimer();
		if (main_thread_target_)
			DestroyWindow(main_thread_target_);
	}

	void MainMessageLoopExternalPumpWin::Quit()
	{
		PostMessage(NULL, WM_QUIT, 0, 0);
	}

	int MainMessageLoopExternalPumpWin::Run()
	{
		// Run the message loop.
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		KillTimer();

		// We need to run the message pump until it is idle. However we don't have
		// that information here so we run the message loop "for a while".
		for (int i = 0; i < 10; ++i)
		{
			// Do some work.
			CefDoMessageLoopWork();

			// Sleep to allow the CEF proc to do work.
			Sleep(50);
		}

		return 0;
	}

	void MainMessageLoopExternalPumpWin::OnScheduleMessagePumpWork(int64 delay_ms)
	{
		// This method may be called on any thread.
		PostMessage(main_thread_target_, kMsgHaveWork, 0,
			static_cast<LPARAM>(delay_ms));
	}

	void MainMessageLoopExternalPumpWin::SetTimer(int64 delay_ms)
	{
		DCHECK(!timer_pending_);
		DCHECK_GT(delay_ms, 0);
		timer_pending_ = true;
		::SetTimer(main_thread_target_, 1, static_cast<UINT>(delay_ms), NULL);
	}

	void MainMessageLoopExternalPumpWin::KillTimer()
	{
		if (timer_pending_)
		{
			::KillTimer(main_thread_target_, 1);
			timer_pending_ = false;
		}
	}

	LRESULT CALLBACK MainMessageLoopExternalPumpWin::WndProc(HWND hwnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam)
	{
		if (msg == WM_TIMER || msg == kMsgHaveWork)
		{
			MainMessageLoopExternalPumpWin* message_loop = GetUserDataPtr<MainMessageLoopExternalPumpWin*>(hwnd);
			if (msg == kMsgHaveWork)
			{
				// OnScheduleMessagePumpWork() request.
				const int64 delay_ms = static_cast<int64>(lparam);
				message_loop->OnScheduleWork(delay_ms);
			}
			else
			{
				// Timer timed out.
				message_loop->OnTimerTimeout();
			}
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

}

scoped_ptr<MainMessageLoopExternalPump> MainMessageLoopExternalPump::Create()
{
	return scoped_ptr<MainMessageLoopExternalPump>(new MainMessageLoopExternalPumpWin());
}
