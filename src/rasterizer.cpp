#include "rasterizer.h"
#include "display.h"
#include "rmath.h"
#include "time.h"

#include "drawTriangle.h"
#define TEXTURED
#include "drawTriangle.h"

#ifdef GINT
#include <gint/dma.h>
#include <gint/mmu.h>
void cache_ocbp(void *buffer, size_t size){
	for(int i = 0; i < size / 32; i++) {
		__asm__("ocbp @%0" :: "r"(buffer));
		buffer += 32;
	}
}
void *mmu_translate_uram(void *ptr){
	return mmu_uram() + ((uint32_t)ptr - 0x08100000);
}
#endif

struct Plane {
	vec3<fp> n;
	fp d;
};

namespace Rasterizer {
	Plane clippingPlanes[5];

	fp fov_d = 1;

	void init(){
		clippingPlanes[0] = {{0, 0, 1}, fp(0)-fp(NEAR_PLANE)}; // near
		clippingPlanes[1] = {{fp(I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // left
		clippingPlanes[2] = {{fp(-I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // right
		clippingPlanes[3] = {{0, fp(I_SQRT_2), fp(I_SQRT_2)}, 0}; // bottom
		clippingPlanes[4] = {{0, fp(-I_SQRT_2), fp(I_SQRT_2)}, 0}; // top
	}

	void setFOV(int fov){
		fp half_fov_rad = fp(fov) * fp(PI) / fp(180) / fp(2);
		fov_d = fp_tan(half_fov_rad) * fp(2);

		half_fov_rad = half_fov_rad + fp(0.1);
		fp a = fp_cos(half_fov_rad - fp(HALF_PI));
		fp b = fp_sin(half_fov_rad - fp(HALF_PI));

		half_fov_rad = fp_atan(fp_tan(half_fov_rad) * (fp(RENDER_HEIGHT)/fp(RENDER_WIDTH)));
		fp c = fp_cos(half_fov_rad - fp(HALF_PI));
		fp d = fp_sin(half_fov_rad - fp(HALF_PI));

		clippingPlanes[0] = {{0, 0, 1}, fp(0)-fp(NEAR_PLANE)}; // near
		clippingPlanes[1] = {{b, 0, a}, 0}; // left
		clippingPlanes[2] = {{fp(0)-b, 0, a}, 0}; // right
		clippingPlanes[3] = {{0, d, c}, 0}; // bottom
		clippingPlanes[4] = {{0, fp(0)-d, c}, 0}; // top
	}

	vec3<fp> toDevice(vec3<fp> p){
		return {
			p.x / p.z / fov_d,
			p.y / p.z / fov_d,
			p.z
		};
	}
	vec3<int> toScreen(vec3<fp> p){
		return {
			p.x * fp(RENDER_WIDTH) + fp(RENDER_WIDTH / 2.0f),
			p.y * fp(RENDER_WIDTH) + fp(RENDER_HEIGHT / 2.0f),
			(int)((p.z - fp(NEAR_PLANE)) * (fp(1000) / fp(FAR_PLANE))) + 1
		};
	}

	inline Mesh clipTriangleAgainstPlane(Triangle triangle, Plane plane){
		int numClipped = 0;
		bool isClippedOut[3] = {false, false, false};

		for(int i = 0; i < 3; i++){
			if(dot(plane.n, triangle.points[i]) + plane.d < 0){
				numClipped++;
				isClippedOut[i] = true;
			}
		}

		Mesh m = {0, nullptr};
		int clippedIn[3] = {-1, -1, -1};
		int clippedOut[3] = {-1, -1, -1};

		for(int i = 0; i < 3; i++){
			if(isClippedOut[i]){
				int j = 0;
				while(clippedOut[j] != -1){
					j++;
				}
				clippedOut[j] = i;
			} else {
				int j = 0;
				while(clippedIn[j] != -1){
					j++;
				}
				clippedIn[j] = i;
			}
		}

		switch(numClipped){
			case 0:
				{
					m = {1, (Triangle*)malloc(sizeof(Triangle))};
					m.triangles[0] = triangle;
					break;
				}
			case 1:
				{
					vec3<fp> inPoint0 = triangle.points[clippedIn[0]];
					vec3<fp> inPoint1 = triangle.points[clippedIn[1]];
					vec3<fp> outPoint = triangle.points[clippedOut[0]];

					fp d0 = dot(plane.n, outPoint - inPoint0);
					fp d1 = dot(plane.n, outPoint - inPoint1);
					if(d0 == 0 || d1 == 0)
						return {0, nullptr};

					vec3<fp> A = inPoint0 + (outPoint - inPoint0) * ((fp(0) - plane.d - dot(plane.n, inPoint0))/d0);
					vec3<fp> B = inPoint1 + (outPoint - inPoint1) * ((fp(0) - plane.d - dot(plane.n, inPoint1))/d1);

					vec2<fp> texCoords0[3];
					vec2<fp> texCoords1[3];
					texCoords0[0] = triangle.texCoords[clippedIn[0]];
					texCoords0[1] = triangle.texCoords[clippedIn[0]] + (triangle.texCoords[clippedOut[0]] - triangle.texCoords[clippedIn[0]]) * ((fp(0) - plane.d - dot(plane.n, inPoint0))/d0);
					texCoords0[2] = triangle.texCoords[clippedIn[1]];

					texCoords1[0] = triangle.texCoords[clippedIn[1]];
					texCoords1[1] = texCoords0[1];
					texCoords1[2] = triangle.texCoords[clippedIn[1]] + (triangle.texCoords[clippedOut[0]] - triangle.texCoords[clippedIn[1]]) * ((fp(0) - plane.d - dot(plane.n, inPoint1))/d1);

					m = {2, (Triangle*)malloc(2*sizeof(Triangle))};
					m.triangles[0] = {{inPoint0, A, inPoint1}, triangle.normal, triangle.c, {texCoords0[0], texCoords0[1], texCoords0[2]}};
					m.triangles[1] = {{inPoint1, A, B}, triangle.normal, triangle.c, {texCoords1[0], texCoords1[1], texCoords1[2]}};
					break;
				}
			case 2:
				{
					vec3<fp> inPoint = triangle.points[clippedIn[0]];
					vec3<fp> outPoint0 = triangle.points[clippedOut[0]];
					vec3<fp> outPoint1 = triangle.points[clippedOut[1]];

					fp d0 = dot(plane.n, outPoint0 - inPoint);
					fp d1 = dot(plane.n, outPoint1 - inPoint);
					if(d0 == 0 || d1 == 0)
						break;

					vec3<fp> B = inPoint + (outPoint0 - inPoint) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d0);
					vec3<fp> C = inPoint + (outPoint1 - inPoint) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d1);

					vec2<float> texCoords[3];
					texCoords[0] = triangle.texCoords[clippedIn[0]];
					texCoords[1] = triangle.texCoords[clippedIn[0]] + (triangle.texCoords[clippedOut[0]] - triangle.texCoords[clippedIn[0]]) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d0);
					texCoords[2] = triangle.texCoords[clippedIn[0]] + (triangle.texCoords[clippedOut[1]] - triangle.texCoords[clippedIn[0]]) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d1);

					m = {1, (Triangle*)malloc(sizeof(Triangle))};
					m.triangles[0] = {{inPoint, B, C}, triangle.normal, triangle.c, {texCoords[0], texCoords[1], texCoords[2]}};
					break;
				}
		}

		return m;
	}

	inline Mesh clipMeshAgainstPlane(Mesh mesh, Plane plane){
		Mesh r = {0, nullptr};

		for(int i = 0; i < mesh.numTriangles; i++){
			Mesh m = clipTriangleAgainstPlane(mesh.triangles[i], plane);
			r.numTriangles += m.numTriangles;
			if(r.triangles == nullptr)
				r.triangles = (Triangle*) malloc(r.numTriangles*sizeof(Triangle));
			else
				r.triangles = (Triangle*) realloc(r.triangles, r.numTriangles*sizeof(Triangle));

			for(int i = r.numTriangles - m.numTriangles; i < r.numTriangles; i++){
				r.triangles[i] = m.triangles[i - (r.numTriangles - m.numTriangles)];
			}

			free(m.triangles);
		}
		free(mesh.triangles);
		return r;
	}

	inline Mesh clipMesh(Mesh mesh){
		for(int i = 0; i < 5; i++){
			mesh = clipMeshAgainstPlane(mesh, clippingPlanes[i]);
		}
		return mesh;
	}

	inline Mesh clipTriangle(Triangle triangle){
		Mesh m = {1, (Triangle*)malloc(sizeof(Triangle))};
		m.triangles[0] = triangle;
		m = clipMesh(m);
		return m;
	}

	inline void drawTriangle(Model *model, Triangle triangle, bool isShaded, bool clip, bool divide){
		if(triangle.points[0] == triangle.points[1] || triangle.points[1] == triangle.points[2] || triangle.points[2] == triangle.points[0]){
			return;
		}

		int inside = 5;
		if(clip){
			for(int i = 0; i < 5; i++){
				if(dot(clippingPlanes[i].n, triangle.points[0]) + clippingPlanes[i].d < 0
						|| dot(clippingPlanes[i].n, triangle.points[1]) + clippingPlanes[i].d < 0
						|| dot(clippingPlanes[i].n, triangle.points[2]) + clippingPlanes[i].d < 0){
					inside--;
					break;
				}
			}
		}

		if(inside == 5){
			if(model->texture != nullptr){
				_drawTriangle_textured(model, triangle, isShaded);
			} else {
				_drawTriangle(model, triangle, isShaded);
			}
		} else if(inside != 0 && divide){
			Mesh mesh = clipTriangle(triangle);
			for(int i = 0; i < mesh.numTriangles; i++){
				if(model->texture != nullptr){
					_drawTriangle_textured(model, mesh.triangles[i], isShaded);
				} else {
					_drawTriangle(model, mesh.triangles[i], isShaded);
				}
			}
			free(mesh.triangles);
		}
	}

};

Model::Model(){
	mesh = {0, nullptr};
}
Model::Model(Mesh mesh, Texture *texture){
	this->mesh = mesh;
	this->texture = texture;
	fp radius2 = 0;
	for(int i = 0; i < mesh.numTriangles; i++){
		for(int i = 0; i < 3; i++){
			radius2 = max(radius2, mesh.triangles[i].points[i].length2());
		}
	}
	float i_radius = _isqrt(radius2);
	radius = 1.0f/i_radius;
}

void Model::draw(bool isShaded, bool divideTriangles, bool clipModel){
	if(!clipModel){
		vec3<fp> center = viewMatrix * modelMatrix * vec3<fp>(0, 0, 0);
		for(int i = 0; i < 5; i++){
			if(dot(Rasterizer::clippingPlanes[i].n, center) + Rasterizer::clippingPlanes[i].d < radius){
				return;
			}
		}
	}

	for(int i = 0; i < mesh.numTriangles; i++){
		if(dot(mat4::toMat3(viewMatrix) * mat4::toMat3(modelMatrix) * mesh.triangles[i].normal, vec3<fp>(0, 0, 1)) > 0.3){
			continue;
		}

		Triangle t = mesh.triangles[i];
		for(int j = 0; j < 3; j++){
			t.points[j] = viewMatrix * modelMatrix * t.points[j];
		}
		Rasterizer::drawTriangle(this, t, isShaded, clipModel, divideTriangles);
	}
}
