#ifndef __APICALLCEF_H__
#define __APICALLCEF_H__
#pragma once
#include <string>

#include "CEntity.h"

namespace API
{
	namespace Cef
	{
		void OnCefFinishLoad(void *Instance, CEntity entity);
		void OnCefSendData(void *Instance, CEntity entity, const char * data);
	}
}
#endif