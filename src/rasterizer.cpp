#include "rasterizer.h"
#include "display.h"
#include "rmath.h"
#include "time.h"

struct Plane {
	vec3d n;
	fp d;
};

int min(int a, int b){
	if(a < b)
		return a;
	return b;
}

int max(int a, int b){
	if(a > b)
		return a;
	return b;
}

#ifdef PRIZM
float abs(float v){
	if(v < 0){
		return 0 - v;
	} else {
		return v;
	}
}
#endif

fp dot3(vec3d a, vec3d b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

fp cross(vec3d a, vec3d b){
	return (a.x*b.y) - (a.y*b.x);
}

namespace Rasterizer {
	Plane clippingPlanes[5];

	fp *depthBuffer;

	void init(){
#define I_SQRT_2 0.707106
		clippingPlanes[0] = {{0, 0, 1}, fp(0)-fp(NEAR_PLANE)}; // near
		// clippingPlanes[1] = {{0, 0, 1}, fp(0)-fp(FAR_PLANE)}; // far
		clippingPlanes[1] = {{fp(I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // left
		clippingPlanes[2] = {{fp(-I_SQRT_2), 0, fp(I_SQRT_2)}, 0}; // right
		clippingPlanes[3] = {{0, fp(I_SQRT_2), fp(I_SQRT_2)}, 0}; // bottom
		clippingPlanes[4] = {{0, fp(-I_SQRT_2), fp(I_SQRT_2)}, 0}; // top
	}

	int edge(vec3i p, vec3i e1, vec3i e2){
		return (p.x - e2.x) * (e1.y - e2.y) - (p.y - e2.y) * (e1.x - e2.x);
	}

	void reset(){
		for(int i = 0; i < RENDER_WIDTH*RENDER_HEIGHT; i++){
			depthBuffer[i] = -1;
		}
	}

	vec3d toDevice(vec3d p){
		return {
			p.x / p.z,
			p.y / p.z,
			p.z
		};
	}

	vec3i toScreen(vec3d p){
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
	Edge *newEdge(vec3i p0, vec3i p1){
		Edge *e = (Edge*) malloc(sizeof(Edge));

		if(p0.y > p1.y){
			*e = {
				p1.y,
				p0.y,
				p1.x,
				fp(p0.x - p1.x) / fp(p0.y - p1.y),
				nullptr
			};
		} else if(p0.y < p1.y){
			*e = {
				p0.y,
				p1.y,
				p0.x,
				fp(p1.x - p0.x) / fp(p1.y - p0.y),
				nullptr
			};
		} else {
			free(e);
			return nullptr;
		}

		return e;
	}

	void _drawTriangle(Triangle triangle, Shader shader, bool useDepth){
		vec3d p0_d = toDevice(triangle.p0);
		vec3d p1_d = toDevice(triangle.p1);
		vec3d p2_d = toDevice(triangle.p2);

		vec3i p0 = toScreen(p0_d);
		vec3i p1 = toScreen(p1_d);
		vec3i p2 = toScreen(p2_d);

		int minY = max(min(min(p0.y, p1.y), p2.y), 0);
		int maxY = min(max(max(p0.y, p1.y), p2.y), RENDER_HEIGHT);
		fp z = (p0.z + p1.z + p2.z) / 3;

		Edge *edgeTable[RENDER_HEIGHT];
		Edge *edges[3] = {
			newEdge(p0, p1),
			newEdge(p1, p2),
			newEdge(p2, p0),
		};

		for(int y = 0; y < RENDER_HEIGHT; y++){
			edgeTable[y] = nullptr;
			for(int i = 0; i < 3; i++){
				if(edges[i] != nullptr){
					if(edges[i]->minY == y){
						Edge **e = &(edgeTable[y]);
						while(*e != nullptr){
							e = &((*e)->next);
						}
						*e = edges[i];
					}
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

			// sort by intersection point using injection sort:
			Edge *sortedEdgeList = nullptr;

			// sort
			for(Edge *e = activeEdgeList; e != nullptr;){
				Edge *next = e->next;
				for(Edge **f = &sortedEdgeList;; f = &((*f)->next)){
					if(*f == nullptr){
						*f = e;
						(*f)->next = nullptr;
						break;
					}
					if((*f)->x > e->x){
						Edge *a = *f;
						*f = e;
						e->next = a;
						break;
					}
				}
				e = next;
			}

			activeEdgeList = sortedEdgeList;

			// draw
			for(Edge *e = activeEdgeList; e != nullptr; e = e->next->next){
				int minX = min(max(e->x, 0), RENDER_WIDTH);
				int maxX = max(min(e->next->x, RENDER_WIDTH), 0);
				for(int x = minX; x < maxX; x++){
					if(z < depthBuffer[x+y*RENDER_WIDTH] || depthBuffer[x+y*RENDER_WIDTH] == -1 || !useDepth){
						if(useDepth)
							depthBuffer[x+y*RENDER_WIDTH] = z;
#if PIXEL_SIZE == 1
						Display::drawPoint(x, y, triangle.c);
#else
						Display::fillRect(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, triangle.c);
#endif
					}
				}
				// if(z < depthBuffer[y] || depthBuffer[y] == -1 || !useDepth){
				// 	if(useDepth)
				// 		depthBuffer[y] = z;
				// 	Display::fillRect(minX * PIXEL_SIZE, y*PIXEL_SIZE, (maxX - minX) * PIXEL_SIZE, PIXEL_SIZE, triangle.c);
				// }
			}
		}

		for(int i = 0; i < 3; i++){
			if(edges[i] != nullptr){
				free(edges[i]);
			}
		}
	}

	Mesh clipTriangleAgainstPlane(Triangle triangle, Plane plane){
		int numClipped = 0;
		bool clippedP0 = false;
		bool clippedP1 = false;
		bool clippedP2 = false;

		if(dot3(plane.n, triangle.p0) + plane.d < 0){
			numClipped++;
			clippedP0 = true;
		}
		if(dot3(plane.n, triangle.p1) + plane.d < 0){
			numClipped++;
			clippedP1 = true;
		}
		if(dot3(plane.n, triangle.p2) + plane.d < 0){
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
			vec3d clipped[2];
			vec3d notClipped;
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

			fp d0 = dot3(plane.n, clipped[0] - notClipped);
			fp d1 = dot3(plane.n, clipped[1] - notClipped);
			if(d0 == 0 || d1 == 0)
				return {0, nullptr};

			vec3d B = notClipped + (clipped[0] - notClipped) * ((fp(0) - plane.d - dot3(plane.n, notClipped))/d0);
			vec3d C = notClipped + (clipped[1] - notClipped) * ((fp(0) - plane.d - dot3(plane.n, notClipped))/d1);

			Mesh m = {1, (Triangle*)malloc(sizeof(Triangle))};
			m.triangles[0] = {notClipped, B, C, triangle.c};
			return m;
		} else if(numClipped == 1){
			vec3d clipped;
			vec3d notClipped[2];
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

			fp d0 = dot3(plane.n, clipped - notClipped[0]);
			fp d1 = dot3(plane.n, clipped - notClipped[1]);
			if(d0 == 0 || d1 == 0)
				return {0, nullptr};

			vec3d A = notClipped[0] + (clipped - notClipped[0]) * ((fp(0) - plane.d - dot3(plane.n, notClipped[0]))/d0);
			vec3d B = notClipped[1] + (clipped - notClipped[1]) * ((fp(0) - plane.d - dot3(plane.n, notClipped[1]))/d1);

			Mesh m = {2, (Triangle*)malloc(2*sizeof(Triangle))};
			m.triangles[0] = {notClipped[0], A, notClipped[1], triangle.c};
			m.triangles[1] = {notClipped[1], A, B, triangle.c};
			return m;
		}

		return {0, nullptr};
	}

	Mesh clipMeshAgainstPlane(Mesh mesh, Plane plane){
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

	Mesh clipMesh(Mesh mesh){
		for(int i = 0; i < 5; i++){
			mesh = clipMeshAgainstPlane(mesh, clippingPlanes[i]);
		}
		return mesh;
	}

	Mesh clipTriangle(Triangle triangle){
		Mesh m = {1, (Triangle*)malloc(sizeof(Triangle))};
		m.triangles[0] = triangle;
		m = clipMesh(m);
		return m;
	}

	void drawTriangle(Triangle triangle, Shader shader, bool useDepth){
		triangle.p0 = shader.vertexShader(triangle.p0, shader.uniforms);
		triangle.p1 = shader.vertexShader(triangle.p1, shader.uniforms);
		triangle.p2 = shader.vertexShader(triangle.p2, shader.uniforms);

		if(triangle.p0 == triangle.p1 || triangle.p1 == triangle.p2 || triangle.p2 == triangle.p0){
			return;
		}

		Mesh mesh = clipTriangle(triangle);
		for(int i = 0; i < mesh.numTriangles; i++){
			_drawTriangle(mesh.triangles[i], shader, useDepth);
		}
		free(mesh.triangles);
	}

	void drawModel(Model model, bool useDepth){
		for(int i = 0; i < model.mesh.numTriangles; i++){
			drawTriangle(model.mesh.triangles[i], model.shader, useDepth);
		}
	}
};
