#ifndef __CCHECKPOINT_H__
#define __CCHECKPOINT_H__
#include "CEntity.h"

class CCheckpoint : public CEntity
{
private:
	struct Data
	{
		int type = 45;
		float radius = 5.f;
		Color color = { 255, 255, 255, 255 };
		int reserved;

		float nearHeight = 10.f;
		float farHeight = 20.f;

		CVector3 pointTo = { 0, 0, 0 };
	} data;

	CBlip	blip;

public:
	CCheckpoint() = delete;
	CCheckpoint(const CVector3 position, const float radius, const Color color, const int reserved);
	~CCheckpoint();

	// Copy constructor
	CCheckpoint(const CCheckpoint & e);

	// Copy Assign 
	CCheckpoint &operator=(const CCheckpoint &e);

	void SyncData(RakNet::RakNetGUID requester);

	//const EntityType GetType() { return data.type; }
	void SetType(const int type) { data.type = type; }

	const Color GetColor() { return data.color; }
	void SetColor(const Color color) { data.color = color; }

	const float GetRadius() { return data.radius; }
	void SetRadius(const float radius) { data.radius = radius; }

	const int GetReserved() { return data.reserved; }

	const float GetNearHeight() { return data.nearHeight; }
	void SetNearHeight(const float height) { data.nearHeight = height; }

	const float GetFarHeight() { return data.farHeight; }
	void SetFarHeight(const float height) { data.farHeight = height; }

	void PointTo(const CVector3 pointTo) { data.pointTo = pointTo; }
	const CVector3	GetPointTo() { return data.pointTo; }

	CBlip* GetBlip() { return &blip; }
};
extern std::vector<CCheckpoint> g_Checkpoints;
#endif