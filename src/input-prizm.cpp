#ifdef PRIZM
#include "input.h"
#include "util.h"

#define keyboard_register ((unsigned short*)0xA44B0000)
unsigned short lastkey[8] = {0,0,0,0,0,0,0,0};
unsigned short holdkey[8] = {0,0,0,0,0,0,0,0};

namespace Input {
	bool keyDown(int basic_keycode){
		int row, col, word, bit; 
		row = basic_keycode%10; 
		col = basic_keycode/10-1; 
		word = row>>1; 
		bit = col + 8*(row&1); 
		return (0 != (lastkey[word] & 1<<bit)); 
	}
	bool keyDownLast(int basic_keycode){
		int row, col, word, bit; 
		row = basic_keycode%10; 
		col = basic_keycode/10-1; 
		word = row>>1; 
		bit = col + 8*(row&1); 
		return (0 != (holdkey[word] & 1<<bit)); 
	}

	void init(){
	}

	void _updateKeys(){
		memcpy(holdkey, lastkey, sizeof(unsigned short)*8);
		memcpy(lastkey, keyboard_register, sizeof(unsigned short)*8);
	}
};
#endif
