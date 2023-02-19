#pragma once

#include "vector.h"
#include "renderer.h"

class Track {
	float width, tolerance;

	int numPoints;
	vec3<float> *points;
	Renderer::Model *cones;
	Renderer::Model model;
	Renderer::Material material;
public:
	static Renderer::Model coneModel;

	Track(int numPoints, vec3<float> *points, float width = 10.0f, float tolerance = 1.2f);
	void render(mat4 viewMatrix, vec3<float> carPos);
	bool isInside(vec3<float> p);
};
