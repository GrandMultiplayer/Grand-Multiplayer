#ifndef __CBLIP_H__
#define __CBLIP_H__
#pragma once
#include <vector>

#include "CEntity.h" // The base class

//hook
#include "hook/types.h"

class CBlip : public CEntity
{
private:
	bool hasBlip = false;

	struct Data
	{
		std::string text;
		int sprite = 1;
		int color = 0;
		bool shortRange	= false;

		bool visible = false;
	} data;

public:
	CBlip();
	CBlip(const int id, const CVector3 pos);
	~CBlip();

	// Copy constructor
	CBlip(const CBlip & e);

	// Copy Assign 
	CBlip &operator=(const CBlip &e);

	void Create(const int id);

	void CreateBlip(Entity ent);
	void CreateBlip();

	void Remove();

	const bool HasBlip() { return hasBlip; }

	void Show(Entity ent);
	void Show();

	void Hide();

	void SetColor(const int color);
	void SetSprite(const int sprite);
	void SetPosition(const CVector3 pos);
	void SetText(const std::string text);
	void SetShortRange(const bool state);
};
extern std::vector<CBlip> g_Blips;

#endif