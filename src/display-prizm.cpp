#ifdef PRIZM
#include "display.h"
#include "util.h"

Color newColor(int r, int g, int b){
	return {
		.r = r,
		.g = g,
		.b = b,
		.color = ((r & 0b11111000)<<8) + ((g & 0b11111100)<<3)+(b>>3),
	};
}

namespace Display {
	int textHeight = 10;
	unsigned short *VRAMAddress;

	void init(){
		VRAMAddress = (unsigned short*)GetVRAMAddress();
	}

	void clear(Color color){
		long v = color.color | (color.color << (8*sizeof(unsigned short)));
		for(int i = 0; i < DISPLAY_WIDTH*DISPLAY_HEIGHT/2; i++){
			((long*)VRAMAddress)[i] = v;
		}
	}
	void destroy(){}
	int textWidth(const char *text){
		return strlen(text)*6;
	}
	void drawText(int x, int y, const char *text, Color color){
		int x2 = x;
		int y2 = y;
		PrintMiniMini(&x2, &y2, text, (1 << 6) | (1 << 1), TEXT_COLOR_WHITE, 0);
	}

	void show() {
		Bdisp_PutDisp_DD();
	}
};
#endif
