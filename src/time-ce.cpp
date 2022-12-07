#ifdef CE
#include "time.h"
#include <sys/rtc.h>

namespace Time {
	void init(){
		rtc_Enable(0);
	}
	void update(){
		const float lastTime = time;
		time = rtc_Time();
		delta = time - lastTime;
	}
};
#endif
