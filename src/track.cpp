#include "track.h"

Track::Track(int numPoints, vec3<float> *points, float width, float tolerance){
	this->numPoints = numPoints;
	this->points = points;
	this->width = width;
	this->tolerance = tolerance;

	triangles = (Triangle*)malloc(numPoints*2 * sizeof(Triangle));
	cones = (Model*)malloc(numPoints*2 * sizeof(Model));

	for(int i = 0; i < numPoints; i++){
		vec3<float> pos = points[i];
		vec3<float> direction = points[(i+1)%numPoints] - points[i];
		vec3<float> nextDirection = points[(i+2)%numPoints] - points[(i+1)%numPoints];
		vec3<float> perpendicular = {direction.z, direction.y, -direction.x};
		perpendicular = perpendicular * width * perpendicular.i_length();
		vec3<float> nextPerpendicular = {nextDirection.z, nextDirection.y, -nextDirection.x};
		nextPerpendicular = nextPerpendicular * width * nextPerpendicular.i_length();

		vec3<fp> p0 = pos - perpendicular;
		vec3<fp> p1 = pos + perpendicular;
		vec3<fp> p2 = pos - nextPerpendicular + direction;
		vec3<fp> p3 = pos + nextPerpendicular + direction;
		triangles[i*2] = {
			{p0, p1, p2},
			{0, -1, 0},
			newColor(50, 50, 50),
			{{0,0},{0,0},{0,0}}
		};
		triangles[i*2+1] = {
			{p1, p2, p3},
			{0, -1, 0},
			newColor(50, 50, 50),
			{{0,0},{0,0},{0,0}}
		};

		cones[i*2] = Model(coneMesh);
		cones[i*2+1] = Model(coneMesh);
		cones[i*2].modelMatrix = mat4::translate(cones[i*2].modelMatrix, pos - perpendicular);
		cones[i*2+1].modelMatrix = mat4::translate(cones[i*2+1].modelMatrix, pos + perpendicular);
	}
	model = Model({numPoints * 2, triangles});
}

void Track::render(mat4 viewMatrix, vec3<float> carPos){
	model.viewMatrix = viewMatrix;
	model.draw(false, false, true, true);

	for(int i = 0; i < numPoints; i+=2){
		float d = (points[i] - carPos).length2();
		if(d > 150*150){
			continue;
		}
		cones[i*2].viewMatrix = viewMatrix;
		cones[i*2+1].viewMatrix = viewMatrix;
		cones[i*2].draw(true, true, false, true);
		cones[i*2+1].draw(true, true, false, true);
	}
}

float distanceToLine2(vec3<float> p, vec3<float> p1, vec3<float> p2){
	float l2 = (p1-p2).length2();

	if(l2 == 0) return (p1 - p).length2();

	float t = dot(p - p1, p2 - p1) / l2;
	if(t < 0) t = 0;
	if(t > 1) t = 1;
	vec3<float> projection = p1 + (p2 - p1) * t;
	return (p - projection).length2();
}

bool Track::isInside(vec3<float> p){
	float minDistance2 = -1;
	for(int i = 0; i < numPoints; i++){
		float d = distanceToLine2(p, points[i], points[(i+1)%numPoints]);
		if(d < minDistance2 || minDistance2 == -1)
			minDistance2 = d;
	}
	return minDistance2 < width*width*tolerance*tolerance;
}

Mesh Track::coneMesh = {0, nullptr};
