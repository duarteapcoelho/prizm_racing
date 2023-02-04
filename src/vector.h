#pragma once
#include "fp.h"
#include "rmath.h"

template <typename T>
struct vec2 {
	T x;
	T y;
	inline vec2(){
		x = 0;
		y = 0;
	}
	inline vec2(T x, T y){
		this->x = x;
		this->y = y;
	}
	inline vec2<T> operator + (vec2<T> o){
		return {x + o.x, y + o.y};
	}
	inline vec2<T> operator - (vec2<T> o){
		return {x - o.x, y - o.y};
	}
	inline vec2<T> operator * (vec2<T> o){
		return {x * o.x, y * o.y};
	}
	inline vec2<T> operator / (vec2<T> o){
		return {x / o.x, y / o.y};
	}
	inline vec2<T> operator * (T o){
		return {x * o, y * o};
	}
	inline vec2<T> operator / (T o){
		return {x / o, y / o};
	}

	inline bool operator == (vec2<T> o){
		return x == o.x && y == o.y;
	}

	inline vec2<T> normalized(){
		T i_d = isqrt<T>(x*x + y*y);
		return (*this) * i_d;
	}
	inline T i_length(){
		return isqrt<T>(x*x + y*y);
	}
	inline T length2(){
		return x*x + y*y;
	}
	template <typename U>
	inline operator vec2<U>(){
		return {x, y};
	}
};

template <typename T>
struct vec3 {
	T x;
	T y;
	T z;
	inline vec3(){
		x = 0;
		y = 0;
		z = 0;
	}
	inline vec3(T x, T y, T z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	inline vec3<T> operator + (vec3<T> o){
		return {x + o.x, y + o.y, z + o.z};
	}
	inline vec3<T> operator - (vec3<T> o){
		return {x - o.x, y - o.y, z - o.z};
	}
	inline vec3<T> operator * (vec3<T> o){
		return {x * o.x, y * o.y, z * o.z};
	}
	inline vec3<T> operator / (vec3<T> o){
		return {x / o.x, y / o.y, z / o.z};
	}
	inline vec3<T> operator * (T o){
		return {x * o, y * o, z * o};
	}
	inline vec3<T> operator / (T o){
		return {x / o, y / o, z / o};
	}

	inline bool operator == (vec3<T> o){
		return x == o.x && y == o.y && z == o.z;
	}

	inline vec3<T> normalized(){
		T i_d = isqrt<T>(x*x + y*y + z*z);
		return (*this) * i_d;
	}
	inline T i_length(){
		return isqrt<T>(x*x + y*y + z*z);
	}
	inline T length2(){
		return x*x + y*y + z*z;
	}
	template <typename U>
	inline operator vec3<U>(){
		return {x, y, z};
	}
	template <typename U>
	inline operator vec2<U>(){
		return {x, y};
	}
};

template <typename T>
inline vec3<T> smoothDamp(vec3<T> current, vec3<T> target, vec3<T> *currentVelocity, T smoothTime, T deltaTime, T maxSpeed){
	if(smoothTime < 0.0001f) smoothTime = 0.0001f;
	T omega = 2.0f / smoothTime;

	T x = omega * deltaTime;
	T exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

	vec3<T> change = current - target;
	vec3<T> originalTo = target;

	T maxChange = maxSpeed * smoothTime;
	T maxChangeSq = maxChange * maxChange;
	T sqDist = change.x * change.x + change.y * change.y + change.z * change.z;
	if(sqDist > maxChangeSq){
		T mag = 1.0f/isqrt(sqDist);
		change = change / mag * maxChange;
	}

	target = current - change;

	vec3<T> temp = ((*currentVelocity) + change * omega) * deltaTime;

	(*currentVelocity) = ((*currentVelocity) - temp * omega) * exp;

	vec3<T> output = target + (change + temp) * exp;

	vec3<T> origMinusCurrent = originalTo - current;
	vec3<T> outMinusOrig = output - originalTo;

	if(origMinusCurrent.x * outMinusOrig.x + origMinusCurrent.y * outMinusOrig.y > 0){
		output = originalTo;

		(*currentVelocity) = (output - originalTo) / deltaTime;
	}
	return output;
}

template <typename T>
inline T dot(vec3<T> a, vec3<T> b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

template <typename T>
inline T dot(vec2<T> a, vec2<T> b){
	return a.x*b.x + a.y*b.y;
}

template <typename T>
inline T cross(vec2<T> a, vec2<T> b){
	return (a.x*b.y) - (a.y*b.x);
}

template <typename T>
inline T cross(vec3<T> a, vec3<T> b){
	return (a.x*b.y) - (a.y*b.x);
}
