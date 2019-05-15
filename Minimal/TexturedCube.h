#pragma once

#ifndef TEXTUREDCUBE_H
#define TEXTUREDCUBE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

class TexturedCube{
public:
	TexturedCube(const char * tex);
	~TexturedCube();

	glm::mat4 toWorld;

	void draw(glm::mat4 projection, glm::mat4 headPose, GLint shader, glm::mat4 M);
	void update();

	void setPosition(glm::vec3 pos);
	void setScale(float scale);

private:
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;

	GLuint VBO, VAO, EBO, TEX, VBO2;

	void initCube(float size);
	void initBuffers();
	void initTextures(const char * tex);
};

#endif //TEXTURECUBE_H
