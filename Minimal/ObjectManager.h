#pragma once
#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER

#include <glm/glm.hpp>

class ObjectManager {
public:
	ObjectManager();
	~ObjectManager();

	void draw(glm::mat4 headPose, glm::mat4 projection);
	void update(double deltaTime);
	void updateHands(glm::mat4 handL, glm::mat4 handR);

private:
	void initShaders();
	void initModels();
	void initObjects();
};

#endif
