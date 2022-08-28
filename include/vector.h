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
};
