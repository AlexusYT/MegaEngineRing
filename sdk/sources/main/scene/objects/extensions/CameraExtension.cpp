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


#include <EngineSDK/main/scene/objects/extensions/CameraExtension.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "EngineSDK/main/scene/Scene.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"

namespace mer::sdk::main {
CameraExtension::CameraExtension() : fov(glm::radians(60.0f)), zNear(0.1f), zFar(1000.0f) {}

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

void CameraExtension::updateMatrix() {
	const auto &objectSelf = getObject();
	const auto position = objectSelf->getPosition();
	setMatrix(glm::perspective(fov, aspect, zNear, zFar) * lookAt(position, position + direction, {0, 1, 0}));
}
} // namespace mer::sdk::main