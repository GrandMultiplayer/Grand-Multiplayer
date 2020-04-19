#include "stdafx.h"

#include <fstream>
#include <memory>
#include <TlHelp32.h>
#include <string>
#include <algorithm>
#include <vector>
#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>

// Vendor.LibCurl
//#include <LibCurl/curl/curl.h>
//#include "Json\json.h"

namespace {
	std::size_t callback(
		const char* in,
		std::size_t size,
		std::size_t num,
		std::string* out) {
		const std::size_t totalBytes(size * num);
		out->append(in, totalBytes);
		return totalBytes;
	}
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

std::string RSLauncherFolder()
{
	char SteamDir[MAX_PATH];
	Util::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\WOW6432Node\\Rockstar Games\\Launcher", "InstallFolder", NULL, SteamDir, sizeof(SteamDir));

	return SteamDir;
}

std::string SocialClubFolder()
{
	char SteamDir[MAX_PATH];
	Util::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\WOW6432Node\\Rockstar Games\\Rockstar Games Social Club", "InstallFolder", NULL, SteamDir, sizeof(SteamDir));

	return SteamDir;
}


std::string SteamFolder()
{
	char SteamDir[MAX_PATH];
	Util::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\WOW6432Node\\Valve\\Steam", "InstallPath", NULL, SteamDir, sizeof(SteamDir));

	return SteamDir;
}

std::vector<std::string> SteamLibraryFolders()
{
	std::vector<std::string> folders;
	
	std::string steamFolder = SteamFolder();
	folders.push_back(steamFolder);
	std::string configFile = steamFolder + "\\config\\config.vdf";

	std::regex regex = std::regex("BaseInstallFolder[^\"]*\"\\s*\"([^\"]*)\"");



	std::ifstream fs;
	fs.open(configFile);

	std::string line;
	while (std::getline(fs, line))
	{
		std::smatch res;
		if (std::regex_search(line, res, regex))
		{
			folders.push_back(res[1]);
		}
	}

	fs.close();

	return folders;
}

std::vector<std::string> get_directories(const std::string& s)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(s))
		if (p.is_directory())
			r.push_back(p.path().string());
	return r;
}

std::string GetSteamGTAVInstall()
{
	std::vector<std::string> slf = SteamLibraryFolders();

	for (int i = 0; i < slf.size(); i++) 
	{
		DWORD ftyp = GetFileAttributesA(std::string(slf[i] + "\\steamapps\\common").c_str());
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY) 
		{
			for (auto& p : std::filesystem::directory_iterator(slf[i] + "\\steamapps\\common"))
			{
				if (p.path().string().find("Grand Theft Auto V") != std::string::npos) {
					std::string path = p.path().string();
					replace(path, "\\\\", "\\");
					return path;
				}
			}
		}
	}

	return "NOPE";
}


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

DWORD FindProcessId(std::string processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processSnapshot);
	return 0;
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

HANDLE GetProcessByName(char* name)
{
	DWORD pid = 0;

	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			if (strcmp(process.szExeFile, name) == 0)
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	if (pid != 0)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	}

	// Not found
	return NULL;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	SetConsoleTitle("Grand Multiplayer Launcher");
	//::SetWindowPos(::FindWindowEx(0, 0, "Grand Multiplayer Launcher", 0), HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	char InstallExe[MAX_PATH];
	char CorePath[MAX_PATH];
	char LaunchPath[MAX_PATH];
	char HookPath[MAX_PATH];
	char CurDir[MAX_PATH];
	char GameDir[MAX_PATH];


	std::string CmdLine = lpCmdLine;
	bool customDir = false;
	GetCurrentDirectory(sizeof(CurDir), CurDir);

	// SC
	//if (Util::ReadRegistryString(HKEY_LOCAL_MACHINE, INFO_GAME_LOC_REG_WAREHOUSE, INFO_GAME_LOC_REG2, NULL, GameDir, sizeof(GameDir)))
	//{
	//	std::string str = GameDir;
	//	str.erase((str.end() - 5), str.end());
	//	str += "\\Grand Theft Auto V";

	//	strcpy(GameDir, str.c_str());
	//}
	//// Steam
	//else if (Util::ReadRegistryString(HKEY_LOCAL_MACHINE, INFO_GAME_LOC_REG_STEAM, INFO_GAME_LOC_REG3, NULL, GameDir, sizeof(GameDir)))
	//{
	//	std::string str = GameDir;
	//	str.erase((str.end() - 5), str.end());

	//	strcpy(GameDir, str.c_str());
	//}

	bool Steam = false;
	bool Other = false;
	HKEY subKey = nullptr;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam\\Apps\\" INFO_GAME_STEAMAPPID, 0, KEY_READ, &subKey) == ERROR_SUCCESS) 
	{
		std::cout << "Found Steam Version of the game and will use that!" << std::endl;
		//std::cout << GetSteamGTAVInstall() << std::endl;

		sprintf_s(InstallExe, "%s\\"/*GTA5.exe"*/ INFO_GAME_EXECUTABLE, GetSteamGTAVInstall().c_str());
		




		Steam = true;
	}
	else if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{5EFC6C07-6B87-43FC-9524-F9E967241741}", 0, KEY_READ, &subKey) == ERROR_SUCCESS &&
		Util::ReadRegistryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{5EFC6C07-6B87-43FC-9524-F9E967241741}", "InstallLocation", NULL, GameDir, sizeof(GameDir)))
	{
		std::cout << "Found Warehouse Version of the game and will use that!" << std::endl;
		Other = true;

		sprintf_s(InstallExe, "%s\\"/*GTA5.exe"*/ INFO_GAME_EXECUTABLE, GameDir);
		//std::cout << GameDir << std::endl;

		// Trim string from junk
		std::string str = GameDir;
		//str.erase((str.end() - 3), str.end());

		//str.erase(str.begin(), (str.begin() + 1));

		//strcpy(GameDir, str.c_str());

		std::cout << GameDir << std::endl;
	}
	// Could not find Install Path need help
	else
	{
		std::cout << "Unable to find a standard install of the game, Searching..." << std::endl;

		// This key is only created/needed if the standard install keys can't be found for some reason.
		if (!Util::ReadRegistryString(HKEY_CURRENT_USER, INFO_LOC_REG, INFO_LOC_REG2, NULL, GameDir, sizeof(GameDir)) || !Util::Exists(GameDir))
		{
			if (Util::ShowMessageBox("We could not find your " INFO_GAME_NAME " install directory, do you wish to set it now?",
				(MB_ICONEXCLAMATION | MB_OKCANCEL)) == IDOK) {

				BROWSEINFOA browseInfo = { 0 };
				browseInfo.lpszTitle = INFO_NAME " - Pick a Directory";
				ITEMIDLIST * pItemIdList = SHBrowseForFolderA(&browseInfo);

				if (pItemIdList != NULL) {
					// Get the name of the selected folder
					if (SHGetPathFromIDList(pItemIdList, GameDir))
						customDir = true;

					// Free any memory used
					IMalloc * pIMalloc = 0;
					if (SUCCEEDED(SHGetMalloc(&pIMalloc))) {
						pIMalloc->Free(pItemIdList);
						pIMalloc->Release();
					}
				}
			}

			if (!customDir) {
				Util::ShowMessageBox("Failed to retrieve the install directory from registry or browser window. Cannot launch " INFO_NAME ".", MB_ICONEXCLAMATION);
				return 1;
			}
		}
	}

	//sprintf_s(InstallExe, "%s\\"/*GTA5.exe"*/ INFO_GAME_EXECUTABLE, GameDir);
	sprintf_s(HookPath, "%s\\Hook.dll", CurDir);
	sprintf_s(LaunchPath, "%s\\Client.dll", CurDir);
	sprintf_s(CorePath, "%s\\" INFO_CLIENT_CORE, CurDir);

	if (customDir)
		Util::WriteRegistryString(HKEY_CURRENT_USER, INFO_LOC_REG, INFO_LOC_REG2, GameDir, strlen(GameDir));

	if (!Util::Exists(HookPath) || !Util::Exists(CorePath) || !Util::Exists(HookPath))
		return Util::ShowMessageBox("Key Grand Multiplayer Client files are missing, Please verify your Files!", MB_ICONEXCLAMATION);


	if (Steam) 
	{
		char* pValue;
		size_t len;
		_dupenv_s(&pValue, &len, "PATH");

		std::stringstream ss;
		ss << "PATH=" << pValue << ";" << CurDir << '\0';
		std::string env = ss.str();

		_putenv(env.c_str());

		HWND hwndConsole = FindWindowA(NULL, "Grand Multiplayer Launcher"); // abouve does nothing
		ShellExecute(hwndConsole, 0, "steam://rungameid/" INFO_GAME_STEAMAPPID, 0, NULL, SW_SHOW);
	}
	else if (Other /*|| Steam*/)
	{
		char* pValue;
		size_t len;
		_dupenv_s(&pValue, &len, "PATH");

		std::stringstream ss;
		ss << "PATH=" << pValue << ";" << CurDir << /*";" << RSLauncherFolder().c_str() << ";" << GetSteamGTAVInstall().c_str() << ";" << SocialClubFolder().c_str() <<*/'\0';
		std::string env = ss.str();

		STARTUPINFO siStartupInfo;
		PROCESS_INFORMATION piProcessInfo;
		memset(&siStartupInfo, 0, sizeof(siStartupInfo));
		memset(&piProcessInfo, 0, sizeof(piProcessInfo));
		siStartupInfo.cb = sizeof(siStartupInfo);

		if (!CreateProcess(InstallExe, NULL, NULL, NULL, false, CREATE_SUSPENDED, NULL/*LPTSTR(env.c_str())*/, NULL/*GameDir*/, &siStartupInfo, &piProcessInfo)) {
			Util::ShowMessageBox("Failed to start " INFO_GAME_EXECUTABLE ". Cannot launch " INFO_NAME ".");
			return 1;
		}

		ResumeThread(piProcessInfo.hThread);
	}

	///


	//if (!Util::Exists(InstallExe))
	//{
	//	//if(Util::ShowMessageBox("Failed to find " INFO_GAME_EXECUTABLE " in directory. Cannot launch " INFO_NAME ".", MB_ICONEXCLAMATION | MB_OK) == MB_OK);
	//	if (Util::ShowMessageBox("Failed to find GTA5.exe in directory. Cannot launch " INFO_NAME ".", MB_ICONEXCLAMATION | MB_OK) == MB_OK);
	//	{
	//		Util::ShowMessageBox(GameDir, MB_ICONEXCLAMATION | MB_OK);


	//		BROWSEINFOA browseInfo = { 0 };
	//		browseInfo.lpszTitle = INFO_NAME " - Select your GTAV directory";
	//		ITEMIDLIST * pItemIdList = SHBrowseForFolderA(&browseInfo);

	//		if (pItemIdList != NULL)
	//		{
	//			// Get the name of the selected folder
	//			if (SHGetPathFromIDList(pItemIdList, GameDir))
	//				customDir = true;

	//			// Free any memory used
	//			IMalloc * pIMalloc = 0;
	//			if (SUCCEEDED(SHGetMalloc(&pIMalloc)))
	//			{
	//				pIMalloc->Free(pItemIdList);
	//				pIMalloc->Release();
	//			}
	//		}
	//	}
	//}

	/*if (customDir)
		Util::WriteRegistryString(HKEY_CURRENT_USER, INFO_LOC_REG, INFO_LOC_REG2, GameDir, strlen(GameDir));*/

	/*if (!Util::Exists(HookPath) || !Util::Exists(CorePath) || !Util::Exists(HookPath))
		return Util::ShowMessageBox("Key Client files are missing, Please verify your Files!", MB_ICONEXCLAMATION);*/

	/*if (Steam) 
	{
		ShellExecute(0, 0, "steam://rungameid/" INFO_GAME_STEAMAPPID, 0, 0, SW_SHOW);
	}
	else
	{
		STARTUPINFO siStartupInfo;
		PROCESS_INFORMATION piProcessInfo;
		memset(&siStartupInfo, 0, sizeof(siStartupInfo));
		memset(&piProcessInfo, 0, sizeof(piProcessInfo));
		siStartupInfo.cb = sizeof(siStartupInfo);


		if (!CreateProcess(InstallExe, NULL, NULL, NULL, true, CREATE_SUSPENDED, NULL, GameDir, &siStartupInfo, &piProcessInfo)) {
			Util::ShowMessageBox("Failed to start " INFO_GAME_EXECUTABLE ". Cannot launch " INFO_NAME ".");
			return 1;
		}

		ResumeThread(piProcessInfo.hThread);
	}*/

	HANDLE hProcess = GetProcessByName("GTA5.exe");
	while (hProcess == NULL)
	{
		Sleep(1000);
		hProcess = GetProcessByName("GTA5.exe");
	}


	HWND windowHandle = NULL;
	while (windowHandle == NULL)
	{
		windowHandle = FindWindow("grcWindow", NULL);
		Sleep(100);
	}

	while (!InjectDynamicLibrary(MyGetProcessId("GTA5.exe"), LaunchPath))
		Sleep(500);
	
	FreeConsole();
}
