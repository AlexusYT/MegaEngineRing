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

#ifndef CAMERAEXTENSION_H
#define CAMERAEXTENSION_H
#include <glm/mat4x4.hpp>

#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "ICamera.h"
#include "PerspectiveProjectionCameraMod.h"

namespace mer::sdk::main {
class CameraExtension : public Extension, public PerspectiveProjectionCameraMod, public ICamera {

	DECLARE_PROPERTY(glm::mat4, Matrix);
	ADD_PROPERTY_EVENT(CameraExtension, Matrix, "Projection-View matrix", "");

	DECLARE_PROPERTY(glm::vec3, Direction);
	ADD_PROPERTY_EVENT(CameraExtension, Direction, "View direction", "");

	DECLARE_PROPERTY(glm::vec2, Angle);
	ADD_PROPERTY_SET_EVENT(CameraExtension, Angle, "View angle", "");

protected:
	CameraExtension();

public:
	METHOD_CREATE(CameraExtension)

	EXT_TYPE_NAME("CameraExtension")

	[[nodiscard]] const glm::mat4 &getMatrix() const override { return propertyMatrix; }

	[[nodiscard]] ValueChangedArgs<const glm::mat4 &> &getOnMatrixChanged() override { return onMatrixChanged; }

	[[nodiscard]] const glm::vec3 &getDirection() const { return propertyDirection; }

	[[nodiscard]] ValueChangedArgs<const glm::vec3 &> &getOnDirectionChanged() { return onDirectionChanged; }

	[[nodiscard]] ValueChangedArgs<const glm::vec2 &> &getOnAngleChanged() { return onAngleChanged; }

	[[nodiscard]] const glm::vec2 &getAngle() const { return propertyAngle; }

	void setAngle(const glm::vec2 &pAngle);

	void addAngle(const glm::vec2 &pAngleToAdd) { setAngle(getAngle() + pAngleToAdd); }


protected:
	utils::ReportMessagePtr onInit() override;

	void setMatrix(const glm::mat4 &pMatrix) {
		propertyMatrix = pMatrix;
		onMatrixChanged(pMatrix);
	}

	void setDirection(const glm::vec3 &pDirection) {
		propertyDirection = pDirection;
		onDirectionChanged(pDirection);
		updateMatrix();
	}

	void onWindowSizeChanged(int pWidth, int pHeight) override;

private:
	void projectionMatrixChanged(const glm::mat4 &pNewMatrix) override;

	void updateMatrix() override;
};
} // namespace mer::sdk::main


#endif //CAMERAEXTENSION_H
