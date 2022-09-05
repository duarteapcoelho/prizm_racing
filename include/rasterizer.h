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
	vec3d p0;
	vec3d p1;
	vec3d p2;
	vec3d normal;
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
	extern fp *depthBuffer;

	void setFOV(int fov);

	vec3d toDevice(vec3d p);
	vec3i toScreen(vec3d p);
	void drawLine(vec3d p0, vec3d p1);
	void drawTriangle(Model *model, Triangle triangle, Shader shader, bool useDepth, bool isShaded, bool clipTriangles);
};
