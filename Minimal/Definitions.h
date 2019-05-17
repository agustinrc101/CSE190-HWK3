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


//========
//PRINTING
//========
//Print Matrices
static void print(glm::mat4 c){
	std::cout << "============MATRIX============" << std::endl;
	std::cout << c[0][0] << ", " << c[1][0] << ", " << c[2][0] << ", " << c[3][0] << std::endl;
	std::cout << c[0][1] << ", " << c[1][1] << ", " << c[2][1] << ", " << c[3][1] << std::endl;
	std::cout << c[0][2] << ", " << c[1][2] << ", " << c[2][2] << ", " << c[3][2] << std::endl;
	std::cout << c[0][3] << ", " << c[1][3] << ", " << c[2][3] << ", " << c[3][3] << std::endl;
	std::cout << "==============================" << std::endl;
};
//Print Vec4
static void print(glm::vec4 v){
	std::cout << "Vector4: " << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
};
//Print Vec3
static void print(glm::vec3 v) {
	std::cout << "Vector3: " << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
};
//Print Vec2
static void print(glm::vec2 v) {
	std::cout << "Vector2: " << v[0] << ", " << v[1] << std::endl;
}

/*
//========
//BINDING
//========
//Bind Vertices, Indices, and TexCoords
static std::vector<GLuint> bindBuffers(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, std::vector<glm::vec2> texCoords){
	std::vector<GLuint> buf;	//VAO, VBO, EBO, and VBO2
	buf.push_back(0);
	buf.push_back(0);
	buf.push_back(0);
	buf.push_back(0);

	//Begin
	glGenVertexArrays(1, &buf[0]);
	glGenBuffers(1, &buf[1]);
	glGenBuffers(1, &buf[2]);
	glGenBuffers(1, &buf[3]);

	//Pass vertices
	glBindVertexArray(buf[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Pass indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Pass texture coords
	glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &(texCoords[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);

	//Finish
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return buf;
}

//Bind Vertices and Indices
static std::vector<GLuint> bindBuffers(std::vector<glm::vec3> vertices, std::vector<GLuint> indices) {
	std::vector<GLuint> buf;	//VAO, VBO, EBO, and VBO2
	buf.push_back(0);
	buf.push_back(0);
	buf.push_back(0);

	//Begin
	glGenVertexArrays(1, &buf[0]);
	glGenBuffers(1, &buf[1]);
	glGenBuffers(1, &buf[2]);
	glGenBuffers(1, &buf[3]);

	//Pass vertices
	glBindVertexArray(buf[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Pass indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Finish
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return buf;
}
//Load Textures
static GLuint loadTextures(const char * path) {
	GLuint TEX = 0;
	int width = 0;
	int height = 0;
	int nrcChannels = 0;
	unsigned char * ppm = loadPPM(path, width, height);

	glGenTextures(1, &TEX);
	glBindTexture(GL_TEXTURE_2D, TEX);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, &ppm[0]);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	delete(ppm);

	return TEX;
}
*/

//========
//Helpers
//========
//Calculate triangle normals
static glm::vec3 calculateNormal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
	//Vertex variables
	glm::vec3 pa = p1;
	glm::vec3 pb = p2;
	glm::vec3 pc = p0;

	//Init the direction variables
	glm::vec3 vr, vu, vn;

	//Calculate right, up, and normal vectors
	vr = pb - pa;
	vr = glm::normalize(vr);	//right

	vu = pc - pa;
	vu = glm::normalize(vu);	//up

	vn = glm::cross(vr, vu);
	return glm::normalize(vn);	//normal
}
#endif