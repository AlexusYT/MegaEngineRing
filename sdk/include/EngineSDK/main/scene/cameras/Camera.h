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
// Created by alexus on 24.02.24.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <EngineSDK/main/scene/actor/IActor.h>
#include <glm/mat4x4.hpp>
#include <sigc++/signal.h>

#include "ICamera.h"

namespace mer::sdk::main {
class Camera : public ICamera, public IActor {
	glm::mat4 matrix{1};
	glm::vec3 position{};
	glm::vec3 direction{};
	glm::vec2 angle{};
	float aspect;
	sigc::signal<void(const glm::mat4 &pMatrix)> onMatrixChangedSignal;

	bool fwdPressed{}, bwdPressed{}, strafeLeftPressed{}, strafeRightPressed{}, liftUpPressed{}, liftDownPressed{};

public:
	Camera() = default;

	const glm::mat4 &getMatrix() override { return matrix; }

	bool onCursorPosChanged(const double pX, const double pY) override {
		glm::dvec2 pos{pX, pY};
		static glm::dvec2 lastCursorPos = pos;

		const glm::vec2 delta = lastCursorPos - pos;
		addAngle(0.9f * glm::vec2(delta.y, delta.x));
		lastCursorPos = pos;
		return true;
	}

	void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, utils::ModifierKeys pMods) override;

	void update() override;

	void addAngle(const glm::vec2 &pAngleToAdd) { setAngle(angle + pAngleToAdd); }

	void setAngle(const glm::vec2 &pNewAngle);

	void setViewportAspect(float pAspect);

	sigc::connection connectOnMatrixChangedSignal(const sigc::slot<void(const glm::mat4 &pMatrix)> &pSlot) {
		return onMatrixChangedSignal.connect(pSlot);
	}

private:
	void updateMatrix();
};
} // namespace mer::sdk::main

#endif //CAMERA_H
