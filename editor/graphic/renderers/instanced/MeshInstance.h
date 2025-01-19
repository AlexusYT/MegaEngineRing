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
// Created by alexus on 29.07.23.
//

#ifndef MAIN_MESHINSTANCE_H
#define MAIN_MESHINSTANCE_H
#if

	#include <glm/gtx/euler_angles.hpp>

	#include "InstancedSceneMeshFwd.h"
	#include "MeshInstanceData.h"
	#include "MeshInstanceFwd.h"
	#include "graphic/renderers/MeshMaterial.h"
	#include "graphic/viewport/OpenGLFwd.h"

struct MeshFocusData {
	glm::vec3 playerPos;
	glm::vec2 playerAngle;
	glm::vec3 maxPos;
	glm::vec3 minPos;
};

class MeshInstance {
	friend InstancedSceneMesh;

private:
	OpenGL* render{};

	std::string name;
	glm::vec3 posGlobal{};
	glm::vec3 scale{1};
	glm::vec3 rotation{};
	glm::vec3 posLocal{};
	glm::vec3 posCorrection{};
	bool visible{};
	bool castsShadows = true;
	MeshInstanceData data{};
	InstancedSceneMesh* mesh{};
	MeshInstance* parent{};
	std::vector<MeshInstance*> children;
	std::vector<MeshInstance*> childrenInterior;

public:
	template<typename T, typename... Args>
	static MeshInstanceUPtr create(Args... args) {
		return MeshInstanceUPtr(new (std::nothrow) T(args...));
	}

	MeshInstance() {
		MeshInstance::setMaterial({{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 1.0f});
	}

	virtual ~MeshInstance() = default;

	[[nodiscard]] const glm::vec3 &getPosLocal() const { return posLocal; }

	virtual void setPosLocal(const glm::vec3 &newPosLocal) {
		MeshInstance::posLocal = newPosLocal;
		updateGlobalPos();
	}

	[[nodiscard]] const glm::vec3 &getPosCorrection() const { return posCorrection; }

	void setPosCorrection(const glm::vec3 &newPosCorrection) {
		MeshInstance::posCorrection = newPosCorrection;
		updateGlobalPos();
	}

	[[nodiscard]] bool isVisible() const { return visible; }

	virtual void setVisible(bool newVisible) {
		MeshInstance::visible = newVisible;
		updateData();
	}

	//NOLINTNEXTLINE
	void setVisibleRecursive(bool newVisible = true) {
		setVisible(newVisible);
		for (const auto &child: children) { child->setVisibleRecursive(newVisible); }
	}

	//NOLINTNEXTLINE
	void setVisibleRecursiveInterior(bool newVisible = true) {
		setVisible(newVisible);
		for (const auto &child: childrenInterior) { child->setVisibleRecursiveInterior(newVisible); }
	}

	[[nodiscard]] const glm::vec3 &getPosGlobal() const { return posGlobal; }

	[[nodiscard]] const glm::vec3 &getScale() const { return scale; }

	virtual void setScale(const glm::vec3 &newScale) {
		scale = newScale;
		updateModelViewMatrix();
	}

	[[nodiscard]] const glm::vec3 &getRotation() const { return rotation; }

	virtual void setRotation(const glm::vec3 &newRotation) {
		rotation = newRotation;
		updateModelViewMatrix();
	}

	[[nodiscard]] const MeshMaterial &getMaterial() const { return data.material; }

	virtual void setMaterial(const MeshMaterial &newMaterial) {
		data.material = newMaterial;
		updateData();
	}

	[[nodiscard]] const glm::mat4 &getModelViewMatrix() const { return data.modelViewMatrix; }

	[[nodiscard]] bool isLighted() const { return data.lighted; }

	virtual void setLighted(bool state) {
		data.lighted = state;
		updateData();
	}

	void setType(uint32_t type) {
		data.type = type;
		updateData();
	}

	[[nodiscard]] bool isCastsShadows() const { return castsShadows; }

	void setCastsShadows(bool state) { MeshInstance::castsShadows = state; }

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &newName) { MeshInstance::name = newName; }

	[[nodiscard]] OpenGL* getRender() const { return render; }

	[[nodiscard]] InstancedSceneMesh* getMesh() const { return mesh; }

	void addChild(MeshInstance* child);

	[[nodiscard]] MeshInstance* getParent() const { return parent; }

	[[nodiscard]] const std::vector<MeshInstance*> &getChildren() const { return children; }

	[[nodiscard]] const std::vector<MeshInstance*> &getChildrenInterior() const { return childrenInterior; }

	//NOLINTNEXTLINE(misc-no-recursion)
	void getChildrenInteriorRecursive(std::vector<MeshInstance*> &childrenRecursive) const {
		childrenRecursive.insert(childrenRecursive.end(), childrenInterior.begin(), childrenInterior.end());
		for (const auto &item: childrenInterior) { item->getChildrenInteriorRecursive(childrenRecursive); }
	}

	void activateObject() { onActivated(); }

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-parameter"

	virtual bool getFocusData(MeshFocusData &focusData) { return false; } // NOLINT(*-unused-parameters)
protected:
	virtual void onGlobalPosChanged(const glm::vec3 &newPos) {}

	virtual void onParentGlobalPosChanged(const glm::vec3 &newPos) {}

	virtual void onWorldChanged(OpenGL* newWorld) {}

	virtual void onInstanceAdded() {}

	virtual void onActivated() {}

	virtual void onUpdateStarted() {}

	virtual void onUpdateStopped() {}

	virtual void onUpdate(double deltaTime) {}

	//NOLINTNEXTLINE(misc-no-recursion)
	virtual void setPosGlobal(const glm::vec3 &newPos) {

		if (MeshInstance::posGlobal == posCorrection + newPos) return;
		MeshInstance::posGlobal = posCorrection + newPos;
		updateModelViewMatrix();
		for (const auto &child: children) { child->parentGlobalPosChanged(MeshInstance::posGlobal); }
		onGlobalPosChanged(MeshInstance::posGlobal);
	}

	#pragma GCC diagnostic pop
private:
	void setWorld(OpenGL* newWorld) {
		if (MeshInstance::render == newWorld) return;
		MeshInstance::render = newWorld;
		onWorldChanged(newWorld);
	}

	void setParentMesh(MeshInstance* newParent) {
		MeshInstance::parent = newParent;
		updateGlobalPos();
	}

	//NOLINTNEXTLINE(misc-no-recursion)
	void parentGlobalPosChanged(const glm::vec3 &newPos) {
		updateGlobalPos();
		onParentGlobalPosChanged(newPos);
	}

	//NOLINTNEXTLINE(misc-no-recursion)
	void updateGlobalPos() {

		if (parent) setPosGlobal(parent->getPosGlobal() + posLocal);
		else
			setPosGlobal(posLocal);
	}

	void setMesh(InstancedSceneMesh* newMesh) { MeshInstance::mesh = newMesh; }

	void updateModelViewMatrix() {

		data.modelViewMatrix =
			glm::translate(glm::mat4(1), posGlobal) *
			glm::eulerAngleXZY(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)) *
			glm::scale(glm::mat4(1), scale);
		updateData();
	}

	void updateData();
};

#endif


#endif //MAIN_MESHINSTANCE_H
