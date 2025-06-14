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
// Created by alexus on 04.12.24.
//


#include "EngineSDK/utils/Transformation.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/polar_coordinates.hpp>

namespace mer::sdk {
std::shared_ptr<Transformation> Transformation::clone() const {
	auto other = create();
	other->modelMatrix = this->modelMatrix;
	other->positionVal = this->positionVal;
	other->positionValChanged = this->positionValChanged;
	other->translationMatrix = this->translationMatrix;
	other->rotationVal = this->rotationVal;
	other->rotationValChanged = this->rotationValChanged;
	other->rotationMatrix = this->rotationMatrix;
	other->scaleVal = this->scaleVal;
	other->scaleValChanged = this->scaleValChanged;
	other->scaleMatrix = this->scaleMatrix;
	return other;
}

void Transformation::calculateModelMatrix() {
	if (!positionValChanged && !rotationValChanged && !scaleValChanged) return;
	if (positionValChanged) {
		translationMatrix = glm::translate(glm::mat4(1.0f), positionVal);
		positionValChanged = false;
	}
	if (rotationValChanged) {
		rotationMatrix = glm::mat4_cast(rotationVal);
		rotationValChanged = false;
	}
	if (scaleValChanged) {
		scaleMatrix = glm::scale(glm::mat4(1.0f), scaleVal);
		scaleValChanged = false;
	}

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	onChanged(modelMatrix);
}
} // namespace mer::sdk
