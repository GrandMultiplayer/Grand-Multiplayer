// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

#ifdef __GRANDMSERVER__
#include "../Server/Launcher/Server.h"
#include "../Server/Launcher/Console.h"
#include "../Server/Launcher/General.h"
#include "../Server/Launcher/Log.h"
#endif
#ifdef __GRANDMCLIENT__
#include "../Client/Core/CNetworkManager.h"
#include "../Client/Core/Console.h"
#include "../Client/Core/General.h"
#include "../Client/Core/Log.h"
#endif

#include "SyncValue.h"

///
/// * Float Sync Variable
///
#pragma region SyncFloat
void SyncFloat::Set(float _v, RakNet::RakNetGUID sender, int ent, int type)
{
	if (var != _v)
	{
		LOG_DEBUG("<SyncFloat> Updating var %s to  %f.", varName, _v);

		var = _v;

		RakNet::BitStream bitstream;
		bitstream.Write(type);
		bitstream.Write(ent);
		bitstream.Write(var);

#ifdef __GRANDMSERVER__
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
#ifdef __GRANDMCLIENT__
		CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
	}
}

void SyncFloat::Set(float _v)
{
	if (var != _v)
	{
		var = _v;
	}
}

void SyncFloat::Sync(RakNet::RakNetGUID user, int ent, int type) {
	RakNet::BitStream bitstream;
	bitstream.Write(type);
	bitstream.Write(ent);
	bitstream.Write(var);

#ifdef __GRANDMSERVER__
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
#ifdef __GRANDMCLIENT__
	CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
}
#pragma endregion

///
/// * Int Sync Variable
///
#pragma region SyncInt
SyncInt::SyncInt() : var(0), varName("NULL") {}

SyncInt::SyncInt(int _v, char * _varName) : var(_v), varName(_varName) {}

void SyncInt::Set(int _v, RakNet::RakNetGUID sender, int ent, int type)
{
	if (var != _v)
	{
		LOG_DEBUG("<SyncInt> Updating var %s to  %i.", varName, _v);

		var = _v;

		RakNet::BitStream bitstream;
		bitstream.Write(type);
		bitstream.Write(ent);
		bitstream.Write(_v);

#ifdef __GRANDMSERVER__
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
#ifdef __GRANDMCLIENT__
		CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
	}
}

void SyncInt::Set(int _v)
{
	if (var != _v)
	{
		var = _v;
	}
}

void SyncInt::Sync(RakNet::RakNetGUID user, int ent, int type) {
	RakNet::BitStream bitstream;
	bitstream.Write(type);
	bitstream.Write(ent);
	bitstream.Write(var);

#ifdef __GRANDMSERVER__
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
#ifdef __GRANDMCLIENT__
	CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
}
#pragma endregion

///
/// * Bool Sync Variable
///
#pragma region SyncBool
void SyncBool::Set(bool _v, RakNet::RakNetGUID sender, int ent, int type)
{
	if (var != _v)
	{
		LOG_DEBUG("<SyncBool> Updating var %s to  %i.", varName, _v);

		var = _v;

		RakNet::BitStream bitstream;
		bitstream.Write(type);
		bitstream.Write(ent);
		bitstream.Write(_v);

#ifdef __GRANDMSERVER__
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
#ifdef __GRANDMCLIENT__
		CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
	}
}

void SyncBool::Set(bool _v)
{
	if (var != _v)
	{
		var = _v;
	}
}

void SyncBool::Sync(RakNet::RakNetGUID user, int ent, int type) {
	RakNet::BitStream bitstream;
	bitstream.Write(type);
	bitstream.Write(ent);
	bitstream.Write(var);

#ifdef __GRANDMSERVER__
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
#ifdef __GRANDMCLIENT__
	CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
}
#pragma endregion

///
/// * String Sync Variable
///
#pragma region SyncString
void SyncString::Set(std::string _v, RakNet::RakNetGUID sender, int ent, int type)
{
	if (var.compare(_v) != 0)
	{
		LOG_DEBUG("<SyncString> Updating var %s to  %s.", varName, _v);

		var = _v;

		RakNet::BitStream bitstream;
		bitstream.Write(type);
		bitstream.Write(ent);
		bitstream.Write(RakNet::RakString(var.c_str()));

#ifdef __GRANDMSERVER__
		Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
#ifdef __GRANDMCLIENT__
		CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true, false);
#endif
	}
}

void SyncString::Set(std::string _v)
{
	if (var.compare(_v) != 0)
	{
		var = _v;
	}
}

void SyncString::Sync(RakNet::RakNetGUID user, int ent, int type) {
	RakNet::BitStream bitstream;
	bitstream.Write(type);
	bitstream.Write(ent);
	bitstream.Write(RakNet::RakString(var.c_str()));

#ifdef __GRANDMSERVER__
	Server::GetServer()->GetNetworkManager()->GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
#ifdef __GRANDMCLIENT__
	CNetworkManager::GetRPC().Signal(varName, &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, user, false, false);
#endif
}
#pragma endregion