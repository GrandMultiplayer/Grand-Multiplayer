#ifndef __NETWORKSYNC_H__
#define __NETWORKSYNC_H__
namespace NetworkSync
{
	void SyncPlayersLocalData(RakNet::RakNetGUID guid, int id);
	void SyncServerWorld(RakNet::RakNetGUID user);
	void SyncEntities(RakNet::RakNetGUID user, int dim);
}
#endif
