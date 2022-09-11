#pragma once

#include "vector.h"
#include "rasterizer.h"

class Track {
	float width, tolerance;

	int numPoints;
	vec3<float> *points;
	Triangle *triangles;
	Model *cones;
	Model model;
public:
	static Mesh coneMesh;
	static Mesh simpleConeMesh;

	Track(int numPoints, vec3<float> *points, float width = 10.0f, float tolerance = 1.2f);
	void render(mat4 viewMatrix, vec3<float> carPos);
	bool isInside(vec3<float> p);
};
