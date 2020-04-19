#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <vector>
#include <chrono>
#include <stdint.h>

#include "RakNetTypes.h"

#include "CEntity.h"
#include "CVitals.h"
#include "CBlip.h"

class CObject : public CEntity, public CVitals
{
protected:
	const int getID() override { return identity.entityId; }
	const int getType() override { return identity.type; }
private:
	struct Data
	{
		struct Model
		{
			uint32_t hash;

			bool dynamic = false;

			int texture = 0;
		} Model;
	} data;

	CBlip	blip;

	struct Network
	{
		std::chrono::time_point<std::chrono::system_clock> lastSync;

		// The Assigned user for physics sync
		RakNet::RakNetGUID physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	} network;

public:
	CObject(const uint32_t hash, CVector3 position, CVector3 rotation, bool dynamic);
	~CObject();

	// Copy constructor
	CObject(const CObject & e);

	// Copy Assign 
	CObject &operator=(const CObject &e);

	void Pulse();
	void Update(RakNet::Packet *packet);
	void SyncData(RakNet::RakNetGUID requester);

	RakNet::RakNetGUID GetAssignee() { return network.physicsAssignee; }
	void SetAssignee(RakNet::RakNetGUID assignee);

	const uint32_t GetModel() { return data.Model.hash; }
	bool IsDynamic() { return data.Model.dynamic; }

	const int GetTextureVariantion() { return data.Model.texture; }
	void SetTextureVariation(const int textureindex) { data.Model.texture = textureindex; }

	CBlip* GetBlip() { return &blip; }
};
extern std::vector<CObject> g_Objects;
#endif