#pragma once

#include "fp.h"

#define PI 3.141592654
#define HALF_PI 1.570796327

inline fp fp_abs(fp v){
	if(v < 0){
		return fp(0) - v;
	} else {
		return v;
	}
}
inline fp fp_floor(fp x){
	int i = (int)x;
	return i - (fp(i) > x);
}

#define SIN_SAMPLES 100
extern fp *sinTable;
void createSinTable();
fp _fp_sin(fp x);

fp fp_sin(fp x);
fp fp_cos(fp x);
fp fp_atan2(fp y, fp x);

float isqrt(float x);
fp fp_isqrt(fp x);
