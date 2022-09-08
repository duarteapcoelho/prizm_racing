#pragma once
#include "fp.h"
#include "rmath.h"

struct vec3i {
	int x;
	int y;
	int z;
};

struct vec3d {
	fp x;
	fp y;
	fp z;
	inline vec3d operator + (vec3d o){
		return {x + o.x, y + o.y, z + o.z};
	}
	inline vec3d operator - (vec3d o){
		return {x - o.x, y - o.y, z - o.z};
	}
	inline vec3d operator * (vec3d o){
		return {x * o.x, y * o.y, z * o.z};
	}
	inline vec3d operator / (vec3d o){
		return {x / o.x, y / o.y, z / o.z};
	}
	inline vec3d operator * (fp o){
		return {x * o, y * o, z * o};
	}
	inline vec3d operator / (fp o){
		return {x / o, y / o, z / o};
	}

	inline bool operator == (vec3d o){
		return x == o.x && y == o.y && z == o.z;
	}

	inline vec3d normalized(){
		fp i_d = fp_isqrt(x*x + y*y + z*z);
		return (*this) * i_d;
	}
	inline fp i_length(){
		return fp_isqrt(x*x + y*y + z*z);
	}
};

struct vec3f {
	float x;
	float y;
	float z;
	inline vec3f operator + (vec3f o){
		return {x + o.x, y + o.y, z + o.z};
	}
	inline vec3f operator - (vec3f o){
		return {x - o.x, y - o.y, z - o.z};
	}
	inline vec3f operator * (vec3f o){
		return {x * o.x, y * o.y, z * o.z};
	}
	inline vec3f operator / (vec3f o){
		return {x / o.x, y / o.y, z / o.z};
	}
	inline vec3f operator * (float o){
		return {x * o, y * o, z * o};
	}
	inline vec3f operator / (float o){
		return {x / o, y / o, z / o};
	}

	inline bool operator == (vec3f o){
		return x == o.x && y == o.y && z == o.z;
	}
	inline operator vec3d(){
		return {.x = x, .y = y, .z = z};
	}
	inline vec3f normalized(){
		fp i_d = fp_isqrt(x*x + y*y + z*z);
		return (*this) * i_d;
	}
	inline float i_length(){
		return isqrt(x*x + y*y + z*z);
	}
};

inline vec3f smoothDamp(vec3f current, vec3f target, vec3f *currentVelocity, float smoothTime, float deltaTime, float maxSpeed){
	if(smoothTime < 0.0001f) smoothTime = 0.0001f;
	float omega = 2.0f / smoothTime;

	float x = omega * deltaTime;
	float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

	vec3f change = current - target;
	vec3f originalTo = target;

	float maxChange = maxSpeed * smoothTime;
	float maxChangeSq = maxChange * maxChange;
	float sqDist = change.x * change.x + change.y * change.y + change.z * change.z;
	if(sqDist > maxChangeSq){
		float mag = 1.0f/isqrt(sqDist);
		change = change / mag * maxChange;
	}

	target = current - change;

	vec3f temp = ((*currentVelocity) + change * omega) * deltaTime;

	(*currentVelocity) = ((*currentVelocity) - temp * omega) * exp;

	vec3f output = target + (change + temp) * exp;

	vec3f origMinusCurrent = originalTo - current;
	vec3f outMinusOrig = output - originalTo;

	if(origMinusCurrent.x * outMinusOrig.x + origMinusCurrent.y * outMinusOrig.y > 0){
		output = originalTo;

		(*currentVelocity) = (output - originalTo) / deltaTime;
	}
	return output;
}
