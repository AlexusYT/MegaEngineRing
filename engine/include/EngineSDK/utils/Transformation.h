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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/mat4x4.hpp>

#include "Property.h"

namespace mer::sdk {
class Transformation {
protected:
	glm::vec3 positionVal{};
	glm::mat4 translationMatrix{1};
	bool positionValChanged{};
	glm::quat rotationVal{1, 0, 0, 0};
	glm::mat4 rotationMatrix{1};
	bool rotationValChanged{};
	glm::vec3 scaleVal{1};
	glm::mat4 scaleMatrix{1};
	bool scaleValChanged{};
	glm::mat4 modelMatrix{1};
	sigc::signal<void(const glm::mat4 &pNewMatrix)> onChanged;

	Transformation(const glm::mat4 &pModelMatrix) { setFromMatrix(pModelMatrix); }

	Transformation(const glm::vec3 &pPosition, const glm::quat &pRotation, const glm::vec3 &pScale) {
		setFromArgs(pPosition, pRotation, pScale);
	}

	Transformation() = default;

public:
	static std::shared_ptr<Transformation> create(const glm::mat4 &pModelMatrix) {
		return std::shared_ptr<Transformation>(new Transformation(pModelMatrix));
	}

	static std::shared_ptr<Transformation> create(const glm::vec3 &pPosition, const glm::quat &pRotation = {},
												  const glm::vec3 &pScale = {}) {
		return std::shared_ptr<Transformation>(new Transformation(pPosition, pRotation, pScale));
	}

	static std::shared_ptr<Transformation> create() { return std::shared_ptr<Transformation>(new Transformation()); }

	void reset() {
		positionVal = glm::vec3(0);
		translationMatrix = glm::mat4(1);
		positionValChanged = false;
		rotationVal = glm::quat();
		rotationMatrix = glm::mat4(1);
		rotationValChanged = false;
		scaleVal = glm::vec3(0);
		scaleMatrix = glm::mat4(1);
		scaleValChanged = false;

		modelMatrix = glm::mat4(1);
		onChanged(modelMatrix);
	}

	void setFromArgs(const glm::vec3 &pPosition, const glm::quat &pRotation, const glm::vec3 &pScale) {
		positionVal = pPosition;
		positionValChanged = true;
		rotationVal = pRotation;
		rotationValChanged = true;
		scaleVal = pScale;
		scaleValChanged = true;
		calculateModelMatrix();
	}

	void addFromArgs(const glm::vec3 &pPosition, const glm::quat &pRotation, const glm::vec3 &pScale) {
		positionVal += pPosition;
		positionValChanged = true;
		rotationVal += pRotation;
		rotationValChanged = true;
		scaleVal += pScale;
		scaleValChanged = true;
		calculateModelMatrix();
	}

	void setFromMatrix(const glm::mat4 &pNewMatrix) {
		//Early exit if the matrix is identity
		if (pNewMatrix == glm::identity<glm::mat4>()) {
			reset();
			return;
		}
		glm::vec3 scaleTmp{};
		glm::quat rotationTmp{};
		glm::vec3 translationTmp{};
		glm::vec3 skewTmp{};
		glm::vec4 perspectiveTmp{};

		glm::decompose(pNewMatrix, scaleTmp, rotationTmp, translationTmp, skewTmp, perspectiveTmp);

		setFromArgs(translationTmp, rotationTmp, scaleTmp);
	}

	void addFromMatrix(const glm::mat4 &pNewMatrix) {
		glm::vec3 scaleTmp{};
		glm::quat rotationTmp{};
		glm::vec3 translationTmp{};
		glm::vec3 skewTmp{};
		glm::vec4 perspectiveTmp{};

		glm::decompose(pNewMatrix, scaleTmp, rotationTmp, translationTmp, skewTmp, perspectiveTmp);

		addFromArgs(translationTmp, rotationTmp, scaleTmp);
	}

	void translate(const float pX, const float pY, const float pZ) {
		if (pX == 0 && pY == 0 && pZ == 0) return;
		setPositionImpl(positionVal + glm::vec3(pX, pY, pZ));
	}

	void translate(const glm::vec3 &pCoords) {
		if (pCoords.x == 0 && pCoords.y == 0 && pCoords.z == 0) return;
		setPositionImpl(positionVal + pCoords);
	}

	void setPosition(const glm::vec3 &pPositionVal) {
		if (positionVal == pPositionVal) return;
		setPositionImpl(pPositionVal);
	}

	void setPosition(const float pX, const float pY, const float pZ) {
		if (positionVal.x == pX && positionVal.y == pY && positionVal.z == pZ) return;
		setPositionImpl(glm::vec3(pX, pY, pZ));
	}

	[[nodiscard]] const glm::vec3 &getPosition() const { return positionVal; }

	void scale(const float pX, const float pY, const float pZ) {
		if (pX == 0 && pY == 0 && pZ == 0) return;
		setScaleImpl(scaleVal + glm::vec3(pX, pY, pZ));
	}

	void scale(const glm::vec3 &pScale) {
		if (pScale.x == 0 && pScale.y == 0 && pScale.z == 0) return;
		setScaleImpl(scaleVal + pScale);
	}

	void setScale(const glm::vec3 &pScaleVal) {
		if (scaleVal == pScaleVal) return;
		setScaleImpl(pScaleVal);
	}

	void setScale(const float pX, const float pY, const float pZ) {
		if (scaleVal.x == pX && scaleVal.y == pY && scaleVal.z == pZ) return;
		setScaleImpl(glm::vec3(pX, pY, pZ));
	}

	[[nodiscard]] const glm::vec3 &getScale() const { return scaleVal; }

	void rotateAxis(const float pRadians, const float pX, const float pY, const float pZ) {
		rotateAxis(pRadians, glm::vec3(pX, pY, pZ));
	}

	void rotateAxis(float pRadians, const glm::vec3 &pVector) {
		//TODO Add checks for values that can be ignored
		rotateQuaternion(glm::quat_cast(glm::rotate(glm::mat4(1), pRadians, pVector)));
	}

	void rotateAxis(const glm::vec4 &pValues) { rotateAxis(pValues.w, glm::vec3(pValues)); }

	void rotateQuaternion(const glm::quat &pQuat) { setRotationQuaternion(rotationVal + pQuat); }

	[[nodiscard]] const glm::quat &getRotation() const { return rotationVal; }

	void setRotationQuaternion(const glm::quat &pRotationVal) {
		if (rotationVal == pRotationVal) return;
		rotationVal = pRotationVal;
		rotationValChanged = true;
		calculateModelMatrix();
	}

	// ReSharper disable once CppInconsistentNaming
	void eulerAngleXZY(const float pX, const float pY, const float pZ) {
		//TODO Add checks for values that can be ignored
		rotateQuaternion(glm::quat_cast(glm::eulerAngleXZY(pX, pY, pZ)));
	}

	// ReSharper disable once CppInconsistentNaming
	void eulerAngleXZY(const glm::vec3 &pVector) { eulerAngleXZY(pVector.x, pVector.y, pVector.z); }

	void yawPitchRoll(const float pX, const float pY, const float pZ) {
		//TODO Add checks for values that can be ignored
		rotateQuaternion(glm::quat_cast(glm::eulerAngleXZY(pX, pY, pZ)));
	}

	void yawPitchRoll(const glm::vec3 &pVector) { yawPitchRoll(pVector.x, pVector.y, pVector.z); }

	[[nodiscard]] const glm::mat4 &getModelMatrix() const { return modelMatrix; }

	std::shared_ptr<Transformation> clone() const;

	[[nodiscard]] const sigc::signal<void(const glm::mat4 &)> &getOnChangedSignal() const { return onChanged; }

	sigc::connection connectOnChanged(const sigc::slot<void(const glm::mat4 &pNewMatrix)> &pSlot) {
		pSlot(modelMatrix);
		return onChanged.connect(pSlot);
	}

protected:
	void calculateModelMatrix();

	void setPositionImpl(const glm::vec3 &pPositionVal) {
		positionVal = pPositionVal;
		positionValChanged = true;
		calculateModelMatrix();
	}

	void setScaleImpl(const glm::vec3 &pScaleVal) {
		scaleVal = pScaleVal;
		scaleValChanged = true;
		calculateModelMatrix();
	}
};
} // namespace mer::sdk

#endif //TRANSFORMATION_H
