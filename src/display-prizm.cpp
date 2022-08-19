#ifdef PRIZM
#include "display.h"
#include "util.h"
#include <fxcg/display.h>

Color newColor(int r, int g, int b){
	return {
		.color = ((r & 0b11111000)<<8) + ((g & 0b11111100)<<3)+(b>>3),
		.textColor = r == 255 ? TEXT_COLOR_WHITE : TEXT_COLOR_BLACK
	};
}

namespace Display {
	int textHeight = 10;
	unsigned short *VRAMAddress;

	void init(){
		VRAMAddress = (unsigned short*)GetVRAMAddress();
	}

	void clear(Color color){
		unsigned short *s=(unsigned short*)GetVRAMAddress();
		for(int y = 0; y < DISPLAY_HEIGHT; y++){
			for(int x = 0; x < DISPLAY_WIDTH; x++){
				*s = color.color;
				s++;
			}
		}
	}
	void destroy(){}
	void fillRect(int x, int y, int w, int h, Color color){
		unsigned short*s=VRAMAddress;
		s+=(y*384)+x;
		while(h--){
			unsigned w2=w;
			while(w2--)
				*s++=color.color;
			s+=384-w;
		}
	}
	void drawPoint(int x, int y, Color color){
		// Bdisp_SetPoint_VRAM(x, y, color.color);
		*(VRAMAddress + x + y * DISPLAY_WIDTH) = color.color;
	}
	int textWidth(const char *text){
		return strlen(text)*6;
	}
	void drawText(int x, int y, const char *text, Color color){
		int x2 = x;
		int y2 = y;
		PrintMiniMini(&x2, &y2, text, (1 << 6) | (1 << 1), color.textColor, 0);
	}

	void show() {
		Bdisp_PutDisp_DD();
	}
};
#endif
