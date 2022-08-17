#pragma once
#include "fp.h"
#include "vector.h"
#include "rmath.h"

class mat4 {
public:
	fp m[4][4];

	mat4(){
		m[0][0] = fp(1);
		m[0][1] = fp(0);
		m[0][2] = fp(0);
		m[0][3] = fp(0);
		
		m[1][0] = fp(0);
		m[1][1] = fp(1);
		m[1][2] = fp(0);
		m[1][3] = fp(0);

		m[2][0] = fp(0);
		m[2][1] = fp(0);
		m[2][2] = fp(1);
		m[2][3] = fp(0);

		m[3][0] = fp(0);
		m[3][1] = fp(0);
		m[3][2] = fp(0);
		m[3][3] = fp(1);
	}

	mat4(fp a, fp b, fp c, fp d,
			fp e, fp f, fp g, fp h,
			fp i, fp j, fp k, fp l,
			fp m, fp n, fp o, fp p){
		this->m[0][0] = a;
		this->m[1][0] = b;
		this->m[2][0] = c;
		this->m[3][0] = d;

		this->m[0][1] = e;
		this->m[1][1] = f;
		this->m[2][1] = g;
		this->m[3][1] = h;

		this->m[0][2] = i;
		this->m[1][2] = j;
		this->m[2][2] = k;
		this->m[3][2] = l;

		this->m[0][3] = m;
		this->m[1][3] = n;
		this->m[2][3] = o;
		this->m[3][3] = p;
	}

	mat4 operator * (mat4 o){
		mat4 r;
		r.m[0][0] = m[0][0] * o.m[0][0]
			+ m[1][0] * o.m[0][1]
			+ m[2][0] * o.m[0][2]
			+ m[3][0] * o.m[0][3];
		r.m[1][0] = m[0][0] * o.m[1][0]
			+ m[1][0] * o.m[1][1]
			+ m[2][0] * o.m[1][2]
			+ m[3][0] * o.m[1][3];
		r.m[2][0] = m[0][0] * o.m[2][0]
			+ m[1][0] * o.m[2][1]
			+ m[2][0] * o.m[2][2]
			+ m[3][0] * o.m[2][3];
		r.m[3][0] = m[0][0] * o.m[3][0]
			+ m[1][0] * o.m[3][1]
			+ m[2][0] * o.m[3][2]
			+ m[3][0] * o.m[3][3];

		r.m[0][1] = m[0][1] * o.m[0][0]
			+ m[1][1] * o.m[0][1]
			+ m[2][1] * o.m[0][2]
			+ m[3][1] * o.m[0][3];
		r.m[1][1] = m[0][1] * o.m[1][0]
			+ m[1][1] * o.m[1][1]
			+ m[2][1] * o.m[1][2]
			+ m[3][1] * o.m[1][3];
		r.m[2][1] = m[0][1] * o.m[2][0]
			+ m[1][1] * o.m[2][1]
			+ m[2][1] * o.m[2][2]
			+ m[3][1] * o.m[2][3];
		r.m[3][1] = m[0][1] * o.m[3][0]
			+ m[1][1] * o.m[3][1]
			+ m[2][1] * o.m[3][2]
			+ m[3][1] * o.m[3][3];

		r.m[0][2] = m[0][2] * o.m[0][0]
			+ m[1][2] * o.m[0][1]
			+ m[2][2] * o.m[0][2]
			+ m[3][2] * o.m[0][3];
		r.m[1][2] = m[0][2] * o.m[1][0]
			+ m[1][2] * o.m[1][1]
			+ m[2][2] * o.m[1][2]
			+ m[3][2] * o.m[1][3];
		r.m[2][2] = m[0][2] * o.m[2][0]
			+ m[1][2] * o.m[2][1]
			+ m[2][2] * o.m[2][2]
			+ m[3][2] * o.m[2][3];
		r.m[3][2] = m[0][2] * o.m[3][0]
			+ m[1][2] * o.m[3][1]
			+ m[2][2] * o.m[3][2]
			+ m[3][2] * o.m[3][3];

		r.m[0][3] = m[0][3] * o.m[0][0]
			+ m[1][3] * o.m[0][1]
			+ m[2][3] * o.m[0][2]
			+ m[3][3] * o.m[0][3];
		r.m[1][3] = m[0][3] * o.m[1][0]
			+ m[1][3] * o.m[1][1]
			+ m[2][3] * o.m[1][2]
			+ m[3][3] * o.m[1][3];
		r.m[2][3] = m[0][3] * o.m[2][0]
			+ m[1][3] * o.m[2][1]
			+ m[2][3] * o.m[2][2]
			+ m[3][3] * o.m[2][3];
		r.m[3][3] = m[0][3] * o.m[3][0]
			+ m[1][3] * o.m[3][1]
			+ m[2][3] * o.m[3][2]
			+ m[3][3] * o.m[3][3];

		return r;
	}

	vec3d operator * (vec3d o){
		return {
			m[0][0] * o.x + m[1][0] * o.y + m[2][0] * o.z + m[3][0],
			m[0][1] * o.x + m[1][1] * o.y + m[2][1] * o.z + m[3][1],
			m[0][2] * o.x + m[1][2] * o.y + m[2][2] * o.z + m[3][2]
		};
	}

	static mat4 translate(mat4 matrix, fp x, fp y, fp z){
		mat4 t;
		t.m[3][0] = x;
		t.m[3][1] = y;
		t.m[3][2] = z;
		return matrix * t;
	}
	static mat4 scale(mat4 matrix, fp x, fp y, fp z){
		mat4 t;
		t.m[0][0] = x;
		t.m[1][1] = y;
		t.m[2][2] = z;
		return matrix * t;
	}
	static mat4 rotateX(mat4 matrix, fp angle){
		mat4 t;
		t.m[1][1] = fp_cos(angle);
		t.m[2][1] = fp(0) - fp_sin(angle);
		t.m[1][2] = fp_sin(angle);
		t.m[2][2] = fp_cos(angle);
		return matrix * t;
	}

	static mat4 rotateY(mat4 matrix, fp angle){
		mat4 t;
		t.m[0][0] = fp_cos(angle);
		t.m[2][0] = fp_sin(angle);
		t.m[0][2] = fp(0) - fp_sin(angle);
		t.m[2][2] = fp_cos(angle);
		return matrix * t;
	}

	static mat4 rotateZ(mat4 matrix, fp angle){
		mat4 t;
		t.m[0][0] = fp_cos(angle);
		t.m[1][0] = fp(0) - fp_sin(angle);
		t.m[0][1] = fp_sin(angle);
		t.m[1][1] = fp_cos(angle);
		return matrix * t;
	}
};
