#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 pos;

uniform sampler2D texture_diffuse1;
uniform vec3 eye;

void main(){
	vec4 base = texture(texture_diffuse1, TexCoords);
	
	//vec2 center = vec2(0.5, 0.5);
	//float d = distance(TexCoords.xy, center);
	//base *= smoothstep(0.8, 0.01, d);

	FragColor = base;
}