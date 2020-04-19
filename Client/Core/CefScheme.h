#ifndef __CEFSCHEME_H__
#define __CEFSCHEME_H__

#include <vector>

#include "include/cef_scheme.h"

namespace Scheme
{

	// Register the custom scheme name/type. This must be done in all processes.
	// See browser/scheme_test.h for creation/registration of the custom scheme
	// handler which only occurs in the browser process. Called from
	// client_app_delegates_common.cc.
	void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar, std::vector<CefString>& cookiable_schemes);

}

#endif 