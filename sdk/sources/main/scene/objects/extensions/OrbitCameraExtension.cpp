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
// Created by alexus on 24.03.24.
//

#include "EngineSDK/main/scene/objects/extensions/cameras/OrbitCameraExtension.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/polar_coordinates.hpp>

#include "EngineSDK/main/scene/Scene.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"

namespace mer::sdk::main {


utils::ReportMessagePtr OrbitCameraExtension::onInit() { return nullptr; }

void OrbitCameraExtension::onWindowSizeChanged(const int pWidth, const int pHeight) {
	if (pHeight <= 0) return;
	setAspect(static_cast<float>(pWidth) / static_cast<float>(pHeight));
}

void OrbitCameraExtension::projectionMatrixChanged(const glm::mat4 & /*pNewMatrix*/) { updateMatrix(); }

void OrbitCameraExtension::updateMatrix() {

	float dist = 25;
	glm::vec3 globalUp{0, 1, 0};
	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(angle.y), glm::radians(angle.x), 0.0f);

	auto position = targetPosition + glm::vec3(rotationMatrix * glm::vec4(0, 0, dist, 0.0f));
	//auto look = glm::normalize(targetPosition - position);
	auto up = glm::vec3(rotationMatrix * glm::vec4(globalUp, 0.0f));
	/*auto right = glm::cross(look, up);
	auto V = glm::lookAt(position, target, up);*/
	setMatrix(getProjMatrix() * lookAt(position, targetPosition, up));
}

void OrbitCameraExtension::getProperties(ExtensionProperties &pProperties) {
	pProperties.emplace_back(this, "View Angle", "", &OrbitCameraExtension::getAngle, &OrbitCameraExtension::setAngle);
	getProjectionProperties(pProperties);
}
} // namespace mer::sdk::main