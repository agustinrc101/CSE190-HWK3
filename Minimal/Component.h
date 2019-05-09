#ifndef COMPONENT_H
#define COMPONENT_H

class Transform;

class Component {
public:
	Component() {}
	virtual ~Component() {}
	virtual void update(double deltaTime) = 0;

	void setTransform(Transform * p) { transform = p; }

protected:
	Transform * transform;
};

#endif