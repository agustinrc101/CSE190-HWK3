#include "Transform.h"



Transform::Transform(Model * m, GLint s, glm::vec3 c){
	model = m;
	shader = s;
	color = c;
}


Transform::~Transform(){
	if (model != NULL) delete(model);
}

void Transform::draw(glm::mat4 headPose, glm::mat4 projection) {
	if (model != NULL) model->draw(projection, headPose, shader, color, toWorld);
}

void Transform::update(double deltaTime) {

}



