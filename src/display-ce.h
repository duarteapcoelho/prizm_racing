#include <graphx.h>
#define DISPLAY_WIDTH GFX_LCD_WIDTH
#define DISPLAY_HEIGHT GFX_LCD_HEIGHT

struct Color {
	int r;
	int g;
	int b;
	unsigned char color;
};

namespace Display {
	inline void fillRect(int x, int y, int w, int h, Color color){
		gfx_SetColor(color.color);
		gfx_FillRectangle(x, y, w, h);
	}
	inline void drawPoint(int x, int y, Color color){
		gfx_SetColor(color.color);
		gfx_SetPixel(x, y);
	}
};
