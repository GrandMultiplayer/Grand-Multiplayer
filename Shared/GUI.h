#ifndef __GRANDM_GUI_H__
#define __GRANDM_GUI_H__
#include <string>

namespace GrandM
{
	namespace GUI
	{
		struct Tag
		{
			std::string		text			= "GrandM";

			bool			textVisible		= true;
			bool			healthVisible	= true;

			float			distance		= 20.0f;
			Color			textColor		= { 255,255,255,255 };
		};

		struct GameHud
		{
			bool			displayHud		= true;
			bool			displayMinimap	= true;
			bool			displayCash		= true;
			bool			displayAmmo		= true;
		};
	}
}

#endif