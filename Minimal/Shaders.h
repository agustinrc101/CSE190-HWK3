#pragma once
#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>

class Shaders{
public:
	//Setters
	static void setColorShader(GLint s) {colorShader = s;}
	static void setTextureShader(GLint s) { textureShader = s;}
	static void setSkyboxShader(GLint s) { skyboxShader = s; }
	static void setRenderedTextureShader(GLint s) { renderedTextureShader = s; }
	static void setLCDisplayShader(GLint s) { LCDisplayShader = s; }

	//Getters
	static GLint getColorShader() { return colorShader; }
	static GLint getTextureShader() { return textureShader; }
	static GLint getSkyboxShader() { return skyboxShader; }
	static GLint getRenderedTextureShader() { return renderedTextureShader; }
	static GLint getLCDisplayShader() { return LCDisplayShader; }

	//delete shaders
	static void deleteShaders(){
		glDeleteProgram(colorShader);
		glDeleteProgram(textureShader);
		glDeleteProgram(skyboxShader);
		glDeleteProgram(renderedTextureShader);
		glDeleteProgram(LCDisplayShader);
	}

protected:
	static GLint colorShader; 
	static GLint textureShader;
	static GLint skyboxShader;
	static GLint renderedTextureShader;
	static GLint LCDisplayShader;
};

#endif