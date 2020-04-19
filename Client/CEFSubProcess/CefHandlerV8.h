#ifndef __CEFHANDLERV8_H__
#define __CEFHANDLERV8_H__

#include "include/cef_v8.h"

class CefHandlerV8 : public CefV8Handler
{
public:
	CefHandlerV8() {}

	bool Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception) OVERRIDE;

	IMPLEMENT_REFCOUNTING(CefHandlerV8);
};
#endif