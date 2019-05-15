#include "Cave.h"
#include "Quad.h"
#include "Skybox.h"
#include "TexturedCube.h"
#include "Definitions.h"
#include "Shaders.h"
#include "Lines.h"

//Rendering specs
#define TEX_WIDTH 1024
#define TEX_HEIGHT 1024
#define NEAR_PLANE 0.01f
#define FAR_PLANE 10000.0f

//Planes
#define CAVE_SIZE 2.4f
Quad * planeL;
Quad * planeR;
Quad * planeB;

//Plane variables
glm::mat4 rotation = glm::mat4(1);
glm::vec3 corners[10];	//0-2 planeL, 3-5 planeR, 6-8 planeB

//Debug Lines
Lines * lines;

//Skybox
Skybox * skyboxL;
Skybox * skyboxR;

//Cube
TexturedCube * cube;

//Cube variables
#define CUBE_SCALE glm::vec3(0.3f)
#define CUBE_POSITION glm::vec3(0, -0.1f, -1.0f)
glm::vec3 cubePosition;
glm::vec3 cubeScaleFactor;

//=========================//
//======METHODS BEGIN======//
//=========================//

Cave::~Cave() {
	//Delete planes
	delete(planeL);
	delete(planeR);
	delete(planeB);
	//Delete lines
	delete(lines);
	//Delete skyboxes
	delete(skyboxL);
	delete(skyboxR);
	//Delete cube
	delete(cube);
	//Deallocate OpenGL buffers, textures, etc
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &renderedTexture);
	glDeleteRenderbuffers(1, &rboId);
}

Cave::Cave(){
	initPlanes();
	initCorners();
	initLines();
	initSkybox();
	initObjects();
	initFrameBuffer();
}

void Cave::initPlanes() {
	planeL = new Quad(CAVE_SIZE);
	planeR = new Quad(CAVE_SIZE);
	planeB = new Quad(CAVE_SIZE);

	//Rotate
	float rad = MATH_PI / 180.0f;
	planeL->toWorld = planeL->toWorld * glm::rotate(glm::mat4(1.0f), 90.0f * rad, glm::vec3(0, 1, 0));
	planeB->toWorld = planeB->toWorld * glm::rotate(glm::mat4(1.0f), -90.0f * rad, glm::vec3(1, 0, 0));

	//Translate
	float half = CAVE_SIZE / 2.0f;
	planeL->toWorld[3] = glm::vec4(-half, 0, 0, 1);
	planeR->toWorld[3] = glm::vec4(0, 0, -half, 1);
	planeB->toWorld[3] = glm::vec4(0, -half, 0, 1);

	//Rotate "this" obj
	rotation = rotation * glm::rotate(glm::mat4(1.0f), -45.0f * rad, glm::vec3(0, 1, 0));

}

void Cave::initCorners() {
	float half = CAVE_SIZE / 2.0f;

	corners[0] = glm::vec3(-half, half, half); corners[1] = glm::vec3(-half, -half, half); corners[2] = glm::vec3(-half, -half, -half);
	corners[3] = glm::vec3(-half, half, -half); corners[4] = glm::vec3(-half, -half, -half); corners[5] = glm::vec3(half, -half, -half);
	corners[6] = glm::vec3(-half, -half, -half); corners[7] = glm::vec3(-half, -half, half); corners[8] = glm::vec3(half, -half, half);
	corners[9] = glm::vec3(half, half, -half);

	for (int i = 0; i < 10; i++) {
		glm::mat4 a = glm::mat4(1.0f);
		a[3] = glm::vec4(corners[i].x, corners[i].y, corners[i].z, 1.0f);
		a = rotation * a;
		corners[i] = a[3];
	}
}

void Cave::initLines() {
	lines = new Lines();
	
	//add eyepos placeholder
	lines->addVertex(glm::vec3(0));

	//add corners
	for (int i = 0; i < 10; i++) lines->addVertex(corners[i]);

}

void Cave::initSkybox() {
	skyboxL = new Skybox(TEXTURE_SKYBOX_LEFT);
	skyboxR = new Skybox(TEXTURE_SKYBOX_RIGHT);
}

void Cave::initObjects() {
	cube = new TexturedCube(TEXTURE_CUBE_STEAM);
	cubePosition = CUBE_POSITION;
	cubeScaleFactor = CUBE_SCALE;
}

void Cave::initFrameBuffer() {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	initRenderedTexture();
}

void Cave::initRenderedTexture() {
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	initDepthBuffer();

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "=== SOMETHING WENT WRONG ===" << std::endl;
}

void Cave::initDepthBuffer() {
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEX_WIDTH, TEX_HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
}

void Cave::update(double deltaTime) {

}

void Cave::drawDebugLines(glm::mat4 headPose, glm::mat4 projection, glm::vec3 eyepos, int eye) {
	lines->updateEyePos(eyepos);
	if (eye == 0)	lines->draw(projection, headPose, Shaders::getColorShader(), glm::mat4(1), glm::vec3(COLOR_GREEN));
	else			lines->draw(projection, headPose, Shaders::getColorShader(), glm::mat4(1), glm::vec3(COLOR_RED));
}

void Cave::draw(glm::mat4 headPose, glm::mat4 projection, int eye) {
	//Set rotation matrix
	glm::mat4 m = glm::mat4(1.0f) * rotation;

	//LEFT PLANE
	{
		//Draw to framebuffer 
		doFrameBuffer(generateProjection(eye, 0), eye);

		//Draw texture for LEFT plane
		glBindFramebuffer(GL_FRAMEBUFFER, 1);
		glViewport((GLint)viewport[eye].x, (GLint)viewport[eye].y, (GLsizei)viewport[eye].z, (GLsizei)viewport[eye].w);
		//planeL->draw(projection, headPose, Shaders::getRenderedTextureShader(), m, renderedTexture, eyePos[eye]);
		planeL->draw(projection, headPose, Shaders::getLCDisplayShader(), m, renderedTexture, getDisplayNormal(0), eyePos[eye]);
		glClearDepth(rboId);
	}
	//RIGHT PLANE
	{
		//Draw to framebuffer 
		doFrameBuffer(generateProjection(eye, 1), eye);

		//Draw texture for RIGHT plane
		glBindFramebuffer(GL_FRAMEBUFFER, 1);
		glViewport((GLint)viewport[eye].x, (GLint)viewport[eye].y, (GLsizei)viewport[eye].z, (GLsizei)viewport[eye].w);
		//planeR->draw(projection, headPose, Shaders::getRenderedTextureShader(), m, renderedTexture, eyePos[eye]);
		planeR->draw(projection, headPose, Shaders::getLCDisplayShader(), m, renderedTexture, getDisplayNormal(1), eyePos[eye]);
		glClearDepth(rboId);
	}
	//BOTTOM PLANE
	{
		//Draw to framebuffer 
		doFrameBuffer(generateProjection(eye, 2), eye);

		//Draw texture for BOTTOM plane
		glBindFramebuffer(GL_FRAMEBUFFER, 1);
		glViewport((GLint)viewport[eye].x, (GLint)viewport[eye].y, (GLsizei)viewport[eye].z, (GLsizei)viewport[eye].w);
		//planeB->draw(projection, headPose, Shaders::getRenderedTextureShader(), m, renderedTexture, eyePos[eye]);
		planeB->draw(projection, headPose, Shaders::getLCDisplayShader(), m, renderedTexture, getDisplayNormal(2), eyePos[eye]);
		glClearDepth(rboId);
	}
}

void Cave::doFrameBuffer(glm::mat4 projection, int eye) {
	//Bind Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, TEX_WIDTH, TEX_HEIGHT);
	glFlush();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw Cube
	cube->toWorld = glm::translate(glm::mat4(1.0f), cubePosition) * glm::scale(glm::mat4(1.0f), cubeScaleFactor);
	cube->draw(projection, glm::mat4(1.0f), Shaders::getTextureShader(), glm::mat4(1.0f));

	//Draw Skybox
	if (eye == 0)	skyboxL->draw(projection, glm::mat4(1.0f), Shaders::getSkyboxShader());
	else			skyboxR->draw(projection, glm::mat4(1.0f), Shaders::getSkyboxShader());
}

glm::mat4 Cave::generateProjection(int eye, int plane) {
	int cornerIndex = 3 * plane;

	//Corner variables
	glm::vec3 pa = corners[cornerIndex + 1];
	glm::vec3 pb = corners[cornerIndex + 2];
	glm::vec3 pc = corners[cornerIndex + 0];
	glm::vec3 pe = eyePos[eye];

	//Init the rest of the variables
	glm::vec3 vr, vu, vn, va, vb, vc;
	float l, r, b, t, d;
	glm::mat4 M_T = glm::mat4(1.0f);
	glm::mat4 P = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);

	//Calculate right, up, and normal vectors
	vr = pb - pa;
	vr = glm::normalize(vr);	//right

	vu = pc - pa;
	vu = glm::normalize(vu);	//up

	vn = glm::cross(vr, vu);
	vn = glm::normalize(vn);	//normal

	//Get vectors from corner to eye
	va = pa - pe;
	vb = pb - pe;
	vc = pc - pe;

	//Calculate distance from eye to screen plane
	d = -(glm::dot(vn, va));

	//P
	l = glm::dot(vr, va) * NEAR_PLANE / d;
	r = glm::dot(vr, vb) * NEAR_PLANE / d;
	b = glm::dot(vu, va) * NEAR_PLANE / d;
	t = glm::dot(vu, vc) * NEAR_PLANE / d;

	P = glm::frustum(l, r, b, t, NEAR_PLANE, FAR_PLANE);

	//M_T (manually set all elements)
	M_T[0][0] = vr.x; M_T[1][0] = vr.y; M_T[2][0] = vr.z; M_T[3][0] = 0;
	M_T[0][1] = vu.x; M_T[1][1] = vu.y; M_T[2][1] = vu.z; M_T[3][1] = 0;
	M_T[0][2] = vn.x; M_T[1][2] = vn.y; M_T[2][2] = vn.z; M_T[3][2] = 0;
	M_T[0][3] = 0;    M_T[1][3] = 0;    M_T[2][3] = 0;    M_T[3][3] = 1;

	//T
	T[3] = glm::vec4(-pe, 1.0f);

	//Generalized perspective projection
	return P * M_T * T;
}

glm::vec3 Cave::getDisplayNormal(int plane) {
	int cornerIndex = 3 * plane;

	//Corner variables
	glm::vec3 pa = corners[cornerIndex + 1];
	glm::vec3 pb = corners[cornerIndex + 2];
	glm::vec3 pc = corners[cornerIndex + 0];

	//Init the rest of the variables
	glm::vec3 vr, vu, vn;

	//Calculate right, up, and normal vectors
	vr = pb - pa;
	vr = glm::normalize(vr);	//right

	vu = pc - pa;
	vu = glm::normalize(vu);	//up

	vn = glm::cross(vr, vu);
	return glm::normalize(vn);	//normal
}

//Setters
void Cave::setCubeScale(float s) { cubeScaleFactor = glm::vec3(cubeScaleFactor.x + s); }

void Cave::resetCubeScale() { cubeScaleFactor = CUBE_SCALE; }

void Cave::moveCube(glm::vec3 t) { cubePosition += glm::vec3(t.x, t.y, -t.z); }

void Cave::resetCubePosition() { cubePosition = CUBE_POSITION; }
