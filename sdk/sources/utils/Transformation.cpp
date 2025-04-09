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


#define GLM_ENABLE_EXPERIMENTAL

#include "EngineSDK/utils/Transformation.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/polar_coordinates.hpp>

namespace mer::sdk {
Transformation::Transformation(const glm::mat4 &pModelMatrix) : modelMatrix(pModelMatrix) {}

std::shared_ptr<Transformation> Transformation::create(const glm::mat4 &pModelMatrix) {
	return std::shared_ptr<Transformation>(new Transformation(pModelMatrix));
}

void Transformation::translate(const glm::vec3 &pCoords) {
	modelMatrix = glm::translate(modelMatrix, pCoords);
	onChanged(modelMatrix);
}

void Transformation::scale(const glm::vec3 &pScale) {
	modelMatrix = glm::scale(modelMatrix, pScale);
	onChanged(modelMatrix);
}

void Transformation::rotate(const float pRadians, const glm::vec3 &pVector) {
	modelMatrix = glm::rotate(modelMatrix, pRadians, pVector);
	onChanged(modelMatrix);
}

void Transformation::rotateQuaternion(const glm::quat &pQuat) {
	modelMatrix = modelMatrix * glm::mat4_cast(pQuat);
	onChanged(modelMatrix);
}

// ReSharper disable once CppInconsistentNaming
void Transformation::eulerAngleXZY(const glm::vec3 &pVector) {
	modelMatrix = modelMatrix * glm::eulerAngleXZY(pVector.x, pVector.y, pVector.z);
	onChanged(modelMatrix);
}

void Transformation::yawPitchRoll(const glm::vec3 &pVector) {
	modelMatrix = modelMatrix * glm::yawPitchRoll(pVector.x, pVector.y, pVector.z);
	onChanged(modelMatrix);
}

std::shared_ptr<Transformation> Transformation::clone() const {
	auto other = create();
	other->modelMatrix = this->modelMatrix;
	return other;
}

void Transformation::applyTransformation(const std::shared_ptr<Transformation> &pTransformation) {
	this->modelMatrix = this->modelMatrix * pTransformation->modelMatrix;
	onChanged(this->modelMatrix);
}

void Transformation::swapTransformation(const std::shared_ptr<Transformation> &pTransformation) {
	auto tmp = pTransformation->modelMatrix;
	pTransformation->modelMatrix = this->modelMatrix;
	this->modelMatrix = tmp;
	onChanged(this->modelMatrix);
	pTransformation->onChanged(pTransformation->modelMatrix);
}

} // namespace mer::sdk