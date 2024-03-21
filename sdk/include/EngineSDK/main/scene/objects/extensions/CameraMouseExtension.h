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
// Created by alexus on 29.02.24.
//

#ifndef CAMERAMOUSEEXTENSION_H
#define CAMERAMOUSEEXTENSION_H
#include <glm/vec2.hpp>

#include "Extension.h"

namespace mer::sdk::main {
class CameraMouseExtension : public Extension {
	glm::vec2 mouseSensitivity{0.6f};
	ValueChangedArgs<const glm::vec2 &> onMouseSensitivityChanged;

	Method<const glm::vec2 &> methodAddAngle;

protected:
	CameraMouseExtension() = default;

public:
	METHOD_CREATE(CameraMouseExtension)

	EXT_TYPE_NAME("CameraMouseExtension")

	[[nodiscard]] const glm::vec2 &getMouseSensitivity() const { return mouseSensitivity; }

	void setMouseSensitivity(const glm::vec2 &pMouseSensitivity) { mouseSensitivity = pMouseSensitivity; }

	[[nodiscard]] ValueChangedArgs<const glm::vec2 &> &getOnMouseSensitivityChanged() {
		return onMouseSensitivityChanged;
	}

	[[nodiscard]] const Method<const glm::vec2 &> &getMethodAddAngle() const { return methodAddAngle; }

	void setMethodAddAngle(const Method<const glm::vec2 &> &pMethodAddAngle) { methodAddAngle = pMethodAddAngle; }

protected:
	void onCursorPosChanged(const double pX, const double pY) override {

		const glm::dvec2 pos{pX, pY};
		static glm::dvec2 lastCursorPos = pos;

		const glm::vec2 delta = lastCursorPos - pos;
		methodAddAngle(mouseSensitivity * glm::vec2(delta.y, delta.x));
		lastCursorPos = pos;
	}

	void getProperties(std::vector<std::shared_ptr<ExtensionPropertyBase>> &pProperties) override {
		pProperties.emplace_back(createProperty("Sensitivity", "Mouse Sensitivity",
												&CameraMouseExtension::getMouseSensitivity,
												&CameraMouseExtension::setMouseSensitivity));
	}
};
} // namespace mer::sdk::main

#endif //CAMERAMOUSEEXTENSION_H
