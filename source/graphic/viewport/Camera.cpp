//
// Created by alexus on 06.12.23.
//

#include "Camera.h"

#include <glm/glm.hpp>

void Camera::updateViewMatrix() { viewMatrix = glm::lookAt(position, position + direction, {0, 1, 0}); }