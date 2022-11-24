#include "rasterizer.h"
#include "display.h"
#include "rmath.h"
#include "time.h"

struct Plane {
	vec3<fp> n;
	fp d;
};

inline int min(int a, int b){
	if(a < b)
		return a;
	return b;
}

inline int max(int a, int b){
	if(a > b)
		return a;
	return b;
}

namespace Rasterizer {
	Plane clippingPlanes[5];

	fp *depthBuffer;

	fp fov_d = 1;

	void init(){
		clippingPlanes[0] = {{0, 0, 1}, fp(0)-fp(NEAR_PLANE)}; // near
		clippingPlanes[1] = {{fp(I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // left
		clippingPlanes[2] = {{fp(-I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // right
		clippingPlanes[3] = {{0, fp(I_SQRT_2), fp(I_SQRT_2)}, 0}; // bottom
		clippingPlanes[4] = {{0, fp(-I_SQRT_2), fp(I_SQRT_2)}, 0}; // top
	}

	void reset(){
		for(int i = 0; i < RENDER_WIDTH*RENDER_HEIGHT; i++){
			depthBuffer[i] = -1;
		}
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

	inline vec3<fp> toDevice(vec3<fp> p){
		return {
			p.x / p.z / fov_d,
			p.y / p.z / fov_d,
			p.z
		};
	}

	inline vec3<int> toScreen(vec3<fp> p){
		return {
			p.x * fp(RENDER_WIDTH) + fp(RENDER_WIDTH / 2.0f),
			p.y * fp(RENDER_WIDTH) + fp(RENDER_HEIGHT / 2.0f),
			(int)((p.z - fp(NEAR_PLANE)) * (fp(1000) / fp(FAR_PLANE))) + 1
		};
	}

	struct Edge {
		int minY;
		int maxY;
		fp x;
		fp m;
		Edge *next;
	};
	inline Edge newEdge(vec3<int> p0, vec3<int> p1){
		if(p0.y > p1.y){
			return {
				p1.y,
				p0.y,
				p1.x,
				fp(p0.x - p1.x) / fp(p0.y - p1.y),
				nullptr
			};
		} else if(p0.y < p1.y){
			return {
				p0.y,
				p1.y,
				p0.x,
				fp(p1.x - p0.x) / fp(p1.y - p0.y),
				nullptr
			};
		} else {
			return {p0.y, p1.y, p0.x, 0, nullptr};
		}
	}

	inline void _drawTriangle(Model *model, Triangle triangle, bool useDepth, bool isShaded){
		vec3<fp> p0_d = toDevice(triangle.p0);
		vec3<fp> p1_d = toDevice(triangle.p1);
		vec3<fp> p2_d = toDevice(triangle.p2);

		vec3<int> p0 = toScreen(p0_d);
		vec3<int> p1 = toScreen(p1_d);
		vec3<int> p2 = toScreen(p2_d);

		if(dot(mat4::toMat3(model->viewMatrix) * mat4::toMat3(model->modelMatrix) * triangle.normal, vec3<fp>(0, 0, 1)) > 0){
			return;
		}

		int minY = max(min(min(p0.y, p1.y), p2.y), 0);
		int maxY = min(max(max(p0.y, p1.y), p2.y), RENDER_HEIGHT);
		fp z = (p0.z + p1.z + p2.z) / 3;

		if(isShaded){
			fp brightness = dot(mat4::toMat3(model->modelMatrix) * triangle.normal, vec3<fp>(I_SQRT_3, -I_SQRT_3, -I_SQRT_3)) * fp(0.6) + fp(0.4);
			if(brightness > 1)
				brightness = 1;
			if(brightness < 0)
				brightness = 0;
			triangle.c.r = fp(triangle.c.r) * brightness;
			triangle.c.g = fp(triangle.c.g) * brightness;
			triangle.c.b = fp(triangle.c.b) * brightness;
			triangle.c = newColor(triangle.c.r, triangle.c.g, triangle.c.b);
		}

		Edge *edgeTable[RENDER_HEIGHT];
		Edge edges[3] = {
			newEdge(p0, p1),
			newEdge(p1, p2),
			newEdge(p2, p0),
		};

		edgeTable[0] = nullptr;
		for(int i = 0; i < 3; i++){
			if(edges[i].minY < minY){
				Edge **e = &(edgeTable[0]);
				while(*e != nullptr){
					e = &((*e)->next);
				}
				*e = &edges[i];
				(*e)->x = (*e)->x + fp(-(*e)->minY) * (*e)->m;
			}
		}

		for(int y = minY; y < maxY; y++){
			if(y != 0)
				edgeTable[y] = nullptr;
			for(int i = 0; i < 3; i++){
				if(edges[i].minY == y){
					Edge **e = &(edgeTable[y]);
					while(*e != nullptr){
						e = &((*e)->next);
					}
					*e = &edges[i];
				}
			}
		}

		Edge *activeEdgeList = nullptr;

		for(int y = minY; y < maxY; y++){
			// add new edges to the list
			if(edgeTable[y] != nullptr){
				Edge **e;
				for(e = &activeEdgeList; *e != nullptr; e = &((*e)->next)){
				}
				*e = edgeTable[y];
			}

			// remove edges from the list
			for(Edge **e = &activeEdgeList; *e != nullptr;){
				if(y == (*e)->maxY){
					*e = (*e)->next;
				} else {
					e = &((*e)->next);
				}
			}

			// calculate intersection point
			for(Edge *e = activeEdgeList; e != nullptr; e = e->next){
				e->x = e->x + e->m;
			}

			// draw
			for(Edge *e = activeEdgeList; e != nullptr && e->next != nullptr; e = e->next->next){
				int a = e->x;
				int b = e->next->x;
				if(a < 0) a = 0;
				else if(a > RENDER_WIDTH) a = RENDER_WIDTH;
				if(b < 0) b = 0;
				else if(b > RENDER_WIDTH) b = RENDER_WIDTH;
				int minX, maxX;
				if(a > b){
					maxX = a;
					minX = b;
				} else {
					maxX = b;
					minX = a;
				}
				int p = minX+y*RENDER_WIDTH;
#ifdef PRIZM
				unsigned short *vram = Display::VRAMAddress + p;
#endif
				for(int x = minX; x < maxX; x++){
					if(z < depthBuffer[p] || depthBuffer[p] == -1 || !useDepth){
						if(useDepth)
							depthBuffer[p] = z;
#if PIXEL_SIZE == 1
#ifdef PRIZM
						*vram = triangle.c.color;
#else
						Display::drawPoint(x, y, triangle.c);
#endif
#else
						Display::fillRect(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, triangle.c);
#endif
					}
					p++;
#ifdef PRIZM
					vram++;
#endif
				}
			}
		}
	}

	inline Mesh clipTriangleAgainstPlane(Triangle triangle, Plane plane){
		int numClipped = 0;
		bool clippedP0 = false;
		bool clippedP1 = false;
		bool clippedP2 = false;

		if(dot(plane.n, triangle.p0) + plane.d < 0){
			numClipped++;
			clippedP0 = true;
		}
		if(dot(plane.n, triangle.p1) + plane.d < 0){
			numClipped++;
			clippedP1 = true;
		}
		if(dot(plane.n, triangle.p2) + plane.d < 0){
			numClipped++;
			clippedP2 = true;
		}

		if(numClipped == 0){
			Mesh m = {1, (Triangle*)malloc(sizeof(Triangle))};
			m.triangles[0] = triangle;
			return m;
		} else if(numClipped == 3){
			return {0, nullptr};
		} else if(numClipped == 2){
			vec3<fp> clipped[2];
			vec3<fp> notClipped;
			if(!clippedP0){
				notClipped = triangle.p0;
				clipped[0] = triangle.p1;
				clipped[1] = triangle.p2;
			} else if(!clippedP1){
				notClipped = triangle.p1;
				clipped[0] = triangle.p0;
				clipped[1] = triangle.p2;
			} else {
				notClipped = triangle.p2;
				clipped[0] = triangle.p0;
				clipped[1] = triangle.p1;
			}

			fp d0 = dot(plane.n, clipped[0] - notClipped);
			fp d1 = dot(plane.n, clipped[1] - notClipped);
			if(d0 == 0 || d1 == 0)
				return {0, nullptr};

			vec3<fp> B = notClipped + (clipped[0] - notClipped) * ((fp(0) - plane.d - dot(plane.n, notClipped))/d0);
			vec3<fp> C = notClipped + (clipped[1] - notClipped) * ((fp(0) - plane.d - dot(plane.n, notClipped))/d1);

			Mesh m = {1, (Triangle*)malloc(sizeof(Triangle))};
			m.triangles[0] = {notClipped, B, C, triangle.normal, triangle.c};
			return m;
		} else if(numClipped == 1){
			vec3<fp> clipped;
			vec3<fp> notClipped[2];
			if(clippedP0){
				clipped = triangle.p0;
				notClipped[0] = triangle.p1;
				notClipped[1] = triangle.p2;
			}

			if(clippedP1){
				clipped = triangle.p1;
				notClipped[0] = triangle.p0;
				notClipped[1] = triangle.p2;
			}

			if(clippedP2){
				clipped = triangle.p2;
				notClipped[0] = triangle.p0;
				notClipped[1] = triangle.p1;
			}

			fp d0 = dot(plane.n, clipped - notClipped[0]);
			fp d1 = dot(plane.n, clipped - notClipped[1]);
			if(d0 == 0 || d1 == 0)
				return {0, nullptr};

			vec3<fp> A = notClipped[0] + (clipped - notClipped[0]) * ((fp(0) - plane.d - dot(plane.n, notClipped[0]))/d0);
			vec3<fp> B = notClipped[1] + (clipped - notClipped[1]) * ((fp(0) - plane.d - dot(plane.n, notClipped[1]))/d1);

			Mesh m = {2, (Triangle*)malloc(2*sizeof(Triangle))};
			m.triangles[0] = {notClipped[0], A, notClipped[1], triangle.normal, triangle.c};
			m.triangles[1] = {notClipped[1], A, B, triangle.normal, triangle.c};
			return m;
		}

		return {0, nullptr};
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

	inline void drawTriangle(Model *model, Triangle triangle, bool useDepth, bool isShaded, bool clipTriangles){
		triangle.p0 = model->viewMatrix * model->modelMatrix * triangle.p0;
		triangle.p1 = model->viewMatrix * model->modelMatrix * triangle.p1;
		triangle.p2 = model->viewMatrix * model->modelMatrix * triangle.p2;

		if(triangle.p0 == triangle.p1 || triangle.p1 == triangle.p2 || triangle.p2 == triangle.p0){
			return;
		}

		int inside = 5;
		for(int i = 0; i < 5; i++){
			if(dot(clippingPlanes[i].n, triangle.p0) + clippingPlanes[i].d < 0
					|| dot(clippingPlanes[i].n, triangle.p1) + clippingPlanes[i].d < 0
					|| dot(clippingPlanes[i].n, triangle.p2) + clippingPlanes[i].d < 0){
				inside--;
				break;
			}
		}

		if(inside == 5){
			_drawTriangle(model, triangle, useDepth, isShaded);
		} else if(inside != 0 && clipTriangles){
			Mesh mesh = clipTriangle(triangle);
			for(int i = 0; i < mesh.numTriangles; i++){
				_drawTriangle(model, mesh.triangles[i], useDepth, isShaded);
			}
			free(mesh.triangles);
		}
	}

};

Model::Model(){
	mesh = {0, nullptr};
}
Model::Model(Mesh mesh){
	this->mesh = mesh;
}
void Model::draw(bool useDepth, bool isShaded, bool clipTriangles){
	for(int i = 0; i < mesh.numTriangles; i++){
		Rasterizer::drawTriangle(this, mesh.triangles[i], useDepth, isShaded, clipTriangles);
	}
}
