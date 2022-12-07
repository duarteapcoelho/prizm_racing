#ifdef PRIZM
#include "time.h"
#include <fxcg/rtc.h>

namespace Time {
	void init(){}
	void update(){
		const float lastTime = time;
		time = RTC_GetTicks();
		delta = time - lastTime;
	}
};
#endif
