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

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <glm/detail/type_quat.hpp>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

#include "EngineUtils/utils/Property.h"

namespace mer::sdk {
class Transformation {

	glm::mat4 modelMatrix{1};
	sigc::signal<void(const glm::mat4 &pNewMatrix)> onChanged;

protected:
	Transformation(const glm::mat4 &pModelMatrix);

public:
	static std::shared_ptr<Transformation> create(const glm::mat4 &pModelMatrix = glm::mat4{1});

	void reset() {
		modelMatrix = glm::mat4(1);
		onChanged(modelMatrix);
	}

	void setMatrix(const glm::mat4 &pNewMatrix) {
		modelMatrix = pNewMatrix;
		onChanged(modelMatrix);
	}

	void translate(const float pX, const float pY, const float pZ) { translate(glm::vec3(pX, pY, pZ)); }

	void translate(const glm::vec3 &pCoords);

	void scale(const float pX, const float pY, const float pZ) { scale(glm::vec3(pX, pY, pZ)); }

	void scale(const glm::vec3 &pScale);

	void rotate(const float pRadians, const float pX, const float pY, const float pZ) {
		rotate(pRadians, glm::vec3(pX, pY, pZ));
	}

	void rotate(float pRadians, const glm::vec3 &pVector);

	void rotateQuaternion(const glm::quat &pQuat);

	// ReSharper disable once CppInconsistentNaming
	void eulerAngleXZY(const float pX, const float pY, const float pZ) { eulerAngleXZY(glm::vec3(pX, pY, pZ)); }

	// ReSharper disable once CppInconsistentNaming
	void eulerAngleXZY(const glm::vec3 &pVector);

	void yawPitchRoll(const float pX, const float pY, const float pZ) { yawPitchRoll(glm::vec3(pX, pY, pZ)); }

	void yawPitchRoll(const glm::vec3 &pVector);

	glm::vec3 getPosition() const { return modelMatrix[3]; }

	[[nodiscard]] const glm::mat4 &getModelMatrix() const { return modelMatrix; }

	std::shared_ptr<Transformation> clone() const;

	[[nodiscard]] const sigc::signal<void(const glm::mat4 &)> &getOnChangedSignal() const { return onChanged; }

	sigc::connection connectOnChanged(const sigc::slot<void(const glm::mat4 &pNewMatrix)> &pSlot) {
		pSlot(modelMatrix);
		return onChanged.connect(pSlot);
	}

	void applyTransformation(const std::shared_ptr<Transformation> &pTransformation);

	void swapTransformation(const std::shared_ptr<Transformation> &pTransformation);
};

} // namespace mer::sdk

#endif //TRANSFORMATION_H
