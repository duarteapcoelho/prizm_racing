#ifdef CE
#include "display.h"
#include <ti/screen.h>
Color newColor(int r, int g, int b){
	return {
		.color = uint8_t((g >> 5) | ((b >> 6) << 3) | ((r >> 5) << 5))
	};
}

namespace Display {
	int textHeight = 0; // TODO
	void init(){
		gfx_Begin();
		os_ClrHome();
		gfx_SetDrawBuffer();
	}
	void clear(Color color){
		fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, color);
	}
	void destroy(){
		gfx_End();
	}

	void show(){
		gfx_SwapDraw();
	}

	int textWidth(const char *text){
		return 0; // TODO
	}
	void drawText(int x, int y, const char *text, Color color){
		// TODO
	}
};

#endif
