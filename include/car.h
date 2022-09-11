#pragma once

#include "vector.h"
#include "rasterizer.h"

class Car {
public:
	static Model model;

	vec3<float> position = {0, 0, 0};
	vec3<float> speed = {0, 0, 0};
	float wheelSpeed = 0;
	float targetDirection = 0, direction = 0;
	Car();
	void processInput();
	void update(bool isOnTrack);
	void render(mat4 viewMatrix);
};
