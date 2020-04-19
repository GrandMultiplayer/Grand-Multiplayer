#ifndef __APICALLNETWORK_H__
#define __APICALLNETWORK_H__
#pragma once
#include <string>

#include "CEntity.h"

namespace API
{
	namespace Network
	{
		bool OnPlayerConnecting(void *Instance, CEntity entity);
		bool OnPlayerConnected(void *Instance, CEntity entity);

		void OnPlayerDisconnected(void *Instance, CEntity entity, int reason);

		void OnPlayerCommand(void *Instance, CEntity entity, const wchar_t * message);
		void OnPlayerMessage(void *Instance, CEntity entity, const wchar_t * message);
	}
}
#endif