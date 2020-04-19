#ifndef __CDISCORD_H__
#define __CDISCORD_H__
// Discord
#include "discord_rpc.h"

class CDiscord
{
private:
	static const char* APPLICATION_ID;
	enum GameStatus
	{
		NILL = -1,
		InMenus = 0,
		Connected
	};

	static GameStatus gameStatus;
public:
	static void discordInit();
	static void handleDiscordReady(const DiscordUser* connectedUser);
	static void handleDiscordDisconnected(int errcode, const char* message);
	static void handleDiscordError(int errcode, const char* message);
	static void handleDiscordJoin(const char* secret);
	static void handleDiscordJoinRequest(const DiscordUser* request);
	static void updateDiscordPresence();
};

#endif