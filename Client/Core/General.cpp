#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#else
#define _BSD_SOURCE
#define PATH_MAX 1024

#include <dlfcn.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <iostream>

#include "General.h"

#ifdef _WIN32
static HMODULE ourModule;
#endif

const std::string Utility::GetRunningExecutableFolder() {
#ifndef _WIN32
	char fileName[PATH_MAX];
	readlink("/proc/self/", fileName, PATH_MAX);
#else
	char fileName[MAX_PATH];
	GetModuleFileNameA(NULL, fileName, MAX_PATH);
#endif

	std::cout << fileName << std::endl;

	std::string currentPath = fileName;
	return currentPath.substr( 0, currentPath.find_last_of( "\\" ) );
}

const std::string Utility::GetOurModuleFolder() 
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));

	std::string filePath = buffer;
	filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size());

	return filePath;
}
