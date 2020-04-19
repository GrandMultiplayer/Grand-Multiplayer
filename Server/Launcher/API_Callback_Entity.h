#ifndef __APICALLENTITY_H__
#define __APICALLENTITY_H__
#pragma once
#include <string>

#include "CVector3.h"
#include "CEntity.h"

namespace API
{
	namespace Callback
	{
		namespace Entity
		{
			const bool OnEntityDamage(void *Instance, CEntity entity, const int damage, CEntity attacker, const char * weapon);
			void OnProjectileImpact(void *Instance, CEntity player, const char * weapon, CVector3 position, CEntity entity, const int bone);

			void OnEntityDeath(void *Instance, CEntity entity, CEntity killer);
		}
	}
}
#endif