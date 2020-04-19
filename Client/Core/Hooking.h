#ifndef __HOOKING_H__
#define __HOOKING_H__
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <memory>
#include <fstream>
#include <Shlwapi.h>
#include <Detours.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <map>
#include <set>

#include <deque>
#include <functional>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shlwapi.lib")

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace Hooking
{
	template <typename F>
	class DetourHook;

	template <typename R, typename... T>
	class DetourHook<R(T...)>
	{
	public:
		using F = R(*)(T...);

	protected:
		F _original;
		F _detour;
		const char* _name;
		DETOUR_TRAMPOLINE* _trampoline;

	public:
		DetourHook()
			: _original(nullptr)
			, _detour(nullptr)
			, _trampoline(nullptr)
			, _name(nullptr)
		{
		}

		~DetourHook()
		{
			if (_trampoline)
			{
				UnHook();
			}
		}

		bool Hook(F original, F detour, const char* name)
		{
			_original = original;
			_detour = detour;
			_name = name;

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			DetourAttachEx(reinterpret_cast<void**>(&_original), _detour, &_trampoline, NULL, NULL);

			if (DetourTransactionCommit() != NO_ERROR)
			{
				LOG_ERROR("Could not hook '%s'", name);
				DetourTransactionAbort();
				return nullptr;
			}

			LOG_DEBUG("Hooked %s at %llX", name, normalise_base(original));

			return true;
		}

		bool UnHook()
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			DetourDetach(reinterpret_cast<void**>(&_original), _detour);

			bool success = DetourTransactionCommit() == NO_ERROR;

			if (!success)
			{
				LOG_ERROR("Could not unhook '%s'", _name);
				DetourTransactionAbort();
			}
			else
			{
				LOG_DEBUG("Unhooked %s at %llX", _name, normalise_base(_original));
			}

			_original = nullptr;
			_detour = nullptr;
			_trampoline = nullptr;

			return success;
		}

		R operator()(T&... args)
		{
			return reinterpret_cast<F>(_trampoline)(std::forward<T>(args)...);
		}
	};

	template <typename R, typename... T>
	class DetourHook<R(*)(T...)> : public DetourHook<R(T...)> {};

	template <typename R, typename... T>
	class DetourHook<R(&)(T...)> : public DetourHook<R(T...)> {};

	template<typename InputType, typename ReturnType>
	InputType RCast(InputType Input, ReturnType Ret)
	{
		return reinterpret_cast<decltype(Input)>(Ret);
	}

	std::uintptr_t normalise_base(mem::handle address, const std::uintptr_t& offset = 0x140000000);
};

namespace Utility
{
	template <typename T>
	inline void SafeRelease(T *&p)
	{
		if (nullptr != p)
		{
			p->Release();
			p = nullptr;
		}
	}
};

#endif