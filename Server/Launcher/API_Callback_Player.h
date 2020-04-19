#ifndef __APICALLPLAYER_H__
#define __APICALLPLAYER_H__
#pragma once
#include <string>

#include "CEntity.h"

namespace API
{
	namespace Players
	{
		void OnPlayerShot(void *Instance, CEntity entity, const char * weapon);
		void OnPlayerPickup(void *Instance, CEntity entity, const int player);
	}
}
#endif