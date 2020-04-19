#ifndef __SERVER_H__
#define __SERVER_H__
#include <memory>
#include <chrono>
#include <atomic>

#include "NetworkManager.h"

class Server
{
private:
	static std::unique_ptr<Server> pServer;
	std::unique_ptr<NetworkManager> g_NetworkManager;

	bool			p_Active;

	bool			p_ShowFPS;
	unsigned long	p_LastFPSUpdateTime;
	unsigned long	p_FrameCount;
	unsigned long	p_FPS;
	unsigned int	p_MaxFPS;
	
	unsigned int	p_SyncRate;

	std::chrono::system_clock::time_point timeNow;
	std::chrono::system_clock::time_point timeLast;
	std::chrono::duration<double, std::milli> timeTook;

public:
	Server();
	~Server();

	NetworkManager *GetNetworkManager() { return g_NetworkManager.get(); }

	bool Load(int argc, char ** argv);
	void Stop();

	void Update();
	void SetActive(bool bActive) { p_Active = bActive; }
	bool IsActive() { return p_Active; }

	void ShowFPS();

	static void Input(std::atomic<bool>& run);

	static void		Create() { pServer = std::unique_ptr<Server>(new Server); }
	static Server	*GetServer() { return pServer.get(); }
	unsigned int	GetSyncRate() { return p_SyncRate; }
};
#endif
#ifndef _WIN32
unsigned int timeGetTime();
#endif