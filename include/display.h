#pragma once

#ifdef PRIZM
#include <fxcg/display.h>

#define DISPLAY_WIDTH LCD_WIDTH_PX
#define DISPLAY_HEIGHT LCD_HEIGHT_PX
#endif

#ifdef SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define DISPLAY_WIDTH 384
#define DISPLAY_HEIGHT 216
#endif

struct Color {
#ifdef PRIZM
	unsigned short color;
	char textColor;
	bool operator != (Color o){
		return color != o.color;
	}
#endif
#ifdef SDL
	int r;
	int g;
	int b;
	bool operator != (Color o){
		return r != o.r || g != o.g || b != o.b;
	}
#endif
};

Color newColor(int r, int g, int b);

namespace Display {
	extern int textHeight;
	void init();
	void destroy();
	void show();
	void clear(Color color);
	void fillRect(int x, int y, int w, int h, Color color);
	void drawPoint(int x, int y, Color color);
	int textWidth(const char *text);
	void drawText(int x, int y, const char *text, Color color);
};
