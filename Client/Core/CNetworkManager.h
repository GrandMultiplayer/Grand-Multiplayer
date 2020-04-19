#ifndef __CNETWORKMANAGER_H__
#define __CNETWORKMANAGER_H__
#include <string>

// RakNet
#include <RakPeerInterface.h>
#include <RPC4Plugin.h>
#include <FileListTransfer.h>
#include <DirectoryDeltaTransfer.h>

class CNetworkManager
{
private:
	// RakNet Interfaces
	static RakNet::RakPeerInterface *g_RakPeer;
	static RakNet::RPC4 *g_RPC;
	static RakNet::DirectoryDeltaTransfer *g_DirTransfer;
	static RakNet::FileListTransfer *g_FileTransfer;

	/* Previous/Current connection */
	static std::string g_lastIP;
	static std::string g_lastPass;
	static int g_lastPort;
	static RakNet::SystemAddress g_SystemAddr;

public:
	enum ConenctionState
	{
		Disconnected,
		Connecting,
		Connected,
		Failed,
	};

	static bool triggerDisconnect;

	static void Initialize();
	static void Destroy();

	static bool Start();
	static void Stop();

	static void Connect(const char *ip, const char *pass, int port);
	static void Disconnect();
	static void Closed();

	static void Pulse();

	static void SetLastConnection(const char *ip, const char *pass, int port) { g_lastIP = ip; g_lastPass = pass; g_lastPort = port; }

	static const std::string GetLastIP() { return g_lastIP; }
	static const std::string GetLastPass() { return g_lastPass; }
	static const int GetLastPort() { return g_lastPort; }

	static RakNet::RakPeerInterface *GetInterface() { return g_RakPeer; }
	static RakNet::SystemAddress GetSystemAddress() { return g_SystemAddr; }
	static RakNet::RPC4& GetRPC() { return *g_RPC; };

	static ConenctionState g_ConnectionState; /* 0 = Disconnected - 1 = Connecting - 2 = Connected - 3 = Connection Failed */
};
#endif