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
// Created by alexus on 17.01.24.
//

#include "ViewObjectProperties.h"

#include "EngineSDK/extensions/MainObjectExtension.h"
#include "EngineSDK/gltf/Material.h"
#include "EngineSDK/gltf/Mesh.h"
#include "EngineSDK/gltf/MeshInstance.h"
#include "EngineSDK/gltf/Primitive.h"
#include "EngineSDK/scene/Scene3D.h"
#include "EngineSDK/utils/Transformation.h"
#include "mvp/contexts/IWidgetContext.h"

namespace mer::editor::mvp {
ViewObjectProperties::ViewObjectProperties(const std::shared_ptr<IWidgetContext> &pContext)
	: EditorTool("ViewObjectPropertiesTool"), context(pContext) {}

void ViewObjectProperties::onUpdate(bool /*pVisible*/) {
	if (!ImGui::BeginTabBar("MyTabBar", 0)) return;
	if (ImGui::BeginTabItem("Scene", nullptr, 0)) { ImGui::EndTabItem(); }

	// ReSharper disable once CppDFAConstantConditions
	if (selectedNode) {
		// ReSharper disable once CppDFAUnreachableCode
		if (ImGui::BeginTabItem("Instance", nullptr, ImGuiTabItemFlags_SetSelected)) {

			auto nodeName = selectedNode->getName();
			if (ImGui::InputText("Name", nodeName)) selectedNode->setName(nodeName);
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			if (ImGui::TreeNodeEx("Transformation", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
				drawTransformation();
				ImGui::TreePop();
			}
			if (auto meshNode = dynamic_cast<sdk::MeshInstance*>(selectedNode)) {

				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
					drawMaterial(meshNode);
					ImGui::TreePop();
				}
			}
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
}

void ViewObjectProperties::openView() { context->addTool(this); }

void ViewObjectProperties::closeView() { context->removeWidget(); }

bool DragProperty(const char* pLabel, float* pV, float pSpeed, const char* pRoundFormat, const char* pDisplayFormat) {
	return ImGui::DragFloat(pLabel, pV, pSpeed, 0, 0, pRoundFormat, pDisplayFormat, ImGuiSliderFlags_NoSpeedTweaks);
}

int rotationUnit = 1;

void ViewObjectProperties::drawTransformation() {
	float speed = 0.005f;
	const char* roundFormat = "%.2f";
	if (ImGui::IsWindowFocused()) {
		auto ctrlDown = ImGui::IsKeyDown(ImGuiMod_Ctrl);
		auto shiftDown = ImGui::IsKeyDown(ImGuiMod_Shift);
		if (ctrlDown && !shiftDown) { //Only ctrl is pressed
			roundFormat = "%0.0f";
			speed = 0.01f;
		} else if (!ctrlDown && shiftDown) { //Only shift is pressed
			roundFormat = "%0.3f";
			speed = 0.0005f;
		} else if (ctrlDown && shiftDown) { //Both ctrl and shift are pressed
			roundFormat = "%0.1f";
			speed = 0.005f;
		}
	}
	auto &local = selectedNode->getLocalTransform();

	ImGui::Text("Position");
	auto pos = local->getPosition();
	bool posChanged = false;
	posChanged |= DragProperty("X##pos", &pos.x, speed, roundFormat, "%0.3f");
	posChanged |= DragProperty("Y##pos", &pos.y, speed, roundFormat, "%0.3f");
	posChanged |= DragProperty("Z##pos", &pos.z, speed, roundFormat, "%0.3f");
	if (posChanged) local->setPosition(pos);

	ImGui::Text("Rotation");
	static int curType = 0;
	ImGui::Combo("Type", &curType, "Quaternion\0XYZ Euler\0Axis Angle\0");
	bool rotChanged = false;
	auto rotationQuat = local->getRotation();
	if (curType == 0) {
		rotChanged |= DragProperty("W##rotQuat", &rotationQuat.w, speed, roundFormat, "%0.3f");
		rotChanged |= DragProperty("X##rotQuat", &rotationQuat.x, speed, roundFormat, "%0.3f");
		rotChanged |= DragProperty("Y##rotQuat", &rotationQuat.y, speed, roundFormat, "%0.3f");
		rotChanged |= DragProperty("Z##rotQuat", &rotationQuat.z, speed, roundFormat, "%0.3f");
		if (rotChanged) local->setRotationQuaternion(glm::normalize(rotationQuat));
	} else if (curType == 1) {
		glm::vec3 euler = glm::eulerAngles(rotationQuat);
		const char* displayFormat = "%0.3f";
		if (rotationUnit == 1) {
			euler = glm::degrees(euler);
			displayFormat = "%0.3f°";
		}
		rotChanged |= ImGui::DragFloat("X##rotEuler", &euler.x, speed * 10, 0, 0, roundFormat, displayFormat,
									   ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_WrapAround);
		rotChanged |= ImGui::DragFloat("Y##rotEuler", &euler.y, speed * 10, 0, 0, roundFormat, displayFormat,
									   ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_WrapAround);
		rotChanged |= ImGui::DragFloat("Z##rotEuler", &euler.z, speed * 10, 0, 0, roundFormat, displayFormat,
									   ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_WrapAround);
		if (rotationUnit == 1) {
			/*if (euler.y > 90) {
						euler.y = 90;
						euler.x += 180;
						euler.z -= 180;
					}
					if (euler.z > 180) { euler.z = 360 - euler.z; }*/
			/*if (euler.y > 90.f) {
						euler.x -= 180.f;
						euler.y -= 180.f;
						euler.y *= -1.f;
						euler.z += 180.f;

						if (euler.x > 0.f) { euler.x += 360.f; }
					}*/
			euler = glm::radians(euler);
		}
		if (rotChanged) local->setRotationQuaternion(glm::fquat{euler});
	} else {
		auto axisVal = axis(rotationQuat);
		auto angleVal = angle(rotationQuat);
		if (rotationUnit == 1) { angleVal = glm::degrees(angleVal); }
		rotChanged |=
			DragProperty("W##rotQuat", &angleVal, speed * 10, roundFormat, rotationUnit == 1 ? "%0.3f" : "%0.3f°");
		rotChanged |= DragProperty("X##rotQuat", &axisVal.x, speed, roundFormat, "%0.3f");
		rotChanged |= DragProperty("Y##rotQuat", &axisVal.y, speed, roundFormat, "%0.3f");
		rotChanged |= DragProperty("Z##rotQuat", &axisVal.z, speed, roundFormat, "%0.3f");
		if (rotationUnit == 1) { angleVal = glm::radians(angleVal); }
		if (rotChanged) local->setRotationQuaternion(glm::normalize(glm::angleAxis(angleVal, axisVal)));
	}


	auto scale = local->getScale();
	bool scaleChanged = false;
	ImGui::Text("Scale");
	scaleChanged |= DragProperty("X##scale", &scale.x, speed, roundFormat, "%0.3f");
	scaleChanged |= DragProperty("Y##scale", &scale.y, speed, roundFormat, "%0.3f");
	scaleChanged |= DragProperty("Z##scale", &scale.z, speed, roundFormat, "%0.3f");
	if (scaleChanged) local->setScale(scale);
}

void ViewObjectProperties::drawMaterial(sdk::MeshInstance* pMeshNode) {
	auto mesh = pMeshNode->getMesh();
	std::shared_ptr<sdk::Primitive> curPrimitive{};
	for (auto primitive: mesh->getPrimitives()) {
		curPrimitive = primitive;
		break; //TODO Take only first primitive. For now.
	}
	if (!curPrimitive) {
		ImGui::Text("No primitives in mesh. This message is a stub");
		return;
	}
	auto selectedMaterial = curPrimitive->getMaterial();

	const char* comboPreviewValue = selectedMaterial == nullptr ? "None" : selectedMaterial->getName().c_str();
	if (ImGui::BeginCombo("Material", comboPreviewValue, 0)) {
		if (scene) {
			bool isSelected = selectedMaterial == nullptr;
			if (ImGui::Selectable("None", isSelected)) {
				selectedMaterial = nullptr;
				curPrimitive->setMaterial(nullptr);
			}
			if (isSelected) ImGui::SetItemDefaultFocus();
			for (auto material: scene->getMaterials()) {
				isSelected = selectedMaterial == material;
				if (ImGui::Selectable(material->getName().c_str(), isSelected)) {
					selectedMaterial = material;
					curPrimitive->setMaterial(material);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isSelected) ImGui::SetItemDefaultFocus();
			}
		} else {
			ImGui::Text("Programmer error. No Scene specified for %s", __FILE_NAME__);
		}
		ImGui::EndCombo();
	}
}
} // namespace mer::editor::mvp