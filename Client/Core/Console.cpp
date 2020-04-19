#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#else
#define _BSD_SOURCE

#include <dlfcn.h>
#include <sys/time.h>
#include <unistd.h>
#endif
#include <stdlib.h>

#include "Console.h"

namespace Utility {

	static Console g_Console;

	void Console::Allocate() {

		if ( isAllocated ) {
			return;
		}

		/*AllocConsole();

		freopen( "CONOUT$", "w", stdout );
		freopen( "CONOUT$", "w", stderr );*/

		//outputHandle = GetStdHandle( STD_OUTPUT_HANDLE );

		const int width = 110;
		const int height = 30;

		// Add some more scrolling
		/*COORD size;
		size.X = width;
		size.Y = height * 10;
		SetConsoleScreenBufferSize( outputHandle, size );*/

		// Resize our console window
		/*SMALL_RECT rect;
		rect.Left = rect.Top = 0;
		rect.Right = width - 1;
		rect.Bottom = height - 1;
		SetConsoleWindowInfo( outputHandle, TRUE, &rect );*/

		isAllocated = true;
	}

	void Console::DeAllocate() {

		if ( !isAllocated ) {
			return;
		}

		//FreeConsole();
	}

	void Console::Clear() {

		system( "cls" );
	}

#ifdef _WIN32
	void Console::SetTextColor( const int color ) {

		if ( !isAllocated ) {
			return;
		}

		CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
		//GetConsoleScreenBufferInfo( outputHandle, &screenBuffer );

		WORD attributes = screenBuffer.wAttributes & ~FOREGROUND_RED & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
		attributes |= color;

		//SetConsoleTextAttribute( outputHandle, attributes );
	}
#endif

	Console * GetConsole() {

		return &g_Console;
	}
}