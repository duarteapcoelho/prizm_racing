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
extern SDL_Renderer *renderer;
#endif

struct Color {
	int r;
	int g;
	int b;
#ifdef PRIZM
	unsigned short color;
	bool operator != (Color o){
		return color != o.color;
	}
#endif
#ifdef SDL
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
	int textWidth(const char *text);
	void drawText(int x, int y, const char *text, Color color);

#ifdef PRIZM
	extern unsigned short *VRAMAddress;
	inline void fillRect(int x, int y, int w, int h, Color color){
		unsigned short*s=VRAMAddress;
		s+=(y*384)+x;
		while(h--){
			unsigned w2=w;
			while(w2--)
				*s++=color.color;
			s+=384-w;
		}
	}
	inline void drawPoint(int x, int y, Color color){
		*(VRAMAddress + x + y * DISPLAY_WIDTH) = color.color;
	}
#endif
#ifdef SDL
	inline void fillRect(int x, int y, int w, int h, Color color){
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_RenderFillRect(renderer, &rect);
	}
	inline void drawPoint(int x, int y, Color color){
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_RenderDrawPoint(renderer, x, y);
	}
#endif
};
