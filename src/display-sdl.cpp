#ifdef SDL
#include "display.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *displayTexture;
TTF_Font *font;

Color newColor(int r, int g, int b){
	return {
		.r = r,
		.g = g,
		.b = b
	};
}
Color newColor(unsigned short color){
	return {
		.r = ((color & 0b1111100000000000) >> 11) << 3,
		.g = ((color & 0b0000011111100000) >> 5) << 2,
		.b =  (color & 0b0000000000011111) << 3,
	};
}

namespace Display {
	int textHeight = 0;

	void init() {
		window = SDL_CreateWindow("Window", 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
		if(window == NULL)
			return;
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		if(renderer == NULL)
			return;
		displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH, DISPLAY_HEIGHT);

		SDL_SetRenderTarget(renderer, displayTexture);
		clear(newColor(255, 255, 255));
		show();

		TTF_Init();
		font = TTF_OpenFont("resources/font.ttf", 10);
		TTF_SizeText(font, "A", NULL, &textHeight);
	}

	void destroy(){
		TTF_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void show() {
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, displayTexture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_SetRenderTarget(renderer, displayTexture);
	}

	void clear(Color color){
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_RenderClear(renderer);
	}
	int textWidth(const char *text){
		int w;
		TTF_SizeText(font, text, &w, NULL);
		return w;
	}

	void drawText(int x, int y, const char *text, Color color){
		SDL_Color textColor = {(Uint8)color.r, (Uint8)color.g, (Uint8)color.b, 255};
		SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, textColor);
		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_Rect textRect;
		textRect.x = x;
		textRect.y = y;
		TTF_SizeText(font, text, &textRect.w, &textRect.h);
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

		SDL_DestroyTexture(textTexture);
		SDL_FreeSurface(textSurface);
	}
};
#endif
