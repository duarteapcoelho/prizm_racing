#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define DISPLAY_WIDTH 384
#define DISPLAY_HEIGHT 216

struct Color {
	int r;
	int g;
	int b;
};

extern SDL_Renderer *renderer;

namespace Display {
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
};
