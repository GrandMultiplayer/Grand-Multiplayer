#ifndef __NETWORKMANAGER_H__
#define __NETWORKMANAGER_H__
#include <RPC4Plugin.h>
#include <RakPeerInterface.h>
#include <FileListTransfer.h>
#include <DirectoryDeltaTransfer.h>

class NetworkManager
{
private:
	RakNet::RakPeerInterface		*g_RakPeer;
	RakNet::RPC4					*g_RPC;
	RakNet::DirectoryDeltaTransfer	*g_DirTransfer;
	RakNet::FileListTransfer		*g_FileTransfer;

	unsigned long		p_LastMasterUpdate;

public:
	NetworkManager();
	~NetworkManager();

	bool Start();
	void Pulse();
	void PlayerLeft(RakNet::RakNetGUID guid, const int reason);

	void NewIncomingConnection(RakNet::Packet *g_Packet);

	RakNet::RakPeerInterface *GetInterface() { return g_RakPeer; }
	RakNet::RPC4& GetRPC() { return *g_RPC; }
};
#endif