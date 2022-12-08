#ifdef GINT
#include "time.h"
#include <gint/rtc.h>
#include <libprof.h>

namespace Time {
	prof_t prof;
	void init(){
		prof_init();
		prof = prof_make();
	}
	void update(){
		prof_leave(prof);

		const float lastTime = time;
		time = rtc_ticks();
		delta = prof_time(prof) / 1000.0f / (1000.0f / 128.0f);

		prof = prof_make();
		prof_enter(prof);
	}
};
#endif
