#include <Windows.h>
#include <TlHelp32.h>

#include <sstream>

/*
	This DLL's Primary Objective is to add the Grand Multiplayer install Directory
	into the games local PATH Environment Variable avoiding the need to permantly
	set the System's PATH Env Var which could have potential unwanted conflicts.
*/

bool InjectDynamicLibrary(DWORD processId, char* dllPath)
{
	// Open a new handle to the target process
	HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	if (hTargetProcess) // if the handle is valid
	{
		// Kernel32.dll is always mapped to the same address in each process
		// So we can just copy the address of it & LoadLibraryA in OUR process and
		// expect it to be same in the remote process too.
		LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		// We must allocate more memory in the target process to hold the path for our dll in it's addresspace.
		LPVOID LoadPath = VirtualAllocEx(hTargetProcess, 0, strlen(dllPath),
			MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		// Write the string name of our DLL in the memory allocated 
		WriteProcessMemory(hTargetProcess, (LPVOID)LoadPath, dllPath, strlen(dllPath), NULL);

		// Load our DLL 
		CreateRemoteThread(hTargetProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, (LPVOID)LoadPath, NULL, NULL);

		CloseHandle(hTargetProcess);
		return true;
	}
	return false;
}

DWORD MyGetProcessId(LPCTSTR ProcessName) // non-conflicting function name
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt))
	{ // must call this first
		do
		{
			if (!lstrcmpi(pt.szExeFile, ProcessName))
			{
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap); // close handle on failure
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));

		std::string filePath = buffer;
		filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size());


		char *pValue;
		size_t len;
		_dupenv_s(&pValue, &len, "PATH");

		std::stringstream ss;
		ss << "PATH=" << pValue << ";" << filePath.c_str() << '\0';
		std::string env = ss.str();

		_putenv(env.c_str());

		Sleep(100);

		filePath = filePath + "\\Core.dll";

		InjectDynamicLibrary(MyGetProcessId(L"GTA5.exe"), (char*)filePath.c_str());

		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
	}
	break;
	}

	return TRUE;
}