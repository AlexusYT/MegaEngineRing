//
// Created by alexus on 06.12.23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "CameraFwd.h"
#include "OpenGLRenderFwd.h"

class Camera {
	glm::mat4 viewMatrix{};
	glm::vec3 direction{};
	glm::vec3 position{};
	OpenGLRender* render;

	explicit Camera(OpenGLRender* glRender) : render(glRender) {}

public:
	static CameraUPtr create(OpenGLRender* glRender) { return CameraUPtr(new (std::nothrow) Camera(glRender)); }

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

private:
	void updateViewMatrix();
};


#endif //CAMERA_H
