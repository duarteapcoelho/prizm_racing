#ifdef GINT
#include "time.h"
#include <gint/rtc.h>
namespace Time {
	void init(){}
	void update(){
		const float lastTime = time;
		time = rtc_ticks();
		delta = time - lastTime;
	}
};
#endif
