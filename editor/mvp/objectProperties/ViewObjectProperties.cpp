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

#include <glm/ext/scalar_common.hpp>

#include "EngineSDK/extensions/ExtensionRegistry.h"
#include "EngineSDK/extensions/LightExtension.h"
#include "EngineSDK/extensions/MainObjectExtension.h"
#include "EngineSDK/extensions/MeshExtension.h"
#include "EngineSDK/gltf/Light.h"
#include "EngineSDK/gltf/Material.h"
#include "EngineSDK/gltf/Mesh.h"
#include "EngineSDK/gltf/Node.h"
#include "EngineSDK/gltf/Primitive.h"
#include "EngineSDK/scene/Scene3D.h"
#include "EngineSDK/utils/Transformation.h"
#include "imgui_internal.h"
#include "mvp/contexts/UiWindowContext.h"

namespace mer::editor::mvp {
std::unordered_map<std::type_index, std::function<ImGuiID(const std::shared_ptr<sdk::Extension> &pExt)>>
ViewObjectProperties::extRenderers;

ViewObjectProperties::ViewObjectProperties(const std::shared_ptr<IWidgetContext> &pContext)
	: EditorTool("ViewObjectPropertiesTool"), context(pContext) {
	extRenderers.emplace(std::type_index(typeid(sdk::MeshExtension)),
						 [this](const std::shared_ptr<sdk::Extension> &pExt) {
							 return this->drawMeshTab(std::dynamic_pointer_cast<sdk::MeshExtension>(pExt));
						 });
	extRenderers.emplace(std::type_index(typeid(sdk::LightExtension)),
						 [this](const std::shared_ptr<sdk::Extension> &pExt) {
							 return this->drawLightTab(std::dynamic_pointer_cast<sdk::LightExtension>(pExt));
						 });
}

void ViewObjectProperties::onUpdate(bool /*pVisible*/) {
	if (!scene) {
		ImGui::Text("Programmer error. No Scene specified for %s", __FILE_NAME__);
		return;
	}
	if (!ImGui::BeginTabBar("MyTabBar", 0)) return;
	if (ImGui::BeginTabItem(tr("SceneProperties"), nullptr, 0)) { ImGui::EndTabItem(); }
	// ReSharper disable once CppDFAConstantConditions
	if (selectedNode) {
		// ReSharper disable once CppDFAUnreachableCode
		if (ImGui::BeginTabItem(tr("NodeProperties"), nullptr, ImGuiTabItemFlags_None)) {
			auto nodeName = selectedNode->getName();
			if (ImGui::InputText(tr("NodeName"), nodeName)) selectedNode->setName(nodeName);
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			if (ImGui::TreeNodeEx(tr("NodeTransformation"),
								  ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
				drawTransformation();
				ImGui::TreePop();
			}
			ImGui::EndTabItem();
		}
		auto hoveredItem = ImGui::GetHoveredID();
		static std::optional<std::type_index> contextExtension;
		for (auto &value: selectedNode->getExtensions()) {
			if (auto iter = extRenderers.find(value.first); iter != extRenderers.end()) {
				auto curItem = iter->second(value.second);

				if (hoveredItem == curItem && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
					ImGui::OpenPopup("ExtensionContextMenu");
					contextExtension = value.first;
				}
			}
		}
		if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
			ImGui::OpenPopup("AddExtensionMenu");
		}
		if (ImGui::BeginPopup("AddExtensionMenu")) {
			for (auto &extension: sdk::ExtensionRegistry::getExtensionsByType()) {
				if (selectedNode->hasExtension(extension.first)) continue;
				try {
					if (ImGui::MenuItem(I18n::trExtensionsMap.at(extension.second.typeName).c_str())) {
						selectedNode->addExtension(extension.second.createFunc());
					}
				}
				catch (...) {}
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopup("ExtensionContextMenu")) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

			ImGui::BeginDisabled(!contextExtension.has_value());
			if (confirmationMenuItem(tr("RemoveExtension"))) {
				selectedNode->removeExtension(contextExtension.value());
			}
			ImGui::EndDisabled();
			ImGui::PopStyleColor();
			//translators: instruction for removing an element (extension, object, etc)
			ImGui::SetItemTooltip("%s", tr("Hold for 0.5 sec to confirm removing"));
			ImGui::EndPopup();
		} else
			contextExtension.reset();
	}
	ImGui::EndTabBar();
}

bool ViewObjectProperties::confirmationMenuItem(const char* pLabel, const char* pShortcut, float pTimeout,
												bool pSelected, bool pEnabled) {
	ImGui::PushItemFlag(ImGuiItemFlags_AutoClosePopups, false);
	ImGui::MenuItem(pLabel, pShortcut, pSelected, pEnabled);
	ImGui::PopItemFlag();
	if (!ImGui::IsItemHovered() || !ImGui::IsItemFocused()) return false;


	auto mouseButton = ImGuiMouseButton_Left;
	const ImGuiContext &g = *GImGui;
	const ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImGuiStyle &style = g.Style;

	const ImVec2 pos = window->DC.CursorPos - style.FramePadding;
	const auto size = ImVec2(ImGui::GetItemRectSize().x, 5);

	const ImRect bb(pos, pos + size);

	float fraction = glm::max(g.ActiveIdTimer, 0.0f, g.IO.MouseDownDuration[mouseButton]) / pTimeout;
	constexpr float fillN0 = 0.0f;
	const float fillN1 = std::isnan(fraction) ? 0.0f : fraction;

	ImGui::RenderRectFilledRangeH(window->DrawList, bb, ImGui::GetColorU32(ImGuiCol_PlotHistogram), fillN0, fillN1,
								  style.FrameRounding);

	bool confirmed = glm::max(g.LastActiveIdTimer, 0.0f) / pTimeout >= 1.0f && ImGui::IsItemDeactivated();
	confirmed = confirmed || (glm::max(0.0f, g.IO.MouseDownDurationPrev[mouseButton]) / pTimeout >= 1.0f &&
							  g.IO.MouseReleased[mouseButton]);

	if (confirmed && (window->Flags & ImGuiWindowFlags_Popup) && (g.CurrentItemFlags & ImGuiItemFlags_AutoClosePopups))
		ImGui::CloseCurrentPopup();
	return confirmed;
}

void ViewObjectProperties::openView() { context->add(this); }

void ViewObjectProperties::closeView() { context->remove(); }

bool DragProperty(const char* pLabel, float* pV, float pSpeed, const char* pRoundFormat, const char* pDisplayFormat) {
	ImGui::SetNextItemWidth(-1);
	return ImGui::DragFloat(pLabel, pV, pSpeed, 0, 0, pRoundFormat, pDisplayFormat, ImGuiSliderFlags_NoSpeedTweaks);
}

void ViewObjectProperties::drawTransformation() {
	float speed;
	const char* roundFormat;
	getSpeeds(speed, roundFormat);
	auto &local = selectedNode->getLocalTransform();

	ImGui::TextUnformatted(tr("NodePosition"));
	auto pos = local->getPosition();
	bool posChanged = false;
	posChanged |= DragProperty("##posX", &pos.x, speed, roundFormat, "X: %0.3f");
	posChanged |= DragProperty("##posY", &pos.y, speed, roundFormat, "Y: %0.3f");
	posChanged |= DragProperty("##posZ", &pos.z, speed, roundFormat, "Z: %0.3f");
	if (posChanged) local->setPosition(pos);

	ImGui::TextUnformatted(tr("NodeOrientation"));
	static size_t curType = 0;
	std::array types = {trs("Quaternion"), trs("XYZ Euler"), trs("Axis Angle")};
	ImGui::SetNextItemWidth(-1);
	if (ImGui::BeginCombo("##NodeOrientationType", types[curType].c_str())) {
		for (size_t i = 0; i < types.size(); i++) {
			if (ImGui::Selectable(types[i].c_str(), curType == i)) curType = i;
		}
		ImGui::EndCombo();
	}

	bool rotChanged = false;
	auto rotationQuat = local->getRotation();
	if (curType == 0) {
		rotChanged |= DragProperty("##rotQuatW", &rotationQuat.w, speed, roundFormat, "W: %0.3f");
		rotChanged |= DragProperty("##rotQuatX", &rotationQuat.x, speed, roundFormat, "X: %0.3f");
		rotChanged |= DragProperty("##rotQuatY", &rotationQuat.y, speed, roundFormat, "Y: %0.3f");
		rotChanged |= DragProperty("##rotQuatZ", &rotationQuat.z, speed, roundFormat, "Z: %0.3f");
		if (rotChanged) local->setRotationQuaternion(glm::normalize(rotationQuat));
	} else if (curType == 1) {
		glm::vec3 euler = glm::eulerAngles(rotationQuat);
		auto displayFormatX = std::format("X: %0.3f{}", getAngleUnitStr());
		auto displayFormatY = std::format("Y: %0.3f{}", getAngleUnitStr());
		auto displayFormatZ = std::format("Z: %0.3f{}", getAngleUnitStr());
		if (isDegrees()) { euler = glm::degrees(euler); }
		ImGui::SetNextItemWidth(-1);
		rotChanged |= ImGui::DragFloat("##rotEulerX", &euler.x, speed * 10, 0, 0, roundFormat, displayFormatX.c_str(),
									   ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_WrapAround);
		ImGui::SetNextItemWidth(-1);
		rotChanged |= ImGui::DragFloat("##rotEulerY", &euler.y, speed * 10, 0, 0, roundFormat, displayFormatY.c_str(),
									   ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_WrapAround);
		ImGui::SetNextItemWidth(-1);
		rotChanged |= ImGui::DragFloat("##rotEulerZ", &euler.z, speed * 10, 0, 0, roundFormat, displayFormatZ.c_str(),
									   ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_WrapAround);
		if (isDegrees()) {
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
		auto displayFormat = std::format("W: %0.3f{}", getAngleUnitStr());
		if (isDegrees()) { angleVal = glm::degrees(angleVal); }
		rotChanged |= DragProperty("##rotQuatW", &angleVal, speed * 10, roundFormat, displayFormat.c_str());
		rotChanged |= DragProperty("##rotQuatX", &axisVal.x, speed, roundFormat, "X: %0.3f");
		rotChanged |= DragProperty("##rotQuatY", &axisVal.y, speed, roundFormat, "Y: %0.3f");
		rotChanged |= DragProperty("##rotQuatZ", &axisVal.z, speed, roundFormat, "Z: %0.3f");
		if (isDegrees()) { angleVal = glm::radians(angleVal); }
		if (rotChanged) local->setRotationQuaternion(glm::normalize(glm::angleAxis(angleVal, axisVal)));
	}


	auto scale = local->getScale();
	bool scaleChanged = false;
	ImGui::TextUnformatted(tr("Scale"));
	scaleChanged |= DragProperty("##scaleX", &scale.x, speed, roundFormat, "X: %0.3f");
	scaleChanged |= DragProperty("##scaleY", &scale.y, speed, roundFormat, "Y: %0.3f");
	scaleChanged |= DragProperty("##scaleZ", &scale.z, speed, roundFormat, "Z: %0.3f");
	if (scaleChanged) local->setScale(scale);
}

ImGuiID ViewObjectProperties::drawMeshTab(const std::shared_ptr<sdk::MeshExtension> &pExt) {
	if (!ImGui::BeginTabItem(tr("MeshExtension"), nullptr, ImGuiTabItemFlags_None)) return ImGui::GetItemID();
	auto id = ImGui::GetItemID();

	ImGui::TextUnformatted(tr("Select mesh to use"));

	auto selectedMesh = pExt->mesh.getValue();
	if (ImGui::BeginListBox("##MeshListBox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
		bool isSelected = selectedMesh == nullptr;
		if (ImGui::Selectable(tr("MeshNone"), isSelected)) {
			selectedMesh = nullptr;
			pExt->mesh.setValue(nullptr);
		}
		if (isSelected) ImGui::SetItemDefaultFocus();
		for (auto mesh: scene->getMeshes()) {
			isSelected = selectedMesh == mesh;
			if (ImGui::Selectable(mesh->getName().c_str(), isSelected)) {
				selectedMesh = mesh;
				pExt->mesh.setValue(mesh);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (isSelected) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}
	ImGui::EndTabItem();
	return id;
}

ImGuiID ViewObjectProperties::drawLightTab(const std::shared_ptr<sdk::LightExtension> &pExt) {
	if (!ImGui::BeginTabItem(tr("LightExtension"), nullptr, ImGuiTabItemFlags_None)) return ImGui::GetItemID();
	auto id = ImGui::GetItemID();
	auto &lights = scene->getLights();
	auto selectedLightId = pExt->lightDataId.getValue();
	auto selectedLight = selectedLightId == -1 ? nullptr : lights.at(static_cast<size_t>(selectedLightId));
	ImGui::TextUnformatted(tr("Select light source to use"));

	if (ImGui::BeginListBox("##LightSourcesListBox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
		bool isSelected = selectedLightId == -1;
		int32_t hoveredLightId = -1;
		if (ImGui::Selectable(tr("LightNone"), isSelected)) {
			selectedLightId = -1;
			pExt->lightDataId.setValue(-1);
		}
		ImGui::OpenPopupOnItemClick("ContextMenu");
		if (isSelected) ImGui::SetItemDefaultFocus();
		for (int32_t i = 0; i < static_cast<int32_t>(lights.size()); i++) {
			isSelected = selectedLightId == i;
			if (ImGui::Selectable(lights.at(static_cast<size_t>(i))->getName().c_str(), isSelected)) {
				selectedLightId = i;
				pExt->lightDataId.setValue(i);
			}
			ImGui::OpenPopupOnItemClick("ContextMenu");
			if (ImGui::IsItemHovered()) { hoveredLightId = i; }
			if (isSelected) ImGui::SetItemDefaultFocus();
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonRight) &&
			!ImGui::IsPopupOpen("ContextMenu") && hoveredLightId == -1) { ImGui::OpenPopup("ContextMenu"); }

		if (ImGui::BeginPopup("ContextMenu")) {
			static int32_t lightId = -1;
			if (ImGui::IsWindowAppearing()) lightId = hoveredLightId;
			if (ImGui::MenuItem(tr("NewLight"))) {
				auto newLight = sdk::Light::create(tr("LightUnnamed"));
				scene->addLightSource(newLight);
			}
			if (lightId != -1) {
				if (ImGui::MenuItem(tr("DuplicateLight"))) {
					auto &hoveredLight = lights.at(static_cast<size_t>(lightId));
					auto newLight =
						hoveredLight->duplicate(std::format("{} ({})", hoveredLight->getName(), tr("LightCopy")));
					scene->addLightSource(newLight);
				}
				ImGui::BeginDisabled();
				if (ImGui::MenuItem(tr("RemoveLight"))) {}
				ImGui::SetItemTooltip("%s", tr("Not implemented yet"));
				ImGui::EndDisabled();
			}
			ImGui::EndPopup();
		}


		ImGui::EndListBox();
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (selectedLight && ImGui::TreeNodeEx(tr("Light source settings"),
										   ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
		drawLightSourceSettings(selectedLight);
		ImGui::TreePop();
	}
	ImGui::EndTabItem();
	return id;
}

void ViewObjectProperties::drawLightSourceSettings(const std::shared_ptr<sdk::Light> &pLight) {
	float speed;
	const char* roundFormat;
	getSpeeds(speed, roundFormat);
	auto lightName = pLight->getName();
	if (ImGui::InputText(tr("LightName"), lightName)) pLight->setName(lightName);

	auto lightColor = pLight->getColor();
	if (ImGui::ColorEdit3(tr("LightColor"), &lightColor.x)) pLight->setColor(lightColor);

	auto intensity = pLight->getIntensity();
	ImGui::SetNextItemWidth(-1);
	if (std::string display = std::format("{}: %0.3f", tr("LightIntensity"));
		ImGui::DragFloat("##LightIntensity", &intensity, speed, 0.0f, FLT_MAX, roundFormat, display.c_str(),
						 ImGuiSliderFlags_NoSpeedTweaks))
		pLight->setIntensity(intensity);

	auto lightType = pLight->getType();
	std::array lightTypes = {trs("LightTypeDirectional"), trs("LightTypePoint"), trs("LightTypeSpot")};
	int lightTypeId = static_cast<int>(lightType);
	ImGui::SetNextItemWidth(-1);
	if (std::string display = std::format("{}: {}", tr("LightType"), lightTypes[static_cast<size_t>(lightTypeId)]);
		ImGui::SliderInt("##LightType", &lightTypeId, 0, static_cast<int>(sdk::LightType::LIGHT_TYPE_MAX) - 1,
						 display.c_str())) { pLight->setType(static_cast<sdk::LightType>(lightTypeId)); }

	if (lightType == sdk::LightType::POINT || lightType == sdk::LightType::SPOT) {
		float lightRange = pLight->getRange();
		ImGui::SetNextItemWidth(-1);
		if (std::string display =
				std::format("{}: {}", tr("LightRange"), lightRange == 0.0f ? tr("LightRangeInfinite") : tr("%0.3f m"));
			ImGui::DragFloat("##LightRange", &lightRange, speed, 0.0f, FLT_MAX, roundFormat, display.c_str(),
							 ImGuiSliderFlags_NoSpeedTweaks))
			pLight->setRange(lightRange);
	}
	if (lightType == sdk::LightType::SPOT) {
		float maxAngle = std::numbers::pi_v<float> / 2.0f;
		float lightInnerCone = pLight->getInnerConeAngle();
		float lightOuterCone = pLight->getOuterConeAngle();
		if (isDegrees()) {
			lightInnerCone = glm::degrees(lightInnerCone);
			lightOuterCone = glm::degrees(lightOuterCone);
			maxAngle = glm::degrees(maxAngle);
		}
		float lightInnerConeMax = std::min(maxAngle, lightOuterCone == 0.0f ? maxAngle : lightOuterCone);
		ImGui::SetNextItemWidth(-1);
		if (std::string display = std::format("{}: %0.3f{}", tr("Inner Cone"), getAngleUnitStr());
			ImGui::DragFloat("##InnerCone", &lightInnerCone, speed, 0.0f, lightInnerConeMax, roundFormat,
							 display.c_str(), ImGuiSliderFlags_NoSpeedTweaks)) {
			if (isDegrees()) lightInnerCone = glm::radians(lightInnerCone);
			pLight->setInnerConeAngle(lightInnerCone);
		}

		ImGui::SetNextItemWidth(-1);
		if (std::string display = std::format("{}: %0.3f{}", tr("Outer Cone"), getAngleUnitStr());
			ImGui::DragFloat("##OuterCone", &lightOuterCone, speed, std::max(0.0f, lightInnerCone), maxAngle,
							 roundFormat, display.c_str(), ImGuiSliderFlags_NoSpeedTweaks)) {
			if (isDegrees()) lightOuterCone = glm::radians(lightOuterCone);
			pLight->setOuterConeAngle(lightOuterCone);
		}
	}
}

/*
 //TODO
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
}*/
} // namespace mer::editor::mvp
