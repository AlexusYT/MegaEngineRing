// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 24.02.24.
//

#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <math.h>

#include "EngineUtils/utils/Logger.h"

namespace mer::sdk::main {
void Camera::onKeyStateChanged(const utils::KeyboardKey pKey, const bool pPressed, utils::ModifierKeys /*pMods*/) {
	if (pKey == utils::KeyboardKey::KEY_W) {
		fwdPressed = pPressed;
	} else if (pKey == utils::KeyboardKey::KEY_S)
		bwdPressed = pPressed;
	else if (pKey == utils::KeyboardKey::KEY_A)
		strafeLeftPressed = pPressed;
	else if (pKey == utils::KeyboardKey::KEY_D)
		strafeRightPressed = pPressed;
}

void Camera::update() {
	if (fwdPressed || bwdPressed || strafeLeftPressed || strafeRightPressed || liftDownPressed || liftUpPressed) {
		const float speed = 1.4f; // 3 units / second
		const float angleYRad = glm::radians(this->angle.y);
		const float angleYRight = angleYRad - 3.14f / 2.0f;
		const glm::vec3 right = glm::normalize(glm::vec3(std::sin(angleYRight), 0.0L, std::cos(angleYRight))) * speed;

		const glm::vec3 fwdBwd = glm::normalize(glm::vec3(std::sin(angleYRad), 0, std::cos(angleYRad))) * speed;

		if (fwdPressed) { position += fwdBwd; }
		if (bwdPressed) { position -= fwdBwd; }

		if (strafeRightPressed) position += right;
		if (strafeLeftPressed) position -= right;

		if (liftUpPressed) position.y += speed;
		if (liftDownPressed) position.y -= speed;
		updateMatrix();
	}
}

void Camera::setAngle(const glm::vec2 &pNewAngle) {
	if (angle == pNewAngle) return;
	glm::vec2 angleTmp = pNewAngle;
	constexpr float delta = 0.0001f;
	if (angleTmp.x > 90.0f - delta) angleTmp.x = 90.0f - delta;
	if (angleTmp.x < -90.0f + delta) angleTmp.x = -90.0f + delta;
	if (angleTmp.y > 360) angleTmp.y -= 360;
	if (angleTmp.y < 0) angleTmp.y += 360;
	angle = angleTmp;
	direction = euclidean(radians(angleTmp));
	updateMatrix();
}

void Camera::setViewportAspect(const float pAspect) {
	aspect = pAspect;
	updateMatrix();
}

void Camera::updateMatrix() {
	matrix = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f) *
			 lookAt(position, position + direction, {0, 1, 0});
	onMatrixChangedSignal(matrix);
}
} // namespace mer::sdk::main