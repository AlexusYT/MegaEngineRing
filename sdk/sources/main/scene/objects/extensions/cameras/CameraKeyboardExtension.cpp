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
// Created by alexus on 29.02.24.
//


#include <EngineSDK/main/scene/objects/extensions/cameras/CameraKeyboardExtension.h>

#include <cmath>
#include <glm/detail/func_geometric.inl>
#include <glm/detail/func_trigonometric.inl>
#include <glm/vec3.hpp>

#include "EngineSDK/main/scene/objects/SceneObject.h"

namespace mer::sdk::main {

void CameraKeyboardExtension::onKeyStateChanged(const utils::KeyboardKey pKey, const bool pPressed,
												const utils::ModifierKeys & /*pMods*/) {
	if (pKey == propertyForwardKey) fwdPressed = pPressed;
	if (pKey == propertyBackwardKey) bwdPressed = pPressed;
	if (pKey == propertyStrafeLeftKey) strafeLeftPressed = pPressed;
	if (pKey == propertyStrafeRightKey) strafeRightPressed = pPressed;
}

void CameraKeyboardExtension::onRender() {
	if (fwdPressed || bwdPressed || strafeLeftPressed || strafeRightPressed) {
		const auto &objectSelf = getObject();
		const float angleYRad = glm::radians(propertyAngle.y);
		const float angleYRight = angleYRad - 3.14f / 2.0f;
		const glm::vec3 right =
			glm::normalize(glm::vec3(std::sin(angleYRight), 0.0f, std::cos(angleYRight))) * propertySpeed;

		const glm::vec3 fwdBwd =
			glm::normalize(glm::vec3(std::sin(angleYRad), 0.0f, std::cos(angleYRad))) * propertySpeed;
		auto position = objectSelf->getPosition();
		if (fwdPressed) { position += fwdBwd; }
		if (bwdPressed) { position -= fwdBwd; }

		if (strafeRightPressed) position += right;
		if (strafeLeftPressed) position -= right;
		objectSelf->setPosition(position);
	}
}
} // namespace mer::sdk::main