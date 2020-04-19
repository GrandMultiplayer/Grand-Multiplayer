#ifndef __GRANDM_VEHICLE_H__
#define __GRANDM_VEHICLE_H__

namespace GrandM
{
	namespace VehicleData
	{
		struct Paint
		{
			int color = 0;
			int type = 0;
			bool custom = 0;
			Color customCol;
		};

		struct Mod
		{
			int		index = 0;
			bool	Edited = false;
		};

		struct Extra
		{
			bool enabled = false;
			bool edited = false;
		};
	}
}

#endif