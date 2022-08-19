#pragma once

#include "fp.h"
#include "vector.h"
#include "display.h"

#define NEAR_PLANE 0.1
#define FAR_PLANE 100

#define PIXEL_SIZE 4
#define RENDER_WIDTH (DISPLAY_WIDTH/PIXEL_SIZE)
#define RENDER_HEIGHT (DISPLAY_HEIGHT/PIXEL_SIZE)

struct Triangle {
	vec3d p0;
	vec3d p1;
	vec3d p2;
	Color c;
};

struct Mesh {
	int numTriangles;
	Triangle *triangles;
};

struct Shader {
	vec3d (*vertexShader)(vec3d, void*);
	Color (*fragmentShader)(Color, void*);
	void *uniforms;
};

struct Model {
	Mesh mesh;
	Shader shader;
};

namespace Rasterizer {
	void init();
	void reset();
	extern fp *depthBuffer;

	vec3d toDevice(vec3d p);
	vec3i toScreen(vec3d p);
	void drawLine(vec3d p0, vec3d p1);
	void drawTriangle(Triangle triangle, Shader shader, bool useDepth);
	void drawModel(Model model, bool useDepth);

};
