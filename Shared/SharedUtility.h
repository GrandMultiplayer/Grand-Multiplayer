#ifndef __SHAREDUTILS__
#define __SHAREDUTILS__

#include <cstddef>
#ifdef _WIN32
#include <codecvt>
#endif
#include <sstream>
#include <stdint.h>
#include <iomanip>

template <typename T, std::size_t N>
inline
std::size_t SizeOfArray(const T(&)[N])
{
	return N;
}

class CString 
{
public:
	static const std::wstring utf8ToUtf16(const std::string& str)
	{
#ifdef _WIN32
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(str);
#else
		std::wostringstream wstm;
		wstm.imbue(std::locale("en_US.UTF-8"));
		const std::ctype<wchar_t>& ctfacet =
			std::use_facet< std::ctype<wchar_t> >(wstm.getloc());
		for (size_t i = 0; i<str.size(); ++i)
			wstm << ctfacet.widen(str[i]);
		return wstm.str();
#endif
	}

	static const std::string utf16ToUtf8(const std::wstring& wstr)
	{
#ifdef _WIN32
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.to_bytes(wstr);
#else
		std::ostringstream stm;
		stm.imbue(std::locale("en_US"));
		const std::ctype<char>& ctfacet =
			std::use_facet< std::ctype<char> >(stm.getloc());
		for (size_t i = 0; i<wstr.size(); ++i)
			stm << ctfacet.narrow(wstr[i], 0);
		return stm.str();
#endif
	}

	template <typename T>
	static const std::string to_string_with_precision(const T a_value, const int n = 6)
	{
		std::ostringstream out;
		out << std::setprecision(n) << a_value;
		return out.str();
	}
};

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include < time.h >

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

uint64_t getticktime();

#endif // !