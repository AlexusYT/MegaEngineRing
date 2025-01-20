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

#ifndef CAMERAKEYBOARDEXTENSION_H
#define CAMERAKEYBOARDEXTENSION_H
#include <glm/vec2.hpp>

#include "EngineSDK/extensions/Extension.h"
#include "EngineSDK/utils/KeyboardKey.h"

namespace mer::sdk::main {
class CameraKeyboardExtension : public Extension {

	bool fwdPressed{}, bwdPressed{}, strafeLeftPressed{}, strafeRightPressed{};


protected:
	CameraKeyboardExtension()
		: propertySpeed(this, "Speed"), propertyForwardKey(this, "ForwardKey"),
		  propertyStrafeLeftKey(this, "StrafeLeftKey"), propertyBackwardKey(this, "BackwardKey"),
		  propertyStrafeRightKey(this, "StrafeLeftKey"), propertyAngle(this, "Angle") {
		propertySpeed = 1.4f;
		propertyForwardKey = utils::KeyboardKey::KEY_W;
		propertyStrafeLeftKey = utils::KeyboardKey::KEY_A;
		propertyBackwardKey = utils::KeyboardKey::KEY_S;
		propertyStrafeRightKey = utils::KeyboardKey::KEY_D;
	}

public:
	ExtensionProperty<float> propertySpeed;
	ExtensionProperty<utils::KeyboardKey> propertyForwardKey;
	ExtensionProperty<utils::KeyboardKey> propertyStrafeLeftKey;
	ExtensionProperty<utils::KeyboardKey> propertyBackwardKey;
	ExtensionProperty<utils::KeyboardKey> propertyStrafeRightKey;

	ExtensionProperty<glm::vec2> propertyAngle;

	METHOD_CREATE(CameraKeyboardExtension)

	EXT_TYPE_NAME("CameraKeyboardExtension")

protected:
	void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) override;

	void onRender() override;
};
} // namespace mer::sdk::main


#endif //CAMERAKEYBOARDEXTENSION_H
