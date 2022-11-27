#include <fxcg/display.h>

#define DISPLAY_WIDTH LCD_WIDTH_PX
#define DISPLAY_HEIGHT LCD_HEIGHT_PX

struct Color {
	int r;
	int g;
	int b;
	unsigned short color;
};

namespace Display {
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
};
