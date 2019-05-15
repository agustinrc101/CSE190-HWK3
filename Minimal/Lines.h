#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

class Lines{
public:
	Lines();
	~Lines();

	glm::mat4 toWorld = glm::mat4(1.0f);

	void draw(glm::mat4 projection, glm::mat4 headPose, GLint shader, glm::mat4 M, glm::vec3 rgb);
	void updateEyePos(glm::vec3 leftPos);
	void addVertex(glm::vec3 v);

private:
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	GLuint VBO, VAO, EBO, VBO2;
};

