#ifndef __CVITALS_H__
#define __CVITALS_H__

//Shared
#include <SyncValue.h>

/*
	CVitals is an abstract class.
*/
class CVitals
{
protected:
	SyncInt health = SyncInt(200, "EntityHealth");
	SyncInt armour = SyncInt(100, "EntityArmour");

	virtual const int getID() = 0;
	virtual const int getType() = 0;

public:
	const int GetHealth() { return health.Get(); }
	void SetHealth(const int hp, RakNet::RakNetGUID sender) { health.Set(hp, sender, getID(), getType()); }

	const int GetArmour() { return armour.Get(); }
	void SetArmour(const int hp, RakNet::RakNetGUID sender) { armour.Set(hp, sender, getID(), getType()); }
};
#endif // !__CVITALS_H__