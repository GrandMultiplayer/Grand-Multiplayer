#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

#include "CVisual.h"

void CVisual::ShowMessageAboveMap(const char *message, const char *pic, const int icontype, const int color, const char *sender, const char *subject)
{
	UI::_SET_NOTIFICATION_BACKGROUND_COLOR(color);
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)message);
	UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG((char*)pic, (char*)pic, false, icontype, (char*)sender, (char*)subject, 1.0f, "");
	UI::_DRAW_NOTIFICATION(FALSE, FALSE);
}

void CVisual::ShowSubtitle(const char *message, const int duration, const bool shownow)
{
	UI::BEGIN_TEXT_COMMAND_PRINT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)message);
	UI::END_TEXT_COMMAND_PRINT(duration, shownow);
}


void CVisual::TextDraw(const char *text, float x, float y, float scale, int font, int r, int g, int b, int a, bool center, bool shadow, bool outline)
{
	//UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scale, scale);
	UI::SET_TEXT_COLOUR(r, g, b, a);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(center);
	if (shadow)
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	if (outline)
		UI::SET_TEXT_OUTLINE();
	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
}