#include "RootWindow.h"

#include "MainContext.h"
#include "RootWindowManager.h"

RootWindowConfig::RootWindowConfig()
	: with_controls(true),
	with_osr(false),
	with_extension(false),
	initially_hidden(false),
	url(MainContext::Get()->GetMainURL())
{
}

RootWindow::RootWindow() : delegate_(NULL) {}

RootWindow::~RootWindow() {}

scoped_refptr<RootWindow> RootWindow::GetForBrowser(int browser_id)
{
	return MainContext::Get()->GetRootWindowManager()->GetWindowForBrowser(
		browser_id);
}

void RootWindow::OnExtensionsChanged(const ExtensionSet& extensions)
{
	REQUIRE_MAIN_THREAD();
	DCHECK(delegate_);
	DCHECK(!WithExtension());

	if (extensions.empty())
		return;

	ExtensionSet::const_iterator it = extensions.begin();
	for (; it != extensions.end(); ++it)
	{
		delegate_->CreateExtensionWindow(*it, CefRect(), NULL, base::Closure(),
			WithWindowlessRendering());
	}
}

scoped_refptr<RootWindow> RootWindow::Create(bool use_views)
{
	if (use_views)
	{
		return new RootWindowViews();
	}

	return new RootWindowWin();
}