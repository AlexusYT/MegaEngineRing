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
// Created by alexus on 24.03.24.
//

#ifndef ORBITCAMERAEXTENSION_H
#define ORBITCAMERAEXTENSION_H
#include <glm/mat4x4.hpp>

#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "ICamera.h"
#include "PerspectiveProjectionCameraMod.h"

namespace mer::sdk::main {
class OrbitCameraExtension : public Extension, public PerspectiveProjectionCameraMod, public ICamera {
	glm::mat4 matrix{1};
	sigc::signal<void(const glm::mat<4, 4, float> &)> onMatrixChanged;
	glm::vec2 angle{};
	ValueChangedArgs<const glm::vec2 &> onAngleChanged;
	glm::vec3 targetPosition{};
	ValueChanged onTargetPositionChanged;

protected:
	OrbitCameraExtension() = default;

public:
	METHOD_CREATE(OrbitCameraExtension)

	EXT_TYPE_NAME("OrbitCameraExtension")

	[[nodiscard]] sigc::signal<void(const glm::mat4 &)> &getOnMatrixChanged() override { return onMatrixChanged; }

	[[nodiscard]] const glm::mat4 &getMatrix() const override { return matrix; }

	[[nodiscard]] ValueChangedArgs<const glm::vec2 &> &getOnAngleChanged() { return onAngleChanged; }

	[[nodiscard]] const glm::vec3 &getTargetPosition() const { return targetPosition; }

	void setTargetPosition(const glm::vec3 &pTargetPosition) {
		targetPosition = pTargetPosition;
		updateMatrix();
	}

	[[nodiscard]] ValueChanged &getOnTargetPositionChanged() { return onTargetPositionChanged; }

	[[nodiscard]] const glm::vec2 &getAngle() const { return angle; }

	void setAngle(const glm::vec2 &pAngle) {
		if (angle == pAngle) return;
		angle = pAngle;
		onAngleChanged(angle);
		updateMatrix();
	}

	void addAngle(const glm::vec2 &pAngleToAdd) { setAngle(getAngle() + pAngleToAdd); }


protected:
	utils::ReportMessagePtr onInit() override;

	void setMatrix(const glm::mat4 &pMatrix) {
		matrix = pMatrix;
		onMatrixChanged(pMatrix);
	}

	void onWindowSizeChanged(int pWidth, int pHeight) override;

private:
	void projectionMatrixChanged(const glm::mat4 &pNewMatrix) override;

	void updateMatrix();

	void getProperties(ExtensionProperties &pProperties) override;
};
} // namespace mer::sdk::main


#endif //ORBITCAMERAEXTENSION_H
