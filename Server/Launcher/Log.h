#ifndef __LOG_H__
#define __LOG_H__
#include <map>


#define LOG_PRINT( fmt, ...)	Utility::GetLog()->Write( Utility::eLogType::LogTypePrint,	fmt, ##__VA_ARGS__ )
#define LOG_DEBUG( fmt, ...)	Utility::GetLog()->Write( Utility::eLogType::LogTypeDebug,	fmt, ##__VA_ARGS__ )
#define LOG_WARNING( fmt, ...)	Utility::GetLog()->Write( Utility::eLogType::LogTypeWarning,	fmt, ##__VA_ARGS__ )
#define LOG_ERROR( fmt, ...)	Utility::GetLog()->Write( Utility::eLogType::LogTypeError,	fmt, ##__VA_ARGS__ )

#define PRINT( color, toFile, fmt, ...)	Utility::GetLog()->Write2( color, toFile,	fmt, ##__VA_ARGS__ )

namespace Utility {

	enum eLogType {
		LogTypePrint,
		LogTypeDebug,
		LogTypeWarning,
		LogTypeError,
	};

	typedef std::map<int32_t, int32_t> intIntMap;
	typedef std::map<int32_t, std::string> intStringMap;

	class Log {
	public:

		Log();
		~Log();

		void				Write( eLogType logType, const char * fmt, ... );
		void				Write2( const int color, const bool toFile, const char * fmt, ...);
				
	private:
		std::string			LogFile;

		void				LogToFile( const char * buff );

		const std::string	GetTimeFormatted() const;

#ifdef _WIN32
		intIntMap			logTypeToColorMap;
#else
		intStringMap		logTypeToColorMap;
#endif
		intStringMap		logTypeToFormatMap;
		bool				firstEntry = true;
	};

	Log *					GetLog();
}

#endif // __LOG_H__