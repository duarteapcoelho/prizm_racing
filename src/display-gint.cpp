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

namespace Display {
	int textHeight = 0;

	void init(){
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
