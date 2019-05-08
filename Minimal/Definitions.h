#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS

#include <iostream>

//Shaders
#define SHADER_COLOR_VERTEX "./shaders/color.vert"
#define SHADER_COLOR_FRAGMENT "./shaders/color.frag"
#define SHADER_TEXTURE_VERTEX "./shaders/TextureShader.vert"
#define SHADER_TEXTURE_FRAGMENT "./shaders/TextureShader.frag"
#define SHADER_SKYBOX_VERTEX "./shaders/skybox.vert"
#define SHADER_SKYBOX_FRAGMENT "./shaders/skybox.frag"

//Textures
#define TEXTURE_SKYBOX_LEFT "skybox/left"
#define TEXTURE_SKYBOX_RIGHT "skybox/right"
#define TEXTURE_SKYBOX_CUSTOM "skybox/custom"
#define TEXTURE_CUBE_STEAM "textures/steam"
#define TEXTURE_CUBE_LEFT "textures/left"
#define TEXTURE_CUBE_RIGHT "textures/right"

//Models
#define MODEL_SPHERE "models/sphere.obj"

//Colors
#define COLOR_RED 1, 0, 0
#define COLOR_GREEN 0, 1, 0
#define COLOR_BLUE 0, 0, 1
#define COLOR_PURPLE 0.5f, 0, 1
#define COLOR_YELLOW 1, 1, 0
#define COLOR_CYAN 0, 1, 1
#define COLOR_BLACK 0, 0, 0
#define COLOR_WHITE .8f, .8f, .8f

//Enums
enum StereoMode {
	STEREO_BOTH = 0,
	STEREO_SKY = 1,
	STEREO_ONESKY = 2,
	STEREO_CUSTOM_SKYBOX = 3,
};

enum DisplayMode {
	MODE_STEREO = 0,
	MODE_MONO = 1,
	MODE_LEFT = 2,
	MODE_RIGHT = 3,
	MODE_INVERTED = 4,
};

enum TrackingMode {
	TRACKING_FULL = 0,
	TRACKING_ORIENTATION = 1,
	TRACKING_POSITION = 2,
	TRACKING_NONE = 3,
};

//Print functions
static void print(glm::mat4 c){
	std::cout << "=============================" << std::endl;
	std::cout << c[0][0] << ", " << c[1][0] << ", " << c[2][0] << ", " << c[3][0] << std::endl;
	std::cout << c[0][1] << ", " << c[1][1] << ", " << c[2][1] << ", " << c[3][1] << std::endl;
	std::cout << c[0][2] << ", " << c[1][2] << ", " << c[2][2] << ", " << c[3][2] << std::endl;
	std::cout << c[0][3] << ", " << c[1][3] << ", " << c[2][3] << ", " << c[3][3] << std::endl;
	std::cout << "=============================" << std::endl;
};

static void print(glm::vec4 v){
	std::cout << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
};

static void print(glm::vec3 v) {
	std::cout << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
};

static void print(glm::vec2 v) {
	std::cout << v[0] << ", " << v[1] << std::endl;
}

#endif