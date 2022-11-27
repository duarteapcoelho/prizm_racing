#include "display.h"
#include "rasterizer.h"
#include "time.h"
#include "util.h"
#include "input.h"
#include "mat4.h"
#include "rmath.h"
#include "math.h"

#include "car.h"
#include "track.h"

#ifdef GINT
#include <gint/gint.h>
#include <gint/mmu.h>
#include <gint/dma.h>
#endif

#ifdef PRIZM
#include <fxcg/system.h>
#include <fxcg/serial.h>
int timer;
#endif

mat4 view;
vec3<float> cameraPos = {0, 0, 0};
vec3<float> cameraSpeed = {0, 0, 0};
float cameraAngle = 0;

#ifdef GINT
static GALIGNED(32) fp depthBuffer[RENDER_WIDTH*RENDER_HEIGHT];
#include "models.h"
#endif

int main(){
#ifndef GINT
	fp depthBuffer[RENDER_WIDTH*RENDER_HEIGHT];
#include "models.h"
#endif
	Rasterizer::depthBuffer = depthBuffer;

	createSinTable();

	Triangle simpleConeTriangles[] = {
		{
			{-1, 0, 0},
			{1, 0, 0},
			{0, -2, 0},
			{0, -2, 0},
			newColor(120, 50, 0)
		},
		{
			{0, 0, -1},
			{0, 0, 1},
			{0, -2, 0},
			{0, -2, 0},
			newColor(120, 50, 0)
		}
	};

	Car::model = Model({230, car_triangles});
	Track::coneMesh = {22, cone_triangles};
	Track::simpleConeMesh = {2, simpleConeTriangles};

	Display::init();
	Display::clear(newColor(70, 180, 220));
	Display::show();

	Time::update();

	Input::init();

	srand(0);

	Triangle sunTriangles[2] = {
		{
			{-1, -1, 0},
			{-1, 1, 0},
			{1, -1, 0},
			{0, 1, 0},
			newColor(255, 255, 0)
		},
		{
			{-1, 1, 0},
			{1, 1, 0},
			{1, -1, 0},
			{0, 1, 0},
			newColor(255, 255, 0)
		},
	};

	Model sun({2, sunTriangles});

	vec3<float> trackPoints[] = {
		{0, 0, 0},
		{2, 0, 0},
		{2+I_SQRT_2, 0, 1-I_SQRT_2},
		{3, 0, 1},
		{4-I_SQRT_2, 0, 1+I_SQRT_2},
		{4, 0, 2},
		{4+I_SQRT_2, 0, 1+I_SQRT_2},
		{5, 0, 1},
		{6-I_SQRT_2, 0, 1-I_SQRT_2},
		{6, 0, 0},
		{7, 0, 0},
		{7+I_SQRT_2, 0, -1+I_SQRT_2},
		{8, 0, -1},
		{7+I_SQRT_2, 0, -1-I_SQRT_2},
		{7, 0, -2},
		{5, 0, -2},
		{5-I_SQRT_2, 0, -3+I_SQRT_2},
		{4, 0, -3},
		{3+I_SQRT_2, 0, -3-I_SQRT_2},
		{3, 0, -4},
		{3-I_SQRT_2, 0, -3-I_SQRT_2},
		{2, 0, -3},
		{1+I_SQRT_2, 0, -3+I_SQRT_2},
		{1, 0, -2},
		{0, 0, -2},
		{-I_SQRT_2, 0, -1-I_SQRT_2},
		{-1, 0, -1},
		{-I_SQRT_2, 0, -1+I_SQRT_2},
	};
	for(int i = 0; i < 28; i++){
		trackPoints[i] = trackPoints[i] * 40;
		trackPoints[i].y = 0;
	}
	Track track = Track(28, trackPoints, 10, 1.0);

	Rasterizer::init();
	Rasterizer::setFOV(70);

	Car car = Car();
	Car enemyCar = Car();
	enemyCar.position = {-1000, 0, 0};

#ifdef PRIZM
	while(Serial_IsOpen() != 1){
		unsigned char mode[6] = {0, 5, 0, 0, 0, 0}; // 9600 bps 8n1
		Serial_Open(mode);
	}
	Serial_ClearTX();
	Serial_ClearRX();
#endif

	while(true){
		Rasterizer::reset();

		Time::update();
		Input::update();

#ifdef PRIZM
		// The first 4 bits of a sent byte are 0b0001 if it's the first being sent and 0b0000 otherwise.
		// The last 4 bits contain data. Each sent byte contains half a byte of data.

		while(Serial_PollRX() > 0){
			unsigned char o;
			Serial_Peek(0, &o);
			if(!(o & (1 << 4))){
				Serial_ReadSingle(NULL);
			} else {
				if(Serial_PollRX() >= sizeof(Car)*2){
					unsigned char enemyCarData[sizeof(Car)*2];
					Serial_Read(enemyCarData, sizeof(Car)*2, NULL);
					for(int i = 0; i < sizeof(Car); i++){
						*(((unsigned char*)&enemyCar) + i) = ((enemyCarData[i*2] & 0x0F) << 4) | (enemyCarData[i*2+1] & 0x0F);
					}
				}
				break;
			}
		}

		while(Serial_PollTX() < sizeof(Car)*2);
		unsigned char carData[sizeof(Car)*2];
		for(int i = 0; i < sizeof(Car); i++){
			carData[i*2] = ((*(((unsigned char*)&car) + i)) & 0xF0) >> 4;
			carData[i*2+1] = ((*(((unsigned char*)&car) + i)) & 0x0F);
		}
		carData[0] = carData[0] | (1 << 4);
		Serial_Write(carData, sizeof(Car)*2);
#endif

		if(Input::keyPressed(KEY_MENU)){
#ifdef GINT
			gint_osmenu();
#endif
#ifdef PRIZM
			while(Input::keyDown(KEY_MENU))
				Input::update();
			timer = Timer_Install(0, []() {
					Keyboard_PutKeycode(4, 9, 0);
					Timer_Stop(timer);
					Timer_Deinstall(timer);
					}, 1);
			Timer_Start(timer);
			int k = 0;
			Bdisp_EnableColor(1);
			GetKey(&k);

			continue;
#endif
#ifdef SDL
			return 0;
#endif
			int t = Time::delta;
			Time::update();
			Time::delta = t;
		}

		car.processInput();
		car.update(track.isInside(car.position));

		Rasterizer::setFOV(fp(70 + 50.0f / car.speed.i_length()));

		cameraAngle = cameraAngle + (-cameraAngle * 0.02 + 0.02 * car.direction) * Time::delta;

		vec3<float> lc = cameraPos;
		vec3<float> ls = cameraSpeed;
		cameraPos = smoothDamp(cameraPos, car.position, &cameraSpeed, 5.0f, Time::delta, 1000.0f);
		if(cameraPos == car.position){
			cameraPos = lc;
			cameraSpeed = ls;
		}

		view = mat4();
		view = mat4::rotateX(view, HALF_PI*0.1);
		view = mat4::translate(view, 0, 2, 6);
		view = mat4::rotateY(view, -cameraAngle - HALF_PI);
		view = mat4::translate(view, -cameraPos.x, 0, -cameraPos.z);

		int floorY = fp_tan(-HALF_PI*0.1) / Rasterizer::fov_d * fp(DISPLAY_WIDTH) + fp(DISPLAY_HEIGHT/2);
#ifdef PRIZM
		long v1 = newColor(70, 180, 220).color | (newColor(70, 180, 220).color << (8*sizeof(unsigned short)));
		long v2 = newColor(32, 79, 19).color | (newColor(32, 79, 19).color << (8*sizeof(unsigned short)));
		for(int i = 0; i < floorY*DISPLAY_WIDTH/2; i++){
			((long*)Display::VRAMAddress)[i] = v1;
		}
		for(int i = floorY*DISPLAY_WIDTH/2; i < DISPLAY_WIDTH*DISPLAY_HEIGHT/2; i++){
			((long*)Display::VRAMAddress)[i] = v2;
		}
#else
#ifdef GINT
		uint16_t sky = newColor(70, 180, 220).color;
		uint16_t grass = newColor(32, 79, 10).color;
		int minY = floorY - (floorY%4);
		int maxY = floorY + (4 - (floorY%4));
		dma_memset(gint_vram, (sky << 16) | sky, DISPLAY_WIDTH*minY*2);
		Display::fillRect(0, minY, DISPLAY_WIDTH, floorY-minY, newColor(70, 180, 220));
		Display::fillRect(0, floorY, DISPLAY_WIDTH, maxY-floorY, newColor(32, 79, 10));
		dma_memset(gint_vram + DISPLAY_WIDTH*maxY, (grass << 16) | grass, DISPLAY_WIDTH*DISPLAY_HEIGHT*2 - DISPLAY_WIDTH*maxY*2);
#else
		Display::clear(newColor(70, 180, 220));
		Display::fillRect(0, floorY, DISPLAY_WIDTH, DISPLAY_HEIGHT-floorY, newColor(32, 79, 19));
#endif
#endif

		sun.viewMatrix = view;
		sun.modelMatrix = mat4();
		sun.modelMatrix = mat4::translate(sun.modelMatrix, 20, -6, -20);
		sun.modelMatrix = mat4::translate(sun.modelMatrix, cameraPos.x, 0, cameraPos.z);
		sun.modelMatrix = mat4::rotateY(sun.modelMatrix, cameraAngle + HALF_PI);
		sun.draw(false, false, true);

		track.render(view, car.position);

		enemyCar.render(view);
		car.render(view);

		char buffer[20];
#ifdef PRIZM
		itoa((int)(1.0f / (Time::delta / 128.0f)), (unsigned char*)buffer);
#else
		sprintf(buffer, "%d", (int)(1.0f / (Time::delta / 128.0f)));
#endif
		Display::drawText(0, 0, "FPS: ", newColor(255, 255, 255));
		Display::drawText(Display::textWidth("FPS: "), 0, buffer, newColor(255, 255, 255));

		float speed2 = car.speed.length2();
		float speed;
		if(speed2 != 0)
			speed = (1.0f / car.speed.i_length()) * 128.0f / 1000.0f * 3600.0f;
		else
			speed = 0;
#ifdef PRIZM
		itoa((int)speed, (unsigned char*)buffer);
#else
		sprintf(buffer, "%d", (int)speed);
#endif
		Display::drawText(0, DISPLAY_HEIGHT-Display::textHeight, "SPEED: ", newColor(255, 255, 255));
		Display::drawText(Display::textWidth("SPEED: "), DISPLAY_HEIGHT-Display::textHeight, buffer, newColor(255, 255, 255));

		Display::show();
	}
}



