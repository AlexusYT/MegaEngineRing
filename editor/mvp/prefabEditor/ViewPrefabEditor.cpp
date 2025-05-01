//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 15.02.25.
//

#include "ViewPrefabEditor.h"

#include <epoxy/gl.h>

#include "EngineSDK/buffers/ProgramWideShaderBuffer.h"
#include "EngineSDK/extensions/cameras/OrbitCameraExtension.h"
#include "EngineSDK/prefabs/Prefab.h"
#include "EngineSDK/prefabs/elements/MeshPrefabElement.h"
#include "EngineSDK/prefabs/elements/PrefabElement.h"
#include "EngineSDK/resources/shaders/builtin/DefaultProgram.h"
#include "EngineSDK/resources/shaders/builtin/PrefabProgram.h"
#include "IPresenterPrefabEditor.h"
#include "imgui_internal.h"
#include "mvp/contexts/IWidgetContext.h"
#include "mvp/sceneEditor/SceneOverlayElements.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "EngineSDK/buffers/Framebuffer.h"
#include "EngineSDK/gltf/GltfModel.h"

namespace mer::editor::mvp {
ViewPrefabEditor::ViewPrefabEditor(const std::shared_ptr<IWidgetContext> &pContext)
	: UiWindow("PrefabEditor", "Prefab Editor"), context(pContext) {
	auto prefab = prefabs.emplace_back(sdk::Prefab::create(), false).first;
	prefab->setName("Prefab");
	auto program = sdk::DefaultProgram::getInstance();
	if (auto msg = program->initialize()) sdk::Logger::error(msg);
	prefab->setShaderProgram(program);
	prefab->initialize();
	instance = sdk::PrefabInstance::create();
	prefab->addInstance(instance);
	overlay = SceneOverlayElements::create();
	overlay->initialize();
	camera = sdk::OrbitCameraExtension::create();
	camera->propertyDistance = 2.0f;
	programBuffer = std::make_shared<sdk::ProgramWideShaderBuffer>();
	camera->getPosition().connectEvent([this](const glm::vec3 &pPosition) { programBuffer->setCameraPos(pPosition); });
	camera->getOnMatrixChanged().connect(
		[this](const glm::mat4 &pMatrix) { programBuffer->setViewProjMatrix(pMatrix); });
}

std::shared_ptr<ViewPrefabEditor> ViewPrefabEditor::create(const std::shared_ptr<IWidgetContext> &pContext) {
	return std::shared_ptr<ViewPrefabEditor>(new ViewPrefabEditor(pContext));
}

void ViewPrefabEditor::updateUi() {
	ImGui::AlignTextToFramePadding();

	static int index = -1;
	updateTabBar(selectedPrefab, index);
	if (presenter) presenter->setSelectedPrefab(selectedPrefab);
	if (index == -1) return;
	size_t selectedIndex = static_cast<size_t>(index);
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Prefab-wide parameters")) {
		std::string prefabName = selectedPrefab->getName();
		if (ImGui::InputText("Prefab name", prefabName)) {
			prefabs.at(selectedIndex).second = true;
			selectedPrefab->setName(prefabName);
		}

		bool visible = selectedPrefab->visible;
		if (ImGui::Checkbox("Visible", &visible)) {
			selectedPrefab->visible = visible;
			prefabs.at(selectedIndex).second = true;
		}
		ImGui::SetItemTooltip("Controls visibility of all prefab elements");

		const char* items[] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE",	  "FFFF", "GGGG",
							   "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO"};
		static int itemSelectedIdx = 0; // Here we store our selection data as an index.
		if (ImGui::BeginCombo("Material###GlobalPrefabMaterial", items[itemSelectedIdx])) {
			for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
				const bool isSelected = (itemSelectedIdx == n);
				if (ImGui::Selectable(items[n], isSelected)) { itemSelectedIdx = n; }

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isSelected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SetItemTooltip("Material for all prefab elements");

		ImGui::UuidText("Prefab UUID", selectedPrefab->getUuid());
		ImGui::SetItemTooltip("Prefab UUID. This field is not editable, but copyable");
	}
	//auto mousePos = ImGui::GetMousePos();
	updateElements(selectedPrefab, selectedIndex);

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Preview")) {
		ImGui::BeginChild("Preview");
		auto region = ImGui::GetContentRegionAvail();
		width = static_cast<int>(region.x);
		height = static_cast<int>(region.y);
		ImGui::Image((ImTextureID) frameBuffer->getFrameTexture(), region, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::IsWindowHovered()) {
			zoom += ImGui::GetIO().MouseWheel / 2.0f;
			if (zoom < 2.0f) zoom = 2.0f;
			mouseHeld = ImGui::IsMouseDown(ImGuiMouseButton_Left);
		} else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
			mouseHeld = false;
		auto windowPos = ImGui::GetCursorScreenPos();
		windowPos.y -= ImGui::GetTextLineHeightWithSpacing() * 7 + 5;
		ImGui::SetCursorScreenPos(windowPos);

		ImGui::BeginChild("Controls");
		ImGui::Text("Hold LMB to rotate the camera");
		ImGui::Text("Scroll up to zoom in, scroll down to zoom out");
		ImGui::Text("Transformation");
		ImGui::SameLine();
		ImGui::HelpMarker(
			"Here you can apply the transformations for the preview instance of the prefab. Note that the values "
			"entered here will not affect the final result. \n"
			"Ctrl+Click or double click to directly enter the value.\n"
			"Hold LMB on text box below and slide mouse left or right to increase or decrease the value.\n"
			"Combine it with Shift to accelerate or Alt to decelerate.");


		ImGui::DragFloat3("Translate", &translation.x, 0.01f);
		ImGui::SetItemTooltip("Set the position of the preview instance.");
		ImGui::DragFloat3("Rotation, deg", &rotation.x, 0.01f);
		ImGui::SetItemTooltip("Set the euler rotation of the preview instance in degrees.");
		ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.02f, 1000000.0f);
		ImGui::SetItemTooltip("Set the scale of the preview instance.");
		/*auto transform = instance->getLocalTransform();
		transform->reset();
		transform->translate(translation);
		transform->eulerAngleXZY(radians(rotation));
		transform->scale(scale);*/
		ImGui::EndChild();
		ImGui::EndChild();
	}
	//subWindows->renderWindows();
}

void ViewPrefabEditor::openView() {
	//context->addWindow(this);
	frameBuffer = std::make_shared<sdk::Framebuffer>();
	frameBuffer->initialize();
}

void ViewPrefabEditor::closeView() { context->removeWidget(); }

void ViewPrefabEditor::customRender() {
	//subWindows->customRenderWindows();
	/*glEnable(GL_DEPTH_TEST);
	frameBuffer->bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->propertyDistance = zoom;
	//camera->propertyDistance = 6.0f / std::log2(zoom) - 1;
	static int prevWidth, prevHeight;
	if (width != prevWidth || height != prevHeight) {
		prevWidth = width;
		prevHeight = height;
		camera->propertyAspect = static_cast<float>(width) / static_cast<float>(height);
		frameBuffer->setSize(width, height);
	}
	programBuffer->update();
	programBuffer->bindBufferBase(0);
	//selectedPrefab->render();
	overlay->render();
	selectedPrefab->getShaderProgram()->use();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (gltfModel) gltfModel->getDefaultScene()->render();
	frameBuffer->unbind();
	glDisable(GL_DEPTH_TEST);*/
}

void ViewPrefabEditor::onSizeChanged(int pWidth, int pHeight) { UiWindow::onSizeChanged(pWidth, pHeight); }

void ViewPrefabEditor::onCursorPosChanged(double pX, double pY) {

	static std::optional<glm::dvec2> lastCursorPos{};
	if (!mouseHeld) {
		lastCursorPos.reset();
		return;
	}

	const glm::dvec2 pos{pX, pY};
	if (lastCursorPos) {
		const glm::vec2 delta = lastCursorPos.value() - pos;

		camera->propertyAngle = camera->propertyAngle + glm::vec2(0.6f) * glm::vec2(delta.y, delta.x);
	}
	lastCursorPos = pos;
	UiWindow::onCursorPosChanged(pX, pY);
}

void ViewPrefabEditor::onKeyChanged(int pKey, int pScancode, int pAction, int pMods) {
	if (pAction == 1) return;
	//std::filesystem::path path = "/home/alexus/Downloads/models/bathroom/scene.gltf";
	std::filesystem::path path = "/home/alexus/Downloads/models/VertexColorTest.gltf";
	sdk::ReportMessagePtr errorMsg;
	gltfModel = sdk::GltfModel::createFromFile(path, errorMsg);
	if (!gltfModel) { sdk::Logger::error(errorMsg); }

	//if (auto msg = gltfModel->getDefaultScene()->initialize()) { sdk::Logger::error(msg); }

	UiWindow::onKeyChanged(pKey, pScancode, pAction, pMods);
}

void ViewPrefabEditor::updateTabBar(std::shared_ptr<sdk::Prefab> &pSelectedPrefab, int &pSelectedIndex) {
	if (!ImGui::BeginTabBar("MyTabBar", 0)) return;
	int indexToRemove = -1;

	if (prefabs.empty()) {
		pSelectedIndex = -1;
		pSelectedPrefab.reset();
	}
	for (int i = 0; auto prefab: prefabs) {
		bool opened = true;
		auto &prefabName = prefab.first->getName();
		if (ImGui::BeginTabItem((prefabName + "###" + prefab.first->getUuid().toString()).c_str(), &opened,
								prefab.second ? ImGuiTabItemFlags_UnsavedDocument : 0)) {
			pSelectedIndex = i;
			pSelectedPrefab = prefab.first;
			ImGui::EndTabItem();
		}
		if (!opened) indexToRemove = i;

		i++;
	}
	ImGui::EndTabBar();

	if (indexToRemove > -1) { prefabs.erase(prefabs.begin() + indexToRemove); }
}

void ViewPrefabEditor::updateElements(const std::shared_ptr<sdk::Prefab> &pSelectedPrefab, size_t pPrefabIndex) {
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (!ImGui::CollapsingHeader("Elements")) return;
	static std::shared_ptr<sdk::PrefabElement> selectedElement{};
	if (pSelectedPrefab->getElements().empty()) selectedElement.reset();

	if (ImGui::Button("Add from file")) {
		if (presenter) presenter->addMeshesFrom("/home/alexus/MegaEngineProjects/models/source/Pistol.obj");
	}
	ImGui::SameLine();
	if (ImGui::Button("+")) { pSelectedPrefab->addElement(sdk::MeshPrefabElement::create()); }
	ImGui::SameLine();
	ImGui::BeginDisabled(selectedElement == nullptr);
	if (ImGui::Button("-")) { pSelectedPrefab->removeElement(selectedElement); }
	ImGui::EndDisabled();

	static size_t selectedElementIndex = 0;
	auto elements = pSelectedPrefab->getElements();
	if (ImGui::BeginListBox("##ElementsListBox")) {
		selectedElementIndex = std::clamp(selectedElementIndex, 0ul, std::max(elements.size() - 1ul, 0ul));
		for (size_t n = 0; auto element: elements | std::views::values) {
			const bool isSelected = (selectedElementIndex == n);
			std::string label = element->getName() + "##" + element->getUuid().toString();
			if (ImGui::Selectable(label.c_str(), isSelected)) { selectedElementIndex = n; }

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (isSelected) {
				selectedElement = element;
				ImGui::SetItemDefaultFocus();
			}
			n++;
		}
		ImGui::EndListBox();
	}

	if (!selectedElement) return;

	if (std::string prefabName = selectedElement->getName(); ImGui::InputText("Element name", prefabName)) {
		selectedElement->setName(prefabName);
		prefabs.at(pPrefabIndex).second = true;
	}
	/*if (auto meshElement = std::dynamic_pointer_cast<sdk::MeshPrefabElement>(selectedElement)) {
		meshElement->getMesh()
	}*/
	bool visible = selectedElement->visible;
	if (ImGui::Checkbox("Visible##element", &visible)) {
		selectedElement->visible = visible;
		prefabs.at(pPrefabIndex).second = true;
	}
	ImGui::UuidText("Element UUID", selectedElement->getUuid());
	ImGui::SetItemTooltip("Prefab Element UUID. This field is not editable, but copyable");
}
} // namespace mer::editor::mvp