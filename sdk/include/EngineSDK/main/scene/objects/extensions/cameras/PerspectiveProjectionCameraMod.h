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
// Created by alexus on 22.03.24.
//

#ifndef PROJECTIONCAMERAMOD_H
#define PROJECTIONCAMERAMOD_H
#include <glm/mat4x4.hpp>
#include <memory>
#include <sigc++/signal.h>
#include <vector>

#include "IProjectionCameraMod.h"

namespace mer::sdk::main {

class PerspectiveProjectionCameraMod : public IProjectionCameraMod {
	glm::mat4 projMatrix{1};
	sigc::signal<void(const glm::mat<4, 4, float> &)> onProjMatrixChanged;
	float aspect{};
	sigc::signal<void()> onAspectChanged;
	float fov{};
	sigc::signal<void()> onFovChanged;
	float zNear{};
	sigc::signal<void()> onZNearChanged;
	float zFar{};
	sigc::signal<void()> onZFarChanged;

protected:
	PerspectiveProjectionCameraMod();

public:
	[[nodiscard]] sigc::signal<void(const glm::mat4 &)> &getOnProjMatrixChanged() override {
		return onProjMatrixChanged;
	}

	[[nodiscard]] const glm::mat4 &getProjMatrix() const override { return projMatrix; }

	[[nodiscard]] sigc::signal<void()> &getOnAspectChanged() { return onAspectChanged; }

	[[nodiscard]] float getAspect() const { return aspect; }

	void setAspect(const float pAspect) {
		aspect = pAspect;
		onAspectChanged();
		updateProjMatrix();
	}

	[[nodiscard]] float getFov() const { return fov; }

	void setFov(const float pFov) {
		fov = pFov;
		onFovChanged();
		updateProjMatrix();
	}

	[[nodiscard]] sigc::signal<void()> &getOnFovChanged() { return onFovChanged; }

	[[nodiscard]] float getZNear() const { return zNear; }

	void setZNear(const float pZNear) {
		zNear = pZNear;
		onZNearChanged();
		updateProjMatrix();
	}

	[[nodiscard]] sigc::signal<void()> &getOnZNearChanged() { return onZNearChanged; }

	[[nodiscard]] float getZFar() const { return zFar; }

	void setZFar(const float pZFar) {
		zFar = pZFar;
		onZFarChanged();
		updateProjMatrix();
	}

	[[nodiscard]] sigc::signal<void()> &getOnZFarChanged() { return onZFarChanged; }

protected:
	void getProjectionProperties(ExtensionProperties &pProperties) override;

	virtual void projectionMatrixChanged(const glm::mat4 &pNewMatrix);

private:
	void updateProjMatrix();
};


} // namespace mer::sdk::main

#endif //PROJECTIONCAMERAMOD_H
