#include "rasterizer.h"

namespace Rasterizer {
#ifdef TEXTURED
	inline void _drawTriangle_textured(Triangle t){
#else
	inline void _drawTriangle(Triangle t){
#endif
		Vertex verts[3];
		for(int i = 0; i < 3; i++){
			verts[i] = t.verts[i];
			verts[i].pos = {
				verts[i].pos.x / verts[i].pos.z / fov_d,
				verts[i].pos.y / verts[i].pos.z / fov_d,
				verts[i].pos.z
			};
			verts[i].pos = toScreenCoords(verts[i].pos);
		}

		// sort points by y
		for(int _i = 0; _i < 2; _i++){
			for(int i = 0; i < 2; i++){
				if(verts[i].pos.y > verts[i+1].pos.y){
					Vertex t1 = verts[i];
					verts[i] = verts[i+1];
					verts[i+1] = t1;
				}
			}
		}

		fp x0 = verts[0].pos.x;
		fp x1 = x0;
		fp dx0, dx1, dx1_;
		if(verts[2].pos.y != verts[0].pos.y)
			dx0 = fp(verts[2].pos.x - verts[0].pos.x) / fp(verts[2].pos.y - verts[0].pos.y);
		else dx0 = verts[2].pos.x - verts[0].pos.x;

		if(verts[1].pos.y != verts[0].pos.y)
			dx1 = fp(verts[1].pos.x - verts[0].pos.x) / fp(verts[1].pos.y - verts[0].pos.y);
		else dx1 = verts[1].pos.x - verts[0].pos.x;

		if(verts[2].pos.y != verts[1].pos.y)
			dx1_ = fp(verts[2].pos.x - verts[1].pos.x) / fp(verts[2].pos.y - verts[1].pos.y);
		else dx1_ = verts[2].pos.x - verts[1].pos.x;

		int minY = clamp((int)verts[0].pos.y, 0, RENDER_HEIGHT-1);
		int maxY = clamp((int)verts[2].pos.y, 0, RENDER_HEIGHT-1);

		for(int y = verts[0].pos.y; y < 0; y++){
			x0 = x0 + dx0;
			x1 = x1 + dx1;
			if(y == int(verts[1].pos.y)){
				dx1 = dx1_;
			}
		}

#ifdef TEXTURED
		vec2<fp> texCoord, texCoord_x, texCoord_y;
#endif

#ifdef TEXTURED
		// prepare for interpolation
		vec2<int> v0 = verts[1].pos - verts[0].pos;
		vec2<int> v1 = verts[2].pos - verts[0].pos;
		fp d = cross(v0, v1);
		if(d == 0) return;
		fp b1_x = v1.y;
		fp b1_y = -v1.x;
		fp b2_x = -v0.y;
		fp b2_y = v0.x;
		fp b0_x = fp(0)-b1_x - b2_x;
		fp b0_y = fp(0)-b1_y - b2_y;
		fp b1 = cross(vec2<int>(0,minY) - verts[0].pos, v1);
		fp b2 = cross(v0, vec2<int>(0,minY) - verts[0].pos);
		fp b0 = d - b1 - b2;

		texCoord = (vec2<lfp>(verts[0].texCoord) * lfp(b0) + vec2<lfp>(verts[1].texCoord) * lfp(b1) + vec2<lfp>(verts[2].texCoord) * lfp(b2)) / lfp(d) * vec2<lfp>(t.texture->width-1, t.texture->height-1);
		texCoord_x = ((vec2<lfp>(verts[0].texCoord)) * b0_x + (vec2<lfp>(verts[1].texCoord)) * b1_x + (vec2<lfp>(verts[2].texCoord)) * b2_x) * vec2<lfp>(t.texture->width-1, t.texture->height-1) / d;
		texCoord_y = ((vec2<lfp>(verts[0].texCoord)) * b0_y + (vec2<lfp>(verts[1].texCoord)) * b1_y + (vec2<lfp>(verts[2].texCoord)) * b2_y) * vec2<lfp>(t.texture->width-1, t.texture->height-1) / d;
#endif

		unsigned short c = 0;
#ifndef TEXTURED
		c = (int(fp(t.color.x) * fp(0b11111)) << 11)
			| (int(fp(t.color.y) * fp(0b111111)) << 5)
			| int(fp(t.color.z) * fp(0b11111));
#endif

		for(int y = minY; y <= maxY; y++){
			int minX = clamp(min(x0, x1), 0, RENDER_WIDTH-1);
			int maxX = clamp(max(x0, x1), 0, RENDER_WIDTH-1);

#if PRIZM || GINT
			unsigned short *vram = Display::vram + y * RENDER_WIDTH;
#endif
#ifdef TEXTURED
			vec2<fp> texCoord_i = texCoord + texCoord_x * minX;
#endif
			for(int x = minX; x <= maxX; x++){
#ifdef TEXTURED
				if(texCoord_i.x > 0 && texCoord_i.x < t.texture->width && texCoord_i.y > 0 && texCoord_i.y < t.texture->height){
					c = t.texture->pixels[int(texCoord_i.x) + int(texCoord_i.y) * t.texture->width];
					c = (int(fp(t.color.x) * fp((c & 0b1111100000000000) >> 11)) << 11)
						| (int(fp(t.color.y) * fp((c & 0b11111100000) >> 5)) << 5)
						| int(fp(t.color.z) * fp(c & 0b11111));
				}
#if PRIZM || GINT
				vram[x] = c;
#else
				Display::drawPoint(x, y, newColor(c));
#endif

#else
#if PRIZM || GINT
				vram[x] = c;
#else
				Display::drawPoint(x, y, newColor(c));
#endif
#endif

#ifdef TEXTURED
				texCoord_i = texCoord_i + texCoord_x;
#endif
			}
			x0 = x0 + dx0;
			x1 = x1 + dx1;
			if(y == (int)verts[1].pos.y){
				dx1 = dx1_;
				x1 = verts[1].pos.x;
			}

#ifdef TEXTURED
			texCoord = texCoord + texCoord_y;
#endif
		}
	}
};
