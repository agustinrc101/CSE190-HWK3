#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <float.h>

class Skybox{
public:
	Skybox(std::string path);
	~Skybox();
	unsigned int getTextureID();
	void draw(glm::mat4 projection, glm::mat4 headPose, GLint shader);

	void setPos(glm::vec3 pos);

private:
	unsigned int textureID;
	std::vector<glm::vec3> vertices;
	
	glm::mat4 toWorld = glm::mat4(1.0f);

	GLuint VBO, VAO, EBO, VBO2;

	void loadCubeMap(std::vector<std::string> faces);
	void initVertices(float p);
	void initCubeMap();
	void helperPrint(int i);

};

#endif