//
// Created by alexus on 06.12.23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "CameraFwd.h"
#include "OpenGLFwd.h"

class Camera {
	glm::mat4 viewMatrix{};
	glm::vec3 direction{};
	glm::vec3 position{};
	float fov;

	OpenGL* render;

	explicit Camera(OpenGL* glRender) : render(glRender) {}

public:
	static CameraUPtr create(OpenGL* glRender) { return CameraUPtr(new (std::nothrow) Camera(glRender)); }

	[[nodiscard]] const glm::mat4 &getViewMatrix() const { return viewMatrix; }

	[[nodiscard]] const glm::vec3 &getDirection() const { return direction; }

	void setDirection(const glm::vec3 &newDirection) {
		this->direction = newDirection;
		updateViewMatrix();
	}

	[[nodiscard]] const glm::vec3 &getPosition() const { return position; }

	void setPosition(const glm::vec3 &newPosition) {
		this->position = newPosition;
		updateViewMatrix();
	}

	[[nodiscard]] float getFov() const { return fov; }

	void setFov(float newFov);

private:
	void updateViewMatrix();
};


#endif //CAMERA_H
