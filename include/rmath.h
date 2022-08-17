#pragma once

#include "fp.h"

#define PI 3.141592654
#define HALF_PI 1.570796327

fp fp_abs(fp v);
fp fp_floor(fp x);

#define SIN_SAMPLES 100
extern fp *sinTable;
void createSinTable();
fp _fp_sin(fp x);

fp fp_sin(fp x);
fp fp_cos(fp x);
fp fp_atan(fp x);
