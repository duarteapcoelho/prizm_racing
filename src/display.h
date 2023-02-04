#pragma once

#ifdef GINT
#include "display-gint.h"
#endif

#ifdef PRIZM
#include "display-prizm.h"
#endif

#ifdef CE
#include "display-ce.h"
#endif

#ifdef SDL
#include "display-sdl.h"
#endif

Color newColor(int r, int g, int b);
Color newColor(unsigned short color);

namespace Display {
	extern int textHeight;
	void init();
	void destroy();
	void show();
	void clear(Color color);
	int textWidth(const char *text);
	void drawText(int x, int y, const char *text, Color color);
};
