#include "MainMessageLoopStd.h"

#include "include/cef_app.h"

MainMessageLoopStd::MainMessageLoopStd() {}

int MainMessageLoopStd::Run()
{
	CefRunMessageLoop();
	return 0;
}

void MainMessageLoopStd::Quit()
{
	CefQuitMessageLoop();
}

void MainMessageLoopStd::PostTask(CefRefPtr<CefTask> task)
{
	CefPostTask(TID_UI, task);
}

bool MainMessageLoopStd::RunsTasksOnCurrentThread() const
{
	return CefCurrentlyOn(TID_UI);
}

void MainMessageLoopStd::SetCurrentModelessDialog(HWND hWndDialog)
{
	// Nothing to do here. The Chromium message loop implementation will internally route dialog messages.
}