#include "rasterizer.h"

namespace Rasterizer {
#ifdef TEXTURED
	inline void _drawTriangle_textured(Model *model, Triangle triangle, bool isShaded){
#else
	inline void _drawTriangle(Model *model, Triangle triangle, bool isShaded){
#endif
		vec3<fp> points_d[3] = {
			toDevice(triangle.points[0]),
			toDevice(triangle.points[1]),
			toDevice(triangle.points[2]),
		};

		vec3<int> points[3] = {
			toScreen(points_d[0]),
			toScreen(points_d[1]),
			toScreen(points_d[2]),
		};
		vec2<fp> texCoords[3] = {
			triangle.texCoords[0],
			triangle.texCoords[1],
			triangle.texCoords[2],
		};

		fp brightness = 1;
		if(isShaded){
			brightness = dot(mat4::toMat3(model->modelMatrix) * triangle.normal, vec3<fp>(I_SQRT_3, -I_SQRT_3, -I_SQRT_3)) * fp(0.5) + fp(0.5);
			if(brightness > 1)
				brightness = 1;
			if(brightness < 0)
				brightness = 0;
			triangle.c.r = fp(triangle.c.r) * brightness;
			triangle.c.g = fp(triangle.c.g) * brightness;
			triangle.c.b = fp(triangle.c.b) * brightness;
			triangle.c = newColor(triangle.c.r, triangle.c.g, triangle.c.b);
		}

		// sort points by y
		for(int _i = 0; _i < 2; _i++){
			for(int i = 0; i < 2; i++){
				if(points[i].y > points[i+1].y){
					vec3<int> t1 = points[i];
					points[i] = points[i+1];
					points[i+1] = t1;

					vec2<fp> t2 = texCoords[i];
					texCoords[i] = texCoords[i+1];
					texCoords[i+1] = t2;
				}
			}
		}

		fp x0 = points[0].x;
		fp x1 = x0;
		fp dx0, dx1, dx1_;
		if(points[2].y != points[0].y)
			dx0 = fp(points[2].x - points[0].x) / fp(points[2].y - points[0].y);
		else dx0 = points[2].x - points[0].x;

		if(points[1].y != points[0].y)
			dx1 = fp(points[1].x - points[0].x) / fp(points[1].y - points[0].y);
		else dx1 = points[1].x - points[0].x;

		if(points[2].y != points[1].y)
			dx1_ = fp(points[2].x - points[1].x) / fp(points[2].y - points[1].y);
		else dx1_ = points[2].x - points[1].x;

		int minY = clamp(points[0].y, 0, RENDER_HEIGHT-1);
		int maxY = clamp(points[2].y, 0, RENDER_HEIGHT-1);

		for(int y = points[0].y; y < 0; y++){
			x0 = x0 + dx0;
			x1 = x1 + dx1;
			if(y == points[1].y){
				dx1 = dx1_;
			}
		}

		// prepare for interpolation
#ifdef TEXTURED
		vec2<int> v0 = points[1] - points[0];
		vec2<int> v1 = points[2] - points[0];
		fp d = cross(v0, v1);
		if(d == 0) return;
		fp b1_x = v1.y;
		fp b1_y = -v1.x;
		fp b2_x = -v0.y;
		fp b2_y = v0.x;
		fp b0_x = fp(0)-b1_x - b2_x;
		fp b0_y = fp(0)-b1_y - b2_y;
		fp b1 = cross(vec2<int>(0,minY) - points[0], v1);
		fp b2 = cross(v0, vec2<int>(0,minY) - points[0]);
		fp b0 = d - b1 - b2;

		vec2<fp> texCoord = (vec2<lfp>(texCoords[0]) * lfp(b0) + vec2<lfp>(texCoords[1]) * lfp(b1) + vec2<lfp>(texCoords[2]) * lfp(b2)) / lfp(d) * vec2<lfp>(model->texture->width-1, model->texture->height-1);
		vec2<fp> texCoord_x = ((vec2<lfp>(texCoords[0])) * b0_x + (vec2<lfp>(texCoords[1])) * b1_x + (vec2<lfp>(texCoords[2])) * b2_x) * vec2<lfp>(model->texture->width-1, model->texture->height-1) / d;
		vec2<fp> texCoord_y = ((vec2<lfp>(texCoords[0])) * b0_y + (vec2<lfp>(texCoords[1])) * b1_y + (vec2<lfp>(texCoords[2])) * b2_y) * vec2<lfp>(model->texture->width-1, model->texture->height-1) / d;
#endif

		int p = minY * RENDER_WIDTH;
		for(int y = minY; y <= maxY; y++){
			int minX = clamp(min(x0, x1), 0, RENDER_WIDTH-1);
			int maxX = clamp(max(x0, x1), 0, RENDER_WIDTH-1);

#ifdef PRIZM
			unsigned short *vram = Display::VRAMAddress;
#endif
#ifdef GINT
			color_t *vram = gint_vram;
#endif
#if PRIZM || GINT
			vram += p;
#endif
#ifdef TEXTURED
			vec2<fp> texCoord_i = texCoord + texCoord_x * minX;
#endif
			for(int x = minX; x <= maxX; x++){
#ifdef TEXTURED
					unsigned short color = 0;
					if(texCoord_i.x > 0 && texCoord_i.x < model->texture->width && texCoord_i.y > 0 && texCoord_i.y < model->texture->height){
						color = model->texture->pixels[int(texCoord_i.x) + int(texCoord_i.y) * model->texture->width];
						color = (int(brightness * fp((color & 0b1111100000000000) >> 11)) << 11)
							| (int(brightness * fp((color & 0b11111100000) >> 5)) << 5)
							| int(brightness * fp(color & 0b11111));
					}
#if PRIZM || GINT
					vram[x] = color;
#else
					Display::drawPoint(x, y, newColor(color));
#endif

#else
#if PRIZM || GINT
					vram[x] = triangle.c.color;
#else
					Display::drawPoint(x, y, triangle.c);
#endif
#endif

#ifdef TEXTURED
				texCoord_i = texCoord_i + texCoord_x;
#endif
			}
			x0 = x0 + dx0;
			x1 = x1 + dx1;
			if(y == points[1].y){
				dx1 = dx1_;
				x1 = points[1].x;
			}

			p += RENDER_WIDTH;

#ifdef TEXTURED
			texCoord = texCoord + texCoord_y;
#endif
		}
	}
}
