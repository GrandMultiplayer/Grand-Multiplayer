// Self
#include "API_Log.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

namespace API
{
	namespace Log
	{
		void Print(const char * message)
		{
			LOG_PRINT(message);
		}

		void Debug(const char * message)
		{
			LOG_DEBUG(message);
		}

		void Warning(const char *message)
		{
			LOG_WARNING(message);
		}

		void Error(const char * message)
		{
			LOG_ERROR(message);
		}
	}
}