#include "API_Joaat.h"

#include "SharedUtility.h"
#include "Joaat.h"

namespace API
{
	namespace API_Joaat
	{
		uint32_t Joaat(const char * string)
		{
			return jenkins_joaat(string, strlen(string));
		}
	}
}