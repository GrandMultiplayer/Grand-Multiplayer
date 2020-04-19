#ifndef __CEntity_H__
#define __CEntity_H__
#pragma once

#include <RakNetTypes.h>

// System
#include <chrono>
#include <string>

//GrandM
#include "APIStructs.h"
#include "API_Objects.h"

#include <GUI.h>
#include "CVector3.h"
#include <Transform.h>

/*
	CEntity is an abstract class.
*/
class CEntity
{
protected:
	/// <summary>
	/// A static count thats incremented after each new entity is created
	/// </summary>
	static int count;

	struct Identity
	{
		int	entityId = -1;
		GrandM::EntityType type = GrandM::EntityType::Nill;
	} identity;

	struct World
	{
		float streamDistance = 1000.0f;
		int dim = 0; // Dimension
	} world;

	GrandM::GUI::Tag tag;

	Transform transform;
	float forwardSpeed = 0;

	bool dead = false;
	int	lastAttacker = -1;
	
	struct Sync
	{
		uint64_t lastMovementSync;
	} sync;
public:
	/// <summary>
	/// NotEqual
	/// </summary>
	/// <param name="e">Entity to compare with</param>
	/// <returns>true if not the same else false</returns>
	//const bool operator!=(const CEntity &e);

	const bool operator==(Objects::Entity &e)
	{
		return identity.entityId == e.GetID();
	}

	/// <summary>
	/// Is vector null
	/// </summary>
	/// <returns>If null then true, otherwise false</returns>
	bool IsNull();

	const int GetID() { return identity.entityId; }

	GrandM::EntityType GetType() { return identity.type; }
	void SetType(GrandM::EntityType type) { identity.type = type; }

	const Transform GetTransform() { return transform; }

	const CVector3 GetPosition() { return transform.Position; }
	void SetPosition(const CVector3 position);

	const CVector3 GetRotation() { return transform.Rotation; }
	void SetRotation(const CVector3 rotation);

	CEntity GetLastAttacker();
	void SetLastAttacker(int entityId) { lastAttacker = entityId; }

	const std::string GetNametag() { return tag.text; }
	void SetNametag(const std::string nametag);

	const bool IsNametagVisible() { return tag.textVisible; }
	void SetNametagVisible(const bool visible);
	
	const float GetNametagDistance() { return tag.distance; }
	void SetNametagDistance(const float distance);

	const bool IsHealthbarVisible() { return tag.healthVisible; }
	void SetHealthbarVisible(const bool visible);

	const Color GetNametagColor() { return tag.textColor; }
	void SetNametagColor(const Color color);

	const float GetViewDistance() { return world.streamDistance; }
	void SetViewDistance(const float distance);

	const bool IsDead() { return dead; }

	void SetDimension(const int dim);
	const int GetDimension() { return world.dim; }

	void virtual UpdateMovement(RakNet::Packet *packet);
	void virtual PulseMovement();

	static CEntity* GetEntityFromID(const int id);

	static GrandM::EntityType GetEntityTypeFromID(const int id);
};
#endif // !__H_Entity__