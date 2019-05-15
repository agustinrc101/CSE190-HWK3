#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 normal;
out vec4 FragPos;
out vec3 eyepos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 planeNormal;
uniform vec3 eyePos;

void main(){
	normal = planeNormal;
	FragPos = model * vec4(aPos, 1.0);
	eyepos = eyePos;

	TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}