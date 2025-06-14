//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 24.03.24.
//

#include "EngineSDK/extensions/cameras/OrbitCameraExtension.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/polar_coordinates.hpp>

#include "EngineSDK/extensions/MainObjectExtension.h"
#include "EngineSDK/scene/Scene.h"
#include "EngineSDK/scene/objects/SceneObject.h"

namespace mer::sdk {
OrbitCameraExtension::OrbitCameraExtension()
	: propertyMatrix(this, "Matrix"),
	  propertyAngle(this, "Angle"),
	  propertyTargetPosition(this, "TargetPosition"),
	  propertyDistance(this, "Distance"),
	  propertyPosition(this, "Test"),
	  propertyUp(this, "Up") {
	propertyMatrix.setValue(glm::mat4(1));
	propertyDistance = 3.0f;
	auto updateMatrixSlot = hide(sigc::mem_fun(*this, &OrbitCameraExtension::updateMatrix));
	propertyTargetPosition.getEvent().connect(updateMatrixSlot);
	propertyAngle.getEvent().connect(updateMatrixSlot);
	propertyDistance.getEvent().connect(updateMatrixSlot);
}

ExtensionProperty<glm::vec3> &OrbitCameraExtension::getPosition() {
	//const auto &objectSelf = getObject();
	return propertyPosition;
}

ReportMessagePtr OrbitCameraExtension::onInit() { return nullptr; }

void OrbitCameraExtension::onWindowSizeChanged(const int pWidth, const int pHeight) {
	if (pHeight <= 0) return;
	propertyAspect = static_cast<float>(pWidth) / static_cast<float>(pHeight);
}

void OrbitCameraExtension::projectionMatrixChanged(const glm::mat4 & /*pNewMatrix*/) { updateMatrix(); }

void OrbitCameraExtension::updateMatrix() {
	glm::vec3 globalUp{0, 1, 0};
	auto &angle = propertyAngle.getValue();
	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(angle.y), glm::radians(angle.x), 0.0f);

	auto position =
		propertyTargetPosition + glm::vec3(rotationMatrix * glm::vec4(0, 0, propertyDistance.getValue(), 0.0f));
	getPosition() = position;
	//auto look = glm::normalize(targetPosition - position);
	auto up = glm::vec3(rotationMatrix * glm::vec4(globalUp, 0.0f));
	/*auto right = glm::cross(look, up);
	auto V = glm::lookAt(position, target, up);*/
	propertyMatrix = getProjMatrix() * lookAt(position, propertyTargetPosition.getValue(), up);
	propertyUp = up;
}
} // namespace mer::sdk
