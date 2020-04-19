#include <winsock2.h>
#include <Windows.h>

#include "hook/enums.h"
#include "hook/natives.h"

#include "Functions.h"
#include "CVector3.h"
#include "APIStructs.h"
#include "GUI.h"

#include "CLocalPlayer.h"

#include "Doors.h"

void Doors::Pulse()
{
	for (unsigned int i = 0; i < MAXDOORS; i++)
	{
		DoorsInfo doorinfo = GetDoorInfo(i);
		if (CVector3::Distance(CLocalPlayer::GetPosition(), doorinfo.coords) < 400.0f)
		{
			OBJECT::SET_STATE_OF_CLOSEST_DOOR_OF_TYPE(doorinfo.doorHash, doorinfo.coords.x, doorinfo.coords.y, doorinfo.coords.z, 0, 0.0f, 0);
			OBJECT::SET_STATE_OF_CLOSEST_DOOR_OF_TYPE(doorinfo.modelInfoHash, doorinfo.coords.x, doorinfo.coords.y, doorinfo.coords.z, 0, 0.0f, 0);
		}
	}

	for each(auto door in doors)
	{
		if (CVector3::Distance(CLocalPlayer::GetPosition(), door.coords) < 400.0f)
		{
			OBJECT::SET_STATE_OF_CLOSEST_DOOR_OF_TYPE($(door.doorName), door.coords.x, door.coords.y, door.coords.z, 0, 0.0f, 0);
			OBJECT::SET_STATE_OF_CLOSEST_DOOR_OF_TYPE($(door.doorName), door.coords.x, door.coords.y, door.coords.z, 0, 0.0f, 0);
		}
	}
}