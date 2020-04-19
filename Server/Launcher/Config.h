#ifndef __CONFIG_H__
#define __CONFIG_H__
class Config
{
	struct CConnection
	{
		std::string Ip;
		int Port;
		std::string Pass;
		bool Announce;
	} Connection;

	struct CLimits
	{
		int MaxPlayers;
		int MaxFPS;
		int tickRate;
	} Limits;

	struct CInformation
	{
		std::string Name;
		std::string Gamemode;
		std::string Website;
	} Information;

	struct CPlugins
	{
		std::string Plugins;
	} Plugins;

public:
	bool Read();

	std::string GetIp() { return Connection.Ip; }
	const int GetPort() { return Connection.Port; }
	std::string GetPassword() { return Connection.Pass; }
	bool GetAnnounce() { return Connection.Announce; }
	unsigned short GetMaxPlayers() { return Limits.MaxPlayers; }
	std::string GetServerName() { return Information.Name; }
	std::string GetPlugins() { return Plugins.Plugins; }
	const int GetFPS() { return Limits.MaxFPS; }
	const std::string GetGamemode() { return Information.Gamemode; }
	const std::string GetWebsite() { return Information.Website; }
};
extern Config*	g_Config;
#endif