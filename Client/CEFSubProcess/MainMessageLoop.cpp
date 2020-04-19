#include "MainMessageLoop.h"

#include "include/cef_task.h"
#include "include/wrapper/cef_closure_task.h"

namespace
{

	MainMessageLoop* g_main_message_loop = NULL;

}

MainMessageLoop::MainMessageLoop()
{
	DCHECK(!g_main_message_loop);
	g_main_message_loop = this;
}

MainMessageLoop::~MainMessageLoop()
{
	g_main_message_loop = NULL;
}

MainMessageLoop* MainMessageLoop::Get()
{
	DCHECK(g_main_message_loop);
	return g_main_message_loop;
}

void MainMessageLoop::PostClosure(const base::Closure& closure)
{
	PostTask(CefCreateClosureTask(closure));
}