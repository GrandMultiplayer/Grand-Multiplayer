// Self
#include "API_Callback_Player.h"

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

namespace API
{
	namespace Players
	{
		const char *ThisNamespace = "API::Player";

		void OnPlayerShot(void *Instance, CEntity entity, const char * weapon)
		{
			if (Instance)
			{
				typedef void(*API_OnPlayerShot_t)(const Objects::Entity, const char *);
#ifdef WIN32
				API_OnPlayerShot_t API_OnPlayerShot = (API_OnPlayerShot_t)::GetProcAddress((HMODULE)Instance, "API_OnPlayerShot");
#else
				API_OnPlayerShot_t API_OnPlayerShot = (API_OnPlayerShot_t)dlsym(Instance, "API_OnPlayerShot");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnPlayerShot)
					API_OnPlayerShot(ent, weapon);
			}
		}

		void OnPlayerPickup(void *Instance, CEntity entity, const int pickup)
		{
			if (Instance)
			{
				typedef void(*API_OnPlayerPickup_t)(const Objects::Entity, const int);
#ifdef WIN32
				API_OnPlayerPickup_t API_OnPlayerPickup = (API_OnPlayerPickup_t)::GetProcAddress((HMODULE)Instance, "API_OnPlayerPickup");
#else
				API_OnPlayerPickup_t API_OnPlayerPickup = (API_OnPlayerPickup_t)dlsym(Instance, "API_OnPlayerPickup");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if (API_OnPlayerPickup)
					API_OnPlayerPickup(ent, pickup);
			}
		}
	}
}
