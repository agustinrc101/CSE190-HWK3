#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

#include "Definitions.h"
#include "Model.h"
#include "Component.h"
#include <vector>

class Transform {
public:
	Transform(Model * model = NULL, GLint shader = 0, glm::vec3 color = glm::vec3(COLOR_WHITE));
	~Transform();

	void draw(glm::mat4 headPose, glm::mat4 projection);
	void update(double deltaTime);

	//setters
	void setColor(glm::vec3 c) { color = c; }
	void setShader(GLuint s) { shader = s; }
	void setModel(Model * m) { model = m; }
	void setToWorld(glm::mat4 w) { toWorld = w; }
	void setPosition(glm::vec3 p) { toWorld[3] = glm::vec4(p, 1.0f); }

	//transform
	void scale(float s) { toWorld = glm::scale(toWorld, glm::vec3(s, s, s)); }
	void scale(glm::vec3 s) { toWorld = glm::scale(toWorld, s); }

	//Getters
	glm::mat4 getToWorld() { return toWorld; }

	//Others
	void addUpdate(Component * u) { u->setTransform(this); components.emplace_back((std::unique_ptr<Component>)u); }

private:
	glm::mat4 toWorld = glm::mat4(1);
	Model * model = NULL;
	glm::vec3 color = glm::vec3(1);
	GLuint shader = 0;
	std::vector<std::unique_ptr<Component>> components;
};

#endif