#pragma once

#ifdef PRIZM
#include <fxcg/keyboard.h>

#define KEY_MENU KEY_PRGM_MENU
#define KEY_EXIT KEY_PRGM_EXIT
#define KEY_EXE 31
#define KEY_AC 10
#define KEY_DEL 44
#define KEY_OPTN 68
#define KEY_VARS 58

#define KEY_0 KEY_PRGM_0
#define KEY_1 KEY_PRGM_1
#define KEY_2 KEY_PRGM_2
#define KEY_3 KEY_PRGM_3
#define KEY_4 KEY_PRGM_4
#define KEY_5 KEY_PRGM_5
#define KEY_6 KEY_PRGM_6
#define KEY_7 KEY_PRGM_7
#define KEY_8 KEY_PRGM_8
#define KEY_9 KEY_PRGM_9

#define KEY_A 76
#define KEY_B 66
#define KEY_C 56
#define KEY_D 46
#define KEY_E 36
#define KEY_F 26
#define KEY_G 75
#define KEY_H 65
#define KEY_I 55
#define KEY_J 45
#define KEY_K 35
#define KEY_L 25
#define KEY_M 74
#define KEY_N 64
#define KEY_O 54
#define KEY_P 73
#define KEY_Q 63
#define KEY_R 53
#define KEY_S 43
#define KEY_T 33
#define KEY_U 72
#define KEY_V 62
#define KEY_W 52
#define KEY_X 42
#define KEY_Y 32
#define KEY_Z 71

#define KEY_F1 79
#define KEY_F2 69
#define KEY_F3 59
#define KEY_F4 49
#define KEY_F5 39
#define KEY_F6 29

#define KEY_UP KEY_PRGM_UP
#define KEY_DOWN KEY_PRGM_DOWN
#define KEY_LEFT KEY_PRGM_LEFT
#define KEY_RIGHT KEY_PRGM_RIGHT

#define KEY_SHIFT 78

#define keyboard_register ((unsigned short*)0xA44B0000)
#endif

#ifdef SDL
#include <SDL2/SDL.h>

#define KEY_MENU SDL_SCANCODE_TAB
#define KEY_EXIT SDL_SCANCODE_ESCAPE
#define KEY_EXE SDL_SCANCODE_SPACE
#define KEY_AC SDL_SCANCODE_BACKSPACE
#define KEY_DEL SDL_SCANCODE_DELETE
#define KEY_OPTN SDL_SCANCODE_BACKSPACE
#define KEY_VARS SDL_SCANCODE_BACKSPACE

#define KEY_0 SDL_SCANCODE_0
#define KEY_1 SDL_SCANCODE_1
#define KEY_2 SDL_SCANCODE_2
#define KEY_3 SDL_SCANCODE_3
#define KEY_4 SDL_SCANCODE_4
#define KEY_5 SDL_SCANCODE_5
#define KEY_6 SDL_SCANCODE_6
#define KEY_7 SDL_SCANCODE_7
#define KEY_8 SDL_SCANCODE_8
#define KEY_9 SDL_SCANCODE_9

#define KEY_A SDL_SCANCODE_A
#define KEY_B SDL_SCANCODE_B
#define KEY_C SDL_SCANCODE_C
#define KEY_D SDL_SCANCODE_D
#define KEY_E SDL_SCANCODE_E
#define KEY_F SDL_SCANCODE_F
#define KEY_G SDL_SCANCODE_G
#define KEY_H SDL_SCANCODE_H
#define KEY_I SDL_SCANCODE_I
#define KEY_J SDL_SCANCODE_J
#define KEY_K SDL_SCANCODE_K
#define KEY_L SDL_SCANCODE_L
#define KEY_M SDL_SCANCODE_M
#define KEY_N SDL_SCANCODE_N
#define KEY_O SDL_SCANCODE_O
#define KEY_P SDL_SCANCODE_P
#define KEY_Q SDL_SCANCODE_Q
#define KEY_R SDL_SCANCODE_R
#define KEY_S SDL_SCANCODE_S
#define KEY_T SDL_SCANCODE_T
#define KEY_U SDL_SCANCODE_U
#define KEY_V SDL_SCANCODE_V
#define KEY_W SDL_SCANCODE_W
#define KEY_X SDL_SCANCODE_X
#define KEY_Y SDL_SCANCODE_Y
#define KEY_Z SDL_SCANCODE_Z

#define KEY_F1 SDL_SCANCODE_F1
#define KEY_F2 SDL_SCANCODE_F2
#define KEY_F3 SDL_SCANCODE_F3
#define KEY_F4 SDL_SCANCODE_F4
#define KEY_F5 SDL_SCANCODE_F5
#define KEY_F6 SDL_SCANCODE_F6

#define KEY_UP SDL_SCANCODE_UP
#define KEY_DOWN SDL_SCANCODE_DOWN
#define KEY_LEFT SDL_SCANCODE_LEFT
#define KEY_RIGHT SDL_SCANCODE_RIGHT

#define KEY_SHIFT SDL_SCANCODE_LSHIFT
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
