#ifndef __CBLIP_H__
#define __CBLIP_H__
#include <vector>

#include "CVector3.h"
#include "CEntity.h"

class CBlip : public CEntity
{
private:
	struct Data
	{
		bool hasBlip = false;

		std::string text;
		int sprite = 1;
		int color = 0;
		bool isShortRange = false;

		bool visible = false;
	} data;
public:
	CBlip();
	explicit CBlip(CVector3 position);
	~CBlip();

	// Copy constructor
	CBlip(const CBlip & e);

	// Copy Assign 
	CBlip &operator=(const CBlip &e);

	void CreateE(const int entityId, GrandM::EntityType type);

	void SyncData(RakNet::RakNetGUID user);

	const bool HasBlip() { return data.hasBlip; }

	void Show();
	void Hide();

	const int GetColor() { return data.color; }
	void SetColor(const int color);

	const int GetSprite() { return data.sprite; }
	void SetSprite(const int sprite);

	const std::string GetText() { return data.text; }
	void SetText(const std::string text);

	const bool IsShortRanged() { return data.isShortRange; }
	void SetShortRanged(const bool state);
};
extern std::vector<CBlip> g_Blips;
#endif