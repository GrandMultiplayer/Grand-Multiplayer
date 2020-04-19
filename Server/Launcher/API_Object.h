#ifndef __APIOBJECT_H__
#define __APIOBJECT_H__
#pragma once
#include <stdint.h>

#include "API_Objects.h"
#include "CVector3.h"

namespace API
{
	namespace Object
	{
		DLL_PUBLIC_C Objects::Entity CreateObject(const uint32_t model, const CVector3 position, const CVector3 Rotation, const bool dynamic);
		DLL_PUBLIC_C const int GetTextureVariation(Objects::Entity entity);
		DLL_PUBLIC_C void SetTextureVariation(Objects::Entity entity, const int textureindex);
	}
}
#endif