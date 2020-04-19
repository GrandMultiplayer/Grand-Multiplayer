// Self
#include "API_Callback_Cef.h"

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
	namespace Cef
	{
		void OnCefFinishLoad(void *Instance, CEntity entity)
		{
			if (Instance)
			{
				typedef void(*API_OnCefFinishLoad_t)(Objects::Entity);
#ifdef WIN32
				API_OnCefFinishLoad_t API_OnCefFinishLoad = (API_OnCefFinishLoad_t)::GetProcAddress((HMODULE)Instance, "API_OnCefFinishLoad");
#else
				API_OnCefFinishLoad_t API_OnCefFinishLoad = (API_OnCefFinishLoad_t)dlsym(Instance, "API_OnCefFinishLoad");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnCefFinishLoad)
					API_OnCefFinishLoad(ent);
			}
		}

		void OnCefSendData(void *Instance, CEntity entity, const char * data)
		{
			if (Instance)
			{
				typedef void(*API_OnCefSendData_t)(Objects::Entity, const char *);
#ifdef WIN32
				API_OnCefSendData_t API_OnCefSendData = (API_OnCefSendData_t)::GetProcAddress((HMODULE)Instance, "API_OnCefSendData");
#else
				API_OnCefSendData_t API_OnCefSendData = (API_OnCefSendData_t)dlsym(Instance, "API_OnCefSendData");
#endif
				Objects::Entity ent;
				ent.SetID(entity.GetID());
				ent.SetType(entity.GetType());

				if(API_OnCefSendData)
					API_OnCefSendData(ent, data);
			}
		}
	}
}