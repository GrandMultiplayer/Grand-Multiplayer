#ifndef __CWORLD_H__
#define __CWORLD_H__
#include <vector>

class CWorld {
private:
	struct IPL
	{
		std::string		ipl;
		bool			enabled;
		bool			defaultState;
	};

	static struct wTime
	{
		int hour;
		int minute;
		int second;
	} Time;

public:
	static std::vector<IPL> g_IPLs;

	static void		Initialize();
	static void		Destroy();

	static void		LoadIPL(std::string ipl);
	static void		UnloadIPL(std::string ipl);

	static void		LoadModel(const std::string model);
	static void		LoadModel(const int model);

	static void		SetBlackout(const bool state);
	
	static void		SetTime(const int hour, const int minute, const int second);
	static const int GetHour() { return Time.hour; }
	static const int GetMinute() { return Time.minute; }
	static const int GetSecond() { return Time.second; }
};
#endif