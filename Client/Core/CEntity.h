#ifndef __Entity_H__
#define __Entity_H__

#include "BitStream.h"

// System
#include <chrono>
#include <string>
#include <stdint.h>

#include "hook/types.h"

//GrandM
#include "APIStructs.h"
#include "GUI.h"
#include "CVector3.h"
#include <Transform.h>

// Shared
#include "SyncValue.h"

/*
	CEntity is an abstract class thus has no construct or decontructors.
*/
class CEntity
{
public:
	enum Type
	{
		eNill = -1,
		ePlayer,
		eNPC,
		eVehicle,
		eObject,
		eCheckpoint,
		ePickup,
		eBlip,
	};
protected:
	struct Game
	{
		Entity entity = NULL;
		bool created = false;

		RakNet::RakNetGUID physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;

		int vehicle = -1;
	} game;

	struct Identity
	{
		int	entityId = -1;
		Type type = Type::eNill;
	} identity;

	struct World
	{
		float streamDistance = 1000.0f;
		int dim = 0; // Dimension
	} world;

	GrandM::GUI::Tag tag;

	Transform transform;
	float forwardSpeed = 0;

	SyncInt health = SyncInt(100, "EntityHealth");
	bool dead = false;
	int	lastAttacker = -1;

	struct InterpolationData
	{
		struct
		{
			CVector3      start;
			CVector3      target;
			CVector3      error;
			float         lastAlpha;
			unsigned long startTime;
			unsigned long finishTime;
		} position;

		struct
		{
			CVector3      start;
			CVector3      target;
			CVector3      error;
			float         lastAlpha;
			unsigned long startTime;
			unsigned long finishTime;
		} rotation;
	} interpolation;

	struct Sync
	{
		uint64_t lastMovementSync;
		uint64_t lastSyncReceived;
		uint64_t lastUpdateData;
	} sync;

public:
	/// <summary>
	/// Equal
	/// </summary>
	/// <param name="e">Entity to compare with</param>
	/// <returns>true if the same else false</returns>
	const bool operator==(CEntity &e);

	/// <summary>
	/// NotEqual
	/// </summary>
	/// <param name="e">Entity to compare with</param>
	/// <returns>true if not the same else false</returns>
	const bool operator!=(CEntity &e);

	/// <summary>
	/// Is vector null
	/// </summary>
	/// <returns>If null then true, otherwise false</returns>
	bool IsNull();

	const int GetID() { return identity.entityId; }

	Type GetType() { return identity.type; }
	void SetType(Type type) { identity.type = type; }

	const Transform GetTransform() { return transform; }

	const CVector3 GetPosition() { return transform.Position; }
	void SetPosition(const CVector3 position);

	const CVector3 GetRotation() { return transform.Rotation; }
	void SetRotation(const CVector3 rotation);

	CEntity* GetLastAttacker();
	void SetLastAttacker(int entityId) { lastAttacker = entityId; }

	const std::string GetNametag() { return tag.text; }
	void SetNametag(const std::string nametag) { tag.text = nametag; }

	const bool IsNametagVisible() { return tag.textVisible; }
	void SetNametagVisible(const bool visible) { tag.textVisible = visible; }
	
	const float GetNametagDistance() { return tag.distance; }
	void SetNametagDistance(const float distance) { tag.distance = distance; }

	const bool IsHealthbarVisible() { return tag.healthVisible; }
	void SetHealthbarVisible(const bool visible) { tag.healthVisible = visible; }

	const Color GetNametagColor() { return tag.textColor; }
	void SetNametagColor(const Color color) { tag.textColor = color; }

	const float GetViewDistance() { return world.streamDistance; }
	void SetViewDistance(const float distance) { world.streamDistance = distance; }

	const bool Exists();
	Entity GetEntity() { return game.entity; }
	bool IsCreated() { return game.created; }

	void SetHealth(const int hp, RakNet::RakNetGUID sender);
	const int GetHealth() { return health.Get(); }

	const bool IsDead() { return dead; }

	void SetDimension(const int dim);
	const int GetDimension() { return world.dim; }

	static CEntity* GetEntityFromID(const int id);

	void UpdateMovement(RakNet::Packet *packet);

	void PulseMovement();

	virtual void Interpolate();

	void UpdateTargetPosition();
	virtual void SetTargetPosition();

	void UpdateTargetRotation();
	virtual void SetTargetRotation();

	void DebugUI();
};
#endif // !__H_Entity__