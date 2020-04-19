#ifndef __GRANDM_PED_H__
#define __GRANDM_PED_H__
#pragma once

namespace GrandM
{
	namespace Model
	{
		struct Component
		{
			int drawableid = -1;
			int textureid = -1;
			int paletteid = -1;
		};

		struct HeadBlend
		{
			int shapeFirst = 0;
			int shapeSecond = 0;
			int shapeThird = 0;
			int skinFirst = 0;
			int skinSecond = 0;
			int skinThird = 0;
			float shapeMix = 0;
			float skinMix = 0;
			float thirdMix = 0;
		};

		struct HeadOverlay
		{
			int index = 0;
			float opacity = 0.0f;
			int colorType = 0;
			int colorID = 0;
			int secondColorID = 0;
		};

		struct Prop
		{
			int drawableid = 0;
			int textureid = 0;
		};
		
		struct FaceFeature
		{
			float scale = 0.0f;
		};
	}
}

#endif