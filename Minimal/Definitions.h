#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <iostream>

//Shaders
#define SHADER_COLOR_VERTEX "./shaders/color.vert"
#define SHADER_COLOR_FRAGMENT "./shaders/color.frag"
#define SHADER_TEXTURE_VERTEX "./shaders/TextureShader.vert"
#define SHADER_TEXTURE_FRAGMENT "./shaders/TextureShader.frag"
#define SHADER_SKYBOX_VERTEX "./shaders/skybox.vert"
#define SHADER_SKYBOX_FRAGMENT "./shaders/skybox.frag"
#define SHADER_RENDERED_TEXTURE_VERTEX "./shaders/RenderedTextureShader.vert"
#define SHADER_RENDERED_TEXTURE_FRAGMENT "./shaders/RenderedTextureShader.frag"
#define SHADER_LCDISPLAY_VERTEX "./shaders/LCDisplayShader.vert"
#define SHADER_LCDISPLAY_FRAGMENT "./shaders/LCDisplayShader.frag"

//Textures
#define TEXTURE_SKYBOX_LEFT "skybox/left"
#define TEXTURE_SKYBOX_RIGHT "skybox/right"
#define TEXTURE_SKYBOX_CUSTOM "skybox/custom"
#define TEXTURE_CUBE_STEAM "textures/steam/albedo.ppm"
#define TEXTURE_CUBE_LEFT "textures/left/albedo.ppm"
#define TEXTURE_CUBE_RIGHT "textures/right/albedo.ppm"

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

//Variables
#define MATH_PI 3.1415926535897932384626433832795f

//Enums


//Print Matrices
static void print(glm::mat4 c){
	std::cout << "=============================" << std::endl;
	std::cout << c[0][0] << ", " << c[1][0] << ", " << c[2][0] << ", " << c[3][0] << std::endl;
	std::cout << c[0][1] << ", " << c[1][1] << ", " << c[2][1] << ", " << c[3][1] << std::endl;
	std::cout << c[0][2] << ", " << c[1][2] << ", " << c[2][2] << ", " << c[3][2] << std::endl;
	std::cout << c[0][3] << ", " << c[1][3] << ", " << c[2][3] << ", " << c[3][3] << std::endl;
	std::cout << "=============================" << std::endl;
};
//Print Vec4
static void print(glm::vec4 v){
	std::cout << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
};
//Print Vec3
static void print(glm::vec3 v) {
	std::cout << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
};
//Print Vec2
static void print(glm::vec2 v) {
	std::cout << v[0] << ", " << v[1] << std::endl;
}

#endif