#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#else
#define _BSD_SOURCE

#include <dlfcn.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <time.h>
#include <iostream>
#include <fstream>
#include <stdarg.h>

#include "CConfig.h"

#include "General.h"
#include "Console.h"
#include "Log.h"

namespace Utility {

	static Log g_Log;

	Log::Log() 
	{
		struct tm timeStruct;
		time_t currTime = time(NULL);

#ifdef _WIN32
		localtime_s(&timeStruct, &currTime);
#else
		localtime_r(&currTime, &timeStruct);
#endif

		char buff[64];
		sprintf(buff, "%02d_%02d_%02d_Server", timeStruct.tm_year + 1900, timeStruct.tm_mon + 1, timeStruct.tm_mday);

		LogFile = buff;
#ifdef _WIN32
		logTypeToColorMap[Utility::LogTypePrint] =		ConsoleForeground::WHITE;
		logTypeToColorMap[Utility::LogTypeDebug] =		ConsoleForeground::GRAY;
		logTypeToColorMap[Utility::LogTypeWarning] =	ConsoleForeground::YELLOW;
		logTypeToColorMap[Utility::LogTypeError] =		ConsoleForeground::RED;
#else
		logTypeToColorMap[Utility::LogTypePrint] =		"\x1B[97m";
		logTypeToColorMap[Utility::LogTypeDebug] =		"\x1B[37m";
		logTypeToColorMap[Utility::LogTypeWarning] =	"\x1B[33m";
		logTypeToColorMap[Utility::LogTypeError] =		"\x1B[31m";
#endif

		logTypeToFormatMap[Utility::LogTypePrint] =		"";
		logTypeToFormatMap[Utility::LogTypeDebug] =		" [Debug]";
		logTypeToFormatMap[Utility::LogTypeWarning]	=	" [Warning]";
		logTypeToFormatMap[Utility::LogTypeError] =		" [Error]";
	}

	Log::~Log() {

	}

	void Log::Write( eLogType logType, const char * fmt, ... ) {

		char buf[2048] = { 0 };
		va_list va_alist;
		
		va_start( va_alist, fmt );
		vsprintf( buf, fmt, va_alist );
		va_end( va_alist );

#ifdef _WIN32
		GetConsole()->SetTextColor( logTypeToColorMap[logType] );
#endif
		char buff2[2048] = { 0 };
		sprintf(buff2, "%s %s", GetTimeFormatted().c_str(), buf);

		// Print to console
#ifdef _WIN32
		std::cout << buff2 << std::endl;
#else
		std::cout << logTypeToColorMap[logType] << buff2 << std::endl;
#endif

		if (logType == LogTypeDebug)
		{
			// Print to console
			if (CConfig::IsDebugLog())
				printf(buff2);
		}
		else
		{
			printf(buff2);
		}


		sprintf( buff2, "%s%s %s\n", GetTimeFormatted().c_str(), logTypeToFormatMap[logType].c_str(), buf );
		// Write to log file
		LogToFile( buff2 );
	}

	const std::string Log::GetTimeFormatted() const {
#ifdef _WIN32
		struct tm timeStruct;
		time_t currTime = time(NULL);
		localtime_s(&timeStruct, &currTime);
#else
		time_t currTime = time(NULL);
		struct tm timeStruct;
		localtime_r(&currTime, &timeStruct);
#endif

		char buff[48];
		sprintf( buff, "[%02d:%02d:%02d]", timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec );
		return buff;
	}

	void Log::LogToFile( const char * buff ) 
	{
		const std::string fileName = GetOurModuleFolder() + "\\Client.log";

		std::ofstream logFile;
		if (firstEntry)
		{
			logFile.open(fileName);
			firstEntry = false;
		}
		else
		{
			logFile.open(fileName, std::ios_base::app);
		}

		logFile << buff;
	}

	Log * GetLog()
	{
		return &g_Log;
	}
}