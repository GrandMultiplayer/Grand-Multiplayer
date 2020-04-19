#ifndef __APINPC_H__
#define __APINPC_H__
#pragma once
#include <stdint.h>

#include "API_Objects.h"
#include "CVector3.h"

namespace API
{
	namespace NPC
	{
		DLL_PUBLIC_C Objects::Entity CreateNPC(const uint32_t model, const CVector3 position, const CVector3 rotation);
	}
}

#endif