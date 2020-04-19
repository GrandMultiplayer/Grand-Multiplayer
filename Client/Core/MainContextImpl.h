#ifndef __MAINCONTEXTIMPL_H__
#define __MAINCONTEXTIMPL_H__

#include "include/base/cef_scoped_ptr.h"
#include "include/base/cef_thread_checker.h"
#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "MainContext.h"
//#include "tests/cefclient/browser/root_window_manager.h"

// Used to store global context in the browser process.
class MainContextImpl : public MainContext
{
public:
	MainContextImpl(CefRefPtr<CefCommandLine> command_line,
		bool terminate_when_all_windows_closed);

	// MainContext members.
	std::string GetConsoleLogPath() OVERRIDE;
	std::string GetDownloadPath(const std::string& file_name) OVERRIDE;
	std::string GetAppWorkingDirectory() OVERRIDE;
	std::string GetMainURL() OVERRIDE;
	cef_color_t GetBackgroundColor() OVERRIDE;
	bool UseViews() OVERRIDE;
	bool UseWindowlessRendering() OVERRIDE;
	void PopulateSettings(CefSettings* settings) OVERRIDE;
	void PopulateBrowserSettings(CefBrowserSettings* settings) OVERRIDE;
	void PopulateOffscreenSettings(OffscreenRenderer::Settings* settings) OVERRIDE;
	RootWindowManager* GetRootWindowManager() OVERRIDE;

	// Initialize CEF and associated main context state. This method must be
	// called on the same thread that created this object.
	bool Initialize(const CefMainArgs& args,
		const CefSettings& settings,
		CefRefPtr<CefApp> application,
		void* windows_sandbox_info);

	// Shut down CEF and associated context state. This method must be called on
	// the same thread that created this object.
	void Shutdown();

private:
	// Allow deletion via scoped_ptr only.
	friend struct base::DefaultDeleter<MainContextImpl>;

	~MainContextImpl();

	// Returns true if the context is in a valid state (initialized and not yet
	// shut down).
	bool InValidState() const { return initialized_ && !shutdown_; }

	CefRefPtr<CefCommandLine> command_line_;
	const bool terminate_when_all_windows_closed_;

	// Track context state. Accessing these variables from multiple threads is
	// safe because only a single thread will exist at the time that they're set
	// (during context initialization and shutdown).
	bool initialized_;
	bool shutdown_;

	std::string main_url_;
	cef_color_t background_color_;
	cef_color_t browser_background_color_;
	bool use_windowless_rendering_;
	bool use_views_;

	scoped_ptr<RootWindowManager> root_window_manager_;

	// Used to verify that methods are called on the correct thread.
	base::ThreadChecker thread_checker_;

	DISALLOW_COPY_AND_ASSIGN(MainContextImpl);
};

#endif