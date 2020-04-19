#include <winsock2.h>

#include "discord_rpc.h"

#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CDiscord.h"

const char* CDiscord::APPLICATION_ID = "382312319826264064";
CDiscord::GameStatus CDiscord::gameStatus = CDiscord::GameStatus::NILL;

void CDiscord::discordInit()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = handleDiscordReady;
	handlers.disconnected = handleDiscordDisconnected;
	handlers.errored = handleDiscordError;
	handlers.joinGame = handleDiscordJoin;
	//handlers.spectateGame = handleDiscordSpectate;
	handlers.joinRequest = handleDiscordJoinRequest;
	Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}


void CDiscord::handleDiscordReady(const DiscordUser* connectedUser)
{
	LOG_DEBUG("Discord: connected to user %s#%s - %s",
		connectedUser->username,
		connectedUser->discriminator,
		connectedUser->userId);
}

void CDiscord::handleDiscordDisconnected(int errcode, const char* message)
{
	LOG_DEBUG("Discord: disconnected (%d: %s)", errcode, message);
}

void CDiscord::handleDiscordError(int errcode, const char* message)
{
	LOG_DEBUG("Discord: error (%d: %s)", errcode, message);
}

void CDiscord::handleDiscordJoin(const char* secret)
{
	LOG_DEBUG("Discord: join (%s)", secret);
}

void CDiscord::handleDiscordJoinRequest(const DiscordUser* request)
{
	//int response = -1;
	//char yn[4];
	LOG_DEBUG("Discord: join request from %s - %s - %s",
		request->username,
		request->avatar,
		request->userId);

	/*do
	{
		printf("Accept? (y/n)");
		if (!prompt(yn, sizeof(yn)))
		{
			break;
		}

		if (!yn[0])
		{
			continue;
		}

		if (yn[0] == 'y')
		{
			response = DISCORD_REPLY_YES;
			break;
		}

		if (yn[0] == 'n')
		{
			response = DISCORD_REPLY_NO;
			break;
		}
	} while (1);
	if (response != -1)
	{
		Discord_Respond(request->userId, response);
	}*/
}


void CDiscord::updateDiscordPresence()
{
	GameStatus curStatus = GameStatus::NILL;

	if(CNetworkManager::g_ConnectionState == 0)
		curStatus = GameStatus::InMenus;
	else
		curStatus = GameStatus::Connected;

	if (gameStatus != curStatus)
	{
		gameStatus = curStatus;

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));

		if (gameStatus == GameStatus::InMenus)
		{
			discordPresence.state = "In Menus";
		}
		else if(gameStatus == GameStatus::Connected)
		{
			discordPresence.details = "Playing on:";
			discordPresence.state = "Some Server Name";
			discordPresence.joinSecret = "join";
		}

		discordPresence.largeImageKey = "grandmbg1";
		//discordPresence.startTimestamp = StartTime;
		//discordPresence.endTimestamp = time(0) + 5 * 60;
		//discordPresence.largeImageText = "grandm";
		//discordPresence.smallImageKey = "grandm-s";
		//discordPresence.partyId = "party1234";
		//discordPresence.partySize = 0;
		//discordPresence.partyMax = 0;
		discordPresence.matchSecret = "xyzzy";
		//discordPresence.spectateSecret = "look";
		discordPresence.instance = 0;
		Discord_UpdatePresence(&discordPresence);
	}
}