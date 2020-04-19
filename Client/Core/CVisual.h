#ifndef __CVISUAL_H__
#define __CVISUAL_H__

class CVisual
{
public:
	static void ShowMessageAboveMap(const char *message, const char *pic, const int icontype, const int color, const char *sender, const char *subject);
	static void ShowSubtitle(const char *message, const int duration, const bool shownow);
	static void TextDraw(const char *text, float x, float y, float scale, int font, int r, int g, int b, int a, bool center, bool shadow, bool outline);
};
#endif