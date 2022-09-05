#include "display.h"
#include "rasterizer.h"
#include "time.h"
#include "util.h"
#include "input.h"
#include "mat4.h"
#include "rmath.h"
#include "math.h"

mat4 view;
vec3f carPos = {0, 0, 0};
vec3f cameraPos = {0, 0, 0};
vec3f carSpeed = {0, 0, 0};
float carDirection = 0;
float carAngle = 0;
float cameraAngle = 0;
float wheelSpeed = 0;

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
	Triangle coneTriangles[28] = {
		{
			{1.000, 0.193, 1.000},
			{-0.656, 0.193, 0.656},
			{0.656, 0.193, 0.656},
			{0.000, -1.000, -0.000},
			newColor(255, 100, 0)},
		{
			{-1.000, 0.193, -1.000},
			{0.656, 0.193, -0.656},
			{-0.656, 0.193, -0.656},
			{-0.000, -1.000, 0.000},
			newColor(255, 100, 0)},
		{
			{-1.000, 0.193, 1.000},
			{-0.656, 0.193, -0.656},
			{-0.656, 0.193, 0.656},
			{0.000, -1.000, 0.000},
			newColor(255, 100, 0)},
		{
			{1.000, 0.193, -1.000},
			{0.656, 0.193, 0.656},
			{0.656, 0.193, -0.656},
			{-0.000, -1.000, 0.000},
			newColor(255, 100, 0)},
		{
			{0.328, -0.679, 0.328},
			{-0.328, -0.679, 0.328},
			{0.000, -1.552, 0.000},
			{-0.000, -0.352, 0.936},
			newColor(255, 100, 0)},
		{
			{-0.328, -0.679, -0.328},
			{0.328, -0.679, -0.328},
			{0.000, -1.552, 0.000},
			{0.000, -0.352, -0.936},
			newColor(255, 100, 0)},
		{
			{-0.328, -0.679, 0.328},
			{-0.328, -0.679, -0.328},
			{0.000, -1.552, 0.000},
			{-0.936, -0.352, 0.000},
			newColor(255, 100, 0)},
		{
			{0.328, -0.679, -0.328},
			{0.328, -0.679, 0.328},
			{0.000, -1.552, 0.000},
			{0.936, -0.352, -0.000},
			newColor(255, 100, 0)},
		{
			{0.492, -0.243, 0.492},
			{0.328, -0.679, -0.328},
			{0.492, -0.243, -0.492},
			{0.936, -0.352, -0.000},
			newColor(255, 255, 255)},
		{
			{-0.492, -0.243, -0.492},
			{-0.328, -0.679, 0.328},
			{-0.492, -0.243, 0.492},
			{-0.936, -0.352, 0.000},
			newColor(255, 255, 255)},
		{
			{-0.492, -0.243, -0.492},
			{0.328, -0.679, -0.328},
			{-0.328, -0.679, -0.328},
			{0.000, -0.352, -0.936},
			newColor(255, 255, 255)},
		{
			{0.492, -0.243, 0.492},
			{-0.328, -0.679, 0.328},
			{0.328, -0.679, 0.328},
			{-0.000, -0.352, 0.936},
			newColor(255, 255, 255)},
		{
			{-0.656, 0.193, 0.656},
			{0.492, -0.243, 0.492},
			{0.656, 0.193, 0.656},
			{0.000, -0.352, 0.936},
			newColor(255, 100, 0)},
		{
			{0.656, 0.193, -0.656},
			{-0.492, -0.243, -0.492},
			{-0.656, 0.193, -0.656},
			{0.000, -0.352, -0.936},
			newColor(255, 100, 0)},
		{
			{-0.656, 0.193, -0.656},
			{-0.492, -0.243, 0.492},
			{-0.656, 0.193, 0.656},
			{-0.936, -0.352, 0.000},
			newColor(255, 100, 0)},
		{
			{0.656, 0.193, 0.656},
			{0.492, -0.243, -0.492},
			{0.656, 0.193, -0.656},
			{0.936, -0.352, 0.000},
			newColor(255, 100, 0)},
		{
			{1.000, 0.193, 1.000},
			{-1.000, 0.193, 1.000},
			{-0.656, 0.193, 0.656},
			{0.000, -1.000, -0.000},
			newColor(255, 100, 0)},
		{
			{-1.000, 0.193, -1.000},
			{1.000, 0.193, -1.000},
			{0.656, 0.193, -0.656},
			{0.000, -1.000, 0.000},
			newColor(255, 100, 0)},
		{
			{-1.000, 0.193, 1.000},
			{-1.000, 0.193, -1.000},
			{-0.656, 0.193, -0.656},
			{0.000, -1.000, -0.000},
			newColor(255, 100, 0)},
		{
			{1.000, 0.193, -1.000},
			{1.000, 0.193, 1.000},
			{0.656, 0.193, 0.656},
			{-0.000, -1.000, 0.000},
			newColor(255, 100, 0)},
		{
			{0.492, -0.243, 0.492},
			{0.328, -0.679, 0.328},
			{0.328, -0.679, -0.328},
			{0.936, -0.352, -0.000},
			newColor(255, 255, 255)},
		{
			{-0.492, -0.243, -0.492},
			{-0.328, -0.679, -0.328},
			{-0.328, -0.679, 0.328},
			{-0.936, -0.352, 0.000},
			newColor(255, 255, 255)},
		{
			{-0.492, -0.243, -0.492},
			{0.492, -0.243, -0.492},
			{0.328, -0.679, -0.328},
			{0.000, -0.352, -0.936},
			newColor(255, 255, 255)},
		{
			{0.492, -0.243, 0.492},
			{-0.492, -0.243, 0.492},
			{-0.328, -0.679, 0.328},
			{-0.000, -0.352, 0.936},
			newColor(255, 255, 255)},
		{
			{-0.656, 0.193, 0.656},
			{-0.492, -0.243, 0.492},
			{0.492, -0.243, 0.492},
			{-0.000, -0.352, 0.936},
			newColor(255, 100, 0)},
		{
			{0.656, 0.193, -0.656},
			{0.492, -0.243, -0.492},
			{-0.492, -0.243, -0.492},
			{0.000, -0.352, -0.936},
			newColor(255, 100, 0)},
		{
			{-0.656, 0.193, -0.656},
			{-0.492, -0.243, -0.492},
			{-0.492, -0.243, 0.492},
			{-0.936, -0.352, 0.000},
			newColor(255, 100, 0)},
		{
			{0.656, 0.193, 0.656},
			{0.492, -0.243, 0.492},
			{0.492, -0.243, -0.492},
			{0.936, -0.352, -0.000},
			newColor(255, 100, 0)},
	};

	Triangle carTriangles[134] = {
		{
			{0.794, 0.294, -0.380},
			{-0.535, 0.294, -0.443},
			{-0.535, 0.294, -0.000},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{0.794, 0.294, -0.380},
			{0.794, 0.294, -0.000},
			{1.345, 0.294, -0.000},
			{-0.000, 1.000, -0.000},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{1.886, 0.294, -0.000},
			{1.795, 0.294, -0.605},
			{-0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.000},
			{-0.535, 0.294, -0.443},
			{-1.188, 0.263, -0.431},
			{-0.047, 0.999, 0.000},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, -0.681},
			{-1.188, 0.263, -0.431},
			{-1.188, 0.227, -0.719},
			{-0.440, 0.891, -0.109},
			newColor(203, 2, 1)},
		{
			{1.795, 0.294, -0.605},
			{1.886, 0.294, -0.000},
			{1.886, 0.003, -0.000},
			{0.989, -0.000, -0.149},
			newColor(203, 2, 1)},
		{
			{1.795, 0.003, -0.605},
			{1.886, 0.003, -0.000},
			{0.985, -0.225, -0.000},
			{0.245, -0.969, -0.037},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, -0.546},
			{0.985, -0.225, -0.000},
			{0.302, -0.435, -0.000},
			{0.290, -0.945, -0.151},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, -0.525},
			{0.743, -0.212, -0.546},
			{0.302, -0.424, -0.389},
			{-0.001, -0.594, -0.805},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{-0.559, -0.455, -0.000},
			{-0.474, -0.415, -0.387},
			{0.024, -0.995, -0.098},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, -0.325},
			{-1.287, -0.240, -0.394},
			{-0.474, -0.415, -0.387},
			{-0.088, -0.368, 0.926},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, -0.325},
			{-0.559, -0.455, -0.000},
			{-0.559, -0.240, -0.000},
			{-1.000, -0.000, -0.000},
			newColor(203, 2, 1)},
		{
			{-0.474, -0.415, -0.387},
			{-1.287, -0.240, -0.394},
			{-0.474, -0.239, -0.525},
			{-0.125, -0.612, -0.781},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-0.559, -0.240, -0.325},
			{-0.559, -0.240, -0.000},
			{0.000, -1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{0.756, -0.055, -0.687},
			{0.743, -0.212, -0.546},
			{0.064, -0.671, -0.739},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{1.283, -0.062, -0.380},
			{0.868, -0.070, -0.380},
			{0.000, -0.000, -1.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.674},
			{0.794, 0.294, -0.674},
			{0.756, -0.055, -0.687},
			{-0.000, 0.037, -0.999},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-1.385, -0.217, -0.739},
			{-0.489, -0.177, -0.683},
			{0.049, -0.995, -0.083},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-1.282, -0.240, -0.000},
			{-1.441, -0.240, -0.000},
			{-0.000, -1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.399, 0.134, 0.012},
			{-1.399, 0.128, -0.681},
			{-0.994, 0.112, -0.001},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, -0.431},
			{-1.036, -0.065, -0.719},
			{-1.188, 0.227, -0.719},
			{0.888, 0.460, 0.000},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, -0.431},
			{-0.605, -0.065, -0.443},
			{-0.605, -0.065, -0.697},
			{0.000, 1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.443},
			{-0.605, -0.065, -0.443},
			{-1.036, -0.065, -0.431},
			{-0.027, 0.005, -1.000},
			newColor(0, 0, 0)},
		{
			{-0.605, -0.065, -0.443},
			{-0.535, 0.294, -0.443},
			{-0.535, 0.294, -0.674},
			{-0.982, 0.190, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{-0.605, -0.065, -0.697},
			{-0.535, 0.294, -0.674},
			{0.148, 0.033, -0.988},
			newColor(203, 2, 1)},
		{
			{-0.489, -0.177, -0.683},
			{-1.385, -0.217, -0.739},
			{-1.036, -0.065, -0.719},
			{0.063, -0.014, -0.998},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, -0.681},
			{-1.188, 0.227, -0.719},
			{-1.036, -0.065, -0.719},
			{-0.143, -0.074, -0.987},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{1.345, 0.294, -0.380},
			{1.345, 0.294, -0.711},
			{-0.985, 0.173, 0.000},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, -0.674},
			{0.794, 0.294, -0.380},
			{0.868, -0.070, -0.380},
			{0.980, 0.200, 0.000},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, -0.711},
			{0.756, -0.055, -0.687},
			{0.794, 0.294, -0.674},
			{-0.199, 0.058, -0.978},
			newColor(203, 2, 1)},
		{
			{0.756, -0.055, -0.687},
			{0.868, -0.070, -0.711},
			{0.743, -0.212, -0.546},
			{-0.241, -0.638, -0.731},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.711},
			{1.795, 0.294, -0.605},
			{1.795, 0.003, -0.605},
			{0.229, -0.000, -0.973},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, -0.711},
			{1.283, -0.062, -0.711},
			{0.743, -0.212, -0.546},
			{0.014, -0.764, -0.645},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, -0.546},
			{1.283, -0.062, -0.711},
			{1.795, 0.003, -0.605},
			{0.178, -0.944, -0.277},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{1.283, -0.062, -0.711},
			{0.868, -0.070, -0.711},
			{-0.019, 1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, 0.380},
			{-0.535, 0.294, 0.443},
			{0.794, 0.294, 0.674},
			{0.000, 1.000, -0.000},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.000},
			{0.794, 0.294, -0.000},
			{0.794, 0.294, 0.380},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{1.795, 0.294, 0.605},
			{1.345, 0.294, 0.380},
			{1.345, 0.294, 0.711},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.431},
			{-0.535, 0.294, 0.443},
			{-0.535, 0.294, -0.000},
			{-0.047, 0.999, 0.000},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.431},
			{-1.399, 0.128, 0.681},
			{-1.188, 0.227, 0.719},
			{-0.440, 0.891, 0.109},
			newColor(203, 2, 1)},
		{
			{1.886, 0.003, -0.000},
			{1.886, 0.294, -0.000},
			{1.795, 0.294, 0.605},
			{0.989, 0.000, 0.149},
			newColor(203, 2, 1)},
		{
			{0.985, -0.225, -0.000},
			{1.886, 0.003, -0.000},
			{1.795, 0.003, 0.605},
			{0.245, -0.969, 0.037},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{0.985, -0.225, -0.000},
			{0.743, -0.212, 0.546},
			{0.290, -0.945, 0.151},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, 0.525},
			{-0.474, -0.415, 0.387},
			{0.302, -0.424, 0.389},
			{-0.009, -0.617, 0.787},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{0.302, -0.424, 0.389},
			{-0.474, -0.415, 0.387},
			{-0.011, -1.000, 0.027},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, 0.325},
			{-0.474, -0.415, 0.387},
			{-1.287, -0.240, 0.394},
			{-0.088, -0.368, -0.926},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.455, -0.000},
			{-0.559, -0.240, 0.325},
			{-0.559, -0.240, -0.000},
			{-1.000, -0.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-0.474, -0.415, 0.387},
			{-0.474, -0.239, 0.525},
			{-1.287, -0.240, 0.394},
			{-0.125, -0.612, 0.781},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-0.559, -0.240, 0.325},
			{-1.287, -0.240, 0.394},
			{0.000, -1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{0.743, -0.212, 0.546},
			{0.756, -0.055, 0.687},
			{-0.489, -0.177, 0.683},
			{0.064, -0.671, 0.739},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.380},
			{0.794, 0.294, 0.380},
			{0.868, -0.070, 0.380},
			{-0.000, -0.000, 1.000},
			newColor(0, 0, 0)},
		{
			{0.756, -0.055, 0.687},
			{0.794, 0.294, 0.674},
			{-0.535, 0.294, 0.674},
			{0.000, 0.037, 0.999},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, 0.394},
			{-0.474, -0.239, 0.525},
			{-0.489, -0.177, 0.683},
			{-0.057, -0.931, 0.359},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.282, -0.240, -0.000},
			{-1.287, -0.240, 0.394},
			{0.000, -1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, 0.681},
			{-1.399, 0.134, -0.012},
			{-1.441, -0.240, -0.000},
			{-0.994, 0.112, 0.001},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.431},
			{-1.188, 0.263, 0.431},
			{-1.188, 0.227, 0.719},
			{0.907, 0.419, 0.051},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, 0.431},
			{-1.036, -0.065, 0.719},
			{-0.605, -0.065, 0.697},
			{0.000, 1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.443},
			{-1.188, 0.263, 0.431},
			{-1.036, -0.065, 0.431},
			{-0.018, -0.008, 1.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.674},
			{-0.535, 0.294, 0.443},
			{-0.605, -0.065, 0.443},
			{-0.982, 0.190, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, 0.683},
			{-0.535, 0.294, 0.674},
			{-0.605, -0.065, 0.697},
			{0.148, 0.033, 0.988},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.719},
			{-1.385, -0.217, 0.739},
			{-0.489, -0.177, 0.683},
			{0.063, -0.014, 0.998},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.719},
			{-1.188, 0.227, 0.719},
			{-1.399, 0.128, 0.681},
			{-0.143, -0.074, 0.987},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.711},
			{1.345, 0.294, 0.380},
			{1.283, -0.062, 0.380},
			{-0.985, 0.173, 0.000},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, 0.380},
			{0.794, 0.294, 0.380},
			{0.794, 0.294, 0.674},
			{0.980, 0.200, 0.000},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, 0.711},
			{0.794, 0.294, 0.674},
			{0.756, -0.055, 0.687},
			{-0.199, 0.058, 0.978},
			newColor(203, 2, 1)},
		{
			{0.756, -0.055, 0.687},
			{0.743, -0.212, 0.546},
			{0.868, -0.070, 0.711},
			{-0.241, -0.638, 0.731},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.711},
			{1.283, -0.062, 0.711},
			{1.795, 0.003, 0.605},
			{0.207, -0.036, 0.978},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, 0.711},
			{0.743, -0.212, 0.546},
			{1.283, -0.062, 0.711},
			{0.014, -0.764, 0.645},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, 0.546},
			{1.795, 0.003, 0.605},
			{1.283, -0.062, 0.711},
			{0.178, -0.944, 0.277},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, 0.711},
			{1.283, -0.062, 0.711},
			{1.283, -0.062, 0.380},
			{-0.019, 1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.443},
			{0.794, 0.294, -0.674},
			{-0.535, 0.294, -0.674},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{0.794, 0.294, -0.380},
			{-0.535, 0.294, -0.000},
			{0.794, 0.294, -0.000},
			{-0.000, 1.000, -0.000},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.443},
			{0.794, 0.294, -0.380},
			{0.794, 0.294, -0.674},
			{-0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{0.794, 0.294, -0.380},
			{1.345, 0.294, -0.000},
			{1.345, 0.294, -0.380},
			{-0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{1.886, 0.294, -0.000},
			{1.345, 0.294, -0.380},
			{1.345, 0.294, -0.000},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{1.795, 0.294, -0.605},
			{1.345, 0.294, -0.711},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.000},
			{-1.188, 0.263, -0.431},
			{-1.188, 0.263, 0.012},
			{-0.047, 0.999, -0.001},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.012},
			{-1.188, 0.263, -0.431},
			{-1.399, 0.134, 0.012},
			{-0.522, 0.853, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.134, 0.012},
			{-1.188, 0.263, -0.431},
			{-1.399, 0.128, -0.681},
			{-0.532, 0.847, -0.007},
			newColor(203, 2, 1)},
		{
			{1.795, 0.294, -0.605},
			{1.886, 0.003, -0.000},
			{1.795, 0.003, -0.605},
			{0.989, -0.000, -0.149},
			newColor(203, 2, 1)},
		{
			{1.795, 0.003, -0.605},
			{0.985, -0.225, -0.000},
			{0.743, -0.212, -0.546},
			{0.193, -0.975, -0.109},
			newColor(203, 2, 1)},
		{
			{0.743, -0.212, -0.546},
			{0.302, -0.435, -0.000},
			{0.302, -0.424, -0.389},
			{0.426, -0.904, -0.024},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, -0.525},
			{0.302, -0.424, -0.389},
			{-0.474, -0.415, -0.387},
			{-0.009, -0.617, -0.787},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{-0.474, -0.415, -0.387},
			{0.302, -0.424, -0.389},
			{-0.011, -1.000, -0.027},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.240, -0.325},
			{-0.474, -0.415, -0.387},
			{-0.559, -0.455, -0.000},
			{-0.905, -0.355, -0.235},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-1.287, -0.240, -0.394},
			{-0.559, -0.240, -0.325},
			{0.000, -1.000, -0.000},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{0.743, -0.212, -0.546},
			{-0.474, -0.239, -0.525},
			{0.015, -0.931, -0.366},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.380},
			{0.868, -0.070, -0.380},
			{0.794, 0.294, -0.380},
			{0.000, -0.000, -1.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.674},
			{0.756, -0.055, -0.687},
			{-0.489, -0.177, -0.683},
			{-0.005, 0.018, -1.000},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-0.489, -0.177, -0.683},
			{-0.474, -0.239, -0.525},
			{-0.057, -0.931, -0.359},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, -0.394},
			{-1.441, -0.240, -0.000},
			{-1.385, -0.217, -0.739},
			{-0.101, -0.994, -0.040},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.399, 0.128, -0.681},
			{-1.385, -0.217, -0.739},
			{-0.997, -0.027, -0.076},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, -0.431},
			{-1.188, 0.227, -0.719},
			{-1.188, 0.263, -0.431},
			{0.907, 0.419, -0.051},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, -0.431},
			{-0.605, -0.065, -0.697},
			{-1.036, -0.065, -0.719},
			{0.000, 1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, -0.443},
			{-1.036, -0.065, -0.431},
			{-1.188, 0.263, -0.431},
			{-0.018, -0.008, -1.000},
			newColor(0, 0, 0)},
		{
			{-0.605, -0.065, -0.443},
			{-0.535, 0.294, -0.674},
			{-0.605, -0.065, -0.697},
			{-0.982, 0.190, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.489, -0.177, -0.683},
			{-1.036, -0.065, -0.719},
			{-0.605, -0.065, -0.697},
			{0.052, -0.067, -0.996},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, -0.681},
			{-1.036, -0.065, -0.719},
			{-1.385, -0.217, -0.739},
			{-0.016, 0.165, -0.986},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{1.345, 0.294, -0.711},
			{1.283, -0.062, -0.711},
			{-0.985, 0.173, 0.000},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, -0.674},
			{0.868, -0.070, -0.380},
			{0.868, -0.070, -0.711},
			{0.980, 0.200, 0.000},
			newColor(0, 0, 0)},
		{
			{1.345, 0.294, -0.711},
			{1.795, 0.003, -0.605},
			{1.283, -0.062, -0.711},
			{0.207, -0.036, -0.978},
			newColor(203, 2, 1)},
		{
			{1.283, -0.062, -0.380},
			{0.868, -0.070, -0.711},
			{0.868, -0.070, -0.380},
			{-0.019, 1.000, -0.000},
			newColor(0, 0, 0)},
		{
			{0.794, 0.294, 0.674},
			{-0.535, 0.294, 0.443},
			{-0.535, 0.294, 0.674},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, 0.443},
			{0.794, 0.294, 0.380},
			{-0.535, 0.294, -0.000},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{-0.535, 0.294, -0.000},
			{0.794, 0.294, 0.380},
			{0.794, 0.294, -0.000},
			{-0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, -0.000},
			{0.794, 0.294, 0.380},
			{1.345, 0.294, 0.380},
			{-0.000, 1.000, -0.000},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.380},
			{1.886, 0.294, -0.000},
			{1.345, 0.294, -0.000},
			{0.000, 1.000, 0.000},
			newColor(203, 2, 1)},
		{
			{1.886, 0.294, -0.000},
			{1.345, 0.294, 0.380},
			{1.795, 0.294, 0.605},
			{0.000, 1.000, -0.000},
			newColor(203, 2, 1)},
		{
			{-1.188, 0.263, 0.431},
			{-0.535, 0.294, -0.000},
			{-1.188, 0.263, -0.012},
			{-0.047, 0.999, 0.001},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, 0.681},
			{-1.188, 0.263, 0.431},
			{-1.399, 0.134, -0.012},
			{-0.532, 0.847, 0.007},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.134, -0.012},
			{-1.188, 0.263, 0.431},
			{-1.188, 0.263, -0.012},
			{-0.522, 0.853, 0.000},
			newColor(203, 2, 1)},
		{
			{1.886, 0.003, -0.000},
			{1.795, 0.294, 0.605},
			{1.795, 0.003, 0.605},
			{0.989, -0.000, 0.149},
			newColor(203, 2, 1)},
		{
			{0.985, -0.225, -0.000},
			{1.795, 0.003, 0.605},
			{0.743, -0.212, 0.546},
			{0.193, -0.975, 0.109},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{0.743, -0.212, 0.546},
			{0.302, -0.424, 0.389},
			{0.426, -0.904, 0.024},
			newColor(85, 163, 202)},
		{
			{-0.474, -0.239, 0.525},
			{0.302, -0.424, 0.389},
			{0.743, -0.212, 0.546},
			{-0.001, -0.594, 0.805},
			newColor(203, 2, 1)},
		{
			{0.302, -0.435, -0.000},
			{-0.474, -0.415, 0.387},
			{-0.559, -0.455, -0.000},
			{0.024, -0.995, 0.098},
			newColor(203, 2, 1)},
		{
			{-0.559, -0.455, -0.000},
			{-0.474, -0.415, 0.387},
			{-0.559, -0.240, 0.325},
			{-0.905, -0.355, 0.235},
			newColor(203, 2, 1)},
		{
			{-1.282, -0.240, -0.000},
			{-0.559, -0.240, -0.000},
			{-0.559, -0.240, 0.325},
			{0.000, -1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{0.743, -0.212, 0.546},
			{-0.489, -0.177, 0.683},
			{-0.474, -0.239, 0.525},
			{0.015, -0.931, 0.366},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.380},
			{0.868, -0.070, 0.380},
			{1.283, -0.062, 0.380},
			{0.000, -0.000, 1.000},
			newColor(0, 0, 0)},
		{
			{0.756, -0.055, 0.687},
			{-0.535, 0.294, 0.674},
			{-0.489, -0.177, 0.683},
			{-0.005, 0.018, 1.000},
			newColor(203, 2, 1)},
		{
			{-1.287, -0.240, 0.394},
			{-0.489, -0.177, 0.683},
			{-1.385, -0.217, 0.739},
			{0.049, -0.995, 0.083},
			newColor(203, 2, 1)},
		{
			{-1.441, -0.240, -0.000},
			{-1.287, -0.240, 0.394},
			{-1.385, -0.217, 0.739},
			{-0.101, -0.994, 0.040},
			newColor(203, 2, 1)},
		{
			{-1.399, 0.128, 0.681},
			{-1.441, -0.240, -0.000},
			{-1.385, -0.217, 0.739},
			{-0.997, -0.027, 0.076},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.431},
			{-1.188, 0.227, 0.719},
			{-1.036, -0.065, 0.719},
			{0.888, 0.460, 0.000},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, 0.431},
			{-0.605, -0.065, 0.697},
			{-0.605, -0.065, 0.443},
			{0.000, 1.000, 0.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.443},
			{-1.036, -0.065, 0.431},
			{-0.605, -0.065, 0.443},
			{-0.027, 0.005, 1.000},
			newColor(0, 0, 0)},
		{
			{-0.535, 0.294, 0.674},
			{-0.605, -0.065, 0.443},
			{-0.605, -0.065, 0.697},
			{-0.982, 0.190, -0.000},
			newColor(0, 0, 0)},
		{
			{-1.036, -0.065, 0.719},
			{-0.489, -0.177, 0.683},
			{-0.605, -0.065, 0.697},
			{0.052, -0.067, 0.996},
			newColor(203, 2, 1)},
		{
			{-1.036, -0.065, 0.719},
			{-1.399, 0.128, 0.681},
			{-1.385, -0.217, 0.739},
			{-0.016, 0.165, 0.986},
			newColor(203, 2, 1)},
		{
			{1.345, 0.294, 0.711},
			{1.283, -0.062, 0.380},
			{1.283, -0.062, 0.711},
			{-0.985, 0.173, -0.000},
			newColor(0, 0, 0)},
		{
			{0.868, -0.070, 0.380},
			{0.794, 0.294, 0.674},
			{0.868, -0.070, 0.711},
			{0.980, 0.200, 0.000},
			newColor(0, 0, 0)},
		{
			{1.345, 0.294, 0.711},
			{1.795, 0.003, 0.605},
			{1.795, 0.294, 0.605},
			{0.229, 0.000, 0.973},
			newColor(203, 2, 1)},
		{
			{0.868, -0.070, 0.711},
			{1.283, -0.062, 0.380},
			{0.868, -0.070, 0.380},
			{-0.019, 1.000, 0.000},
			newColor(0, 0, 0)},
	};

	Mesh carMesh = {
		134,
		carTriangles
	};

	Mesh coneMesh = {
		28,
		coneTriangles
	};


	srand(0);

	// Triangle treeTriangles[10*10];
	// for(int x = 0; x < 10; x++){
	// 	for(int y = 0; y < 10; y++){
	// 		// int treeX = (x-50)*10;
	// 		// int treeY = (y-50)*10;
	// 		int treeX = ((rand()%100) - 50)*10;
	// 		int treeY = ((rand()%100) - 50)*10;
	// 		treeTriangles[x+y*10] = {
	// 			{treeX-1, 0, treeY},
	// 			{treeX, -1, treeY},
	// 			{treeX+1, 0, treeY},
	// 			newColor(0, 255, 0)
	// 		};
	// 	}
	// }
	// Mesh treeMesh = {
	// 	10*10,
	// 	treeTriangles
	// };

#define NUM_TRACK_POINTS 29
#define TRACK_WIDTH 10
	vec3f trackPoints[NUM_TRACK_POINTS+1] = {
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
		{0, 0, 0},
		{2, 0, 0},
	};
	for(int i = 0; i < NUM_TRACK_POINTS+1; i++){
		trackPoints[i] = trackPoints[i] * 40;
		trackPoints[i].y = 0;
	}

	Triangle trackTriangles[NUM_TRACK_POINTS*2-2];
	Model coneModels[NUM_TRACK_POINTS*2-2];

	for(int i = 0; i < NUM_TRACK_POINTS-1; i++){
		vec3f pos = trackPoints[i];
		vec3f direction = trackPoints[i+1] - trackPoints[i];
		vec3f nextDirection = trackPoints[i+2] - trackPoints[i+1];
		vec3f perpendicular = {direction.z, direction.y, -direction.x};
		perpendicular = perpendicular * fp(TRACK_WIDTH) * perpendicular.i_length();
		vec3f nextPerpendicular = {nextDirection.z, nextDirection.y, -nextDirection.x};
		nextPerpendicular = nextPerpendicular * fp(TRACK_WIDTH) * nextPerpendicular.i_length();

		trackTriangles[i] = {
			vec3d(pos - perpendicular),
			vec3d(pos + perpendicular),
			vec3d(pos - nextPerpendicular + direction),
			{0, -1, 0},
			newColor(50, 50, 50)
		};
		trackTriangles[i+NUM_TRACK_POINTS-1] = {
			vec3d(pos + perpendicular),
			vec3d(pos - nextPerpendicular + direction),
			vec3d(pos + nextPerpendicular + direction),
			{0, -1, 0},
			newColor(50, 50, 50)
		};

		coneModels[i].mesh = coneMesh;
		coneModels[i].modelMatrix = mat4::scale(mat4::translate(mat4(), vec3d(pos - perpendicular)), 1, 1, 1);
		coneModels[i+NUM_TRACK_POINTS-1].mesh = coneMesh;
		coneModels[i+NUM_TRACK_POINTS-1].modelMatrix = mat4::scale(mat4::translate(mat4(), vec3d(pos + perpendicular)), 1, 1, 1);
	}

	Model trackModel = Model({NUM_TRACK_POINTS*2-2, trackTriangles});

	mat4 carMatrix;

	Model carModel = Model(carMesh);

	Rasterizer::init();
	while(true){
		Rasterizer::reset();

		Time::update();
		Input::update();

		if(Input::keyPressed(KEY_MENU)){
			return 0;
		}

		if(Input::keyDown(KEY_RIGHT) || Input::keyDown(KEY_6)){
			carDirection = carDirection + Time::delta / 40.0f;
		}
		if(Input::keyDown(KEY_LEFT) || Input::keyDown(KEY_4)){
			carDirection = carDirection - Time::delta / 40.0f;
		}

		if(Input::keyDown(KEY_UP) || Input::keyDown(KEY_8)){
			wheelSpeed = wheelSpeed + Time::delta / 200.0f;
		}
		if(Input::keyDown(KEY_DOWN) || Input::keyDown(KEY_5)){
			wheelSpeed = wheelSpeed - Time::delta / 200.0f;
		}

		wheelSpeed = wheelSpeed - (wheelSpeed * 0.001f) * Time::delta;
		if(wheelSpeed > 1)
			wheelSpeed = 1;
		if(wheelSpeed < 0)
			wheelSpeed = 0;

		carSpeed.x = carSpeed.x + wheelSpeed * float(fp_cos(fp(carAngle))) * Time::delta / 150.0f;
		carSpeed.z = carSpeed.z + wheelSpeed * float(fp_sin(fp(0)-fp(carAngle))) * Time::delta / 150.0f;
		carSpeed.x = carSpeed.x - (carSpeed.x * 0.01f) * Time::delta;
		carSpeed.z = carSpeed.z - (carSpeed.z * 0.01f) * Time::delta;

		cameraAngle = cameraAngle + (-cameraAngle * 0.02 + 0.02 * carDirection) * Time::delta;
		carAngle = carAngle + (-carAngle * 0.05 + 0.05 * carDirection) * Time::delta;
		// carAngle = carDirection;
		carPos.x = carPos.x + carSpeed.x * Time::delta;
		carPos.z = carPos.z + carSpeed.z * Time::delta;
		// vec3f zero = {0,0,0};
		// cameraPos = cameraPos + (zero - cameraPos * 0.2f + carPos * 0.2f) * Time::delta;
		cameraPos = carPos;

		Display::clear(newColor(70, 180, 220));

		carMatrix = mat4();
		carMatrix = mat4::translate(carMatrix, carPos.x, -0.5, carPos.z);
		carMatrix = mat4::rotateY(carMatrix, carAngle);

		view = mat4();
		view = mat4::rotateX(view, HALF_PI*0.1);
		view = mat4::translate(view, 0, 2, 6);
		view = mat4::rotateY(view, -cameraAngle - HALF_PI);
		view = mat4::translate(view, -cameraPos.x, 0, -cameraPos.z);

		carModel.modelMatrix = carMatrix;
		carModel.viewMatrix = view;

		trackModel.viewMatrix = view;
		trackModel.draw(false, false);

		for(int i = 0; i < NUM_TRACK_POINTS*2-2; i++){
			coneModels[i].viewMatrix = view;
			coneModels[i].draw(true, true);
		}

		carModel.draw(true, true);

		char buffer[10];
#ifdef SDL
		sprintf(buffer, "%.3f", Time::delta);
#endif
#ifdef PRIZM
		// sprintf(buffer, "%d", (int)Time::delta);
		itoa((int)Time::delta, (unsigned char*)buffer);
#endif
		// Display::fillRect(0, 0, 30, 20, newColor(0, 0, 0));
		Display::drawText(0, 0, buffer, newColor(255, 255, 255));

		Display::show();
	}
}



