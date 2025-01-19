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
// Created by alexus on 28.02.24.
//


#include <EngineSDK/main/scene/objects/extensions/cameras/CameraExtension.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <sigc++/adaptors/hide.h>

#include "EngineSDK/main/scene/Scene.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/MainObjectExtension.h"

namespace mer::sdk::main {

CameraExtension::CameraExtension()
	: propertyMatrix(this, "Matrix"), propertyDirection(this, "Direction"), propertyAngle(this, "Angle") {
	propertyDirection.getEvent().connect(sigc::hide(sigc::mem_fun(*this, &CameraExtension::updateMatrix)));
	propertyDirection = euclidean(radians(propertyAngle.getValue()));
	propertyAngle.getReturnOverride().connect([this](const glm::vec2 &pAngle) {
		if (propertyAngle == pAngle) return pAngle;
		glm::vec2 angleTmp = pAngle;
		constexpr float delta = 0.001f;
		if (angleTmp.x > 90.0f - delta) angleTmp.x = 90.0f - delta;
		if (angleTmp.x < -90.0f + delta) angleTmp.x = -90.0f + delta;
		if (angleTmp.y > 360) angleTmp.y -= 360;
		if (angleTmp.y < 0) angleTmp.y += 360;
		return angleTmp;
	});
	propertyAngle.getEvent().connect(
		[this](const glm::vec2 &pAngle) { propertyDirection = euclidean(radians(pAngle)); });
}

ExtensionProperty<glm::vec3> &CameraExtension::getPosition() {
	const auto &objectSelf = getObject();
	return objectSelf->getMainExtension()->propertyPosition;
}

utils::ReportMessagePtr CameraExtension::onInit() {

	putConnectionToStorage(
		getPosition().getEvent().connect(hide(sigc::mem_fun(*this, &CameraExtension::updateMatrix))));
	return nullptr;
}

void CameraExtension::onWindowSizeChanged(const int pWidth, const int pHeight) {
	if (pHeight <= 0) return;
	propertyAspect = static_cast<float>(pWidth) / static_cast<float>(pHeight);
}

void CameraExtension::projectionMatrixChanged(const glm::mat4 & /*pNewMatrix*/) { updateMatrix(); }

void CameraExtension::updateMatrix() {
	const auto &objectSelf = getObject();
	if (!objectSelf) return;
	const auto position = objectSelf->getMainExtension()->propertyPosition.getValue();
	propertyMatrix = getProjMatrix() * lookAt(position, position + propertyDirection.getValue(), {0, 1, 0});
}

} // namespace mer::sdk::main