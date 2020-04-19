#ifndef __MEMORYACCESS_H__
#define __MEMORYACCESS_H__

struct MemoryPool {
	uintptr_t ListAddr;
	char* BoolAdr;
	int MaxCount;
	int ItemSize;
};

class MemoryAccess {
public:
	MemoryAccess();

	static int HandleToIndex(int Handle);

	uintptr_t GetAddressOfEntity(int Handle) const;
	uint32_t GetMemValue(int handle, int offset) const;
	static uintptr_t FindPattern(const char* pattern, const char* mask);

private:
	static uintptr_t GetAddressOfItemInPool(MemoryPool* PoolAddress, int Handle);
	const char* EntityPoolOpcodeMask = "xxx????xxxxxxx";
	const char* EntityPoolOpcodePattern = "\x4C\x8B\x0D\x00\x00\x00\x00\x44\x8B\xC1\x49\x8B\x41\x08";
	MemoryPool** sAddressEntityPool = nullptr;
};


namespace mem
{
	void init();
	uintptr_t FindPattern(const char* pattern, const char* mask);
	extern uintptr_t(*GetAddressOfEntity)(int entity);
	extern uintptr_t(*GetModelInfo)(unsigned int modelHash, int* index);
}
#endif