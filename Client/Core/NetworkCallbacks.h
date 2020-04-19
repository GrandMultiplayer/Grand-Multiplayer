#ifndef __NETWORKCALLBACKS_H__
#define __NETWORKCALLBACKS_H__
namespace GrandM
{
	namespace Network
	{
		namespace Callback
		{
			void OnProjectileImpact(std::string weapon, Vector3 impact, const int entity, const int bone);

			void OnEntityDamage(const int entity, const int damage, const int attacker, std::string weapon);
		}
	}
}
#endif