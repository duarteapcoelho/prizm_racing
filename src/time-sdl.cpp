#ifdef SDL
#include "time.h"
#include <SDL2/SDL.h>

namespace Time {
	void init(){
	}
	void update(){
		const float lastTime = time;
		time = ((float)(SDL_GetTicks()) / (1000.0/128.0));
		delta = time - lastTime;
	}
};
#endif
