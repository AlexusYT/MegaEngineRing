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
	: propertyFov(glm::radians(60.0f)), propertyZNear(0.1f), propertyZFar(1000.0f) {}

void PerspectiveProjectionCameraMod::projectionMatrixChanged(const glm::mat4 & /*pNewMatrix*/) {}

void PerspectiveProjectionCameraMod::updateProjMatrix() {
	propertyProjMatrix = glm::perspective(propertyFov, propertyAspect, propertyZNear, propertyZFar);
	onProjMatrixChanged(propertyProjMatrix);
	projectionMatrixChanged(propertyProjMatrix);
}
} // namespace mer::sdk::main