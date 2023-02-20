#pragma once

#include "fp.h"
#include "vector.h"
#include "display.h"

#define RENDER_WIDTH DISPLAY_WIDTH
#define RENDER_HEIGHT DISPLAY_HEIGHT

#define NEAR_PLANE 1
#define FAR_PLANE 1000

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

	struct OrdTblNode {
		Triangle *t;
		OrdTblNode *next;
	};

	extern OrdTblNode **ordTbl;

	void init(OrdTblNode **ot);
	void setFOV(int fov);
	extern fp fov_d;

	void drawTriangle(Triangle triangle);
	void addTriangle(Triangle triangle, unsigned char depth);

	void drawOrdTbl();
	void clearOrdTbl();
};
