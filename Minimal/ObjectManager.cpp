#include "ObjectManager.h"
#include "Definitions.h"
#include "Shaders.h"
#include "Shader.h"
#include "Transform.h"
#include "TexturedCube.h"
#include "Skybox.h"

//Init Shaders
GLint Shaders::colorShader = 0;
GLint Shaders::textureShader = 0;
GLint Shaders::skyboxShader = 0;
//Declare Models
Model * sphere;
//Declare Objects
Transform * handL;
Transform * handR;
//Declare Skyboxes
Skybox * skyboxLeft;
Skybox * skyboxRight;
Skybox * skyboxCustom;
//Cube variables
TexturedCube * steamCube;
glm::vec3 cubePositions[2] = { glm::vec3(0, 0, -0.3), glm::vec3(0, 0, -0.9) };
glm::vec3 cubeScaleFactor = glm::vec3(0.1f);

ObjectManager::~ObjectManager() {
	//Delete objects (also deletes models)
	delete(handL);
	delete(handR);
	delete(steamCube);
	delete(skyboxLeft);
	delete(skyboxRight);
	delete(skyboxCustom);
	//Delete shaders
	Shaders::deleteShaders();
}

ObjectManager::ObjectManager() {
	initShaders();
	initModels();
	initObjects();
	initValues();
}

void ObjectManager::initShaders() {
	Shaders::setColorShader(LoadShaders(SHADER_COLOR_VERTEX, SHADER_COLOR_FRAGMENT));
	Shaders::setTextureShader(LoadShaders(SHADER_TEXTURE_VERTEX, SHADER_TEXTURE_FRAGMENT));
	Shaders::setSkyboxShader(LoadShaders(SHADER_SKYBOX_VERTEX, SHADER_SKYBOX_FRAGMENT));
}

void ObjectManager::initModels() {
	sphere = new Model(MODEL_SPHERE);
}

void ObjectManager::initObjects() {
	skyboxLeft = new Skybox(TEXTURE_SKYBOX_LEFT);
	skyboxRight = new Skybox(TEXTURE_SKYBOX_RIGHT);
	skyboxCustom = new Skybox(TEXTURE_SKYBOX_CUSTOM);
	handL = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_CYAN));
	handR = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_RED));
	steamCube = new TexturedCube(TEXTURE_CUBE_STEAM);
}

void ObjectManager::initValues() {
	skyboxLeft->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
	skyboxRight->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
	skyboxCustom->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
}

void ObjectManager::draw(glm::mat4 headPose, glm::mat4 projection, int eye) {
	//Draw stereo skybox
	if (eye == 0) skyboxLeft->draw(Shaders::getSkyboxShader(), projection, headPose);
	else skyboxRight->draw(Shaders::getSkyboxShader(), projection, headPose);
	//Draw hands
	handL->draw(headPose, projection);
	handR->draw(headPose, projection);
	//Draw 2 cubes
	for (int i = 0; i < 2; i++) {
		steamCube->toWorld = glm::translate(glm::mat4(1.0f), cubePositions[i]) * glm::scale(glm::mat4(1.0f), cubeScaleFactor);
		steamCube->draw(Shaders::getSkyboxShader(), projection, headPose);
	}
}

void ObjectManager::update(double deltaTime) {
	handL->update(deltaTime);
	handR->update(deltaTime);
}

void ObjectManager::updateHands(glm::mat4 left, glm::mat4 right) {
	handL->setToWorld(left);
	handL->scale(0.015f);
	handR->setToWorld(right);
	handR->scale(0.015f);
}