#include <winsock2.h>
#include <Windows.h>
#include <cstdint>
#include <Psapi.h>

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "MemoryAccess.h"

MemoryAccess::MemoryAccess()
{

	// DO NOT THING WE NEED THIS ANY MORE BUT WILL LEAVE IT FOR NOW
	const uintptr_t patternAddress = FindPattern(EntityPoolOpcodePattern, EntityPoolOpcodeMask);

	// 3 bytes are opcode and its first argument, so we add it to get relative address to patternAddress. 7 bytes are length of opcode and its parameters.
	sAddressEntityPool = reinterpret_cast<MemoryPool **>(*reinterpret_cast<int *>(patternAddress + 3) + patternAddress + 7);
}

int MemoryAccess::HandleToIndex(int Handle)
{
	return Handle >> 8; // == Handle / 256
}

uint32_t MemoryAccess::GetMemValue(int handle, int offset) const
{
	uintptr_t addr = mem::GetAddressOfEntity(handle);
	if (addr == 0)
	{
		return 0;
	}
	return *reinterpret_cast<uint32_t*>(addr + offset);
}

uintptr_t MemoryAccess::FindPattern(const char* pattern, const char* mask)
{
	MODULEINFO modInfo = { nullptr };
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &modInfo, sizeof(MODULEINFO));

	const char* start_offset = reinterpret_cast<const char *>(modInfo.lpBaseOfDll);
	const uintptr_t size = static_cast<uintptr_t>(modInfo.SizeOfImage);

	intptr_t pos = 0;
	const uintptr_t searchLen = static_cast<uintptr_t>(strlen(mask) - 1);

	for (const char* retAddress = start_offset; retAddress < start_offset + size; retAddress++)
	{
		if (*retAddress == pattern[pos] || mask[pos] == '?')
		{
			if (mask[pos + 1] == '\0')
			{
				return (reinterpret_cast<uintptr_t>(retAddress) - searchLen);
			}

			pos++;
		}
		else
		{
			pos = 0;
		}
	}

	return 0;
}

uintptr_t MemoryAccess::GetAddressOfItemInPool(MemoryPool* PoolAddress, int Handle)
{
	if (PoolAddress == nullptr)
	{
		return 0;
	}

	const int index = HandleToIndex(Handle);
	const int flag = PoolAddress->BoolAdr[index]; // flag should be equal to 2 if everything is ok

												  // parity check? (taken from ScriptHookDotNet for IV
	if (flag & 0x80 || flag != (Handle & 0xFF))
	{
		return 0;
	}

	return (PoolAddress->ListAddr + index * PoolAddress->ItemSize);
}

namespace mem
{

	uintptr_t(*GetAddressOfEntity)(int entity) = nullptr;
	uintptr_t(*GetModelInfo)(unsigned int modelHash, int* index) = nullptr;

	void init()
	{
		auto addr = FindPattern("\x83\xF9\xFF\x74\x31\x4C\x8B\x0D\x00\x00\x00\x00\x44\x8B\xC1\x49\x8B\x41\x08",
			"xxxxxxxx????xxxxxxx");
		if (!addr) LOG_ERROR("Couldn't find GetAddressOfEntity");
		GetAddressOfEntity = reinterpret_cast<uintptr_t(*)(int)>(addr);

		addr = FindPattern("\x0F\xB7\x05\x00\x00\x00\x00"
			"\x45\x33\xC9\x4C\x8B\xDA\x66\x85\xC0"
			"\x0F\x84\x00\x00\x00\x00"
			"\x44\x0F\xB7\xC0\x33\xD2\x8B\xC1\x41\xF7\xF0\x48"
			"\x8B\x05\x00\x00\x00\x00"
			"\x4C\x8B\x14\xD0\xEB\x09\x41\x3B\x0A\x74\x54",
			"xxx????xxxxxxxxxxx????"
			"xxxxxxxxxxxxxx????xxxxxxxxxxx");
		if (!addr) LOG_ERROR("Couldn't find GetModelInfo");
		GetModelInfo = reinterpret_cast<uintptr_t(*)(unsigned int modelHash, int *index)>(addr);
	}

	uintptr_t FindPattern(const char* pattern, const char* mask)
	{
		MODULEINFO modInfo = { nullptr };
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &modInfo, sizeof(MODULEINFO));

		const char* start_offset = reinterpret_cast<const char *>(modInfo.lpBaseOfDll);
		const uintptr_t size = static_cast<uintptr_t>(modInfo.SizeOfImage);

		intptr_t pos = 0;
		const uintptr_t searchLen = static_cast<uintptr_t>(strlen(mask) - 1);

		for (const char* retAddress = start_offset; retAddress < start_offset + size; retAddress++)
		{
			if (*retAddress == pattern[pos] || mask[pos] == '?')
			{
				if (mask[pos + 1] == '\0')
				{
					return (reinterpret_cast<uintptr_t>(retAddress) - searchLen);
				}

				pos++;
			}
			else
			{
				pos = 0;
			}
		}

		return 0;
	}
}