#include "rmath.h"

const double DOUBLE_PI   = 6.283185307179586;
const double SIN_CURVE_A = 0.0415896;
const double SIN_CURVE_B = 0.00129810625032;

float _float_cos(float x) {
    if (x < 0) {
        int q = -x / DOUBLE_PI;
        q += 1;
        float y = q * DOUBLE_PI;
        x = -(x - y);
    }
    if (x >= DOUBLE_PI) {
        int q = x / DOUBLE_PI;
        float y = q * DOUBLE_PI;
        x = x - y;
    }
    int s = 1;
    if (x >= PI) {
        s = -1;
        x -= PI;
    }
    if (x > HALF_PI) {
        x = PI - x;
        s = -s;
    }
    float z = x * x;
    float r = z * (z * (SIN_CURVE_A - SIN_CURVE_B * z) - 0.5) + 1.0;
    if (r > 1.0) r = r - 2.0;
    if (s > 0) return r;
    else return -r;
}

float _float_sin(float x) {
    return _float_cos(x - HALF_PI);
}

fp *sinTable = nullptr;
fp _fp_sin(fp x){
	return fp(_float_sin((float)x));
}

void createSinTable(){
	for(int i = 0; i < SIN_SAMPLES; i++){
		sinTable[i] = _fp_sin(fp(i)*(fp(PI) / fp(SIN_SAMPLES)));
	}
}

fp fp_sin(fp x){
	bool up = true;
	while(x >= fp(PI)){
		x = x - fp(PI);
		up = !up;
	}
	if(x < 0)
		return fp(0)-fp_sin(fp(0)-x);
	int i = x/(fp(PI) / fp(SIN_SAMPLES));
	if(i >= SIN_SAMPLES)
		i = SIN_SAMPLES-1;
	fp r = sinTable[i];
	if(!up)
		r = fp(0) - r;
	return r;
}

fp fp_cos(fp x){
	return fp_sin(x+fp(HALF_PI));
}

fp fp_tan(fp x){
	return fp_sin(x) / fp_cos(x);
}

fp _fp_atan(fp x){
	return fp(PI)/fp(4) * x - x * (fp_abs(x) - fp(1)) * (fp(0.245) + fp(0.066) * fp_abs(x));
}
fp fp_atan(fp x){
	if(x < 1)
		return _fp_atan(x);
	else
		return fp(HALF_PI) - _fp_atan(fp(1)/x);
}
fp fp_atan2(fp y, fp x){
	if(x == 0 || y == 0)
		return 0;
	if(x >= 0) {
		if(y >= 0) {
			if(y < x) {
				return fp_atan(y / x);
			} else {
				return fp(PI)*fp(2) - fp_atan(x / y);
			}
		} else {
			if(fp(0)-y < x){
				return fp_atan(y / x);
			} else {
				return fp(0)-fp(PI)*fp(2) - fp_atan(x / y);
			}
		}
	} else {
		if(y >= 0){
			if(y < fp(0)-x){
				return fp_atan(y / x) + fp(PI);
			} else {
				return fp(PI)*fp(2) - fp_atan(x / y);
			}
		} else {
			if(fp(0)-y < fp(0)-x){
				return fp_atan(y / x) - fp(PI);
			} else {
				return fp(0)-fp(PI)*fp(2) - fp_atan(x / y);
			}
		}
	}
}

float isqrt(float x){
	float xhalf = 0.5f * x;
	int i = *(int*)&x;            // store floating-point bits in integer
	i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
	x = *(float*)&i;              // convert new bits into float
	x = x*(1.5f - xhalf*x*x);     // One round of Newton's method
	return x;
}

fp fp_isqrt(fp x){
	return fp(isqrt(float(x)));
}
