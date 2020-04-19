// Self
#include "API_Callback_Checkpoint.h"

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
	namespace Checkpoints
	{
		void OnEntityEnterCheckpoint(void *Instance, CEntity checkpoint, CEntity entity)
		{
			if (Instance)
			{
				typedef void(*API_OnEntityEnterCheckpoint_t)(Objects::Entity, Objects::Entity);
#ifdef WIN32
				API_OnEntityEnterCheckpoint_t API_OnEntityEnterCheckpoint = (API_OnEntityEnterCheckpoint_t)::GetProcAddress((HMODULE)Instance, "API_OnEntityEnterCheckpoint");
#else
				API_OnEntityEnterCheckpoint_t API_OnEntityEnterCheckpoint = (API_OnEntityEnterCheckpoint_t)dlsym(Instance, "API_OnEntityEnterCheckpoint");
#endif
				Objects::Entity cp;
				cp.SetID(checkpoint.GetID());
				cp.SetType(checkpoint.GetType());

				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnEntityEnterCheckpoint)
					API_OnEntityEnterCheckpoint(cp, ent);
			}
		}

		void OnEntityExitCheckpoint(void *Instance, CEntity checkpoint, CEntity entity)
		{
			if (Instance)
			{
				typedef void(*API_OnEntityExitCheckpoint_t)(Objects::Entity, Objects::Entity);
#ifdef WIN32
				API_OnEntityExitCheckpoint_t API_OnEntityExitCheckpoint = (API_OnEntityExitCheckpoint_t)::GetProcAddress((HMODULE)Instance, "API_OnEntityExitCheckpoint");
#else
				API_OnEntityExitCheckpoint_t API_OnEntityExitCheckpoint = (API_OnEntityExitCheckpoint_t)dlsym(Instance, "API_OnEntityExitCheckpoint");
#endif
				Objects::Entity cp;
				cp.SetID(checkpoint.GetID());
				cp.SetType(checkpoint.GetType());

				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnEntityExitCheckpoint)
					API_OnEntityExitCheckpoint(cp, ent);
			}
		}
	}
}