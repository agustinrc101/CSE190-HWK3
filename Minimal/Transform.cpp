#include "Transform.h"

Transform::Transform(Model * m, GLint s, glm::vec3 c){
	model = m;
	shader = s;
	color = c;
}

Transform::~Transform(){
	//Delete model if it exists
	if (model != NULL) delete(model);
	
	//Delete components
	for (int i = 0; i < components.size(); i++)	delete &components[i];
}

void Transform::draw(glm::mat4 headPose, glm::mat4 projection) {
	if (model != NULL) model->draw(projection, headPose, shader, color, toWorld);
}

void Transform::update(double deltaTime) {
	for (int i = 0; i < components.size(); i++) ((Component *)(&components[i]))->update(deltaTime);
}