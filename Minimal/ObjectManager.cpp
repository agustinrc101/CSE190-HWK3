#include "ObjectManager.h"
#include "Shaders.h"
#include "Shader.h"
#include "Transform.h"

//Init Shaders
GLint Shaders::colorShader = 0;
GLint Shaders::textureShader = 0;
GLint Shaders::skyboxShader = 0;
//Declare Models
Model * sphere;
Model * cube;
//Declare Objects
Transform * handL;
Transform * handR;

ObjectManager::~ObjectManager() {
	//Delete objects (also deletes models)
	delete(handL);
	delete(handR);
	//Delete shaders
	Shaders::deleteShaders();
}

ObjectManager::ObjectManager() {
	initShaders();
	initModels();
	initObjects();
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
	handL = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_CYAN));
	handR = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_RED));
}

void ObjectManager::draw(glm::mat4 headPose, glm::mat4 projection) {
	handL->draw(headPose, projection);
	handR->draw(headPose, projection);
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