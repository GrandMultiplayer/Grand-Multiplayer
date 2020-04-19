// Self
#include "API_Callback_Entity.h"

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
	namespace Callback
	{
		namespace Entity
		{
			const bool OnEntityDamage(void *Instance, CEntity entity, const int damage, CEntity attacker, const char * weapon)
			{
				if (Instance)
				{
					typedef const bool(*API_OnEntityDamage_t)(Objects::Entity, const int, Objects::Entity, const char *);
#ifdef WIN32
					API_OnEntityDamage_t API_OnEntityDamage = (API_OnEntityDamage_t)::GetProcAddress((HMODULE)Instance, "API_OnEntityDamage");
#else
					API_OnEntityDamage_t API_OnEntityDamage = (API_OnEntityDamage_t)dlsym(Instance, "API_OnEntityDamage");
#endif
					Objects::Entity atker;
					atker.SetID(attacker.GetID());
					atker.SetType(attacker.GetType());

					Objects::Entity ent;
					ent.SetID(entity.GetID());
					ent.SetType(entity.GetType());

					if (API_OnEntityDamage)
						return API_OnEntityDamage(ent, damage, atker, weapon);
				}

				return true;
			}

			void OnProjectileImpact(void *Instance, CEntity player, const char * weapon, CVector3 position, CEntity entity, const int bone)
			{
				if (Instance)
				{
					typedef void(*API_OnProjectileImpact_t)(Objects::Entity, const char *, CVector3, Objects::Entity, const int);
#ifdef WIN32
					API_OnProjectileImpact_t API_OnProjectileImpact = (API_OnProjectileImpact_t)::GetProcAddress((HMODULE)Instance, "API_OnProjectileImpact");
#else
					API_OnProjectileImpact_t API_OnProjectileImpact = (API_OnProjectileImpact_t)dlsym(Instance, "API_OnProjectileImpact");
#endif
					Objects::Entity plyer;
					plyer.SetID(player.GetID());
					plyer.SetType(player.GetType());

					Objects::Entity ent;
					ent.SetID(entity.GetID());
					ent.SetType(entity.GetType());

					if (API_OnProjectileImpact)
						API_OnProjectileImpact(plyer, weapon, position, ent, bone);
				}
			}

			void OnEntityDeath(void *Instance, CEntity entity, CEntity killer)
			{
				if (Instance)
				{
					typedef void(*API_OnEntityDeath_t)(Objects::Entity, Objects::Entity);
#ifdef WIN32
					API_OnEntityDeath_t API_OnEntityDeath = (API_OnEntityDeath_t)::GetProcAddress((HMODULE)Instance, "API_OnEntityDeath");
#else
					API_OnEntityDeath_t API_OnEntityDeath = (API_OnEntityDeath_t)dlsym(Instance, "API_OnEntityDeath");
#endif
					Objects::Entity kler;
					kler.SetID(killer.GetID());
					kler.SetType(killer.GetType());

					Objects::Entity ent;
					ent.SetID(entity.GetID());
					ent.SetType(entity.GetType());

					if (API_OnEntityDeath)
						API_OnEntityDeath(ent, kler);
				}
			}
		}
	}
}
