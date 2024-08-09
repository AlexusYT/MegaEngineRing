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

#include "EngineSDK/main/scene/objects/extensions/PropertiesForExtension.h"
#include "IProjectionCameraMod.h"

namespace mer::sdk::main {

class PerspectiveProjectionCameraMod : public IProjectionCameraMod, public virtual PropertiesForExtension {

	DECLARE_PROPERTY(float, Aspect);
	ADD_PROPERTY_SET_EVENT(PerspectiveProjectionCameraMod, Aspect, "Frame aspect", "");
	DECLARE_PROPERTY(float, Fov);
	ADD_PROPERTY_SET_EVENT(PerspectiveProjectionCameraMod, Fov, "Field of view", "");
	DECLARE_PROPERTY(float, ZNear);
	ADD_PROPERTY_SET_EVENT(PerspectiveProjectionCameraMod, ZNear, "Near z-plane", "");
	DECLARE_PROPERTY(float, ZFar);
	ADD_PROPERTY_SET_EVENT(PerspectiveProjectionCameraMod, ZFar, "Far z-plane", "");

	DECLARE_PROPERTY(glm::mat4, ProjMatrix){1};
	ADD_PROPERTY_EVENT(PerspectiveProjectionCameraMod, ProjMatrix, "Projection matrix", "");

protected:
	PerspectiveProjectionCameraMod();

public:
	[[nodiscard]] sigc::signal<void(const glm::mat4 &)> &getOnProjMatrixChanged() override {
		return onProjMatrixChanged;
	}

	[[nodiscard]] const glm::mat4 &getProjMatrix() const override { return propertyProjMatrix; }

	[[nodiscard]] sigc::signal<void(const float &)> &getOnAspectChanged() { return onAspectChanged; }

	[[nodiscard]] float getAspect() const { return propertyAspect; }

	void setAspect(const float pAspect) {
		propertyAspect = pAspect;
		onAspectChanged(pAspect);
		updateProjMatrix();
	}

	[[nodiscard]] float getFov() const { return propertyFov; }

	void setFov(const float pFov) {
		propertyFov = pFov;
		onFovChanged(pFov);
		updateProjMatrix();
	}

	[[nodiscard]] sigc::signal<void(const float &)> &getOnFovChanged() { return onFovChanged; }

	[[nodiscard]] float getZNear() const { return propertyZNear; }

	void setZNear(const float pZNear) {
		propertyZNear = pZNear;
		onZNearChanged(pZNear);
		updateProjMatrix();
	}

	[[nodiscard]] sigc::signal<void(const float &)> &getOnZNearChanged() { return onZNearChanged; }

	[[nodiscard]] float getZFar() const { return propertyZFar; }

	void setZFar(const float pZFar) {
		propertyZFar = pZFar;
		onZFarChanged(pZFar);
		updateProjMatrix();
	}

	[[nodiscard]] sigc::signal<void(const float &)> &getOnZFarChanged() { return onZFarChanged; }

protected:
	virtual void projectionMatrixChanged(const glm::mat4 &pNewMatrix);

private:
	void updateProjMatrix();
};


} // namespace mer::sdk::main

#endif //PROJECTIONCAMERAMOD_H
