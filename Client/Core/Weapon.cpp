#include "CVector3.h"
#include "CMaths.h"

#include "hook/enums.h"
#include "hook/natives.h"
#include "Memory.h"
#include "MemoryAccess.h"

// Self
#include "Weapon.h"

namespace GTAV
{
	namespace Weapon
	{
		/* 
		0 = unarmed, 1 = Small Melee, 2 = Large Melee, 3 = pistol, 4 = smg, 5 = rifle, 6 shutgun, 7 sniper, 8 = heavy, 8 = minigun
		*/
		const int GetWeaponClass(const int weapon)
		{
			switch (weapon)
			{
			case eWeapon::WeaponUnarmed:
				return 0;
			case eWeapon::WeaponKnife://
			case eWeapon::WeaponNightstick:
			case eWeapon::WeaponHammer:
			case eWeapon::WeaponCrowbar:
			case eWeapon::WeaponWrench:
			case eWeapon::WeaponBattleaxe:
			case eWeapon::WeaponDagger:
			case eWeapon::WeaponHatchet:
			case eWeapon::WeaponKnuckleDuster:
			case eWeapon::WeaponMachete:
			case eWeapon::WeaponBottle:
			case eWeapon::WeaponSwitchBlade:
				return 1;
			case eWeapon::WeaponBat:
			case eWeapon::WeaponGolfClub:
			case eWeapon::WeaponPoolcue:
				return 2;
			case eWeapon::WeaponAPPistol:
			case eWeapon::WeaponPistol:
			case eWeapon::WeaponCombatPistol:
			case eWeapon::WeaponHeavyPistol:
			case eWeapon::WeaponPistol50:
			case eWeapon::WeaponSNSPistol:
			case eWeapon::WeaponVintagePistol:
			case eWeapon::WeaponPistolMk2:
			case eWeapon::WeaponMarksmanPistol:
			case eWeapon::WeaponRevolver:
			case eWeapon::WeaponStunGun:
			case eWeapon::WeaponFlareGun:
				return 3;
			case eWeapon::WeaponSMG:
			case eWeapon::WeaponMicroSMG:
			case eWeapon::WeaponAssaultSMG:
			case eWeapon::WeaponMachinePistol:
			case eWeapon::WeaponSMGMk2:
			case eWeapon::WeaponCombatPDW:
			case eWeapon::WeaponMG:
			case eWeapon::WeaponCombatMG:
			case eWeapon::WeaponCombatMGMk2:
			case eWeapon::WeaponGusenberg:
			case eWeapon::WeaponMiniSMG:
				return 4;
			case eWeapon::WeaponAssaultRifle:
			case eWeapon::WeaponAssaultRifleMk2:
			case eWeapon::WeaponCarbineRifle:
			case eWeapon::WeaponCarbineRifleMk2:
			case eWeapon::WeaponAdvancedRifle:
			case eWeapon::WeaponSpecialCarbine:
			case eWeapon::WeaponBullpupRifle:
			case eWeapon::WeaponCompactRifle:
				return 5;
			case eWeapon::WeaponPumpShotgun:
			case eWeapon::WeaponSawnOffShotgun:
			case eWeapon::WeaponBullpupShotgun:
			case eWeapon::WeaponAssaultShotgun:
			case eWeapon::WeaponMusket:
			case eWeapon::WeaponHeavyShotgun:
			case eWeapon::WeaponDoubleBarrelShotgun:
			case eWeapon::WeaponAutoshotgun:
				return 6;
			case eWeapon::WeaponSniperRifle:
			case eWeapon::WeaponHeavySniper:
			case eWeapon::WeaponHeavySniperMk2:
			case eWeapon::WeaponMarksmanRifle:
				return 7;
			case eWeapon::WeaponRPG:
			case eWeapon::WeaponHomingLauncher:
			case eWeapon::WeaponFirework:
			case eWeapon::WeaponGrenadeLauncher:
			case eWeapon::WeaponGrenadeLauncherSmoke:
			case eWeapon::WeaponRailgun:
			case eWeapon::WeaponCompactLauncher:
				return 8;
			case eWeapon::WeaponMinigun:
				return 9;
			default:
				return -1;
			}
		}

		/*
		if (weapon == -1357824103 || weapon == -1074790547 ||
			(weapon == 2132975508 || weapon == -2084633992) ||
			(weapon == -952879014 || weapon == 100416529) ||
			weapon == unchecked((int)WeaponHash.Gusenberg) ||
			weapon == unchecked((int)WeaponHash.MG) || weapon == unchecked((int)WeaponHash.CombatMG) ||
			weapon == unchecked((int)WeaponHash.CombatPDW) ||
			weapon == unchecked((int)WeaponHash.AssaultSMG) ||
			weapon == unchecked((int)WeaponHash.SMG) ||
			weapon == unchecked((int)WeaponHash.HeavySniper) ||
			weapon == unchecked((int)WeaponHash.PumpShotgun) ||
			weapon == unchecked((int)WeaponHash.HeavyShotgun) ||
			weapon == unchecked((int)WeaponHash.Musket) ||
			weapon == unchecked((int)WeaponHash.AssaultShotgun) ||
			weapon == unchecked((int)WeaponHash.BullpupShotgun) ||
			weapon == unchecked((int)WeaponHash.SawnOffShotgun) ||
			weapon == unchecked((int)GTANetworkShared.WeaponHash.Autoshotgun) ||
			weapon == unchecked((int)WeaponHash.CompactRifle))
			return 2;*/
	}
}