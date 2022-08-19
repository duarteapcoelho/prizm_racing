#pragma once
#include "fp.h"

struct vec3i {
	int x;
	int y;
	int z;
};

struct vec3d {
	fp x;
	fp y;
	fp z;
	vec3d operator + (vec3d o){
		return {x + o.x, y + o.y, z + o.z};
	}
	vec3d operator - (vec3d o){
		return {x - o.x, y - o.y, z - o.z};
	}
	vec3d operator * (vec3d o){
		return {x * o.x, y * o.y, z * o.z};
	}
	vec3d operator / (vec3d o){
		return {x / o.x, y / o.y, z / o.z};
	}
	vec3d operator * (fp o){
		return {x * o, y * o, z * o};
	}
	vec3d operator / (fp o){
		return {x / o, y / o, z / o};
	}

	bool operator == (vec3d o){
		return x == o.x && y == o.y && z == o.z;
	}
};

struct vec3f {
	float x;
	float y;
	float z;
	vec3f operator + (vec3f o){
		return {x + o.x, y + o.y, z + o.z};
	}
	vec3f operator - (vec3f o){
		return {x - o.x, y - o.y, z - o.z};
	}
	vec3f operator * (vec3f o){
		return {x * o.x, y * o.y, z * o.z};
	}
	vec3f operator / (vec3f o){
		return {x / o.x, y / o.y, z / o.z};
	}
	vec3f operator * (float o){
		return {x * o, y * o, z * o};
	}
	vec3f operator / (float o){
		return {x / o, y / o, z / o};
	}

	bool operator == (vec3f o){
		return x == o.x && y == o.y && z == o.z;
	}
};
