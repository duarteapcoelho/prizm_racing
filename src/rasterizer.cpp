#include "rasterizer.h"
#include "display.h"
#include "rmath.h"

vec3<int> toScreenCoords(vec3<fp> p){
	return {
		p.x * fp(RENDER_WIDTH) + fp(RENDER_WIDTH / 2),
		p.y * fp(RENDER_HEIGHT) + fp(RENDER_HEIGHT / 2),
		p.z
	};
}

#include "drawTriangle.h"
#define TEXTURED
#include "drawTriangle.h"

namespace Rasterizer {
	OrdTblNode **ordTbl;

	struct Plane {
		vec3<fp> n;
		fp d;
	};
	Plane clippingPlanes[6];
	fp fov_d = 1;
	void initClippingPlanes(){
		clippingPlanes[0] = {{0, 0, 1}, fp(0)-fp(NEAR_PLANE)}; // near
		clippingPlanes[1] = {{0, 0, -1}, fp(FAR_PLANE)}; // far
		clippingPlanes[2] = {{fp(I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // left
		clippingPlanes[3] = {{fp(-I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // right
		clippingPlanes[4] = {{0, fp(I_SQRT_2), fp(I_SQRT_2)}, 0}; // bottom
		clippingPlanes[5] = {{0, fp(-I_SQRT_2), fp(I_SQRT_2)}, 0}; // top
	}

	void setFOV(int fov){
		fp half_fov_rad = fp(fov) * fp(PI) / fp(180) / fp(2);
		fov_d = fp_tan(half_fov_rad) * fp(2);

		half_fov_rad = half_fov_rad + fp(0.1);
		fp a = fp_cos(half_fov_rad - fp(HALF_PI));
		fp b = fp_sin(half_fov_rad - fp(HALF_PI));

		half_fov_rad = fp_atan(fp_tan(half_fov_rad) * (fp(RENDER_HEIGHT)/fp(RENDER_WIDTH)));
		fp c = a;
		fp d = b;

		clippingPlanes[0] = {{0, 0, 1}, fp(0)-fp(NEAR_PLANE)}; // near
		clippingPlanes[1] = {{0, 0, -1}, fp(FAR_PLANE)}; // far
		clippingPlanes[2] = {{b, 0, a}, 0}; // left
		clippingPlanes[3] = {{fp(0)-b, 0, a}, 0}; // right
		clippingPlanes[4] = {{0, d, c}, 0}; // bottom
		clippingPlanes[5] = {{0, fp(0)-d, c}, 0}; // top
	}

	void init(OrdTblNode **ot){
		ordTbl = ot;
		for(int i = 0; i < 255; i++){
			ordTbl[i] = nullptr;
		}

		initClippingPlanes();
	}

	struct Mesh {
		int numTriangles;
		Triangle *triangles;
	};

	inline Mesh clipTriangleAgainstPlane(Triangle triangle, Plane plane){
		int numClipped = 0;
		bool isClippedOut[3] = {false, false, false};

		for(int i = 0; i < 3; i++){
			if(dot(plane.n, triangle.verts[i].pos) + plane.d < 0){
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
					vec3<fp> inPoint0 = triangle.verts[clippedIn[0]].pos;
					vec3<fp> inPoint1 = triangle.verts[clippedIn[1]].pos;
					vec3<fp> outPoint = triangle.verts[clippedOut[0]].pos;

					fp d0 = dot(plane.n, outPoint - inPoint0);
					fp d1 = dot(plane.n, outPoint - inPoint1);
					if(d0 == 0 || d1 == 0)
						return {0, nullptr};

					vec3<fp> A = inPoint0 + (outPoint - inPoint0) * ((fp(0) - plane.d - dot(plane.n, inPoint0))/d0);
					vec3<fp> B = inPoint1 + (outPoint - inPoint1) * ((fp(0) - plane.d - dot(plane.n, inPoint1))/d1);

					vec2<fp> texCoords0[3];
					vec2<fp> texCoords1[3];
					texCoords0[0] = triangle.verts[clippedIn[0]].texCoord;
					texCoords0[1] = triangle.verts[clippedIn[0]].texCoord + (triangle.verts[clippedOut[0]].texCoord - triangle.verts[clippedIn[0]].texCoord) * ((fp(0) - plane.d - dot(plane.n, inPoint0))/d0);
					texCoords0[2] = triangle.verts[clippedIn[1]].texCoord;

					texCoords1[0] = triangle.verts[clippedIn[1]].texCoord;
					texCoords1[1] = texCoords0[1];
					texCoords1[2] = triangle.verts[clippedIn[1]].texCoord + (triangle.verts[clippedOut[0]].texCoord - triangle.verts[clippedIn[1]].texCoord) * ((fp(0) - plane.d - dot(plane.n, inPoint1))/d1);

					m = {2, (Triangle*)malloc(2*sizeof(Triangle))};
					m.triangles[0] = {
						.verts = {
							{inPoint0, texCoords0[0]},
							{A, texCoords0[1]},
							{inPoint1, texCoords0[2]},
						},
						.texture = triangle.texture,
						.color = triangle.color
					};
					m.triangles[1] = {
						.verts = {
							{inPoint1, texCoords1[0]},
							{A, texCoords1[1]},
							{B, texCoords1[2]},
						},
						.texture = triangle.texture,
						.color = triangle.color
					};
					break;
				}
			case 2:
				{
					vec3<fp> inPoint = triangle.verts[clippedIn[0]].pos;
					vec3<fp> outPoint0 = triangle.verts[clippedOut[0]].pos;
					vec3<fp> outPoint1 = triangle.verts[clippedOut[1]].pos;

					fp d0 = dot(plane.n, outPoint0 - inPoint);
					fp d1 = dot(plane.n, outPoint1 - inPoint);
					if(d0 == 0 || d1 == 0)
						break;

					vec3<fp> B = inPoint + (outPoint0 - inPoint) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d0);
					vec3<fp> C = inPoint + (outPoint1 - inPoint) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d1);

					vec2<float> texCoords[3];
					texCoords[0] = triangle.verts[clippedIn[0]].texCoord;
					texCoords[1] = triangle.verts[clippedIn[0]].texCoord + (triangle.verts[clippedOut[0]].texCoord - triangle.verts[clippedIn[0]].texCoord) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d0);
					texCoords[2] = triangle.verts[clippedIn[0]].texCoord + (triangle.verts[clippedOut[1]].texCoord - triangle.verts[clippedIn[0]].texCoord) * ((fp(0) - plane.d - dot(plane.n, inPoint))/d1);

					m = {1, (Triangle*)malloc(sizeof(Triangle))};
					m.triangles[0] = {
						.verts = {
							{inPoint, texCoords[0]},
							{B, texCoords[1]},
							{C, texCoords[2]},
						},
						.texture = triangle.texture,
						.color = triangle.color
					};
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

	void drawTriangle(Triangle t){
		Mesh m = clipTriangle(t);
		for(int i = 0; i < m.numTriangles; i++){
			if(t.texture == nullptr){
				_drawTriangle(m.triangles[i]);
			} else {
				_drawTriangle_textured(m.triangles[i]);
			}
		}
		free(m.triangles);
	}

	void addTriangle(Triangle t, unsigned char z){
		Mesh m = clipTriangle(t);
		for(int i = 0; i < m.numTriangles; i++){
			OrdTblNode *n = (OrdTblNode*) malloc(sizeof(OrdTblNode));
			n->next = ordTbl[z];
			n->t = (Triangle*) malloc(sizeof(Triangle));
			*n->t = m.triangles[i];
			ordTbl[z] = n;
		}
		free(m.triangles);
	}

	void drawOrdTbl(){
		for(int i = 254; i >= 0; i--){
			OrdTblNode *n = ordTbl[i];
			while(n != nullptr){
				if(n->t->texture == nullptr){
					_drawTriangle(*n->t);
				} else {
					_drawTriangle_textured(*n->t);
				}
				n = n->next;
			}
		}
	}

	void clearOrdTbl(){
		for(int i = 0; i < 255; i++){
			OrdTblNode *n = ordTbl[i];
			while(n != nullptr){
				free(n->t);
				OrdTblNode *next = n->next;
				free(n);
				n = next;
			}
			ordTbl[i] = nullptr;
		}
	}
}
