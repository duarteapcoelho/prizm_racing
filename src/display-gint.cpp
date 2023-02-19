#ifdef GINT
#include "display.h"
Color newColor(int r, int g, int b){
	return {
		.r = r,
		.g = g,
		.b = b,
		.color = ((r & 0b11111000)<<8) + ((g & 0b11111100)<<3)+(b>>3),
	};
}
Color newColor(unsigned short color){
	return {
		.r = ((color & 0b1111100000000000) >> 11) << 3,
		.g = ((color & 0b0000011111100000) >> 5) << 2,
		.b =  (color & 0b0000000000011111) << 3,
		.color = color,
	};
}

namespace Display {
	unsigned short *vram = nullptr;
	int textHeight = 0;

	void init(){
		vram = gint_vram;
		dsize("a", NULL, NULL, &textHeight);
	}

	void clear(Color color){
		dclear(color.color);
	}
	void destroy(){}
	int textWidth(const char *text){
		int w;
		dsize(text, NULL, &w, NULL);
		return w;
	}
	void drawText(int x, int y, const char *text, Color color){
		dtext(x, y, color.color, text);
	}

	void show() {
		dupdate();
	}
};
#endif
