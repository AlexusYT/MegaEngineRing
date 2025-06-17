//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 22.03.24.
//

#include "KwasarEngine/extensions/cameras/PerspectiveProjectionCameraMod.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <sigc++/adaptors/hide.h>

#include "KwasarEngine/extensions/Extension.h"

namespace ke {
PerspectiveProjectionCameraMod::PerspectiveProjectionCameraMod()
	: propertyAspect(this, "Aspect"),
	  propertyFov(this, "Fov"),
	  propertyZNear(this, "ZNear"),
	  propertyZFar(this, "ZFar"),
	  propertyProjMatrix(this, "ProjMatrix") {
	propertyAspect.getEvent().connect([this](const float &) { updateProjMatrix(); });
	propertyFov = glm::radians(60.0f);
	auto updateProjMatrixSlot = hide(sigc::mem_fun(*this, &PerspectiveProjectionCameraMod::updateProjMatrix));
	propertyFov.getEvent().connect(updateProjMatrixSlot);
	propertyZNear = 0.1f;
	propertyZNear.getEvent().connect(updateProjMatrixSlot);
	propertyZFar = 1000.0f;
	propertyZFar.getEvent().connect(updateProjMatrixSlot);
	propertyProjMatrix = glm::mat4(1.0f);
	propertyProjMatrix.getEvent().connect(
		sigc::mem_fun(*this, &PerspectiveProjectionCameraMod::projectionMatrixChanged));
}

void PerspectiveProjectionCameraMod::projectionMatrixChanged(const glm::mat4 & /*pNewMatrix*/) {}

void PerspectiveProjectionCameraMod::updateProjMatrix() {
	propertyProjMatrix = glm::perspective(propertyFov.getValue(), propertyAspect.getValue(), propertyZNear.getValue(),
										  propertyZFar.getValue());
}
} // namespace ke