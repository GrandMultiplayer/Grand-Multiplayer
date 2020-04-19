#include <WinSock2.h>
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

#include "Pattern.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "Hooking.h"

//HooksMapType g_hooks;
//
//VOID Hooking::RemoveDetour(PVOID* ppTarget, PVOID pHandler)
//{
//	if (DetourTransactionBegin() != NO_ERROR)
//		return;
//
//	if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
//	{
//		DetourTransactionCommit();
//		return;
//	}
//
//	if (DetourDetach(ppTarget, pHandler) != NO_ERROR)
//	{
//		DetourTransactionCommit();
//		return;
//	}
//
//	if (DetourTransactionCommit() != NO_ERROR)
//	{
//		DetourTransactionAbort();
//		return;
//	}
//
//	auto pair = g_hooks.find(ppTarget);
//	if (pair == g_hooks.end())
//	{
//
//		LOG_ERROR("Could not find function hook '%llX' to remove", normalise_base(ppTarget));
//		return;
//	}
//
//	LOG_DEBUG("Unhooked function pointer at '%llX'", normalise_base(ppTarget));
//	g_hooks.erase(pair);
//}

std::uintptr_t Hooking::normalise_base(mem::handle address, const std::uintptr_t& offset)
{
	auto module = mem::module::main();

	if (module.contains(address))
	{
		address = address.translate(module.base(), offset);
	}

	return address.as<std::uintptr_t>();
};

// Natives
/*
PVOID GAME_WAIT = NULL;
PVOID MY_WAIT(scrNativeCallContext *cxt)
{
	if (auto pThread = GetActiveThread())
	{
		auto pHash = pThread->ThreadContext.ScriptHash;
		auto pState = pThread->ThreadContext.State;

		if (pHash == g_ThreadHash && pState == ThreadStateRunning)
		{
			g_ScriptManagerThread.DoRun();
		}
	}

	return reinterpret_cast<decltype(&MY_WAIT)>(GAME_WAIT)(cxt);
}

BOOL Hooking::Natives()
{
	return TRUE
		// native hooks	
		&& NativeDetour(0x4EDE34FBADD967A6, &MY_WAIT, &GAME_WAIT)
		;
}

BOOL Hooking::NativeDetour(uint64_t hash, PVOID pHandler, PVOID* ppTarget)
{
	auto native = NativeInvoker::NativeInfo(hash);

	if (native.NewHash)
	{
		auto handler = NativeInvoker::GetNativeHandler(native);
		if (handler)
		{
			*ppTarget = (PVOID*)handler;

			if (g_hooks.find(ppTarget) != g_hooks.end())
			{
				LOG_ERROR("Native 0x%I64X is already hooked at '%p'", native, ppTarget);
				return TRUE;
			}

			if (DetourTransactionBegin() != NO_ERROR) return FALSE;

			if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
			{
				DetourTransactionCommit();
				return FALSE;
			}

			PDETOUR_TRAMPOLINE pTrampoline = NULL;

			if (DetourAttachEx(ppTarget, pHandler, &pTrampoline, NULL, NULL) != NO_ERROR)
			{
				DetourTransactionCommit();
				return FALSE;
			}

			if (DetourTransactionCommit() != NO_ERROR)
			{
				DetourTransactionAbort();
				return FALSE;
			}

			g_hooks[ppTarget] = pHandler;

			LOG_DEBUG("Hooked Native %s (0x%I64X) at '%p'", native.Name, native.NewHash, ppTarget);

			return TRUE;
		}
	}

	LOG_DEBUG("Failed to find 0x%016llX", native);

	return FALSE;
}
*/