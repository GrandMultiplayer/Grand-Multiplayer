#ifndef __WORLD_H__
#define __WORLD_H__
#include <vector>
#include <string>

class CWorld
{
private:
	static struct sTime
	{
		int	hour;
		int	minute;
		int second;
	} Time;

	static struct sWeather
	{
		std::string		Weather;
		bool			snow;
	} Weather;

	struct IPL
	{
		std::string		ipl;
		bool			enabled;
	};

	static bool blackout;

public:
	static std::vector<IPL> g_IPLs;

	static void			Init();

	static sTime		GetTime() { return Time; }
	static void			SetTime(int hour, int minute, int second);

	static sWeather		GetWeather() { return Weather; }
	static void			SetWeather(std::string weather);
	static void			SetGroundSnow(const bool state);
	static const bool	IsGroundSnow() { return Weather.snow; }

	static void			LoadIPL(std::string ipl);
	static void			UnloadIPL(std::string ipl);

	static void			SetBlackout(const bool state);
	static const bool	IsBlackout() { return blackout; }
};
#endif