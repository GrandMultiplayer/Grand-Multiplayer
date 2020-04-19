#ifndef __GRANDM_WEAPON_H__
#define __GRANDM_WEAPON_H__
#include <vector>

namespace GrandM
{
	namespace Weapon
	{
		struct Weapon
		{
			std::string		name;
#ifndef _WIN32
			int				hash; // Linux Longs are 64 bytes on windows they are 32 so we have to use a int on linux
#else
			long			hash;
#endif


			int				clipAmmo	= 0;
			int				ammo		= 0;

			bool			equiped		= false;

			bool			lazer		= false;

			// Tints = 0:Normal - 1:Green - 2:Gold - 3:Pink - 4:Army - 5:LSPD  6:Orange - 7:Platinum [think there is more neeed to test, like antique for example]
			int				tint		= 0;

			// GIVE_WEAPON_COMPONENT_TO_PED | GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT 
			// Compontents, Needs some research
		};

		struct WeaponData
		{
#ifndef _WIN32
			int				heldWeapon; // Linux Longs are 64 bytes on windows they are 32 so we have to use a int on linux
#else
			long			heldWeapon;
#endif

			bool			reloading	= false;

			bool			aiming = false;
			CVector3		aimPosition = CVector3::null;

			struct MeleeData
			{
				bool inMeleeCombat = false;
				bool swingFinished = true;
				bool isAnimPlaying = false;

				std::string meleeDic;
				std::string meleeAnim;

				float animationEnd = 0.f;
				unsigned long lastMelee;

				int targetId = -1;
				int targetType = -1;
			} melee;

			std::vector<Weapon> weapons;
		};
	}
}

#endif