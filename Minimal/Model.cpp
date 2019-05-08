#include "Model.h"

Model::Model(const char * path){
	parse(path);
	initBuffers();
}

Model::Model(const Model& model) {
	indices = model.indices;
	vertices = model.vertices;
	normals = model.normals;
	VAO = model.VAO;
	VBO = model.VBO;
	VBO2 = model.VBO2;
	EBO = model.EBO;
}

Model::~Model(){
	indices.clear();
	vertices.clear();
	normals.clear();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void Model::draw(glm::mat4 projection, glm::mat4 headPose, GLint shader, glm::vec3 rgb, glm::mat4 M) {
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &M[0][0]);

	glUniform3f(glGetUniformLocation(shader, "rgb"), rgb.x, rgb.y, rgb.z);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Model::parse(const char * filepath) {
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE * file;		        // File pointer
	float x, y, z;		        // vertex coordinates
	float r, g, b;			    // vertex colors
	int i1, i2, i3, i4, i5, i6;	// Indeces for triangles
	int c1, c2;			        // characters read from file

	std::cout << "    Reading " << filepath << "\n";

	file = fopen(filepath, "rb");



	if (file == NULL) {
		std::cerr << "error loading file" << std::endl;
		exit(-1);
	}

	c1 = fgetc(file);
	c2 = fgetc(file);

	while (c1 != EOF && c2 != EOF) {
		if (c1 == '#' || c2 == '#') {
			while (c1 != '\n' && c1 != EOF)
				c1 = fgetc(file);
		}
		else if (c1 == 'v' && c2 == ' ') {
			fscanf(file, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(file, "%f %f %f", &x, &y, &z);
			normals.push_back(glm::vec3(x, y, z));
		}
		else if (c1 == 'f' || c2 == ' ') {
			fscanf(file, "%d//%d %d//%d %d//%d", &i1, &i2, &i3, &i4, &i5, &i6);
			indices.push_back(i1 - 1);
			indices.push_back(i3 - 1);
			indices.push_back(i5 - 1);
		}

		if (c1 != EOF && c2 != EOF) {
			c1 = fgetc(file);
			c2 = fgetc(file);
		}
	}

	fclose(file);

	std::cout << "\t" << filepath << ", vertices: " << vertices.size() << ", normals: " << normals.size() << ", faces: " << (indices.size() / 3) << std::endl;
}

void Model::initBuffers() {
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

	//passes normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &(normals[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
