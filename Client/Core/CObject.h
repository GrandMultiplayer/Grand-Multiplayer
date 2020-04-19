#ifndef __COBJECT_H__
#define __COBJECT_H__
#include "CEntity.h" // The base class

#include "CBlip.h"

class CObject : public CEntity
{
private:
	CBlip blip;

	struct Data
	{
		struct ModelData
		{
			uint32_t hash;

			bool dynamic;
			int textureIndex;
		} model;
	} data;

	struct Network
	{
		unsigned long lastSyncSent;
		unsigned long lastSyncReceived;
		bool synchronized;
	} network;

	struct Interpolation
	{
		struct
		{
			CVector3 start;
			CVector3 target;
			CVector3 error;
			float lastAlpha;
			unsigned long startTime;
			unsigned long finishTime;
		} position;

		struct
		{
			CVector3 start;
			CVector3 target;
			CVector3 error;
			float lastAlpha;
			unsigned long startTime;
			unsigned long finishTime;
		} rotation;
	} interpolation;

public:

	CObject() = delete;
	CObject(const int entity, const int hash, CVector3 position, CVector3 rotation, const bool dynamic);
	~CObject();

	// Copy constructor
	CObject(const CObject & e);

	// Copy Assign 
	CObject &operator=(const CObject &e);

	bool CreateObject();
	void Destroy();
	void Delete();

	void Pulse();
	void Update(RakNet::Packet *packet);
	void Interpolate();

	void UpdateTargetPosition();
	void SetTargetPosition();
	void UpdateTargetRotation();
	void SetTargetRotation();
	void SetTargetData();

	Object GetEntity() { return game.entity; }
	bool IsCreated() { return game.created; }

	void SetTextureVariation(const int textureIndex);

	CVector3 GetPosition() { return transform.Position; }
	void SetPosition(CVector3 position) { transform.Position = position; }
	CVector3 GetRotation() { return transform.Rotation; }
	void SetRotation(CVector3 rotation) { transform.Rotation = rotation; }

	bool GetSynchronized() { return network.synchronized; }

	RakNet::RakNetGUID GetAssignee() { return game.physicsAssignee; }
	void SetAssignee(RakNet::RakNetGUID assignee) { game.physicsAssignee = assignee; }

	CBlip* Blip() { return &blip; }
};
extern std::vector<CObject> g_Objects;

#endif