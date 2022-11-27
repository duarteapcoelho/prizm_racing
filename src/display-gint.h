#include <gint/display.h>

#define DISPLAY_WIDTH DWIDTH
#define DISPLAY_HEIGHT DHEIGHT

struct Color {
	int r;
	int g;
	int b;
	color_t color;
};

namespace Display {
	inline void fillRect(int x, int y, int w, int h, Color color){
		color_t *s = (color_t*)gint_vram;
		s+=(y*DISPLAY_WIDTH)+x;
		while(h--){
			unsigned w2=w;
			while(w2--)
				*s++=color.color;
			s+=DISPLAY_WIDTH-w;
		}
	}
	inline void drawPoint(int x, int y, Color color){
		gint_vram[DISPLAY_WIDTH*y + x] = color.color;
	}
};
