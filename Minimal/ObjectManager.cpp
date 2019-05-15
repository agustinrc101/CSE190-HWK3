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
GLint Shaders::renderedTextureShader = 0;
//Declare Models
Model * sphere;
//Declare Objects
Transform * handL;
Transform * handR;
//Declare Skyboxes
Skybox * skyboxCustom;

ObjectManager::~ObjectManager() {
	//Delete objects (also deletes models)
	delete(handL);
	delete(handR);
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
	Shaders::setRenderedTextureShader(LoadShaders(SHADER_RENDERED_TEXTURE_VERTEX, SHADER_RENDERED_TEXTURE_FRAGMENT));
}

void ObjectManager::initModels() {
	sphere = new Model(MODEL_SPHERE);
}

void ObjectManager::initObjects() {
	skyboxCustom = new Skybox(TEXTURE_SKYBOX_CUSTOM);
	handL = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_CYAN));
	handR = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_RED));
}

void ObjectManager::initValues() {
	skyboxCustom->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
}

void ObjectManager::draw(glm::mat4 headPose, glm::mat4 projection, int eye) {
	//Draw skybox skybox
	skyboxCustom->draw(Shaders::getSkyboxShader(), projection, headPose);
	//Draw hands
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