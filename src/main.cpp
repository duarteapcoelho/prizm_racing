#include "display.h"
#include "rasterizer.h"
#include "time.h"
#include "util.h"
#include "input.h"
#include "mat4.h"
#include "rmath.h"

#define NUM_CUBES 20

mat4 view;
vec3d carPos = {0, 0, 0};
vec3d carSpeed = {0, 0, 0};
fp carAngle = 0;
fp cameraAngle = 0;
fp wheelSpeed = 0;

vec3d vertexShader(vec3d i, void *uniforms){
	mat4 model = *((mat4*)uniforms);
	return view * model * i;
}

Color fragmentShader(Color i, void *uniforms){
	return i;
}

int main(){
	fp st[SIN_SAMPLES];
	sinTable = st;
	createSinTable();

	Display::init();
	Display::clear(newColor(0, 0, 0));
	Display::show();

	Time::update();

	Input::init();

	fp depthBuffer[RENDER_WIDTH*RENDER_HEIGHT];
	Rasterizer::depthBuffer = depthBuffer;

	Triangle carTriangles[134] = {
		{
			{0.794, 0.294, -0.380},
			{-0.535, 0.294, -0.000},
			{-0.535, 0.294, -0.443},
			newColor(203, 2, 1)},
		{
			{0.794, 0.294, -0.380},
			{1.345, 0.294, -0.000},
			{0.794, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{1.795, 0.294, -0.605},
			{1.886, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.000},
			{-1.188, 0.263, -0.431},
			{-0.535, 0.294, -0.443},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, -0.681},
			{-1.188, 0.227, -0.719},
			{-1.188, 0.263, -0.431},
			newColor(203, 2, 1)},
		{
			{1.795, 0.294, -0.605},
			{1.886, 0.003, -0.000},
			{1.886, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{1.795, 0.003, -0.605},
			{0.985, -0.225, -0.000},
			{1.886, 0.003, -0.000},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, -0.546},
			{0.302, -0.435, -0.000},
			{0.985, -0.225, -0.000},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, -0.525},
			{0.302, -0.424, -0.389},
			{0.743, -0.212, -0.546},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{-0.474, -0.415, -0.387},
			{-0.559, -0.455, -0.000},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, -0.325},
			{-1.287, -0.240, -0.394},
			{-0.474, -0.415, -0.387},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, -0.325},
			{-0.559, -0.455, -0.000},
			{-0.559, -0.240, -0.000},
			newColor(203, 2, 1)},
		{
			{-0.474, -0.415, -0.387},
			{-0.474, -0.239, -0.525},
			{-1.287, -0.240, -0.394},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-0.559, -0.240, -0.325},
			{-0.559, -0.240, -0.000},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{0.743, -0.212, -0.546},
			{0.756, -0.055, -0.687},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{0.868, -0.070, -0.380},
			{1.283, -0.062, -0.380},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.674},
			{0.756, -0.055, -0.687},
			{0.794, 0.294, -0.674},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-0.489, -0.177, -0.683},
			{-1.385, -0.217, -0.739},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-1.441, -0.240, -0.000},
			{-1.282, -0.240, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.399, 0.128, -0.681},
			{-1.399, 0.134, 0.012},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, -0.431},
			{-1.188, 0.227, -0.719},
			{-1.036, -0.065, -0.719},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, -0.431},
			{-0.605, -0.065, -0.697},
			{-0.605, -0.065, -0.443},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.443},
			{-1.036, -0.065, -0.431},
			{-0.605, -0.065, -0.443},
			newColor(0, 0, 0)},
		{
			{-0.605, -0.065, -0.443},
			{-0.535, 0.294, -0.674},
			{-0.535, 0.294, -0.443},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{-0.535, 0.294, -0.674},
			{-0.605, -0.065, -0.697},
			newColor(203, 2, 1)},
		{
			{-0.489, -0.177, -0.683},
			{-1.036, -0.065, -0.719},
			{-1.385, -0.217, -0.739},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, -0.681},
			{-1.036, -0.065, -0.719},
			{-1.188, 0.227, -0.719},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{1.345, 0.294, -0.711},
			{1.345, 0.294, -0.380},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, -0.674},
			{0.868, -0.070, -0.380},
			{0.794, 0.294, -0.380},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, -0.711},
			{0.794, 0.294, -0.674},
			{0.756, -0.055, -0.687},
			newColor(203, 2, 1)},
		{
			{0.756, -0.055, -0.687},
			{0.743, -0.212, -0.546},
			{0.868, -0.070, -0.711},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.711},
			{1.795, 0.003, -0.605},
			{1.795, 0.294, -0.605},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, -0.711},
			{0.743, -0.212, -0.546},
			{1.283, -0.062, -0.711},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, -0.546},
			{1.795, 0.003, -0.605},
			{1.283, -0.062, -0.711},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{0.868, -0.070, -0.711},
			{1.283, -0.062, -0.711},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, 0.380},
			{0.794, 0.294, 0.674},
			{-0.535, 0.294, 0.443},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.000},
			{0.794, 0.294, 0.380},
			{0.794, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{1.795, 0.294, 0.605},
			{1.345, 0.294, 0.711},
			{1.345, 0.294, 0.380},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.431},
			{-0.535, 0.294, -0.000},
			{-0.535, 0.294, 0.443},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.431},
			{-1.188, 0.227, 0.719},
			{-1.399, 0.128, 0.681},
			newColor(203, 2, 1)},
		{
			{1.886, 0.003, -0.000},
			{1.795, 0.294, 0.605},
			{1.886, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{0.985, -0.225, -0.000},
			{1.795, 0.003, 0.605},
			{1.886, 0.003, -0.000},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{0.743, -0.212, 0.546},
			{0.985, -0.225, -0.000},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, 0.525},
			{0.302, -0.424, 0.389},
			{-0.474, -0.415, 0.387},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{-0.474, -0.415, 0.387},
			{0.302, -0.424, 0.389},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, 0.325},
			{-0.474, -0.415, 0.387},
			{-1.287, -0.240, 0.394},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.455, -0.000},
			{-0.559, -0.240, 0.325},
			{-0.559, -0.240, -0.000},
			newColor(203, 2, 1)},
		{
			{-0.474, -0.415, 0.387},
			{-1.287, -0.240, 0.394},
			{-0.474, -0.239, 0.525},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-0.559, -0.240, 0.325},
			{-1.287, -0.240, 0.394},
			newColor(0, 0, 0)},
		{
			{0.743, -0.212, 0.546},
			{-0.489, -0.177, 0.683},
			{0.756, -0.055, 0.687},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.380},
			{0.868, -0.070, 0.380},
			{0.794, 0.294, 0.380},
			newColor(0, 0, 0)},
		{
			{0.756, -0.055, 0.687},
			{-0.535, 0.294, 0.674},
			{0.794, 0.294, 0.674},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, 0.394},
			{-0.489, -0.177, 0.683},
			{-0.474, -0.239, 0.525},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.287, -0.240, 0.394},
			{-1.282, -0.240, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, 0.681},
			{-1.441, -0.240, -0.000},
			{-1.399, 0.134, -0.012},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.431},
			{-1.188, 0.227, 0.719},
			{-1.188, 0.263, 0.431},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, 0.431},
			{-0.605, -0.065, 0.697},
			{-1.036, -0.065, 0.719},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.443},
			{-1.036, -0.065, 0.431},
			{-1.188, 0.263, 0.431},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.674},
			{-0.605, -0.065, 0.443},
			{-0.535, 0.294, 0.443},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, 0.683},
			{-0.605, -0.065, 0.697},
			{-0.535, 0.294, 0.674},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.719},
			{-0.489, -0.177, 0.683},
			{-1.385, -0.217, 0.739},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.719},
			{-1.399, 0.128, 0.681},
			{-1.188, 0.227, 0.719},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.711},
			{1.283, -0.062, 0.380},
			{1.345, 0.294, 0.380},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, 0.380},
			{0.794, 0.294, 0.674},
			{0.794, 0.294, 0.380},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, 0.711},
			{0.756, -0.055, 0.687},
			{0.794, 0.294, 0.674},
			newColor(203, 2, 1)},
		{
			{0.756, -0.055, 0.687},
			{0.868, -0.070, 0.711},
			{0.743, -0.212, 0.546},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.711},
			{1.795, 0.003, 0.605},
			{1.283, -0.062, 0.711},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, 0.711},
			{1.283, -0.062, 0.711},
			{0.743, -0.212, 0.546},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, 0.546},
			{1.283, -0.062, 0.711},
			{1.795, 0.003, 0.605},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, 0.711},
			{1.283, -0.062, 0.380},
			{1.283, -0.062, 0.711},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.443},
			{-0.535, 0.294, -0.674},
			{0.794, 0.294, -0.674},
			newColor(203, 2, 1)},
		{
			{0.794, 0.294, -0.380},
			{0.794, 0.294, -0.000},
			{-0.535, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.443},
			{0.794, 0.294, -0.674},
			{0.794, 0.294, -0.380},
			newColor(203, 2, 1)},
		{
			{0.794, 0.294, -0.380},
			{1.345, 0.294, -0.380},
			{1.345, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{1.886, 0.294, -0.000},
			{1.345, 0.294, -0.000},
			{1.345, 0.294, -0.380},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{1.345, 0.294, -0.711},
			{1.795, 0.294, -0.605},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.000},
			{-1.188, 0.263, 0.012},
			{-1.188, 0.263, -0.431},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.012},
			{-1.399, 0.134, 0.012},
			{-1.188, 0.263, -0.431},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.134, 0.012},
			{-1.399, 0.128, -0.681},
			{-1.188, 0.263, -0.431},
			newColor(203, 2, 1)},
		{
			{1.795, 0.294, -0.605},
			{1.795, 0.003, -0.605},
			{1.886, 0.003, -0.000},
			newColor(203, 2, 1)},
		{
			{1.795, 0.003, -0.605},
			{0.743, -0.212, -0.546},
			{0.985, -0.225, -0.000},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, -0.546},
			{0.302, -0.424, -0.389},
			{0.302, -0.435, -0.000},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, -0.525},
			{-0.474, -0.415, -0.387},
			{0.302, -0.424, -0.389},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{0.302, -0.424, -0.389},
			{-0.474, -0.415, -0.387},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, -0.325},
			{-0.474, -0.415, -0.387},
			{-0.559, -0.455, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-1.287, -0.240, -0.394},
			{-0.559, -0.240, -0.325},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{-0.474, -0.239, -0.525},
			{0.743, -0.212, -0.546},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{0.794, 0.294, -0.380},
			{0.868, -0.070, -0.380},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.674},
			{-0.489, -0.177, -0.683},
			{0.756, -0.055, -0.687},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-0.474, -0.239, -0.525},
			{-0.489, -0.177, -0.683},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-1.385, -0.217, -0.739},
			{-1.441, -0.240, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.385, -0.217, -0.739},
			{-1.399, 0.128, -0.681},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, -0.431},
			{-1.188, 0.263, -0.431},
			{-1.188, 0.227, -0.719},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, -0.431},
			{-1.036, -0.065, -0.719},
			{-0.605, -0.065, -0.697},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.443},
			{-1.188, 0.263, -0.431},
			{-1.036, -0.065, -0.431},
			newColor(0, 0, 0)},
		{
			{-0.605, -0.065, -0.443},
			{-0.605, -0.065, -0.697},
			{-0.535, 0.294, -0.674},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{-0.605, -0.065, -0.697},
			{-1.036, -0.065, -0.719},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, -0.681},
			{-1.385, -0.217, -0.739},
			{-1.036, -0.065, -0.719},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{1.283, -0.062, -0.711},
			{1.345, 0.294, -0.711},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, -0.674},
			{0.868, -0.070, -0.711},
			{0.868, -0.070, -0.380},
			newColor(0, 0, 0)},
		{
			{1.345, 0.294, -0.711},
			{1.283, -0.062, -0.711},
			{1.795, 0.003, -0.605},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{0.868, -0.070, -0.380},
			{0.868, -0.070, -0.711},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, 0.674},
			{-0.535, 0.294, 0.674},
			{-0.535, 0.294, 0.443},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, 0.443},
			{-0.535, 0.294, -0.000},
			{0.794, 0.294, 0.380},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.000},
			{0.794, 0.294, -0.000},
			{0.794, 0.294, 0.380},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.000},
			{1.345, 0.294, 0.380},
			{0.794, 0.294, 0.380},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.380},
			{1.345, 0.294, -0.000},
			{1.886, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{1.886, 0.294, -0.000},
			{1.795, 0.294, 0.605},
			{1.345, 0.294, 0.380},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.431},
			{-1.188, 0.263, -0.012},
			{-0.535, 0.294, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, 0.681},
			{-1.399, 0.134, -0.012},
			{-1.188, 0.263, 0.431},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.134, -0.012},
			{-1.188, 0.263, -0.012},
			{-1.188, 0.263, 0.431},
			newColor(203, 2, 1)},
		{
			{1.886, 0.003, -0.000},
			{1.795, 0.003, 0.605},
			{1.795, 0.294, 0.605},
			newColor(203, 2, 1)},
		{
			{0.985, -0.225, -0.000},
			{0.743, -0.212, 0.546},
			{1.795, 0.003, 0.605},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{0.302, -0.424, 0.389},
			{0.743, -0.212, 0.546},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, 0.525},
			{0.743, -0.212, 0.546},
			{0.302, -0.424, 0.389},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{-0.559, -0.455, -0.000},
			{-0.474, -0.415, 0.387},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.455, -0.000},
			{-0.474, -0.415, 0.387},
			{-0.559, -0.240, 0.325},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-0.559, -0.240, -0.000},
			{-0.559, -0.240, 0.325},
			newColor(0, 0, 0)},
		{
			{0.743, -0.212, 0.546},
			{-0.474, -0.239, 0.525},
			{-0.489, -0.177, 0.683},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.380},
			{1.283, -0.062, 0.380},
			{0.868, -0.070, 0.380},
			newColor(0, 0, 0)},
		{
			{0.756, -0.055, 0.687},
			{-0.489, -0.177, 0.683},
			{-0.535, 0.294, 0.674},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, 0.394},
			{-1.385, -0.217, 0.739},
			{-0.489, -0.177, 0.683},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.385, -0.217, 0.739},
			{-1.287, -0.240, 0.394},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, 0.681},
			{-1.385, -0.217, 0.739},
			{-1.441, -0.240, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.431},
			{-1.036, -0.065, 0.719},
			{-1.188, 0.227, 0.719},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, 0.431},
			{-0.605, -0.065, 0.443},
			{-0.605, -0.065, 0.697},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.443},
			{-0.605, -0.065, 0.443},
			{-1.036, -0.065, 0.431},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.674},
			{-0.605, -0.065, 0.697},
			{-0.605, -0.065, 0.443},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, 0.719},
			{-0.605, -0.065, 0.697},
			{-0.489, -0.177, 0.683},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.719},
			{-1.385, -0.217, 0.739},
			{-1.399, 0.128, 0.681},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.711},
			{1.283, -0.062, 0.711},
			{1.283, -0.062, 0.380},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, 0.380},
			{0.868, -0.070, 0.711},
			{0.794, 0.294, 0.674},
			newColor(0, 0, 0)},
		{
			{1.345, 0.294, 0.711},
			{1.795, 0.294, 0.605},
			{1.795, 0.003, 0.605},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, 0.711},
			{0.868, -0.070, 0.380},
			{1.283, -0.062, 0.380},
			newColor(0, 0, 0)},
	};

	Mesh carMesh = {
		134,
		carTriangles
	};

#define TILE_SIZE 20
	Triangle floorTriangles[2*TILE_SIZE*TILE_SIZE] = {};
	for(int x = 0; x < TILE_SIZE; x++){
		for(int y = 0; y < TILE_SIZE; y++){
			floorTriangles[x+y*TILE_SIZE] = {
				{fp(x) / fp(TILE_SIZE) - fp(0.5), 0, fp(y) / fp(TILE_SIZE) - fp(0.5)},
				{fp(x+1) / fp(TILE_SIZE) - fp(0.5), 0, fp(y) / fp(TILE_SIZE) - fp(0.5)},
				{fp(x+1) / fp(TILE_SIZE) - fp(0.5), 0, fp(y+1) / fp(TILE_SIZE) - fp(0.5)},
				newColor(255, 255, 255)
			};
			floorTriangles[TILE_SIZE*TILE_SIZE + x+y*TILE_SIZE] = {
				{fp(x) / fp(TILE_SIZE) - fp(0.5), 0, fp(y) / fp(TILE_SIZE) - fp(0.5)},
				{fp(x) / fp(TILE_SIZE) - fp(0.5), 0, fp(y+1) / fp(TILE_SIZE) - fp(0.5)},
				{fp(x+1) / fp(TILE_SIZE) - fp(0.5), 0, fp(y+1) / fp(TILE_SIZE) - fp(0.5)},
				newColor(0, 0, 0)
			};
		}
	}
	Mesh floorMesh = {
		2*TILE_SIZE*TILE_SIZE,
		floorTriangles
	};

	srand(0);

	mat4 carMatrix;

	Model car = {
		.mesh = carMesh,
		.shader = {
			.vertexShader = vertexShader,
			.fragmentShader = fragmentShader,
			.uniforms = &carMatrix
		}
	};

	mat4 floorMatrix;
	floorMatrix = mat4::scale(floorMatrix, 50, 1, 50);

	Model floor = {
		.mesh = floorMesh,
		.shader = {
			.vertexShader = vertexShader,
			.fragmentShader = fragmentShader,
			.uniforms = &floorMatrix
		}
	};

	while(true){
		Rasterizer::reset();

		Time::update();
		Input::update();

		if(Input::keyPressed(KEY_MENU)){
			return 0;
		}

		const fp speed = 0.02 * Time::delta;
		const fp acceleration = fp(1);
		if(Input::keyDown(KEY_RIGHT)){
			carAngle = carAngle + speed;
		}
		if(Input::keyDown(KEY_LEFT)){
			carAngle = carAngle - speed;
		}
		if(Input::keyDown(KEY_UP)){
			wheelSpeed = wheelSpeed + acceleration;
		}
		if(Input::keyDown(KEY_DOWN)){
			wheelSpeed = wheelSpeed - acceleration;
		}
		// wheelSpeed = wheelSpeed / (fp(1000) * fp(Time::delta));
		// if(wheelSpeed < 0)
		// 	wheelSpeed = 0;
		wheelSpeed = wheelSpeed * fp(0.7);
		carSpeed.x = carSpeed.x + wheelSpeed * fp(Time::delta) * fp_cos(carAngle) / fp(1000);
		carSpeed.z = carSpeed.z + wheelSpeed * fp(Time::delta) * fp_sin(fp(0)-carAngle) / fp(1000);
		if(carSpeed.z == 0)
			cameraAngle = 0;
		else
			cameraAngle = fp(0) - fp_atan(carSpeed.z / carSpeed.x);
		printf("%.3f %.3f\n", (float)carSpeed.x, (float)carSpeed.z);
		carPos.x = carPos.x + carSpeed.x * fp(Time::delta);
		carPos.z = carPos.z + carSpeed.z * fp(Time::delta);

		Display::clear(newColor(70, 180, 220));

		carMatrix = mat4();
		carMatrix = mat4::translate(carMatrix, carPos.x, -0.5, carPos.z);
		carMatrix = mat4::rotateY(carMatrix, carAngle);

		view = mat4();
		view = mat4::rotateX(view, HALF_PI*0.1);
		view = mat4::translate(view, 0, 2, 6);
		view = mat4::rotateY(view, fp(0) - cameraAngle - fp(HALF_PI));
		view = mat4::translate(view, fp(0)-carPos.x, 0, fp(0)-carPos.z);

		Rasterizer::drawModel(car);
		Rasterizer::drawModel(floor);

		char buffer[10];
#ifdef SDL
		sprintf(buffer, "%.3f", Time::delta);
#endif
#ifdef PRIZM
		// sprintf(buffer, "%d", (int)Time::delta);
		itoa((int)Time::delta, (unsigned char*)buffer);
#endif
		Display::drawText(0, 0, buffer, newColor(255, 255, 255));

		Display::show();
	}
}



