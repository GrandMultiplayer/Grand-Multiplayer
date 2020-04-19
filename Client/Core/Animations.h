#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__
#include <vector>

struct AnimDict
{
public:
	char* dictionary;
	std::vector<char*> anims;
};
//extern std::vector<AnimDict> AnimList;

class Animations
{
public:
	static void GetAnimalAnimation(int modelhash, int movement, std::string *dict, std::string *name);
	static std::string GetMovementAnimation(float speed, bool inCover, bool inCoverFacingLeft, bool inLowCover, bool isAiming);

	//static const bool IsDictUsed(Entity ent, char * dict);
	//static char* GetAnim(Entity ent);
};

#endif