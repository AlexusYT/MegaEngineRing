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
// Created by alexus on 06.02.24.
//

#include "ViewSceneEditor.h"

#include "EngineSDK/extensions/MeshExtension.h"
#include "EngineSDK/gltf/GltfModel.h"
#include "EngineSDK/gltf/Node.h"
#include "EngineSDK/meshes/BlockPlaneMesh.h"
#include "Globals.h"
#include "NodeSelectionHelper.h"
#include "imgui_internal.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/objectProperties/ModelObjectProperties.h"
#include "mvp/objectProperties/PresenterObjectProperties.h"
#include "mvp/objectProperties/ViewObjectProperties.h"
#include "mvp/scenePreview/ModelScenePreview.h"
#include "mvp/scenePreview/ViewScenePreview.h"
#include "mvp/scenePreview/sceneEditor/PresenterSceneEditorPreview.h"
#include "mvp/treeObjectWindow/ModelObjectsTree.h"
#include "mvp/treeObjectWindow/PresenterObjectsTree.h"
#include "mvp/treeObjectWindow/ViewObjectsTree.h"

namespace mer::editor::mvp {

SceneEditor::SceneEditor(const std::string &pName) : Editor(pName) {
	selection = std::make_shared<NodeSelectionHelper>();
	auto previewView = ViewScenePreview::create("SceneEditorPreview", EditorContext::create(this));
	scenePreviewView = previewView;
	auto previewModel = std::make_shared<ModelScenePreview>(selection.get());
	scene3D = sdk::Scene3D::create();
	previewModel->setScene(scene3D);
	scenePreviewPresenter = PresenterSceneEditorPreview::create(previewView, previewModel);
	scenePreviewPresenter->run();
	auto objView = std::make_shared<ViewObjectsTree>(EditorContext::create(this));
	objTreeView = objView;
	auto objTreeModel = std::make_shared<ModelObjectsTree>(selection.get());
	objTreeModel->setScene(scene3D);
	objTreePresenter = std::make_shared<PresenterObjectsTree>(objView, objTreeModel);
	objTreePresenter->run();

	auto propertiesView = std::make_shared<ViewObjectProperties>(EditorContext::create(this));
	propertiesViewTool = propertiesView;
	auto propertiesModel = std::make_shared<ModelObjectProperties>(selection.get());
	propertiesModel->setScene(scene3D);
	propertiesPresenter = std::make_shared<PresenterObjectProperties>(propertiesView, propertiesModel);
	propertiesPresenter->run();
}

void SceneEditor::updateUi() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::BeginMenu("Block")) {
				if (ImGui::MenuItem("Plane")) { addPlane(); }
				ImGui::MenuItem("Cube");
				ImGui::MenuItem("Sphere");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Model")) {
				if (ImGui::BeginMenu("glTF")) {
					if (ImGui::MenuItem("From File...")) {
						IGFD::FileDialogConfig config;
						config.path = Globals::getProjectsPath().string();
						config.countSelectionMax = 1;
						ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
																"glTF files (.glb,.gltf){.glb,.gltf}", config);
					}
					ImGui::BeginDisabled(true);
					if (ImGui::MenuItem("From Sketchfab...")) {}
					if (ImGui::MenuItem("From Favorites...")) {}
					ImGui::EndDisabled();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_Appearing);
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, ImVec2(300, 200))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			addGltfModel(filePathName);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void SceneEditor::loadPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir /*pPanelDir*/) {
	ImGuiID dockMainId = pDockspaceId;
	ImGui::DockBuilderAddNode(dockMainId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockMainId, pDockspaceSize);
	ImGuiID dockIdRight = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.20f, NULL, &dockMainId);
	ImGuiID dockIdRightUp{};
	ImGuiID dockIdRightDown = ImGui::DockBuilderSplitNode(dockIdRight, ImGuiDir_Down, 0.50f, NULL, &dockIdRightUp);

	ImGui::DockBuilderDockWindow(scenePreviewView->getWindowName(currDockspaceId).c_str(), dockMainId);

	ImGui::DockBuilderDockWindow(objTreeView->getWindowName(currDockspaceId).c_str(), dockIdRightUp);
	ImGui::DockBuilderDockWindow(propertiesViewTool->getWindowName(currDockspaceId).c_str(), dockIdRightDown);
	ImGui::DockBuilderFinish(pDockspaceId);
}

void SceneEditor::addPlane() {
	auto model = scenePreviewPresenter->getModel();
	auto mesh = std::make_shared<sdk::BlockPlaneMesh>();
	//mesh->initialize();
	auto scene = model->getScene();
	if (!scene) {
		scene = sdk::Scene3D::create();
		model->setScene(scene);
	}
	auto ext = sdk::MeshExtension::create();
	ext->mesh = mesh;
	auto instance = sdk::Node::create("");
	instance->addExtension(ext);
	scene->addMesh(mesh);
	scene3D->addNode(nullptr, instance);
	//if (scenePreviewPresenter) scenePreviewPresenter->addPlane();
}

void SceneEditor::addGltfModel(const std::string &pPath) {

	sdk::ReportMessagePtr msg;
	auto gltf = sdk::GltfModel::createFromFile(pPath, msg);

	if (!gltf) {
		sdk::Logger::error(msg);
		return;
	}
	auto model = scenePreviewPresenter->getModel();
	auto scene = model->getScene();
	if (!scene) {
		scene = sdk::Scene3D::create();
		model->setScene(scene);
	}
	for (auto material: gltf->getMaterials()) { scene->addMaterial(material); }
	for (auto light: gltf->getLights()) { scene->addLightSource(light); }
	for (auto mesh: gltf->getMeshes()) { scene->addMesh(mesh); }

	scene->mergeNodes(gltf->getNodes());
}
} // namespace mer::editor::mvp