#ifndef __CCHECKPOINT_H__
#define __CCHECKPOINT_H__
#include "CEntity.h" // The base class

#include "CBlip.h"

class CCheckpoint : public CEntity
{
private:
	CBlip blip;

	struct Data
	{
		int type;
		float radius;
		Color color;
		int reserved;

		float nearHeight;
		float farHeight;

		CVector3 pointTo;

		bool triggered;
	} data;

public:
	static int Amount;

	CCheckpoint() = delete;
	CCheckpoint(const int entity, const CVector3 position, const float radius, const Color color, const int reserved);
	~CCheckpoint();
	
	// Copy constructor
	CCheckpoint(const CCheckpoint & e);

	// Copy Assign 
	CCheckpoint &operator=(const CCheckpoint &e);

	void Destroy();

	void Show();
	void Hide();

	void Pulse();

	void SetHeight(const float nearHeight, const float farHeight);

	const int GetCheckpoint() { return game.entity; }

	bool IsTriggered() { return data.triggered; }
	void SetTriggered(bool toggle) { data.triggered = toggle; }

	CVector3 GetPosition() { return transform.Position; }
	void SetPosition(CVector3 position);

	void PointTo(const CVector3 pointTo);

	void SetType(const int type);

	void SetColor(const Color color);

	CBlip* Blip() { return &blip; }
};
extern std::vector<CCheckpoint> g_Checkpoints;

#endif