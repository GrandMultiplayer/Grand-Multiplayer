#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "Functions.h"

Hash $(std::string str) {
	return GAMEPLAY::GET_HASH_KEY(&str[0u]);
}