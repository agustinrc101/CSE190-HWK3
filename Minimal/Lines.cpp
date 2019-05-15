#include "Lines.h"



Lines::Lines(){ }


Lines::~Lines(){
	indices.clear();
	vertices.clear();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void Lines::draw(glm::mat4 projection, glm::mat4 headPose, GLint shader, glm::mat4 M, glm::vec3 rgb) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);

	//passes vertices
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//passes indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	//BEGIN DRAW
	glm::mat4 m = M * toWorld;

	glUseProgram(shader);

	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &m[0][0]);
	glUniform3f(glGetUniformLocation(shader, "rgb"), rgb.x, rgb.y, rgb.z);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINE_STRIP, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void Lines::updateEyePos(glm::vec3 pos) {
	if (vertices.size() > 1)
		vertices[0] = pos;
}

void Lines::addVertex(glm::vec3 v) {
	if (vertices.empty()) {
		vertices.push_back(v);
		return;
	}
	
	vertices.push_back(v);
	indices.push_back(0);
	indices.push_back((GLuint)vertices.size() - 1);
}