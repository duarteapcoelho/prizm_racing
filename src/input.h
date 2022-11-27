#pragma once

#ifdef GINT
#include "input-gint.h"
#endif

#ifdef PRIZM
#include "input-prizm.h"
#endif

#ifdef CE
#include "input-ce.h"
#endif

#ifdef SDL
#include "input-sdl.h"
#endif

namespace Input {
	extern int holdDelay;
	extern int minRepeatDelay;

	void init();
	void update();
	void _updateKeys();

	bool keyPressed(int key);
	bool keyPressedRepeat(int key);
	bool keyDown(int key);
	bool keyDownLast(int key);

	int pressedNumber();
	bool pressedDirection(int *x, int *y);
	char pressedCharacter();
	int pressedFunctionKey();

	bool shiftDown();
	bool alphaDown();
};
