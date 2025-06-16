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
// Created by alexus on 29.02.24.
//

#ifndef CAMERAMOUSEEXTENSION_H
#define CAMERAMOUSEEXTENSION_H
#include <glm/vec2.hpp>

#include "EngineSDK/extensions/Extension.h"

namespace mer::sdk {
class CameraMouseExtension : public Extension {
	std::optional<glm::dvec2> lastCursorPos{};

protected:
	CameraMouseExtension()
		: propertyMouseSensitivity(this, "MouseSensitivity"), propertyAngle(this, "Angle") {
		propertyMouseSensitivity.setValue(glm::vec2(0.6f));
	}

public:
	ExtensionProperty<glm::vec2> propertyMouseSensitivity;
	ExtensionProperty<glm::vec2> propertyAngle;

	METHOD_CREATE(CameraMouseExtension)

	EXT_TYPE_NAME("CameraMouseExtension")

protected:
	void onCursorPosChanged(const double pX, const double pY) override {
		const glm::dvec2 pos{pX, pY};
		if (lastCursorPos) {
			const glm::vec2 delta = lastCursorPos.value() - pos;
			propertyAngle = propertyAngle + propertyMouseSensitivity.getValue() * glm::vec2(delta.y, delta.x);
		}
		lastCursorPos = pos;
	}

	void onEnabledChanged() override { lastCursorPos.reset(); }
};
} // namespace mer::sdk

#endif //CAMERAMOUSEEXTENSION_H
