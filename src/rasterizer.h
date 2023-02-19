#pragma once

#include "fp.h"
#include "vector.h"
#include "display.h"

#define RENDER_WIDTH DISPLAY_WIDTH
#define RENDER_HEIGHT DISPLAY_HEIGHT

namespace Rasterizer {
	struct Texture {
		unsigned short *pixels;
		int width;
		int height;
	};

	struct Vertex {
		vec3<fp> pos;
		vec2<fp> texCoord;
	};

	struct Triangle {
		Vertex verts[3];
		Texture *texture;
		vec3<fp> color;
	};

	void init();
	void setFOV(int fov);
	extern fp fov_d;

	void drawTriangle(Triangle triangle);
};
