#ifndef __APICEF_H__
#define __APICEF_H__
#pragma once

#include "Common.h"
#include "API_Objects.h"

namespace API
{
	namespace CEF
	{
		DLL_PUBLIC_C void LoadURL(Objects::Entity player, const char * frame, const char * url, const char * appcode = "", bool remote = false);
		DLL_PUBLIC_C void ExecuteJavaScript(Objects::Entity player, const char * frame, const char * call);
	}
}
#endif