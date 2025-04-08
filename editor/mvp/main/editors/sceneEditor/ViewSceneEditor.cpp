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

#include <mvp/main/MainWindow.h>

#include "EngineSDK/gltf/GltfModel.h"
#include "EngineSDK/gltf/MeshInstance.h"
#include "EngineSDK/meshes/BlockPlaneMesh.h"
#include "EngineSDK/scene/Scene.h"
#include "EngineSDK/scene/Scene3D.h"
#include "imgui_internal.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/main/centerWindow/TabPlaceholder.h"
#include "mvp/main/editors/IPresenterSceneEditor.h"
#include "mvp/main/treeObjectWindow/ModelObjectsTree.h"
#include "mvp/main/treeObjectWindow/PresenterObjectsTree.h"
#include "mvp/main/treeObjectWindow/ViewObjectsTree.h"
#include "mvp/scenePreview/ModelScenePreview.h"
#include "mvp/scenePreview/ViewScenePreview.h"
#include "mvp/scenePreview/sceneEditor/PresenterSceneEditorPreview.h"

namespace mer::editor::mvp {
ViewSceneEditor::ViewSceneEditor(const std::shared_ptr<IWidgetContext> &pContext)
	: UiWindow("", "Scene Editor"), context(pContext) {
	setWindowFlags(ImGuiWindowFlags_MenuBar);
#ifdef USE_OLD_UI
	modeSwitch.set_margin(5);
	modeSwitch.set_sensitive(false);
	Gtk::Label simulatingLabel("Simulation");
	simulatingLabel.set_margin(5);
	Gtk::Box topBox;
	topBox.append(simulatingLabel);
	topBox.append(modeSwitch);
	gridToggle.set_icon_name("grid");
	gridToggle.set_tooltip_text("Show or hide the grid in the editor");
	gridToggle.signal_toggled().connect([this] {
		if (presenter) presenter->onGridToggled(gridToggle.get_active());
	});
	gridToggle.set_active();
	topBox.append(gridToggle);

	mainWidget.append(topBox);
	mainWidget.append(area);


	area.signal_create_context().connect(
		[this]() -> Glib::RefPtr<Gdk::GLContext> {
			try {
				const auto surface = area.get_native()->get_surface();
				auto glContext = surface->create_gl_context();
				glContext->set_allowed_apis(Gdk::GLApi::GL);

				glContext->set_required_version(4, 0);
				glContext->set_debug_enabled(true);
				glContext->set_forward_compatible();
				glContext->realize();
				return glContext;
			} catch (Gdk::GLError &e) {
				sdk::Logger::out("{}", static_cast<int>(e.code()));
				throw;
			}
		},
		false);
	area.set_expand(true);
	area.set_auto_render(true);
	area.set_has_depth_buffer(true);

	motionController = Gtk::EventControllerMotion::create();
	area.add_controller(motionController);

	gestureClick = Gtk::GestureClick::create();
	gestureClick->set_button();
	area.add_controller(gestureClick);

	keyController = Gtk::EventControllerKey::create();
	mainWidget.add_controller(keyController);
	const auto scroll = Gtk::EventControllerScroll::create();
	mainWidget.add_controller(scroll);
	scroll->set_flags(Gtk::EventControllerScroll::Flags::BOTH_AXES);
	scroll->signal_scroll().connect(
		[this](const double pDx, const double pDy) {
			if (presenter) return presenter->onMouseScroll(pDx, pDy);
			return false;
		},
		true);
	Glib::signal_timeout().connect(
		[this] {
			area.queue_render();
			return true;
		},
		16);
#endif
}

void ViewSceneEditor::updateUi() { ImGui::Text("Scene Editor"); }


#ifdef USE_OLD_UI
sigc::connection ViewSceneEditor::connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) {

	return area.signal_render().connect(pSlot, false);
}

sigc::connection ViewSceneEditor::connectRealize(const sigc::slot<void()> &pSlot) {
	return area.signal_realize().connect(pSlot);
}

sigc::connection ViewSceneEditor::connectUnrealize(const sigc::slot<void()> &pSlot) {
	return area.signal_unrealize().connect(pSlot, false);
}

sigc::connection ViewSceneEditor::connectResize(const sigc::slot<void(int pWidth, int pHeight)> &pSlot) {
	return area.signal_resize().connect(pSlot);
}

sigc::connection ViewSceneEditor::connectKeyPressedSignal(
	const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const {
	return keyController->signal_key_pressed().connect(pSlot, false);
}

sigc::connection ViewSceneEditor::connectKeyReleasedSignal(
	const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const {
	return keyController->signal_key_released().connect(pSlot, false);
}

void ViewSceneEditor::makeCurrent() { area.make_current(); }

void ViewSceneEditor::redraw() { area.queue_render(); }

void ViewSceneEditor::throwIfError() { area.throw_if_error(); }

void ViewSceneEditor::emitResize() { area.queue_resize(); }

void ViewSceneEditor::setTitle(const std::string &pTitle) { context->setTitle(pTitle); }
#endif
void ViewSceneEditor::openView() {
#ifdef USE_OLD_UI
	context->addWidget(&mainWidget);
#else
	context->addWindow(this);
#endif
}

void ViewSceneEditor::closeView() { context->removeWidget(); }

SceneEditor::SceneEditor(const std::string &pName) : Editor(pName) {

	auto previewView = ViewScenePreview::create("SceneEditorPreview", EditorContext::create(this));
	scenePreviewView = previewView;
	auto previewModel = std::make_shared<ModelScenePreview>();
	//previewModel->setScene(sdk::Scene3D::create());
	scenePreviewPresenter = PresenterSceneEditorPreview::create(previewView, previewModel);
	scenePreviewPresenter->run();
	auto objView = std::make_shared<ViewObjectsTree>(EditorContext::create(this));
	objTreeView = objView;
	auto objTreeModel = std::make_shared<ModelObjectsTree>();
	objTreePresenter = std::make_shared<PresenterObjectsTree>(objView, objTreeModel);
	objTreePresenter->run();
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
				if (ImGui::MenuItem("GLTF...")) { addGltfModel(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void SceneEditor::loadPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir /*pPanelDir*/) {
	ImGuiID dockMainId = pDockspaceId;
	ImGui::DockBuilderAddNode(dockMainId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockMainId, pDockspaceSize);
	ImGuiID dockIdRight = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.20f, NULL, &dockMainId);

	ImGui::DockBuilderDockWindow(scenePreviewView->getWindowName(currDockspaceId).c_str(), dockMainId);

	ImGui::DockBuilderDockWindow(objTreeView->getWindowName(currDockspaceId).c_str(), dockIdRight);
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
	//scene->addMesh(mesh);
	auto instance = sdk::MeshInstance::create("", mesh);
	instance->setMesh(mesh);
	scene->addMesh(mesh);
	scenePreviewPresenter->getModel()->addNode(nullptr, instance);
	//if (scenePreviewPresenter) scenePreviewPresenter->addPlane();
}

void SceneEditor::addGltfModel() {

	sdk::ReportMessagePtr msg;
	auto gltf = sdk::GltfModel::createFromFile("/home/alexus/.cache/MegaEngineRing/halo.glb", msg);

	//auto gltf = sdk::GltfModel::createFromFile("/home/alexus/Downloads/models/Sphere.glb", msg);
	//auto gltf = sdk::GltfModel::createFromFile("/home/alexus/Downloads/models/OrientationTest.gltf", msg);
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
	for (auto mesh: gltf->getMeshes()) { scene->addMesh(mesh); }

	scene->mergeNodes(gltf->getNodes());
	//for (auto rootNode: gltf->getDefaultScene()->getRootNodes()) { scene->addNode(nullptr, rootNode); }
	/*for (auto node: gltf->getNodes()) {
		if (auto meshInstance = std::dynamic_pointer_cast<sdk::MeshInstance>(node))
			modelPreview->addNode(meshInstance->getMesh(), meshInstance);
	}*/
	//modelPreview->setScene(gltf->getDefaultScene());
}
} // namespace mer::editor::mvp