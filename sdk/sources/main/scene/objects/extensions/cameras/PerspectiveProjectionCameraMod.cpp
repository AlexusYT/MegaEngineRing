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
// Created by alexus on 22.03.24.
//

#include "EngineSDK/main/scene/objects/extensions/cameras/PerspectiveProjectionCameraMod.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "EngineSDK/main/scene/objects/extensions/Extension.h"

namespace mer::sdk::main {
PerspectiveProjectionCameraMod::PerspectiveProjectionCameraMod()
	: fov(glm::radians(60.0f)), zNear(0.1f), zFar(1000.0f) {}

void PerspectiveProjectionCameraMod::getProjectionProperties(ExtensionProperties &pProperties) {
	using CamerMod = PerspectiveProjectionCameraMod;
	pProperties.emplace_back(this, "Frame aspect", "", &CamerMod::getAspect, &CamerMod::setAspect);
	pProperties.emplace_back(this, "Fov, rad", "", &CamerMod::getFov, &CamerMod::setFov);
	pProperties.emplace_back(this, "Z near", "", &CamerMod::getZNear, &CamerMod::setZNear);
	pProperties.emplace_back(this, "Z far", "", &CamerMod::getZFar, &CamerMod::setZFar);
}

void PerspectiveProjectionCameraMod::projectionMatrixChanged(const glm::mat4 & /*pNewMatrix*/) {}

void PerspectiveProjectionCameraMod::updateProjMatrix() {
	projMatrix = glm::perspective(fov, aspect, zNear, zFar);
	onProjMatrixChanged(projMatrix);
}
} // namespace mer::sdk::main