#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec4 FragPos;
in vec3 eyepos;

uniform sampler2D texture_diffuse1;

void main(){
	//Declare vars
	vec3 fragPos = vec3(FragPos.x, FragPos.y, FragPos.z);
	vec3 direction = normalize(eyepos - fragPos);
	float angle = 0.0;
	float brightness = 1.0;

	//Get angle
	angle = dot(normal, direction) / (dot(length(normal), length(direction)));
	angle = acos(angle);
	angle = degrees(angle);
	
	//Angle checks
	if(angle < 0.0)
		angle = 360 + angle;

	angle = mod(angle, 91.0);

	//Calculate brightness
	brightness = 1.0 - (angle / 90.0);
	
	//Color
	FragColor = texture(texture_diffuse1, TexCoords);
	FragColor = vec4(FragColor.r * brightness, FragColor.g * brightness, FragColor.b * brightness, 1);
}