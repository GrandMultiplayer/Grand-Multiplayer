#include <string>

// RakNet
#include <FileListTransferCBInterface.h>
#include <BitStream.h>
#include <PacketPriority.h>
#include <RakNetTypes.h>
#include <MessageIdentifiers.h>
#include <GetTime.h>
#include <RakNetStatistics.h>

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include <SharedUtility.h>

#include "Common.h"
#include "APIStructs.h"
#include "GUI.h"
#include "CefMainContext.h"
#include "CGui.h"
#include "CRPCManager.h"
#include "CLocalPlayer.h"
#include "CWorld.h"
#include "CConfig.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"

#include "DirectXHook.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "CNetworkManager.h"

RakNet::RakPeerInterface *CNetworkManager::g_RakPeer = nullptr;
RakNet::RPC4 *CNetworkManager::g_RPC = nullptr;
RakNet::DirectoryDeltaTransfer *CNetworkManager::g_DirTransfer = nullptr;
RakNet::FileListTransfer *CNetworkManager::g_FileTransfer = nullptr;

std::string CNetworkManager::g_lastIP;
std::string CNetworkManager::g_lastPass;
int CNetworkManager::g_lastPort;
RakNet::SystemAddress CNetworkManager::g_SystemAddr;

CNetworkManager::ConenctionState CNetworkManager::g_ConnectionState = CNetworkManager::Disconnected;

bool CNetworkManager::triggerDisconnect = false;

void CNetworkManager::Initialize()
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

	char buffer[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));
	std::string filePath = buffer;
	filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size());

	filePath += "//resources";

	// Set our transfer directory
	g_DirTransfer->SetApplicationDirectory(filePath.c_str());

	// Set the right transfer method
	g_DirTransfer->SetFileListTransferPlugin(g_FileTransfer);

	// RakPeerInterface Settings
	g_RakPeer->SetSplitMessageProgressInterval(100);

	g_ConnectionState = CNetworkManager::Disconnected;
}

void CNetworkManager::Destroy()
{
	// Stop RakNet, stops synchronization
	g_RakPeer->Shutdown(2000);

	// Detach RPC4 from RakPeerInterface
	g_RakPeer->DetachPlugin(g_RPC);

	// Detach RPC4, DirectoryDeltaTransfer and FileListTransfer from RakPeerInterface
	g_RakPeer->DetachPlugin(g_RPC);
	g_RakPeer->DetachPlugin(g_DirTransfer);
	g_RakPeer->DetachPlugin(g_FileTransfer);

	// Destroy RPC4, DirectoryDeltaTransfer and FileListTransfer
	RakNet::RPC4::DestroyInstance(g_RPC);
	RakNet::DirectoryDeltaTransfer::DestroyInstance(g_DirTransfer);
	RakNet::FileListTransfer::DestroyInstance(g_FileTransfer);
}

bool CNetworkManager::Start()
{
	LOG_DEBUG("[CNetworkManager] Started.");

	// Return whether Startup worked or not
	return (g_RakPeer->Startup(1, &RakNet::SocketDescriptor(), 1, THREAD_PRIORITY_NORMAL) == RakNet::RAKNET_STARTED);
}

void CNetworkManager::Stop()
{
	// Disconnect if we're connected
	if (g_ConnectionState == CNetworkManager::Connected)
		Disconnect();

	// Stop our RakPeerInterface
	g_RakPeer->Shutdown(500);

	LOG_DEBUG("[CNetworkManager] Stopped.");
}

void CNetworkManager::Connect(const char *ip, const char *pass, int port)
{
	LOG_DEBUG("[CNetworkManager] Attempting to connect to %s:%i.", ip, port);

	// Disconnect if we're already connected
	if (g_ConnectionState == CNetworkManager::Connected)
		Disconnect();

	// Set our last connection so we can connect again later and we set our state to connecting
	SetLastConnection(ip, pass, port);
	g_ConnectionState = CNetworkManager::Connecting;

	// Connect to the server, leaving the result
	int Result;

	if ((pass != NULL) && (pass[0] == '\0'))
		Result = g_RakPeer->Connect(ip, port, 0, 0);
	else
		Result = g_RakPeer->Connect(ip, port, pass, sizeof(pass));


	// Check if connection failed, then set our state to failed
	if (Result != 0)
	{
		LOG_DEBUG("[CNetworkManager] Failed to connect, errorcode: %i.", Result);

		CGui::ShowMainMenu(true, true);

		std::ostringstream message;
		message << "Failed to connect, errorcode: " << Result;
		CGui::ShowConnectionError("Failed to Connect", message.str());

		g_ConnectionState = CNetworkManager::Failed;
		return;
	}
	else
	{
		LOG_DEBUG("[CNetworkManager] Connected %i.", Result);
	}
}

void CNetworkManager::Disconnect()
{
	if (triggerDisconnect)
		triggerDisconnect = false;

	// Don't do anything if we're not connected
	if (g_ConnectionState == CNetworkManager::Disconnected) {
		return;
	}

	// Set our state to disconnected
	g_ConnectionState = CNetworkManager::Disconnected;

	// Stop RakPeer from accepting anymore incoming packets
	if (g_RakPeer->GetConnectionState(g_SystemAddr) == RakNet::ConnectionState::IS_CONNECTED) {
		g_RakPeer->CloseConnection(g_SystemAddr, true);
	}

	// Unregister RPCs
	CRPCManager::UnregisterRPCMessages();

	// Reinitialize our RakPeerInterface
	Stop();
	Start();

	// Register RPCs
	CRPCManager::RegisterRPCMessages();

	// Clean the server GUID
	g_SystemAddr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	BRAIN::TASK_STAND_STILL(CLocalPlayer::GetPed(), 0);
	
	CLocalPlayer::MainMenu();

	CWorld::Destroy();

	LOG_DEBUG("[CNetworkManager] Disconnected.");
}

void CNetworkManager::Closed()
{
	// Don't do anything if we're not connected
	if (g_ConnectionState == CNetworkManager::Disconnected) {
		return;
	}

	// Set our state to disconnected
	g_ConnectionState = CNetworkManager::Disconnected;

	// Clean the server GUID
	g_SystemAddr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	BRAIN::TASK_STAND_STILL(CLocalPlayer::GetPed(), 0);

	CLocalPlayer::MainMenu();

	CWorld::Destroy();

	LOG_DEBUG("[CNetworkManager] Disconnected.");
}

class TransferCallback : public RakNet::FileListTransferCBInterface
{
public:
	bool OnFile(
		OnFileStruct *onFileStruct)
	{
		assert(onFileStruct->byteLengthOfThisFile >= onFileStruct->bytesDownloadedForThisFile);
		LOG_DEBUG("%i. (0%%) %i/%i %s %ib / %ib", onFileStruct->setID, onFileStruct->fileIndex + 1, onFileStruct->numberOfFilesInThisSet, onFileStruct->fileName, onFileStruct->byteLengthOfThisFile, onFileStruct->byteLengthOfThisSet);

		// Return true to have RakNet delete the memory allocated to hold this file.
		// False if you hold onto the memory, and plan to delete it yourself later
		return true;
	}

	virtual void OnFileProgress(FileProgressStruct *fps)
	{
		assert(fps->onFileStruct->byteLengthOfThisFile >= fps->onFileStruct->bytesDownloadedForThisFile);
		LOG_DEBUG("%i (%i%%) %i/%i %s %ib / %ib", fps->onFileStruct->setID, (int)(100.0*(double)fps->partCount / (double)fps->partTotal),
			fps->onFileStruct->fileIndex + 1,
			fps->onFileStruct->numberOfFilesInThisSet,
			fps->onFileStruct->fileName,
			fps->onFileStruct->byteLengthOfThisFile,
			fps->onFileStruct->byteLengthOfThisSet,
			fps->firstDataChunk);

		std::string loadmessage = "Downloading file ";
		loadmessage += fps->onFileStruct->fileName;
		loadmessage += "( ";
		loadmessage += std::to_string((int)(100.0*(double)fps->partCount / (double)fps->partTotal));
		loadmessage += "%% )";

		CGui::SetLoadingText(loadmessage);
	}

	virtual bool OnDownloadComplete(DownloadCompleteStruct *dcs)
	{
		LOG_DEBUG("Download complete.");

		// Returning false automatically deallocates the automatically allocated handler that was created by DirectoryDeltaTransfer
		return false;
	}

} transferCallback;

static uint64_t netUsageSec;
void CNetworkManager::Pulse()
{
	// Don't do anything if we're disconnected
	//if (g_ConnectionState == CONSTATE_DISC || g_ConnectionState == CONSTATE_FAIL)
		//return;

	if (triggerDisconnect)
		return Disconnect();

	uint64_t now = getticktime();
	if (netUsageSec + 1000 <= now)
	{
		RakNet::RakNetStatistics *rss;
		char message[2048];
		rss = CNetworkManager::GetInterface()->GetStatistics(CNetworkManager::GetInterface()->GetSystemAddressFromIndex(0));

		CGui::Debug::outbound = (float)rss->valueOverLastSecond[RakNet::ACTUAL_BYTES_SENT] / 1000.f;
		CGui::Debug::inbound = (float)rss->valueOverLastSecond[RakNet::ACTUAL_BYTES_RECEIVED] / 1000.f;
		CGui::Debug::loss = rss->packetlossLastSecond*100.0f;

		netUsageSec = getticktime();
	}
	//

	RakNet::Packet *g_Packet = NULL;
	while (g_Packet = g_RakPeer->Receive())
	{
		RakNet::BitStream g_BitStream(g_Packet->data + 1, g_Packet->length + 1, false);

		switch (g_Packet->data[0])
		{
			case ID_UNCONNECTED_PONG:
			{
				RakNet::TimeMS time;
				RakNet::BitStream bsIn(g_Packet->data, g_Packet->length, false);
				bsIn.IgnoreBytes(1);
				bsIn.Read(time);

				std::string address = g_Packet->systemAddress.ToString(true);
				std::string ip = address.substr(0, address.find("|"));
				std::string port = address.substr(address.find("|") + 1, address.size());

				//LOG_DEBUG("ID_UNCONNECTED_PONG from SystemAddress %s.", g_Packet->systemAddress.ToString(true));
				//LOG_DEBUG("Ping is %i", (unsigned int)(RakNet::GetTimeMS() - time));

				CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();
				frame->ExecuteJavaScript(CefString("SetPing('" + ip + "', " + port + ", " + std::to_string((unsigned int)(RakNet::GetTimeMS() - time)) + ");"), frame->GetURL(), 0);

				break;
			}
			case ID_ADVERTISE_SYSTEM:
			{
				break;
			}
			case ID_IP_RECENTLY_CONNECTED:
			{
				LOG_DEBUG("[CNetworkManager] Failed to connect, recently connected.");

				CGui::ShowConnectionError("Failed to Connect", "Failed to connect, recently connected.");

				CNetworkManager::Disconnect();
				break;
			}
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			{
				LOG_DEBUG("[CNetworkManager] Failed to connect, incompatible protocol version.");

				CGui::ShowConnectionError("Failed to Connect", "Failed to connect, incompatible protocol version.");

				CNetworkManager::Closed();
				break;
			}
			case ID_ALREADY_CONNECTED:
			{
				LOG_DEBUG("[CNetworkManager] Failed to connect, already connected.");

				CGui::ShowConnectionError("Failed to Connect", "Failed to connect, already connected.");

				CNetworkManager::Closed();
				break;
			}
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				LOG_DEBUG("[CNetworkManager] Failed to connect, max clients.");

				CGui::ShowConnectionError("Failed to Connect", "Failed to connect, no free slots.");

				CNetworkManager::Closed();
				break;
			}
			case ID_INVALID_PASSWORD:
			{
				LOG_DEBUG("[CNetworkManager] Failed to connect, invalid password.");

				CGui::ShowConnectionError("Failed to Connect", "Failed to connect, invalid password.");

				CNetworkManager::Closed();
				break;
			}
			case ID_CONNECTION_ATTEMPT_FAILED:
			{
				LOG_DEBUG("[CNetworkManager] Failed to connect, server not responding");

				CGui::ShowConnectionError("Failed to Connect", "Failed to connect, server not responding.");

				CNetworkManager::Closed();
				break;
			}
			case ID_CONNECTION_BANNED:
			{
				LOG_DEBUG("[CNetworkManager] Failed to connect, banned.");

				CGui::ShowConnectionError("Failed to Connect", "Failed to connect, you are banned from this server.");

				CNetworkManager::Closed();
				break;
			}
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				LOG_DEBUG("[CNetworkManager] Connection Request Accepted.");

				CGui::ShowLoadingScreen(true, true);

				// Set the server Adress
				g_SystemAddr = g_Packet->systemAddress;

				CGui::SetLoadingText("Syncing with Server...");

				CGui::SetConnected(true);

				// Set our last packet update so it sends our own packets too
				//CLocalPlayer::SetLastSync(timeGetTime());

				char buffer[MAX_PATH];//always use MAX_PATH for filepaths
				GetModuleFileNameA(GetModuleHandleA("Client.dll"), buffer, sizeof(buffer));

				std::string filePath = buffer;
				filePath.erase(filePath.find("Client.dll"), std::string("Client.dll").size()); //removes the Client.dll part leaving you with the file path not including the file itself

				char subdir[256] = "";
				char outputSubdir[256] = "./";
				
				unsigned short setId;
				setId = g_DirTransfer->DownloadFromSubdirectory(subdir, outputSubdir, true, g_SystemAddr, &transferCallback, HIGH_PRIORITY, 0, 0);
				if (setId == (unsigned short)-1)
				{
					LOG_WARNING("[CNetworkManager] Download failed - Host unreachable.");
				}
				else
				{
					LOG_DEBUG("[CNetworkManager] Downloading set %i", setId);
				}

				RakNet::BitStream bitstream;
				bitstream.Write((unsigned char)ID_REQUEST_SERVER_SYNC);
				bitstream.Write(RakNet::RakString(CConfig::GetName().c_str()));
				bitstream.Write(RakNet::RakString(CLocalPlayer::GetSecretKey().c_str()));
				bitstream.Write(RakNet::RakString(MAJOR));
				bitstream.Write(RakNet::RakString(MINOR));
				CNetworkManager::GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false);

				break;
			}
			case ID_DISCONNECTION_NOTIFICATION:
			{
				CGui::ShowConnectionError("Connection Lost", "Server has closed its connection.");

				LOG_DEBUG("[CNetworkManager] Connection Closed.");
				CNetworkManager::Closed();
				break;
			}
			case ID_CONNECTION_LOST:
			{
				LOG_DEBUG("[CNetworkManager] Connection Lost.");

				CGui::ShowConnectionError("Connection Lost", "Connection to the server timmedout.");

				CNetworkManager::Closed();
				break;
			}
			case ID_PACKET_MOVEMENT:
			{
				int id, type;
				g_BitStream.Read(type);
				g_BitStream.Read(id);

				if (id == -1)
				{
					LOG_WARNING("Recived movment data for an invalid entity from guid:%s", g_Packet->guid.ToString());
					break;
				}

				switch (type)
				{
				case CEntity::ePlayer:
				{
					for (unsigned int i = 0; i < g_Players.size(); i++)
					{
						if (g_Players[i].GetID() == id)
						{
							g_Players[i].UpdateMovement(g_Packet);
							break;
						}
					}
				}
				case CEntity::eVehicle:
				{
					for (unsigned int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetID() == id)
						{
							g_Vehicles[i].UpdateMovement(g_Packet);
							g_Vehicles[i].Interpolate();
							break;
						}
					}
				}
				default:
					break;
				}

				break;
			}
			case ID_PACKET_PLAYER:
			{
				int entity;
				g_BitStream.Read(entity);

				bool exists = false;
				if (!g_Players.empty())
				{
					for (int i = 0; i < g_Players.size(); i++)
					{
						if (g_Players[i].GetID() == entity)
						{
							g_Players[i].Update(g_Packet);
							exists = true;
						}
					}
				}
				if (!exists)
				{
					int entityId;
					g_BitStream.Read(entityId);

					const int index = g_Players.size();

					CPlayer newPlayer(entityId, "User");
					g_Players.push_back(newPlayer);
					g_Players[index].Update(g_Packet);
				}
				break;
			}
			case ID_PACKET_VEHICLE:
			{
				int t_Id;
				g_BitStream.Read(t_Id);

				if (!g_Vehicles.empty())
				{
					for (int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetID() == t_Id)
						{
							g_Vehicles[i].UpdateVehicleData(g_Packet);
							break;
						}
					}
				}
			}
			case ID_PACKET_OBJECT:
			{
				int t_Id;
				g_BitStream.Read(t_Id);

				bool exists = false;
				if (!g_Objects.empty())
				{
					for (int i = 0; i < g_Objects.size(); i++)
					{
						if (g_Objects[i].GetID() == t_Id)
						{
							g_Objects[i].Update(g_Packet);
							exists = true;
							break;
						}
					}
				}
				break;
			}
			case ID_PLAYER_LEFT:
			{
				int t_Id;
				g_BitStream.Read(t_Id);

				if (!g_Players.empty())
				{
					for (int p = 0; p < g_Players.size(); p++)
					{
						if (g_Players[p].GetID() == t_Id)
						{
							g_Players[p].Destroy();
							g_Players.erase(g_Players.begin() + p);
							g_Players.shrink_to_fit();
							break;
						}
					}
				}
				break;
			}
			case ID_REQUEST_SERVER_SYNC:
			{
				LOG_DEBUG("[CNetworkManager] Connection Sync Request Accepted.");

				// Set our state to connected
				g_ConnectionState = CNetworkManager::Connected;

				// No longer need these
				/*ENTITY::FREEZE_ENTITY_POSITION(CLocalPlayer::GetPed(), false);
				ENTITY::SET_ENTITY_VISIBLE(CLocalPlayer::GetPed(), true, 0);
				ENTITY::SET_ENTITY_ALPHA(CLocalPlayer::GetPed(), 0, false);
				ENTITY::RESET_ENTITY_ALPHA(CLocalPlayer::GetPed());*/

				UI::DISPLAY_RADAR(true);
				UI::DISPLAY_HUD(true);
				UI::DISPLAY_CASH(true);

				CLocalPlayer::SetControllable(true);
				CGui::GiveFocus(false);

				break;
			}
			default:
			{
				LOG_WARNING("[CNetworkManager] def.");
				break;
			}
		}
		g_RakPeer->DeallocatePacket(g_Packet);
	}
}

