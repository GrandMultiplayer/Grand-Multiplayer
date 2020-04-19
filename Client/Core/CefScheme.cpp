#include "include/cef_scheme.h"

#include "CefScheme.h"

namespace Scheme
{
	void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar,
		std::vector<CefString>& cookiable_schemes)
	{
		registrar->AddCustomScheme("client", true, false, false, false, true, false);
	}
}
