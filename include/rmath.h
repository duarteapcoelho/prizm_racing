#pragma once

#include "fp.h"

#define I_SQRT_2 0.707106
#define I_SQRT_3 0.577350
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

#define SIN_SAMPLES 1000
extern fp *sinTable;
void createSinTable();
fp _fp_sin(fp x);

fp fp_sin(fp x);
fp fp_cos(fp x);
fp fp_tan(fp x);
fp fp_atan(fp x);
fp fp_atan2(fp y, fp x);

float _isqrt(float x);

template <typename T>
T isqrt(T x){
	return T(_isqrt(float(x)));
}
