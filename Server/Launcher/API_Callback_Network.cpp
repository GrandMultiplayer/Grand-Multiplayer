// Self
#include "API_Callback_Network.h"

// System
#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#else
#define _BSD_SOURCE
#include <dlfcn.h>
#include <sys/time.h>
#include <unistd.h>
#endif

// RakNet
#include "BitStream.h"

#include "Log.h"

namespace API
{
	namespace Network
	{
		const char *ThisNamespace = "API::Network";

		bool OnPlayerConnecting(void *Instance, CEntity entity)
		{
			if (Instance)
			{
				typedef bool(*API_OnPlayerConnecting_t)(Objects::Entity);
#ifdef WIN32
				API_OnPlayerConnecting_t API_OnPlayerConnecting = (API_OnPlayerConnecting_t)::GetProcAddress((HMODULE)Instance, "API_OnPlayerConnecting");
#else
				API_OnPlayerConnecting_t API_OnPlayerConnecting = (API_OnPlayerConnecting_t)dlsym(Instance, "API_OnPlayerConnecting");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnPlayerConnecting)
					return API_OnPlayerConnecting(ent);
			}
			return true;
		}

		bool OnPlayerConnected(void *Instance, CEntity entity)
		{
			if (Instance)
			{
				typedef bool(*API_OnPlayerConnected_t)(Objects::Entity);
#ifdef WIN32
				API_OnPlayerConnected_t API_OnPlayerConnected = (API_OnPlayerConnected_t)::GetProcAddress((HMODULE)Instance, "API_OnPlayerConnected");
#else
				API_OnPlayerConnected_t API_OnPlayerConnected = (API_OnPlayerConnected_t)dlsym(Instance, "API_OnPlayerConnected");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnPlayerConnected)
					return API_OnPlayerConnected(ent);
			}
			return true;
		}

		// Reason:  0 = Left, 1 = Timeout, 2 = Kicked, 3 = Banned
		void OnPlayerDisconnected(void *Instance, CEntity entity, const int reason)
		{
			if (Instance)
			{
				typedef bool(*API_OnPlayerDisconnected_t)(Objects::Entity, int);
#ifdef WIN32
				API_OnPlayerDisconnected_t API_OnPlayerDisconnected = (API_OnPlayerDisconnected_t)::GetProcAddress((HMODULE)Instance, "API_OnPlayerDisconnected");
#else
				API_OnPlayerDisconnected_t API_OnPlayerDisconnected = (API_OnPlayerDisconnected_t)dlsym(Instance, "API_OnPlayerDisconnected");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnPlayerDisconnected)
					API_OnPlayerDisconnected(ent, reason);
			}
		}

		void OnPlayerCommand(void *Instance, CEntity entity, const wchar_t * message)
		{
			if (Instance)
			{
				typedef void(*API_OnPlayerCommand_t)(Objects::Entity, const wchar_t *);
#ifdef WIN32
				API_OnPlayerCommand_t API_OnPlayerCommand = (API_OnPlayerCommand_t)::GetProcAddress((HMODULE)Instance, "API_OnPlayerCommand");
#else
				API_OnPlayerCommand_t API_OnPlayerCommand = (API_OnPlayerCommand_t)dlsym(Instance, "API_OnPlayerCommand");
#endif
				if (!API_OnPlayerCommand)
					return;

				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				API_OnPlayerCommand(ent, message);
			}
		}

		void OnPlayerMessage(void *Instance, CEntity entity, const wchar_t * message)
		{
			if (Instance)
			{
				typedef void(*API_OnPlayerMessage_t)(Objects::Entity, const wchar_t *);
#ifdef WIN32
				API_OnPlayerMessage_t API_OnPlayerMessage = (API_OnPlayerMessage_t)::GetProcAddress((HMODULE)Instance, "API_OnPlayerMessage");
#else
				API_OnPlayerMessage_t API_OnPlayerMessage = (API_OnPlayerMessage_t)dlsym(Instance, "API_OnPlayerMessage");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnPlayerMessage)
					API_OnPlayerMessage(ent, message);
			}
		}
	}
}
