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
	DECLARE_PROPERTY_DEF(float, Speed, 1.4f);
	ADD_PROPERTY_SET_EVENT(CameraKeyboardExtension, Speed, "Movement speed", "");

	DECLARE_PROPERTY_DEF(utils::KeyboardKey, ForwardKey, utils::KeyboardKey::KEY_W);
	ADD_PROPERTY_SET_EVENT(CameraKeyboardExtension, ForwardKey, "Move forward key", "");
	DECLARE_PROPERTY_DEF(utils::KeyboardKey, StrafeLeftKey, utils::KeyboardKey::KEY_A);
	ADD_PROPERTY_SET_EVENT(CameraKeyboardExtension, StrafeLeftKey, "Move left key", "");
	DECLARE_PROPERTY_DEF(utils::KeyboardKey, BackwardKey, utils::KeyboardKey::KEY_S);
	ADD_PROPERTY_SET_EVENT(CameraKeyboardExtension, BackwardKey, "Move back key", "");
	DECLARE_PROPERTY_DEF(utils::KeyboardKey, StrafeRightKey, utils::KeyboardKey::KEY_D);
	ADD_PROPERTY_SET_EVENT(CameraKeyboardExtension, StrafeRightKey, "Move right key", "");

	DECLARE_PROPERTY(glm::vec2, Angle);
	ADD_PROPERTY_SET_EVENT(CameraKeyboardExtension, Angle, "View angle", "");


	bool fwdPressed{}, bwdPressed{}, strafeLeftPressed{}, strafeRightPressed{};


protected:
	CameraKeyboardExtension() = default;

public:
	METHOD_CREATE(CameraKeyboardExtension)

	EXT_TYPE_NAME("CameraKeyboardExtension")

	[[nodiscard]] ValueChangedArgs<const utils::KeyboardKey &> &getOnForwardKeyChanged() { return onForwardKeyChanged; }

	[[nodiscard]] ValueChangedArgs<const utils::KeyboardKey &> &getOnStrafeLeftKeyChanged() {
		return onStrafeLeftKeyChanged;
	}

	[[nodiscard]] ValueChangedArgs<const utils::KeyboardKey &> &getOnBackwardKeyChanged() {
		return onBackwardKeyChanged;
	}

	[[nodiscard]] ValueChangedArgs<const utils::KeyboardKey &> &getOnStrafeRightKeyChanged() {
		return onStrafeRightKeyChanged;
	}

	[[nodiscard]] utils::KeyboardKey getForwardKey() const { return propertyForwardKey; }

	void setForwardKey(const utils::KeyboardKey pForwardKey) { propertyForwardKey = pForwardKey; }

	[[nodiscard]] utils::KeyboardKey getStrafeLeftKey() const { return propertyStrafeLeftKey; }

	void setStrafeLeftKey(const utils::KeyboardKey pStrafeLeftKey) { propertyStrafeLeftKey = pStrafeLeftKey; }

	[[nodiscard]] utils::KeyboardKey getBackwardKey() const { return propertyBackwardKey; }

	void setBackwardKey(const utils::KeyboardKey pBackwardKey) { propertyBackwardKey = pBackwardKey; }

	[[nodiscard]] utils::KeyboardKey getStrafeRightKey() const { return propertyStrafeRightKey; }

	void setStrafeRightKey(const utils::KeyboardKey pStrafeRightKey) { propertyStrafeRightKey = pStrafeRightKey; }

	[[nodiscard]] float getSpeed() const { return propertySpeed; }

	void setSpeed(const float pSpeed) {
		propertySpeed = pSpeed;
		onSpeedChanged(pSpeed);
	}

	[[nodiscard]] ValueChangedArgs<const float &> &getOnSpeedChanged() { return onSpeedChanged; }

	[[nodiscard]] const glm::vec2 &getAngle() const { return propertyAngle; }

	void setAngle(const glm::vec2 &pPropertyAngle) { propertyAngle = pPropertyAngle; }

	[[nodiscard]] ValueChangedArgs<const glm::vec2 &> &getOnAngleChanged() { return onAngleChanged; }


protected:
	void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) override;

	void onRender() override;
};
} // namespace mer::sdk::main


#endif //CAMERAKEYBOARDEXTENSION_H
