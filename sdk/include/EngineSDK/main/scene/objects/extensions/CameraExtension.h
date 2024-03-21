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
// Created by alexus on 28.02.24.
//

#ifndef CAMERAEXTENSION_H
#define CAMERAEXTENSION_H
#include <glm/gtx/polar_coordinates.hpp>
#include <glm/mat4x4.hpp>

#include "Extension.h"

namespace mer::sdk::main {
class CameraExtension : public Extension {
	glm::mat4 matrix{1};
	ValueChangedArgs<const glm::mat4 &> onMatrixChanged;
	glm::vec3 direction{};
	ValueChanged onDirectionChanged;
	glm::vec2 angle{};
	ValueChangedArgs<const glm::vec2 &> onAngleChanged;
	float aspect{};
	ValueChanged onAspectChanged;
	float fov{};
	ValueChanged onFovChanged;
	float zNear{};
	ValueChanged onZNearChanged;
	float zFar{};
	ValueChanged onZFarChanged;

protected:
	CameraExtension();

public:
	METHOD_CREATE(CameraExtension)

	EXT_TYPE_NAME("CameraExtension")

	[[nodiscard]] ValueChangedArgs<const glm::mat4 &> &getOnMatrixChanged() { return onMatrixChanged; }

	[[nodiscard]] const glm::mat4 &getMatrix() const { return matrix; }

	[[nodiscard]] ValueChanged &getOnDirectionChanged() { return onDirectionChanged; }

	[[nodiscard]] const glm::vec3 &getDirection() const { return direction; }

	[[nodiscard]] ValueChangedArgs<const glm::vec2 &> &getOnAngleChanged() { return onAngleChanged; }

	[[nodiscard]] const glm::vec2 &getAngle() const { return angle; }

	void setAngle(const glm::vec2 &pAngle) {
		if (angle == pAngle) return;
		glm::vec2 angleTmp = pAngle;
		constexpr float delta = 0.001f;
		if (angleTmp.x > 90.0f - delta) angleTmp.x = 90.0f - delta;
		if (angleTmp.x < -90.0f + delta) angleTmp.x = -90.0f + delta;
		if (angleTmp.y > 360) angleTmp.y -= 360;
		if (angleTmp.y < 0) angleTmp.y += 360;
		if (angle == angleTmp) return;
		angle = angleTmp;
		onAngleChanged(angle);
		setDirection(euclidean(radians(angleTmp)));
	}

	void addAngle(const glm::vec2 &pAngleToAdd) { setAngle(getAngle() + pAngleToAdd); }

	[[nodiscard]] ValueChanged &getOnAspectChanged() { return onAspectChanged; }

	[[nodiscard]] float getAspect() const { return aspect; }

	void setAspect(const float pAspect) {
		aspect = pAspect;
		onAspectChanged();
		updateMatrix();
	}

	[[nodiscard]] float getFov() const { return fov; }

	void setFov(const float pFov) {
		fov = pFov;
		onFovChanged();
		updateMatrix();
	}

	[[nodiscard]] ValueChanged &getOnFovChanged() { return onFovChanged; }

	[[nodiscard]] float getZNear() const { return zNear; }

	void setZNear(const float pZNear) {
		zNear = pZNear;
		onZNearChanged();
		updateMatrix();
	}

	[[nodiscard]] ValueChanged &getOnZNearChanged() { return onZNearChanged; }

	[[nodiscard]] float getZFar() const { return zFar; }

	void setZFar(const float pZFar) {
		zFar = pZFar;
		onZFarChanged();
		updateMatrix();
	}

	[[nodiscard]] ValueChanged &getOnZFarChanged() { return onZFarChanged; }

protected:
	utils::ReportMessagePtr onInit() override;

	void setMatrix(const glm::mat4 &pMatrix) {
		matrix = pMatrix;
		onMatrixChanged(pMatrix);
	}

	void setDirection(const glm::vec3 &pDirection) {
		direction = pDirection;
		onDirectionChanged();
		updateMatrix();
	}

	void onWindowSizeChanged(int pWidth, int pHeight) override;

private:
	void updateMatrix();

	void getProperties(std::vector<std::shared_ptr<ExtensionPropertyBase>> &pProperties) override;
};
} // namespace mer::sdk::main


#endif //CAMERAEXTENSION_H
