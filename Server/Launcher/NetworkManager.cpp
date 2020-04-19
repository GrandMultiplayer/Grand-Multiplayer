// System
#include <vector>
#include <chrono>

// RakNet
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"

//GrandM
#include "Common.h"
#include "Server.h"
#include "NetworkSync.h"
#include "ModelCache.h"
#include "Config.h"
#include "Master.h"

#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// API
#include "CAPI.h"
#include "API_Callback_Network.h"

// Self
#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
	// Get RakPeerInterface
	g_RakPeer = RakNet::RakPeerInterface::GetInstance();

	// Retrieve instances of RPC4, DirectoryDeltaTransfer and FileListTransfer
	g_RPC = RakNet::RPC4::GetInstance();
	g_DirTransfer = RakNet::DirectoryDeltaTransfer::GetInstance();
	g_FileTransfer = RakNet::FileListTransfer::GetInstance();

	// Attach RPC4, DirectoryDeltaTransfer and FileListTransfer to RakPeerInterface
	g_RakPeer->AttachPlugin(g_RPC);
	g_RakPeer->AttachPlugin(g_DirTransfer);
	g_RakPeer->AttachPlugin(g_FileTransfer);

	// Set the right transfer method
	g_DirTransfer->SetFileListTransferPlugin(g_FileTransfer);

	// Set our transfer directory and shared folder
	g_DirTransfer->SetApplicationDirectory(".//resources//client");
	g_DirTransfer->AddUploadsFromSubdirectory("");

	// RakPeerInterface Settings
	g_RakPeer->SetSplitMessageProgressInterval(100);
}


NetworkManager::~NetworkManager()
{
	// Stop RakNet, stops synchronization
	g_RakPeer->Shutdown(2000);

	// Detach RPC4, DirectoryDeltaTransfer and FileListTransfer from RakPeerInterface
	g_RakPeer->DetachPlugin(g_RPC);
	g_RakPeer->DetachPlugin(g_DirTransfer);
	g_RakPeer->DetachPlugin(g_FileTransfer);

	// Destroy RPC4, DirectoryDeltaTransfer and FileListTransfer
	RakNet::RPC4::DestroyInstance(g_RPC);
	RakNet::DirectoryDeltaTransfer::DestroyInstance(g_DirTransfer);
	RakNet::FileListTransfer::DestroyInstance(g_FileTransfer);

	// Destroy RakPeerInterface
	RakNet::RakPeerInterface::DestroyInstance(g_RakPeer);
}

bool NetworkManager::Start()
{
	LOG_PRINT("[NetworkManager] Starting...");
	RakNet::SocketDescriptor socketDescriptor;
	socketDescriptor.port = g_Config->GetPort();

	int Startup = g_RakPeer->Startup(g_Config->GetMaxPlayers(), &socketDescriptor, 1, 0);
	if (!Startup)
	{
		// Set all connection data after interface has started
		g_RakPeer->SetMaximumIncomingConnections(g_Config->GetMaxPlayers());

		if (!g_Config->GetPassword().empty())
			g_RakPeer->SetIncomingPassword(g_Config->GetPassword().c_str(), sizeof(g_Config->GetPassword().c_str()));

		g_RakPeer->SetLimitIPConnectionFrequency(true);
		g_RakPeer->SetTimeoutTime(15000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

		LOG_PRINT("[NetworkManager] Successfully started");
		if (g_DirTransfer->GetNumberOfFilesForUpload() > 0)
			LOG_PRINT("[NetworkManager] %i files suitable for upload to clients.", g_DirTransfer->GetNumberOfFilesForUpload());

		// Send data to the masterlist, so others can see that the server is online.
		Master::UpdateMaster();
		p_LastMasterUpdate = clock();
		return true;
	}

	LOG_ERROR("[NetworkManager] Startup error %i", Startup);
	return false;
}

void NetworkManager::Pulse()
{
	RakNet::Packet *g_Packet = NULL;
	while (g_Packet = g_RakPeer->Receive())
	{
		RakNet::BitStream g_BitStream(g_Packet->data + 1, g_Packet->length + 1, false);
		switch (g_Packet->data[0])
		{
		case ID_UNCONNECTED_PING:
			break;
		case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
			break;
		case ID_NEW_INCOMING_CONNECTION:
			break;
		case ID_DISCONNECTION_NOTIFICATION:
		{
			PlayerLeft(g_Packet->guid, 0);
			Master::UpdateMaster();
			p_LastMasterUpdate = clock();
			break;
		}
		case ID_CONNECTION_LOST:
		{
			PlayerLeft(g_Packet->guid, 1);
			Master::UpdateMaster();
			p_LastMasterUpdate = clock();
			break;
		}
		case ID_PACKET_MOVEMENT:
		{
			int id = -1, type = -1;
			g_BitStream.Read(type);
			g_BitStream.Read(id);

			if (id == -1)
			{
				LOG_WARNING("Recived movment data for an invalid entity from guid:%s", g_Packet->guid.ToString());
				break;
			}

			switch (type)
			{
			case GrandM::EntityType::Player:
			{
				for (unsigned int i = 0; i < g_Players.size(); i++)
				{
					if (g_Players[i].GetGUID() == g_Packet->guid)
					{
						g_Players[i].UpdateMovement(g_Packet);
						break;
					}
				}
				break;
			}
			case GrandM::EntityType::Vehicle:
			{
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetID() == id)
					{
						g_Vehicles[i].UpdateMovement(g_Packet);
						break;
					}
				}
				break;
			}
			default:
				LOG_WARNING("Movement Packet Default Reached #%i-%i", type, id);
				break;
			}

			break;
		}
		case ID_PACKET_PLAYER:
		{
			for (auto &ent : g_Players)
			{
				if (ent.GetGUID() == g_Packet->guid)
				{
					ent.Update(g_Packet);
					break;
				}
			}
			/*for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetGUID() == g_Packet->guid)
				{
					g_Players[i].Update(g_Packet);
					break;
				}
			}*/
			break;
		}
		case ID_PACKET_VEHICLE:
		{
			int t_Id;
			g_BitStream.Read(t_Id);

			for (unsigned int i = 0; i < g_Vehicles.size(); i++)
			{
				if (g_Vehicles[i].GetID() == t_Id)
				{
					g_Vehicles[i].UpdateVehicleData(g_Packet);
					break;
				}
			}

			break;
		}
		case ID_PACKET_OBJECT:
		{
			int t_Id;
			g_BitStream.Read(t_Id);

			for (unsigned int i = 0; i < g_Objects.size(); i++)
			{
				if (g_Objects[i].GetID() == t_Id)
				{
					g_Objects[i].Update(g_Packet);
					break;
				}
			}
			break;
		}
		case ID_CHAT_MESSAGE:
		{
			for (unsigned int i = 0; i < g_Players.size(); i++)
			{
				if (g_Players[i].GetGUID() == g_Packet->guid)
				{
					RakNet::RakWString message;
					g_BitStream.Read(message);

					if (message.C_String()[0] == '/')
					{
						for (unsigned int i = 0; i < g_ApiModules.size(); i++)
						{
							void *Instance = g_ApiModules[i].GetInstance();
							API::Network::OnPlayerCommand(Instance, g_Players[i], message.C_String());
						}
					}
					else
					{
						for (unsigned int i = 0; i < g_ApiModules.size(); i++)
						{
							void *Instance = g_ApiModules[i].GetInstance();
							API::Network::OnPlayerMessage(Instance, g_Players[i], message.C_String());
						}
					}
					return;
				}
			}
			break;
		}
		case ID_REQUEST_SERVER_SYNC:
		{
			// New connection
			NewIncomingConnection(g_Packet);

			// Ping Master
			Master::UpdateMaster();
			p_LastMasterUpdate = clock();

			break;
		}
		default:
			LOG_WARNING("Recived packet with ID %s", g_Packet->data[0]);
		}
		g_RakPeer->DeallocatePacket(g_Packet);
	}

	if (clock() - p_LastMasterUpdate > (120 * 1000))
	{
		Master::UpdateMaster();
		p_LastMasterUpdate = clock();
	}
}

void NetworkManager::NewIncomingConnection(RakNet::Packet  *packet)
{
	RakNet::BitStream bitStream(packet->data + 1, packet->length + 1, false);
	RakNet::RakString playerName, playerUid, versionMajor, versionMinor;

	bitStream.Read(playerName);
	bitStream.Read(playerUid);
	bitStream.Read(versionMajor);
	bitStream.Read(versionMinor);

	// IF the clients MAJOR & MINOR version is not the same as the servers version then close their connection and end the funtion.
	if (versionMajor.StrCmp(RakNet::RakString(MAJOR)) != 0 && versionMinor.StrCmp(RakNet::RakString(MINOR)) != 0)
	{
		RakNet::BitStream bitstream;
		bitstream.Write(RakNet::RakString(VERSION));
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal("WrongVersion", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false, false);

		LOG_PRINT("[NetworkManager::NewIncomingConnection] Invalid Version %s.%s != %s.%s for client %s:%s", versionMajor.C_String(), versionMinor.C_String(), MAJOR, MINOR, playerName.C_String(), playerUid.C_String());
		g_RakPeer->CloseConnection(packet->systemAddress, true);
		return;
	}

	// Create the player
	const int index = g_Players.size();
	CPlayer player(playerName.C_String(), playerUid.C_String(), packet->guid, packet->systemAddress);
	g_Players.push_back(player);

	// API::Network::OnPlayerConnecting Execute
	for (unsigned int i = 0; i < g_ApiModules.size(); i++)
	{
		void *Instance = g_ApiModules[i].GetInstance();
		if (!API::Network::OnPlayerConnecting(Instance, g_Players[index]))
		{
			g_RakPeer->CloseConnection(packet->systemAddress, true);
			return;
		}
	}

	// Tell the client to load the models
	ModelCache::LoadModels(packet->guid);

	// Sync the World
	NetworkSync::SyncServerWorld(packet->guid);

	// Tell the client they are done syncing & connecting
	// Also tell the client their entity ID
	RakNet::BitStream bitstream;
	bitstream.Write(player.GetID());
	bitstream.Write(RakNet::RakString(g_Config->GetServerName().c_str()));
	bitstream.Write(RakNet::RakString(g_Config->GetWebsite().c_str()));
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal("FinishedSync", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false, false);

	// API::Network::OnPlayerConnected Execute
	for (unsigned int i = 0; i < g_ApiModules.size(); i++)
	{
		void *Instance = g_ApiModules[i].GetInstance();
		API::Network::OnPlayerConnected(Instance, g_Players[index]);
	}
}

void NetworkManager::PlayerLeft(RakNet::RakNetGUID  guid, const int reason)
{
	for (unsigned int i = 0; i < g_Players.size(); i++)
	{
		if (g_Players[i].GetGUID() == guid)
		{
			for (unsigned int v = 0; v < g_Vehicles.size(); v++)
			{
				if (!g_Players[i].GetVehicle().IsNull() && g_Players[i].GetVehicle().GetID() == g_Vehicles[v].GetID())
				{
					g_Vehicles[v].SetOccupant(g_Players[i].GetVehicleSeat(), -1);
				}

				if (g_Vehicles[v].GetAssignee() == guid)
				{
					g_Vehicles[v].SetAssignee(RakNet::UNASSIGNED_RAKNET_GUID);
				}
			}

			// API::Network::OnPlayerConnected Execute
			for (unsigned int m = 0; m < g_ApiModules.size(); m++)
			{
				void *Instance = g_ApiModules[m].GetInstance();
				API::Network::OnPlayerDisconnected(Instance, g_Players[i], reason);
			}

			g_Players.erase(g_Players.begin() + i);
			g_Players.shrink_to_fit();

			break;
		}
	}

	// UnAssignment of Phsyics types
	for (unsigned int i = 0; i < g_Vehicles.size(); i++)
	{
		if (g_Vehicles[i].GetAssignee() == guid)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(g_Vehicles[i].GetID());
			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DropEntityAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
		}
	}

	for (unsigned int i = 0; i < g_Objects.size(); i++)
	{
		if (g_Objects[i].GetAssignee() == guid)
		{
			RakNet::BitStream bitstream;
			bitstream.Write(g_Objects[i].GetID());
			Server::GetServer()->GetNetworkManager()->GetRPC().Signal("DropEntityAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true, false);
		}
	}
}