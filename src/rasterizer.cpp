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

	void _drawTriangle(Triangle triangle, Shader shader, bool useDepth){
		vec3d p0_d = toDevice(triangle.p0);
		vec3d p1_d = toDevice(triangle.p1);
		vec3d p2_d = toDevice(triangle.p2);

		fp winding = cross({p1_d.x - p0_d.x, p1_d.y - p0_d.y, 0}, {p2_d.x - p0_d.x, p2_d.y - p0_d.y, 0});

		vec3i p0, p1, p2;

		if(winding > 0){
			p0 = toScreen(p0_d);
			p1 = toScreen(p1_d);
			p2 = toScreen(p2_d);
		} else {
			p0 = toScreen(p0_d);
			p1 = toScreen(p2_d);
			p2 = toScreen(p1_d);
		}

		int minX = max(min(min(p0.x, p1.x), p2.x), 0);
		int maxX = min(max(max(p0.x, p1.x), p2.x), RENDER_WIDTH);
		int minY = max(min(min(p0.y, p1.y), p2.y), 0);
		int maxY = min(max(max(p0.y, p1.y), p2.y), RENDER_HEIGHT);

		int e1 = edge({minX, minY, 0}, p0, p1);
		int e2 = edge({minX, minY, 0}, p1, p2);
		int e3 = edge({minX, minY, 0}, p2, p0);

		fp bary_v, bary_w, bary_u;
		fp i_p0z, i_p1z, i_p2z;
		fp bary_v_per_x, bary_w_per_x, bary_u_per_x, bary_v_per_y, bary_w_per_y, bary_u_per_y;
		if(useDepth){
			const vec3i bary_v0 = {(p1.x - p0.x), (p1.y - p0.y), 0};
			const vec3i bary_v1 = {(p2.x - p0.x), (p2.y - p0.y), 0};
			fp bary_denom = (bary_v0.x * bary_v1.y - bary_v1.x * bary_v0.y);
			if(bary_denom == 0)
				return;
			const fp i_bary_denom = fp(1000) / fp(bary_v0.x * bary_v1.y - bary_v1.x * bary_v0.y);

			const vec3i bary_v2 = {minX - p0.x, minY - p0.y, 0};
			bary_v_per_x = fp(bary_v1.y) * fp(i_bary_denom) / fp(1000);
			bary_w_per_x = fp(0) - fp(bary_v0.y) * fp(i_bary_denom) / fp(1000);
			bary_u_per_x = fp(0) - bary_v_per_x - bary_w_per_x;

			bary_v_per_y = fp(0) - fp(bary_v1.x) * fp(i_bary_denom) / fp(1000);
			bary_w_per_y = fp(bary_v0.x) * fp(i_bary_denom) / fp(1000);
			bary_u_per_y = fp(0) -bary_v_per_y - bary_w_per_y;

			bary_v = fp(bary_v2.x * bary_v1.y - bary_v1.x * bary_v2.y) / fp(1000) * fp(i_bary_denom);
			bary_w = fp(bary_v0.x * bary_v2.y - bary_v2.x * bary_v0.y) / fp(1000) * fp(i_bary_denom);
			bary_u = fp(1) - bary_v - bary_w;

			i_p0z = fp(1)/fp(p0.z);
			i_p1z = fp(1)/fp(p1.z);
			i_p2z = fp(1)/fp(p2.z);
		}

		for(int y = minY; y < maxY; y++){
			// bool hasEntered = false;
			int ie1 = e1;
			int ie2 = e2;
			int ie3 = e3;

			fp i_bary_v = bary_v;
			fp i_bary_w = bary_w;
			fp i_bary_u = bary_u;

			for(int x = minX; x < maxX; x++){
				if(ie1 >= 0 && ie2 >= 0 && ie3 >= 0) {
					bool visible = true;
					fp i_z;
					if(useDepth){
						vec3d b = {i_bary_u, i_bary_v, i_bary_w};
						// i_z = (b.x * i_p0z + b.y * i_p1z + b.z * i_p2z);
						i_z.i = ((b.x.i * i_p0z.i) + (b.y.i * i_p1z.i) + (b.z.i * i_p2z.i));
						if(i_z > depthBuffer[x+y*RENDER_WIDTH] || depthBuffer[x+y*RENDER_WIDTH] == -1){
							depthBuffer[x+y*RENDER_WIDTH] = i_z;
#if PIXEL_SIZE == 1
							Display::drawPoint(x, y, triangle.c);
#else
							Display::fillRect(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, triangle.c);
#endif
						}
					} else {
#if PIXEL_SIZE == 1
						Display::drawPoint(x, y, triangle.c);
#else
						Display::fillRect(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, triangle.c);
#endif
					}
				}
				ie1 += (p0.y - p1.y);
				ie2 += (p1.y - p2.y);
				ie3 += (p2.y - p0.y);

				if(useDepth){
					i_bary_v = i_bary_v + bary_v_per_x;
					i_bary_w = i_bary_w + bary_w_per_x;
					i_bary_u = i_bary_u + bary_u_per_x;
				}
			}
			e1 -= (p0.x - p1.x);
			e2 -= (p1.x - p2.x);
			e3 -= (p2.x - p0.x);

			if(useDepth){
				bary_v = bary_v + bary_v_per_y;
				bary_w = bary_w + bary_w_per_y;
				bary_u = bary_u + bary_u_per_y;
			}

		}
	}

	// void drawTriangle(Triangle triangle, Shader shader, bool useDepth){
	// 	triangle.p0 = shader.vertexShader(triangle.p0, shader.uniforms);
	// 	triangle.p1 = shader.vertexShader(triangle.p1, shader.uniforms);
	// 	triangle.p2 = shader.vertexShader(triangle.p2, shader.uniforms);
	//
	// 	if(triangle.p0 == triangle.p1 || triangle.p1 == triangle.p2 || triangle.p2 == triangle.p0){
	// 		return;
	// 	}
	//
	// 	bool clippedP0 = false;
	// 	bool clippedP1 = false;
	// 	bool clippedP2 = false;
	//
	// 	for(int i = 0; i < 5; i++){
	// 		int numClipped = 0;
	// 		if(dot3(clippingPlanes[i].n, triangle.p0) + clippingPlanes[i].d < 0){
	// 			numClipped++;
	// 			clippedP0 = true;
	// 			return;
	// 		}
	// 		if(dot3(clippingPlanes[i].n, triangle.p1) + clippingPlanes[i].d < 0){
	// 			numClipped++;
	// 			clippedP1 = true;
	// 			return;
	// 		}
	// 		if(dot3(clippingPlanes[i].n, triangle.p2) + clippingPlanes[i].d < 0){
	// 			numClipped++;
	// 			clippedP2 = true;
	// 			return;
	// 		}
	// 		if(numClipped == 0){
	// 			_drawTriangle(triangle, shader, useDepth);
	// 		} else if(numClipped == 1){
	// 			vec3d clipped;
	// 			vec3d notClipped[2];
	// 			if(clippedP0){
	// 				clipped = triangle.p0;
	// 				notClipped[0] = triangle.p1;
	// 				notClipped[1] = triangle.p2;
	// 			}
	//
	// 			if(clippedP1){
	// 				clipped = triangle.p1;
	// 				notClipped[0] = triangle.p0;
	// 				notClipped[1] = triangle.p2;
	// 			}
	//
	// 			if(clippedP2){
	// 				clipped = triangle.p2;
	// 				notClipped[0] = triangle.p0;
	// 				notClipped[1] = triangle.p1;
	// 			}
	//
	// 			fp d0 = dot3(clippingPlanes[i].n, clipped - notClipped[0]);
	// 			fp d1 = dot3(clippingPlanes[i].n, clipped - notClipped[1]);
	// 			if(d0 == 0 || d1 == 0)
	// 				return;
	//
	// 			vec3d A = notClipped[0] + (clipped - notClipped[0]) * ((fp(0) - clippingPlanes[i].d - dot3(clippingPlanes[i].n, notClipped[0]))/d0);
	// 			vec3d B = notClipped[1] + (clipped - notClipped[1]) * ((fp(0) - clippingPlanes[i].d - dot3(clippingPlanes[i].n, notClipped[1]))/d1);
	//
	// 			_drawTriangle({notClipped[0], A, notClipped[1], triangle.c}, shader, useDepth);
	// 			_drawTriangle({notClipped[1], A, B, triangle.c}, shader, useDepth);
	// 		} else if(numClipped == 2){
	// 			vec3d clipped[2];
	// 			vec3d notClipped;
	// 			if(!clippedP0){
	// 				notClipped = triangle.p0;
	// 				clipped[0] = triangle.p1;
	// 				clipped[1] = triangle.p2;
	// 			} else if(!clippedP1){
	// 				notClipped = triangle.p1;
	// 				clipped[0] = triangle.p0;
	// 				clipped[1] = triangle.p2;
	// 			} else {
	// 				notClipped = triangle.p2;
	// 				clipped[0] = triangle.p0;
	// 				clipped[1] = triangle.p1;
	// 			}
	//
	// 			fp d0 = dot3(clippingPlanes[i].n, clipped[0] - notClipped);
	// 			fp d1 = dot3(clippingPlanes[i].n, clipped[1] - notClipped);
	// 			if(d0 == 0 || d1 == 0)
	// 				return;
	//
	// 			vec3d B = notClipped + (clipped[0] - notClipped) * ((fp(0) - clippingPlanes[i].d - dot3(clippingPlanes[i].n, notClipped))/d0);
	// 			vec3d C = notClipped + (clipped[0] - notClipped) * ((fp(0) - clippingPlanes[i].d - dot3(clippingPlanes[i].n, notClipped))/d1);
	//
	// 			_drawTriangle({notClipped, B, C, triangle.c}, shader, useDepth);
	// 		}
	// 	}
	// }
	

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
