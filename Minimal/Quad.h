#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

class Quad{
public:
	Quad(float size);
	~Quad();
	
	glm::mat4 toWorld = glm::mat4(1.0f);
	std::vector<glm::vec3> vertices;

	void draw(glm::mat4 projection, glm::mat4 headPose, GLint shader, glm::mat4 M, GLuint texture, glm::vec3 eye);
	void draw(glm::mat4 projection, glm::mat4 headPose, GLint shader, glm::mat4 M, glm::vec3 rgb);
	void draw(glm::mat4 projection, glm::mat4 headPose, GLint shader, glm::mat4 M, GLuint texture, glm::vec3 normal, glm::vec3 eyepos);
	void update();

private:
	std::vector<GLuint> indices;
	std::vector<glm::vec2> texCoords;

	GLuint VBO, VAO, EBO, VBO2;

	void initPlane(float size);
	void initBuffers();
};

#endif