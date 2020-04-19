#ifndef __MAINCONTEXT_H__
#define __MAINCONTEXT_H__

#include <string>

#include "include/base/cef_ref_counted.h"
#include "include/internal/cef_types_wrappers.h"
#include "OffscreenRenderer.h"

class RootWindowManager;

// Used to store global context in the browser process. The methods of this
// class are thread-safe unless otherwise indicated.
class MainContext
{
public:
	// Returns the singleton instance of this object.
	static MainContext* Get();

	// Returns the full path to the console log file.
	virtual std::string GetConsoleLogPath() = 0;

	// Returns the full path to |file_name|.
	virtual std::string GetDownloadPath(const std::string& file_name) = 0;

	// Returns the app working directory including trailing path separator.
	virtual std::string GetAppWorkingDirectory() = 0;

	// Returns the main application URL.
	virtual std::string GetMainURL() = 0;

	// Returns the background color.
	virtual cef_color_t GetBackgroundColor() = 0;

	// Returns true if the Views framework will be used.
	virtual bool UseViews() = 0;

	// Returns true if windowless (off-screen) rendering will be used.
	virtual bool UseWindowlessRendering() = 0;

	// Populate |settings| based on command-line arguments.
	virtual void PopulateSettings(CefSettings* settings) = 0;
	virtual void PopulateBrowserSettings(CefBrowserSettings* settings) = 0;
	virtual void PopulateOffscreenSettings(OffscreenRenderer::Settings* settings) = 0;

	// Returns the object used to create/manage RootWindow instances.
	virtual RootWindowManager* GetRootWindowManager() = 0;

protected:
	MainContext();
	virtual ~MainContext();

private:
	DISALLOW_COPY_AND_ASSIGN(MainContext);
};
#endif 