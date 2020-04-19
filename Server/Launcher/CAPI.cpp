// System
#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#else
#define _BSD_SOURCE

#include <dlfcn.h>
#include <sys/time.h>
#include <unistd.h>
#endif
#include <string>
#include <vector>

// Self
#include "CAPI.h"

// GrandM
#include "Common.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

CAPI::CAPI()
{
	Instance = nullptr;
}


CAPI::~CAPI()
{
	Instance = nullptr;
}

bool CAPI::Load()
{
#ifdef _WIN32
	free(Instance);
	Instance = ::LoadLibraryA(std::string("./plugin/" + Module + LIBRARY_EXTENSION).c_str());
#else
	Instance = dlopen(std::string("./plugin/" + Module + LIBRARY_EXTENSION).c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif
	if (!Instance)
	{
#ifdef _WIN32
		LOG_ERROR("[API] %s could not be loaded.", ModuleName().c_str());
#else
		LOG_ERROR("[API] Cannot open library %s, %s", ModuleName().c_str(), dlerror());
#endif
		return false;
	}

	LOG_PRINT("[API] %s loaded.", ModuleName().c_str());
	return true;
}

bool CAPI::Unload()
{
	if (Instance) 
	{
#ifdef _WIN32
		bool state = FreeLibrary((HMODULE)Instance);

		if (state)
#else
		bool state = dlclose(Instance);

		if (!state)
#endif
		{
			Instance = nullptr;
			LOG_PRINT("[API] %s unloaded.", ModuleName().c_str());
			return true;
		}

		LOG_ERROR("[API] %s failed to unload.", ModuleName().c_str());
		return false;
	}
	return false;
}

bool CAPI::Initialize()
{
	if (Instance) 
	{
		typedef void(*API_Initialize_t)();
#ifdef WIN32
		API_Initialize_t API_Initialize = (API_Initialize_t)::GetProcAddress((HMODULE)Instance, "API_Initialize");
#else
		API_Initialize_t API_Initialize = (API_Initialize_t)dlsym(Instance, "API_Initialize");
#endif
		
		if (!API_Initialize)
			return false;

		API_Initialize();
		
		LOG_PRINT("[API] %s initialized.", ModuleName().c_str());
		return true;
	}
	return false;
}

bool CAPI::Close()
{
	if (Instance)
	{
		typedef void(*API_Close_t)();
#ifdef WIN32
		API_Close_t API_Close = (API_Close_t)::GetProcAddress((HMODULE)Instance, "API_Close");
#else
		API_Close_t API_Close = (API_Close_t)dlsym(Instance, "API_Close");
#endif
		
		if (!API_Close)
			return false;

		API_Close();

		LOG_PRINT("[API] %s closed.", ModuleName().c_str());
		return true;
	}
	return false;
}

bool CAPI::OnTick()
{
	if (Instance)
	{
		typedef void(*API_OnTick_t)();
#ifdef WIN32
		API_OnTick_t API_OnTick = (API_OnTick_t)::GetProcAddress((HMODULE)Instance, "API_OnTick");
#else
		API_OnTick_t API_OnTick = (API_OnTick_t)dlsym(Instance, "API_OnTick");
#endif
		
		if (!API_OnTick)
			return false;

		API_OnTick();
		return true;
	}
	return false;
}