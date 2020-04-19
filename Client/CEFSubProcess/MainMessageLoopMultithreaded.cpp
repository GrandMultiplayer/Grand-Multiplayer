#include "MainMessageLoopMultithreaded.h"

#include "include/base/cef_bind.h"
#include "include/base/cef_logging.h"
#include "include/cef_app.h"

#include "Resource.h"
#include "Util.h"

namespace
{
	const wchar_t kWndClass[] = L"Client_MessageWindow";
	const wchar_t kTaskMessageName[] = L"Client_CustomTask";
}

MainMessageLoopMultithreaded::MainMessageLoopMultithreaded()
	: thread_id_(base::PlatformThread::CurrentId()),
	task_message_id_(RegisterWindowMessage(kTaskMessageName)),
	dialog_hwnd_(NULL),
	message_hwnd_(NULL)
{
}

MainMessageLoopMultithreaded::~MainMessageLoopMultithreaded()
{
	DCHECK(RunsTasksOnCurrentThread());
	DCHECK(!message_hwnd_);
	DCHECK(queued_tasks_.empty());
}

int MainMessageLoopMultithreaded::Run()
{
	DCHECK(RunsTasksOnCurrentThread());

	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	{
		base::AutoLock lock_scope(lock_);

		// Create the hidden window for message processing.
		message_hwnd_ = CreateMessageWindow(hInstance);
		CHECK(message_hwnd_);

		// Store a pointer to |this| in the window's user data.
		SetUserDataPtr(message_hwnd_, this);

		// Execute any tasks that are currently queued.
		while (!queued_tasks_.empty())
		{
			PostTaskInternal(queued_tasks_.front());
			queued_tasks_.pop();
		}
	}

	HACCEL hAccelTable =
		LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CEFCLIENT));

	MSG msg;

	// Run the application message loop.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Allow processing of dialog messages.
		if (dialog_hwnd_ && IsDialogMessage(dialog_hwnd_, &msg))
			continue;

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	{
		base::AutoLock lock_scope(lock_);

		// Destroy the message window.
		DestroyWindow(message_hwnd_);
		message_hwnd_ = NULL;
	}

	return static_cast<int>(msg.wParam);
}

void MainMessageLoopMultithreaded::Quit()
{
	// Execute PostQuitMessage(0) on the main thread.
	PostClosure(base::Bind(::PostQuitMessage, 0));
}

void MainMessageLoopMultithreaded::PostTask(CefRefPtr<CefTask> task)
{
	base::AutoLock lock_scope(lock_);
	PostTaskInternal(task);
}

bool MainMessageLoopMultithreaded::RunsTasksOnCurrentThread() const
{
	return (thread_id_ == base::PlatformThread::CurrentId());
}

void MainMessageLoopMultithreaded::SetCurrentModelessDialog(
	HWND hWndDialog)
{
	DCHECK(RunsTasksOnCurrentThread());

	dialog_hwnd_ = hWndDialog;
}

HWND MainMessageLoopMultithreaded::CreateMessageWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MessageWndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = kWndClass;
	RegisterClassEx(&wc);

	return CreateWindow(kWndClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, hInstance,
		0);
}

LRESULT CALLBACK MainMessageLoopMultithreaded::MessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MainMessageLoopMultithreaded* self =
		GetUserDataPtr<MainMessageLoopMultithreaded*>(hWnd);

	if (self && message == self->task_message_id_)
	{
		// Execute the task.
		CefTask* task = reinterpret_cast<CefTask*>(wParam);
		task->Execute();

		// Release the reference added in PostTaskInternal. This will likely result
		// in |task| being deleted.
		task->Release();
	}
	else
	{
		switch (message)
		{
		case WM_NCDESTROY:
			// Clear the reference to |self|.
			SetUserDataPtr(hWnd, NULL);
			break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void MainMessageLoopMultithreaded::PostTaskInternal(CefRefPtr<CefTask> task)
{
	lock_.AssertAcquired();

	if (!message_hwnd_)
	{
		// Queue the task until the message loop starts running.
		queued_tasks_.push(task);
		return;
	}

	// Add a reference that will be released in MessageWndProc.
	task->AddRef();

	// Post the task for execution by the message window.
	PostMessage(message_hwnd_, task_message_id_,
		reinterpret_cast<WPARAM>(task.get()), 0);
}