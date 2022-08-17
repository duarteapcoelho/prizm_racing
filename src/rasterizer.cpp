#include "rasterizer.h"
#include "display.h"
#include "rmath.h"

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

float dot(vec3f a, vec3f b){
	return a.x*b.x + a.y*b.y;
}

fp cross(vec3d a, vec3d b){
	return (a.x*b.y) - (a.y*b.x);
}

namespace Rasterizer {
	fp *depthBuffer;

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

	void drawTriangle(Triangle triangle, Shader shader){
		// reset();
		// SDL_SetRenderDrawColor(renderer, triangle.c.r, triangle.c.g, triangle.c.b, 255);
		//

		triangle.p0 = shader.vertexShader(triangle.p0, shader.uniforms);
		triangle.p1 = shader.vertexShader(triangle.p1, shader.uniforms);
		triangle.p2 = shader.vertexShader(triangle.p2, shader.uniforms);

		if(triangle.p0.z == 0 || triangle.p1.z == 0 || triangle.p2.z == 0)
			return;

		vec3d p0_d = toDevice(triangle.p0);
		vec3d p1_d = toDevice(triangle.p1);
		vec3d p2_d = toDevice(triangle.p2);

		if((p0_d.z < NEAR_PLANE || p1_d.z < NEAR_PLANE || p2_d.z < NEAR_PLANE)
				|| (p0_d.x < -0.5 && p1_d.x < -0.5 && p2_d.x < -0.5)
				|| (p0_d.y < -0.5 && p1_d.y < -0.5 && p2_d.y < -0.5)
				|| (p0_d.x > 0.5 && p1_d.x > 0.5 && p2_d.x > 0.5)
				|| (p0_d.y > 0.5 && p1_d.y > 0.5 && p2_d.y > 0.5)
		  ){
			return;
		}

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
		if(minX == maxX || minY == maxY)
			return;

		int e1 = edge({minX, minY, 0}, p0, p1);
		int e2 = edge({minX, minY, 0}, p1, p2);
		int e3 = edge({minX, minY, 0}, p2, p0);

		const vec3i bary_v0 = {(p1.x - p0.x), (p1.y - p0.y), 0};
		const vec3i bary_v1 = {(p2.x - p0.x), (p2.y - p0.y), 0};
		fp bary_denom = (bary_v0.x * bary_v1.y - bary_v1.x * bary_v0.y);
		if(bary_denom == 0)
			return;
		const fp i_bary_denom = fp(1000) / fp(bary_v0.x * bary_v1.y - bary_v1.x * bary_v0.y);

		const vec3i bary_v2 = {minX - p0.x, minY - p0.y, 0};
		const fp bary_v_per_x = fp(bary_v1.y) * fp(i_bary_denom) / fp(1000);
		const fp bary_w_per_x = fp(0) - fp(bary_v0.y) * fp(i_bary_denom) / fp(1000);
		const fp bary_u_per_x = fp(0) - bary_v_per_x - bary_w_per_x;

		const fp bary_v_per_y = fp(0) - fp(bary_v1.x) * fp(i_bary_denom) / fp(1000);
		const fp bary_w_per_y = fp(bary_v0.x) * fp(i_bary_denom) / fp(1000);
		const fp bary_u_per_y = fp(0) -bary_v_per_y - bary_w_per_y;

		fp bary_v = fp(bary_v2.x * bary_v1.y - bary_v1.x * bary_v2.y) / fp(1000) * fp(i_bary_denom);
		fp bary_w = fp(bary_v0.x * bary_v2.y - bary_v2.x * bary_v0.y) / fp(1000) * fp(i_bary_denom);
		fp bary_u = fp(1) - bary_v - bary_w;

		fp i_p0z = fp(1)/fp(p0.z);
		fp i_p1z = fp(1)/fp(p1.z);
		fp i_p2z = fp(1)/fp(p2.z);

		for(int y = minY; y < maxY; y++){
			bool hasEntered = false;
			int ie1 = e1;
			int ie2 = e2;
			int ie3 = e3;

			fp i_bary_v = bary_v;
			fp i_bary_w = bary_w;
			fp i_bary_u = bary_u;

			for(int x = minX; x < maxX; x++){
				if(ie1 >= 0 && ie2 >= 0 && ie3 >= 0) {
					vec3d b = {i_bary_u, i_bary_v, i_bary_w};
					// fp i_z = (b.x * i_p0z + b.y * i_p1z + b.z * i_p2z);
					fp i_z;
					i_z.i = ((b.x.i * i_p0z.i) + (b.y.i * i_p1z.i) + (b.z.i * i_p2z.i)) / FP_RIGHT;
					if(i_z > depthBuffer[x+y*RENDER_WIDTH] || depthBuffer[x+y*RENDER_WIDTH] == -1){
						depthBuffer[x+y*RENDER_WIDTH] = i_z;
						Color c = shader.fragmentShader(triangle.c, shader.uniforms);
#if PIXEL_SIZE == 1
						Display::drawPoint(x, y, c);
#else
						Display::fillRect(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, c);
#endif
					}
					hasEntered = true;
				} else if(hasEntered) {
					break;
				}
				ie1 += (p0.y - p1.y);
				ie2 += (p1.y - p2.y);
				ie3 += (p2.y - p0.y);

				i_bary_v = i_bary_v + bary_v_per_x;
				i_bary_w = i_bary_w + bary_w_per_x;
				i_bary_u = i_bary_u + bary_u_per_x;
			}
			e1 -= (p0.x - p1.x);
			e2 -= (p1.x - p2.x);
			e3 -= (p2.x - p0.x);

			bary_v = bary_v + bary_v_per_y;
			bary_w = bary_w + bary_w_per_y;
			bary_u = bary_u + bary_u_per_y;

		}
	}

	void drawModel(Model model){
		for(int i = 0; i < model.mesh.numTriangles; i++){
			drawTriangle(model.mesh.triangles[i], model.shader);
		}
	}
};
