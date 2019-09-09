// Original camera code by SonarSystems (this has been rewritten to fit the parameters of the assignement)
// https://github.com/SonarSystems/Modern-OpenGL-Tutorials/tree/master/%5BGETTING%20STARTED%5D/%5B6%5D%20Camera

#include "pch.h"
#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
	GLfloat velocity = this->movementSpeed * deltaTime;
	if (FORWARD == direction) {
		this->position += this->front * velocity;
	}
	if (BACKWARD == direction) {
		this->position -= this->front * velocity;
	}
	if (LEFT == direction) {
		this->position = glm::rotate(position, velocity, glm::vec3(0.0, 1.0, 0.0));
	}
	if (RIGHT == direction) {
		this->position = glm::rotate(position, velocity, glm::vec3(0.0, -1.0, 0.0));
	}
}

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLint mouseButtonPressed) {

	xOffset *= this->mouseSensitivity;
	yOffset *= this->mouseSensitivity;

	if (mouseButtonPressed == 0) {
		this->position += this->front * yOffset;
	}
	if (mouseButtonPressed == 1) {
		this->front = glm::rotate(front, glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f));
		this->up = glm::rotate(up, glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f));
		this->right = glm::rotate(right, glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (mouseButtonPressed == 2) {
		this->front = glm::rotate(front, glm::radians(xOffset), glm::vec3(0.0f, 0.0f, 1.0f));
		this->up = glm::rotate(up, glm::radians(xOffset), glm::vec3(0.0f, 0.0f, 1.0f));
		this->right = glm::rotate(right, glm::radians(xOffset), glm::vec3(0.0f, 0.0f, 1.0f));
	}

}

void Camera::ProcessMouseScroll(GLfloat yOffset) {
	if (this->zoom >= 1.0f && this->zoom <= 45.0f) {
		this->zoom -= yOffset / 45.0f;
	}
	if (this->zoom <= 1.0f) {
		this->zoom = 1.0f;
	}
	if (this->zoom >= 45.0f) {
		this->zoom = 45.0f;
	}
}

GLfloat Camera::GetZoom() {
	return this->zoom;
}

glm::vec3 Camera::getPosition() {
	return this->position;
}


void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
};

void Camera::resetCamera() {
	this->position = glm::vec3(0.0f, 30.0f, 0.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->yaw = -90.0f;
	this->pitch = -89.0f;
	this->updateCameraVectors();
}