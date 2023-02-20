#pragma once

#include "rasterizer.h"
#include "vector.h"
#include "fp.h"
#include "mat4.h"

namespace Renderer {
	struct Material {
		Rasterizer::Texture *texture;
		vec3<fp> color;
		bool isShaded;
	};

	struct Vertex {
		vec3<fp> pos;
		vec2<fp> texCoord;
	};

	struct Triangle {
		Vertex verts[3];
		vec3<fp> normal;
		unsigned char material;
	};

	struct Mesh {
		int numTriangles;
		Triangle *triangles;
	};

	class Model {
		void drawTriangle(Triangle triangle, bool addToOrdTbl);
	public:
		Material *materials;
		Mesh mesh;
		mat4 modelMatrix, viewMatrix;
		Model();
		Model(Mesh mesh, Material *materials);

		void draw(bool addToOrdTbl);
	};
};
