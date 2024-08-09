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

#include "EngineSDK/main/scene/objects/extensions/Extension.h"

namespace mer::sdk::main {
class CameraMouseExtension : public Extension {
	DECLARE_PROPERTY(glm::vec2, MouseSensitivity);
	ADD_PROPERTY_SET_EVENT(CameraMouseExtension, MouseSensitivity, "Mouse sensitivity", "");
	DECLARE_PROPERTY(glm::vec2, Angle);
	ADD_PROPERTY_EVENT(CameraMouseExtension, Angle, "View angle", "");

	std::optional<glm::dvec2> lastCursorPos{};

protected:
	CameraMouseExtension() : propertyMouseSensitivity(0.6f) {}

public:
	METHOD_CREATE(CameraMouseExtension)

	EXT_TYPE_NAME("CameraMouseExtension")

	[[nodiscard]] const glm::vec2 &getMouseSensitivity() const { return propertyMouseSensitivity; }

	void setMouseSensitivity(const glm::vec2 &pMouseSensitivity) {
		if (propertyMouseSensitivity == pMouseSensitivity) return;
		propertyMouseSensitivity = pMouseSensitivity;
		onMouseSensitivityChanged(pMouseSensitivity);
	}

	[[nodiscard]] ValueChangedArgs<const glm::vec2 &> &getOnMouseSensitivityChanged() {
		return onMouseSensitivityChanged;
	}

	[[nodiscard]] const glm::vec2 &getAngle() const { return propertyAngle; }

	void setAngle(const glm::vec2 &pPropertyAngle) {
		if (pPropertyAngle == propertyAngle) return;
		propertyAngle = pPropertyAngle;
		onAngleChanged(pPropertyAngle);
	}

	[[nodiscard]] ValueChangedArgs<const glm::vec2 &> &getOnAngleChanged() { return onAngleChanged; }

protected:
	void onCursorPosChanged(const double pX, const double pY) override {

		const glm::dvec2 pos{pX, pY};
		if (lastCursorPos) {
			const glm::vec2 delta = lastCursorPos.value() - pos;
			setAngle(propertyAngle + propertyMouseSensitivity * glm::vec2(delta.y, delta.x));
		}
		lastCursorPos = pos;
	}

	void onEnabledChanged() override { lastCursorPos.reset(); }
};
} // namespace mer::sdk::main

#endif //CAMERAMOUSEEXTENSION_H
