#ifdef GINT
#include "input.h"
#include "util.h"
#include <gint/drivers/keydev.h>
uint8_t keyState[12];
uint8_t lastKeyState[12];
namespace Input {
	bool keyDown(int key){
		int row = (key >> 4);
		int col = 0x80 >> (key & 0x7);

		return (keyState[row] & col) != 0;
	}
	bool keyDownLast(int key){
		int row = (key >> 4);
		int col = 0x80 >> (key & 0x7);

		return (lastKeyState[row] & col) != 0;
	}

	void init(){
	}

	void _updateKeys(){
		while(pollevent().type != KEYEV_NONE);
		memcpy(lastKeyState, keyState, sizeof(uint8_t)*12);
		memcpy(keyState, keydev_std()->state_now, sizeof(uint8_t)*12);
	}
};
#endif
