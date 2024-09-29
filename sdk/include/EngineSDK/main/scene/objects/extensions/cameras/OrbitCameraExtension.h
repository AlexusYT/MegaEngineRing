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
#include <sigc++/adaptors/hide.h>

#include "ICamera.h"
#include "PerspectiveProjectionCameraMod.h"

namespace mer::sdk::main {
class OrbitCameraExtension : public PerspectiveProjectionCameraMod, public ICamera {

protected:
	OrbitCameraExtension()
		: propertyMatrix(this, "Matrix"), propertyAngle(this, "Angle"), propertyTargetPosition(this, "TargetPosition") {
		propertyMatrix.setValue(glm::mat4(1));
		auto updateMatrixSlot = hide(sigc::mem_fun(*this, &OrbitCameraExtension::updateMatrix));
		propertyTargetPosition.getEvent().connect(updateMatrixSlot);
		propertyAngle.getEvent().connect(updateMatrixSlot);
	}

public:
	ExtensionProperty<glm::mat4> propertyMatrix;
	ExtensionProperty<glm::vec2> propertyAngle;
	ExtensionProperty<glm::vec3> propertyTargetPosition;

	METHOD_CREATE(OrbitCameraExtension)

	EXT_TYPE_NAME("OrbitCameraExtension")

	[[nodiscard]] sigc::signal<void(const glm::mat4 &)> &getOnMatrixChanged() override {
		return propertyMatrix.getEvent();
	}

	[[nodiscard]] const glm::mat4 &getMatrix() const override { return propertyMatrix; }

	void addAngle(const glm::vec2 &pAngleToAdd) { propertyAngle = propertyAngle + pAngleToAdd; }


protected:
	utils::ReportMessagePtr onInit() override;

	void onWindowSizeChanged(int pWidth, int pHeight) override;

private:
	void projectionMatrixChanged(const glm::mat4 &pNewMatrix) override;

	void updateMatrix() override;
};
} // namespace mer::sdk::main


#endif //ORBITCAMERAEXTENSION_H
