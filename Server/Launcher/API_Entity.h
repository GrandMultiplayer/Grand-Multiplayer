#ifndef __APIENTITY_H__
#define __APIENTITY_H__ 
#pragma once

#include "Common.h"
#include "API_Objects.h"
#include "APIStructs.h"
#include "Ped.h"
#include "CVector3.h"

namespace API
{
	namespace Entity
	{
		DLL_PUBLIC_C void Destroy(Objects::Entity entity);

		DLL_PUBLIC_C const CVector3 GetPosition(Objects::Entity entity);
		DLL_PUBLIC_C void SetPosition(Objects::Entity entity, const CVector3 position);
		DLL_PUBLIC_C const CVector3 GetRotation(Objects::Entity entity);
		DLL_PUBLIC_C void SetRotation(Objects::Entity entity, const CVector3 rotation);

		// NPC & Player
		DLL_PUBLIC_C const GrandM::Model::Component GetPedComponent(Objects::Entity entity, const int componentid);
		DLL_PUBLIC_C void SetPedComponent(Objects::Entity entity, const int componentid, const GrandM::Model::Component component);
		DLL_PUBLIC_C const GrandM::Model::HeadBlend GetPedHeadBlend(Objects::Entity entity);
		DLL_PUBLIC_C void SetPedHeadBlend(Objects::Entity entity, const GrandM::Model::HeadBlend headblend);
		DLL_PUBLIC_C const GrandM::Model::HeadOverlay GetPedHeadOverlay(Objects::Entity entity, const int index);
		DLL_PUBLIC_C void SetPedHeadOverlay(Objects::Entity entity, const int index, const GrandM::Model::HeadOverlay overlay);
		DLL_PUBLIC_C const GrandM::Model::Prop GetPedProp(Objects::Entity entity, const int index);
		DLL_PUBLIC_C void SetPedProp(Objects::Entity entity, const int index, const GrandM::Model::Prop prop);
		DLL_PUBLIC_C const float GetPedFaceFeature(Objects::Entity entity, const int index);
		DLL_PUBLIC_C void SetPedFaceFeature(Objects::Entity entity, const int index, const float scale);

		DLL_PUBLIC_C const float GetViewDistance(Objects::Entity entity);
		DLL_PUBLIC_C void SetViewDistance(Objects::Entity entity, const float distance);

		DLL_PUBLIC_C Objects::Entity * GetEntities(const int type);

		DLL_PUBLIC_C const char * GetNametag(Objects::Entity entity);
		DLL_PUBLIC_C void SetNametag(Objects::Entity entity, const char * nametag);

		DLL_PUBLIC_C const bool IsNametagVisible(Objects::Entity entity);
		DLL_PUBLIC_C void SetNametagVisible(Objects::Entity entity, const bool visible);

		DLL_PUBLIC_C const float GetNametagDistance(Objects::Entity entity);
		DLL_PUBLIC_C void SetNametagDistance(Objects::Entity entity, const float distance);

		DLL_PUBLIC_C const int GetHealth(Objects::Entity entity);
		DLL_PUBLIC_C void SetHealth(Objects::Entity entity, const int health);
		DLL_PUBLIC_C const int GetArmour(Objects::Entity entity);
		DLL_PUBLIC_C void SetArmour(Objects::Entity entity, const int armour);

		DLL_PUBLIC_C const bool IsHealthbarVisible(Objects::Entity entity);
		DLL_PUBLIC_C void SetHealthbarVisible(Objects::Entity entity, const bool visible);

		DLL_PUBLIC_C const Color GetNametagColor(Objects::Entity entity);
		DLL_PUBLIC_C void SetNametagColor(Objects::Entity entity, const Color color);

		DLL_PUBLIC_C void Revive(Objects::Entity entity);
		DLL_PUBLIC_C const bool IsAlive(Objects::Entity entity);

		DLL_PUBLIC_C void SetDimension(Objects::Entity entity, const int dim);
		DLL_PUBLIC_C const int GetDimension(Objects::Entity entity);
	}
}
#endif