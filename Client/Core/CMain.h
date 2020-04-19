#ifndef __CMAIN_H__
#define __CMAIN_H__
#include "Memory.h"

class CMain
{
private:
	static void GetMACaddress();
public:
	static void Start();
	static void Cleaner();
	static void Addit();

	static void FailPatterns(const char* name, Memory::pattern ptn);

	static void InitializeGame(HMODULE hModule);

	static eGameState	GetGameState();
	static uint64_t		GetWorldPtr();
	static uint64_t		GetReplayPtr();
	static BlipList*	GetBlipList();
};
#endif