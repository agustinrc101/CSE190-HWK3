#ifndef CAVE_H
#define CAVE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Cave{
public:
	Cave();
	~Cave();

	void draw(glm::mat4 headPose, glm::mat4 projection, int eye);
	void drawDebugLines(glm::mat4 headPose, glm::mat4 projection, glm::vec3 eyepos, int eye);
	void update(double deltaTime);

	//Setters
	void setEyePos(glm::vec3 pos, int eye) { eyePos[eye] = pos; }
	void setViewport(glm::vec4 vp, int eye) { viewport[eye] = vp; }
	void setCubeScale(float s);
	void resetCubeScale();
	void moveCube(glm::vec3 t);
	void resetCubePosition();

private:
	glm::mat4 toWorld = glm::mat4(1.0f);
	GLuint FBO, rboId, renderedTexture;
	glm::vec3 eyePos[2] = { glm::vec3(1.0f), glm::vec3(1.0f) };
	glm::vec4 viewport[2] = { glm::vec4(1.0f), glm::vec4(1.0f) };

	void initPlanes();
	void initCorners();
	void initLines();
	void initSkybox();
	void initObjects();
	void initFrameBuffer();
	void initRenderedTexture();
	void initDepthBuffer();

	void doFrameBuffer(glm::mat4 projection, int eye);
	glm::mat4 generateProjection(int eye, int plane);
	glm::vec3 getDisplayNormal(int plane);
};

#endif

