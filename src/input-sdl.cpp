#ifdef SDL
#include "input.h"

const Uint8* SDL_Keys = nullptr;
Uint8* lastkey = nullptr;
Uint8* holdkey = nullptr;

int numKeys = 0;

namespace Input {
	bool keyDown(int basic_keycode){
		return lastkey[basic_keycode];
	}
	bool keyDownLast(int basic_keycode){
		return holdkey[basic_keycode];
	}

	void init(){
		SDL_Keys = SDL_GetKeyboardState(&numKeys);
		lastkey = (Uint8*) malloc(sizeof(Uint8)*numKeys);
		holdkey = (Uint8*) malloc(sizeof(Uint8)*numKeys);
	}

	void _updateKeys(){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
		}
		memcpy(holdkey, lastkey, sizeof(Uint8)*numKeys);
		memcpy(lastkey, SDL_Keys, sizeof(Uint8)*numKeys);
	}
};
#endif
