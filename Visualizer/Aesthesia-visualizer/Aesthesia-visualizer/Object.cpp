#include "pch.h"
#include "Object.h"

Object::Object(glm::vec3 aDimensions, glm::vec3 aLocationFromJoint, glm::vec3 aRotation, Object* aParent, glm::vec3 aJointWithParent) {
	this->dimensions = aDimensions;
	this->locationFromJoint = aLocationFromJoint;
	this->parent = aParent;
	this->jointWithParent = aJointWithParent;
	this->rotation = aRotation;
};

glm::vec3 Object::getDimensions() {
	return this->dimensions;
};

Object* Object::getParent() {
	return this->parent;
};

glm::vec3 Object::getJointWithParent() {
	return this->jointWithParent;
};

glm::vec3 Object::getLocation() {
	return this->locationFromJoint;
};

void Object::pushChildren(Object* aChild) {
	children.push_back(aChild);
};


std::vector<Object *> Object::getChildren() {
	return this->children;
}

glm::mat4 Object::getRotation() {
	glm::mat4 rotation = glm::rotate(glm::mat4(), this->rotation.x, glm::vec3(1.0, 0.0, 0.0));
	rotation = glm::rotate(rotation, this->rotation.y, glm::vec3(0.0, 1.0, 0.0));
	return glm::rotate(rotation, this->rotation.z, glm::vec3(0.0, 0.0, 1.0));
}

glm::mat4 Object::getTranformation() {
	glm::mat4 transformation = this->getRotation();
	transformation = glm::translate(transformation, this->getLocation());
	transformation = glm::scale(transformation, this->getDimensions());
	return transformation;
}

void Object::rotate(float angle) {
	rotation = glm::vec3(rotation.x, rotation.y, rotation.z + angle);
}