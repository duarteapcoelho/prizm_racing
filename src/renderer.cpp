#include "renderer.h"
#include "rasterizer.h"

namespace Renderer {
	Model::Model(){
		mesh = {0, nullptr};
		materials = nullptr;
	}
	Model::Model(Mesh mesh, Material *materials){
		this->mesh = mesh;
		this->materials = materials;
	}

	void Model::draw(){
		for(int i = 0; i < mesh.numTriangles; i++){
			drawTriangle(mesh.triangles[i]);
		}
	}

	void Model::drawTriangle(Triangle triangle){
		vec3<fp> color = materials[triangle.material].color;
		fp brightness = 1;
		if(materials[triangle.material].isShaded){
			brightness = dot(mat4::toMat3(modelMatrix) * triangle.normal, vec3<fp>(I_SQRT_3, -I_SQRT_3, -I_SQRT_3)) * fp(0.5) + fp(0.5);
		}
		color = color * brightness;

		mat4 m = viewMatrix * modelMatrix;

		if(dot(mat4::toMat3(m) * triangle.normal, vec3<fp>(0, 0, 1)) > 0)
			return;

		for(int i = 0; i < 3; i++){
			// convert to view space
			triangle.verts[i].pos = m * triangle.verts[i].pos;

			// correct aspect ratio
			triangle.verts[i].pos.y = triangle.verts[i].pos.y * (fp(RENDER_WIDTH) / fp(RENDER_HEIGHT));
		}

		Rasterizer::Triangle t;
		for(int i = 0; i < 3; i++){
			t.verts[i].pos = triangle.verts[i].pos;
			t.verts[i].texCoord = triangle.verts[i].texCoord;
		}
		t.color = color;
		t.texture = materials[triangle.material].texture;
		Rasterizer::drawTriangle(t);
	}
};
