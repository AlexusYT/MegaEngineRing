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
// Created by alexus on 28.02.24.
//


#include <EngineSDK/main/scene/objects/extensions/cameras/CameraExtension.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/polar_coordinates.hpp>

#include "EngineSDK/main/scene/Scene.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"

namespace mer::sdk::main {

void CameraExtension::setAngle(const glm::vec2 &pAngle) {
	if (angle == pAngle) return;
	glm::vec2 angleTmp = pAngle;
	constexpr float delta = 0.001f;
	if (angleTmp.x > 90.0f - delta) angleTmp.x = 90.0f - delta;
	if (angleTmp.x < -90.0f + delta) angleTmp.x = -90.0f + delta;
	if (angleTmp.y > 360) angleTmp.y -= 360;
	if (angleTmp.y < 0) angleTmp.y += 360;
	if (angle == angleTmp) return;
	angle = angleTmp;
	onAngleChanged(angle);
	setDirection(euclidean(radians(angleTmp)));
}

utils::ReportMessagePtr CameraExtension::onInit() {

	const auto &objectSelf = getObject();
	putConnectionToStorage(
		objectSelf->getOnPositionChangedSignal().connect(sigc::mem_fun(*this, &CameraExtension::updateMatrix)));
	return nullptr;
}

void CameraExtension::onWindowSizeChanged(const int pWidth, const int pHeight) {
	if (pHeight <= 0) return;
	setAspect(static_cast<float>(pWidth) / static_cast<float>(pHeight));
}

void CameraExtension::projectionMatrixChanged(const glm::mat4 & /*pNewMatrix*/) { updateMatrix(); }

void CameraExtension::updateMatrix() {
	const auto &objectSelf = getObject();
	const auto position = objectSelf->getPosition();
	setMatrix(getProjMatrix() * lookAt(position, position + direction, {0, 1, 0}));
}

void CameraExtension::getProperties(ExtensionProperties &pProperties) {
	pProperties.emplace_back(this, "View Angle", "", &CameraExtension::getAngle, &CameraExtension::setAngle);
	getProjectionProperties(pProperties);
}

} // namespace mer::sdk::main