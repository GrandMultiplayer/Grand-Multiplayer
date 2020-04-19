#include <string>

#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "Animations.h"


void Animations::GetAnimalAnimation(int modelhash, int movement, std::string *dict, std::string *name)
{
#pragma region Animals
	switch (modelhash)
	{
	case -832573324:
		*dict = "creatures@boar@move";
		if (movement == 1) //walking
		{
			*name = "walk";
		}
		else if (movement == 2) //running
		{
			*name = "trot";
		}
		else if (movement == 3) //sprinting
		{
			*name = "gallop";
		}
		else if (movement == 0) //idle
		{
			*name = "idle";
		}
		break;
	case 1462895032:
		*dict = "creatures@cat@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1430839454:
		*dict = "creatures@chickenhawk@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 51016938:
	case 1126154828:
		*dict = "creatures@dog@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "run";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1457690978:
		*dict = "creatures@cormorant@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -50684386:
		*dict = "creatures@cow@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1682622302:
		*dict = "creatures@coyote@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 402729631:
		*dict = "creatures@crow@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -664053099:
		*dict = "creatures@deer@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1950698411:
		*dict = "creatures@dolphin@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 802685111:
		*dict = "creatures@fish@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1794449327:
		*dict = "creatures@hen@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "run";
		}
		else if (movement == 3)
		{
			*name = "run";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1193010354:
		*dict = "creatures@humpback@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1318032802:
		*dict = "creatures@husky@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1920284487:
		*dict = "creatures@killerwhale@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1323586730:
		*dict = "creatures@pig@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -745300483:
	case 111281960:
		*dict = "creatures@pigeon@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1125994524:
	case 1832265812:
	case -1384627013:
		*dict = "creatures@pug@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -541762431:
		*dict = "creatures@rabbit@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1011537562:
		*dict = "creatures@rat@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 882848737:
		*dict = "creatures@retriever@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1788665315:
		*dict = "creatures@rottweiler@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1015224100:
	case 113504370:
		*dict = "creatures@shark@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1589092019:
		*dict = "creatures@stingray@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
#pragma endregion Animals Animation Dictionaries
#pragma region Female
		
#pragma endregion Female Animation Dictionaries
	default:
		return;
	}
}

std::string Animations::GetMovementAnimation(float speed, bool inCover, bool inCoverFacingLeft, bool inLowCover, bool isAiming)
{
	if (inCover)
	{
		if (/*IsShooting &&*/ !isAiming)
		{
			if (inLowCover)
				return inCoverFacingLeft ? "blindfire_low_l_aim_med" : "blindfire_low_r_aim_med";
			return inCoverFacingLeft ? "blindfire_hi_l_aim_med" : "blindfire_hi_r_aim_med";
		}

		return inCoverFacingLeft ? "idle_l_corner" : "idle_r_corner";
	}

	if (speed < 2.0f && speed > 1.0f)
	{
		return "walk";
	}
	else if (speed > 2.0f && speed <= 5.2f)
	{
		return "run";
	}
	else if (speed > 5.2f)
	{
		return "sprint";
	}
	else if (speed < 1.0f)
	{
		return "idle";
	}

	return NULL;
}

/*const bool Animations::IsDictUsed(Entity ent, char * dict)
{
	for (unsigned int i = 0; i < AnimList.size(); i++)
	{
		if (std::strcmp(AnimList[i].dictionary, dict) == 0)
		{
			for (unsigned int a = 0; a < AnimList[i].anims.size(); a++)
			{
				if (ENTITY::IS_ENTITY_PLAYING_ANIM(ent, AnimList[i].dictionary, AnimList[i].anims[a], 3))
					return true;
			}

			return false;
		}
	}

	return false;
}

char* Animations::GetAnim(Entity ent)
{
	for (unsigned int i = 0; i < AnimList.size(); i++)
	{
		for (unsigned int a = 0; a < AnimList[i].anims.size(); a++)
		{
			if (ENTITY::IS_ENTITY_PLAYING_ANIM(ent, AnimList[i].dictionary, AnimList[i].anims[a], 3))
				return AnimList[i].anims[a];
		}
	}

	return NULL;
}*/
