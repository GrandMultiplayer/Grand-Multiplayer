#ifndef __APICALLCHECKPOINT_H__
#define __APICALLCHECKPOINT_H__
#pragma once

#include "CEntity.h"

namespace API
{
	namespace Checkpoints
	{
		void OnEntityEnterCheckpoint(void *Instance, CEntity checkpoint, CEntity entity);
		void OnEntityExitCheckpoint(void *Instance, CEntity checkpoint, CEntity entity);
	}
}
#endif