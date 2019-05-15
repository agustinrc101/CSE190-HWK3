#include "Skybox.h"

#include <GL/glew.h>
#include <iostream>
#include <vector>


Skybox::Skybox(const std::string dir) : TexturedCube(dir){ }

Skybox::~Skybox(){ }

void Skybox::draw(unsigned skyboxShader, const glm::mat4& p, const glm::mat4& v) {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthMask(GL_FALSE);

	glUseProgram(skyboxShader);
	// ... set view and projection matrix
	uProjection = glGetUniformLocation(skyboxShader, "projection");
	uView = glGetUniformLocation(skyboxShader, "view");

	glm::mat4 modelview = glm::mat4(glm::mat3(v)) * toWorld;

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &p[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &modelview[0][0]);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);



	glDepthMask(GL_TRUE);
	glCullFace(GL_FRONT);
}
