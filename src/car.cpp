#include "car.h"

#include "input.h"
#include "time.h"

Car::Car(){
}

void Car::processInput(){
	if(Input::keyDown(KEY_RIGHT) || Input::keyDown(KEY_6)){
		targetDirection = targetDirection + Time::delta / 80.0f;
	}
	if(Input::keyDown(KEY_LEFT) || Input::keyDown(KEY_4)){
		targetDirection = targetDirection - Time::delta / 80.0f;
	}

	if(Input::keyDown(KEY_UP) || Input::keyDown(KEY_8)){
		wheelSpeed = wheelSpeed + Time::delta / 500.0f;
	}
	if(Input::keyDown(KEY_DOWN) || Input::keyDown(KEY_5)){
		wheelSpeed = wheelSpeed - Time::delta / 200.0f;
	}

	if(wheelSpeed > 1)
		wheelSpeed = 1;
	if(wheelSpeed < 0)
		wheelSpeed = 0;
}

void Car::update(bool isOnTrack){
	if(isOnTrack)
		wheelSpeed = wheelSpeed * (1.0f / (1.0f + (Time::delta * 0.001f)));
	else
		wheelSpeed = wheelSpeed * (1.0f / (1.0f + (Time::delta * 0.002f)));
	speed.x = speed.x + wheelSpeed * float(fp_cos(fp(direction))) * Time::delta / 250.0f;
	speed.z = speed.z + wheelSpeed * float(fp_sin(fp(-direction))) * Time::delta / 250.0f;
	if(isOnTrack)
		speed = speed * (1.0f / (1.0f + (Time::delta * 0.01f)));
	else
		speed = speed * (1.0f / (1.0f + (Time::delta * 0.02f)));
	position = position + speed * Time::delta;

	direction = direction + (-direction * 0.05 + 0.05 * targetDirection) * Time::delta;
}

void Car::render(mat4 viewMatrix){
	model.viewMatrix = viewMatrix;
	model.modelMatrix = mat4();
	model.modelMatrix = mat4::translate(model.modelMatrix, position.x, -0.5, position.z);
	model.modelMatrix = mat4::rotateY(model.modelMatrix, direction);
	model.draw(true);
}

Renderer::Model Car::model = Renderer::Model();
