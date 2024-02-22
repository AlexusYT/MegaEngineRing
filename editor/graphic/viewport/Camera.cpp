//
// Created by alexus on 06.12.23.
//

#include "Camera.h"

#include <glm/glm.hpp>

#include "OpenGL.h"

void Camera::setFov(const float newFov) {
	this->fov = newFov;
	render->updateProjectMatrix();
}

void Camera::updateViewMatrix() {
	viewMatrix = glm::lookAt(position, position + direction, {0, 1, 0});
	render->updateViewProjMatrix();
}