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

#ifndef CAMERAKEYBOARDEXTENSION_H
#define CAMERAKEYBOARDEXTENSION_H
#include <glm/vec2.hpp>

#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "EngineSDK/utils/KeyboardKey.h"

namespace mer::sdk::main {
class CameraKeyboardExtension : public Extension {
	utils::KeyboardKey forwardKey = utils::KeyboardKey::KEY_W;
	utils::KeyboardKey strafeLeftKey = utils::KeyboardKey::KEY_A;
	utils::KeyboardKey backwardKey = utils::KeyboardKey::KEY_S;
	utils::KeyboardKey strafeRightKey = utils::KeyboardKey::KEY_D;

	bool fwdPressed{}, bwdPressed{}, strafeLeftPressed{}, strafeRightPressed{};
	float speed = 1.4f;
	ValueChangedArgs<float> onSpeedChanged;

	sigc::slot<glm::vec2()> methodGetAngle{};

protected:
	CameraKeyboardExtension() = default;

public:
	METHOD_CREATE(CameraKeyboardExtension)

	EXT_TYPE_NAME("CameraKeyboardExtension")

	[[nodiscard]] utils::KeyboardKey getForwardKey() const { return forwardKey; }

	void setForwardKey(const utils::KeyboardKey pForwardKey) { forwardKey = pForwardKey; }

	[[nodiscard]] utils::KeyboardKey getStrafeLeftKey() const { return strafeLeftKey; }

	void setStrafeLeftKey(const utils::KeyboardKey pStrafeLeftKey) { strafeLeftKey = pStrafeLeftKey; }

	[[nodiscard]] utils::KeyboardKey getBackwardKey() const { return backwardKey; }

	void setBackwardKey(const utils::KeyboardKey pBackwardKey) { backwardKey = pBackwardKey; }

	[[nodiscard]] utils::KeyboardKey getStrafeRightKey() const { return strafeRightKey; }

	void setStrafeRightKey(const utils::KeyboardKey pStrafeRightKey) { strafeRightKey = pStrafeRightKey; }

	[[nodiscard]] float getSpeed() const { return speed; }

	void setSpeed(const float pSpeed) {
		speed = pSpeed;
		onSpeedChanged(speed);
	}

	[[nodiscard]] ValueChangedArgs<float> &getOnSpeedChanged() { return onSpeedChanged; }

	[[nodiscard]] const sigc::slot<glm::vec2()> &getMethodGetAngle() const { return methodGetAngle; }

	void setMethodGetAngle(const sigc::slot<glm::vec2()> &pMethodGetAngle) { methodGetAngle = pMethodGetAngle; }

protected:
	void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) override;

	void onRender() override;

	void getProperties(ExtensionProperties &pProperties) override {
		pProperties.emplace_back(this, "Speed", "", &CameraKeyboardExtension::getSpeed,
								 &CameraKeyboardExtension::setSpeed);
		pProperties.emplace_back(this, "Forward key", "", &CameraKeyboardExtension::getForwardKey,
								 &CameraKeyboardExtension::setForwardKey);
		pProperties.emplace_back(this, "Backward key", "", &CameraKeyboardExtension::getBackwardKey,
								 &CameraKeyboardExtension::setBackwardKey);
		pProperties.emplace_back(this, "Strafe left key", "", &CameraKeyboardExtension::getStrafeLeftKey,
								 &CameraKeyboardExtension::setStrafeLeftKey);
		pProperties.emplace_back(this, "Strafe right key", "", &CameraKeyboardExtension::getStrafeRightKey,
								 &CameraKeyboardExtension::setStrafeRightKey);
	}
};
} // namespace mer::sdk::main


#endif //CAMERAKEYBOARDEXTENSION_H
