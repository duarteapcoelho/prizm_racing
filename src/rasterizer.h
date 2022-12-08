#pragma once

#include "fp.h"
#include "vector.h"
#include "display.h"
#include "mat4.h"

#define NEAR_PLANE 0.1
#define FAR_PLANE 100

#define PIXEL_SIZE 1
#define RENDER_WIDTH (DISPLAY_WIDTH/PIXEL_SIZE)
#define RENDER_HEIGHT (DISPLAY_HEIGHT/PIXEL_SIZE)

struct Triangle {
	vec3<fp> p0;
	vec3<fp> p1;
	vec3<fp> p2;
	vec3<fp> normal;
	Color c;
};

struct Mesh {
	int numTriangles;
	Triangle *triangles;
};

class Model {
public:
	Mesh mesh;
	mat4 modelMatrix;
	mat4 viewMatrix;
	Model();
	Model(Mesh mesh);
	void draw(bool useDepth, bool isShaded, bool clipTriangles);
};

namespace Rasterizer {
	void init();
	void reset();
	extern unsigned char *depthBuffer;

	extern fp fov_d;
	void setFOV(int fov);

	vec3<fp> toDevice(vec3<fp> p);
	vec3<int> toScreen(vec3<fp> p);
	void drawLine(vec3<fp> p0, vec3<fp> p1);
};
