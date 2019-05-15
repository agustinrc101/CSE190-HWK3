#include "Skybox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Skybox::Skybox(std::string path){
	std::vector<std::string> faces = 
			{	path + "/left.ppm",		//left		//px
				path + "/right.ppm",	//right		//nx
				path + "/up.ppm",		//up		//py
				path + "/down.ppm",		//down		//ny
				path + "/back.ppm",		//back		//pz
				path + "/front.ppm"		//front		//nz
			};

	initVertices(10.0f);	//parameter is the distance from the center
	initCubeMap();
	loadCubeMap(faces);		
}

Skybox::~Skybox(){
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Skybox::setPos(glm::vec3 pos) {
	toWorld[3] = glm::vec4(pos.x, pos.y, pos.z, 1.0f);
}

void Skybox::draw(glm::mat4 projection, glm::mat4 headPose, GLint shader) {
	glActiveTexture(GL_TEXTURE0);
	glDepthMask(GL_FALSE);
	glUseProgram(shader);

	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &toWorld[0][0]);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

void Skybox::loadCubeMap(std::vector<std::string> faces) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrcChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		helperPrint(i);
		unsigned char * data = stbi_load(faces[i].c_str(), &width, &height, &nrcChannels, 0);
		
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "\tCubemap texture failed to load at path: " << faces[i].c_str() << "\n";
		}
		stbi_image_free(data);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::initVertices(float p) {
	vertices = {
		//right
		{ p, -p, -p },{ p, -p,  p },{ p,  p,  p },
		{ p,  -p,  -p },{ p,  p, p },{ p, p, -p },
		//left
		{-p, -p,  p}, {-p, -p, -p}, {-p,  p, -p},
		{-p,  p, -p}, {-p,  p,  p}, {-p, -p,  p},
		//front
		{-p, -p,  p}, {-p,  p,  p}, {p,  p,  p},
		{p,  p,  p},  {p, -p,  p},  {-p, -p,  p},
		//top
		{ -p, p, -p },{ p, p, -p },{ p, p, p },
		{ p, p, p },{ -p, p, p },{ -p, p, -p },
		//bottom
		{-p, -p, -p}, {-p, -p,  p}, {p, -p, -p},
		{p, -p, -p}, {-p, -p,  p}, {p, -p,  p},
		//back
		{-p,  p, -p},{ -p, -p, -p },{ p, -p, -p },
		{ p, -p, -p },{ p,  p, -p },{ -p,  p, -p },
		//front
		{ -p, -p,  p },{ -p,  p,  p },{ p,  p,  p },
		{ p,  p,  p },{ p, -p,  p },{ -p, -p,  p }
	};
}

void Skybox::initCubeMap() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Skybox::helperPrint(int i) {
	switch (i) {
	case 0:
		std::cout << "\tLoading right texture...\n";
		break;
	case 1:
		std::cout << "\tLoading left texture...\n";
		break;
	case 2:
		std::cout << "\tLoading top texture...\n";
		break;
	case 3:
		std::cout << "\tLoading bottom texture...\n";
		break;
	case 4:
		std::cout << "\tLoading back texture...\n";
		break;
	case 5:
		std::cout << "\tLoading front texture...\n";
		break;
	default:
		std::cout << "\tERROR - invalid texture index\n";
		break;
	}
}

unsigned int Skybox::getTextureID() { return textureID; }