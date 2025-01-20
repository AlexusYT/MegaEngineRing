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

#include "PresenterSceneEditor.h"

#include <EngineSDK/resources/LoadedResources.h>
#include <EngineSDK/scene/IScene.h>

#include "EngineSDK/resources/IResourceLoadExecutor.h"
#include "EngineSDK/scene/objects/ISceneObject.h"
#include "EngineSDK/utils/MouseButton.h"
#include "IModelSceneEditor.h"
#include "IViewSceneEditor.h"
#include "project/Project.h"

namespace mer::sdk::main {
class CameraMouseExtension;
class OrbitCameraExtension;
} // namespace mer::sdk::main

namespace mer::editor::mvp {


PresenterSceneEditor::PresenterSceneEditor(const std::shared_ptr<IModelSceneEditor> &pModelSceneEditor)
	: modelSceneEditor(pModelSceneEditor) {

	modelSceneEditor->connectOnLoadedSignal([this] {
		for (auto view: views) {
			view.first->executeInMainThread(
				[this, view](const std::promise<void> & /*pPromise*/) { view.first->redraw(); });

			view.first->queueResize();
			view.first->makeCurrent();
		}
		modelSceneEditor->initScene();
	});
}

PresenterSceneEditor::~PresenterSceneEditor() {}

void PresenterSceneEditor::addView(const std::shared_ptr<IView> &pNewView) {
	const auto view = std::dynamic_pointer_cast<IViewSceneEditor>(pNewView);
	if (!view) return;
	view->setPresenter(this);
	std::vector<sigc::connection> conns;

	view->connectRealize([this, view] {
		if (!modelSceneEditor->hasScene()) return;
		if (!modelSceneEditor->hasResourcesContext()) {
			view->makeCurrent();
			//modelSceneEditor->setupResourcesContext(view->getResourcesContext());
			modelSceneEditor->initScene();
		}
	});
	view->connectUnrealize([this] { modelSceneEditor->uninitScene(); });
	view->connectResize([this](const int pWidth, const int pHeight) {
		if (const auto scene = modelSceneEditor->getScene()) scene->resize(pWidth, pHeight);
	});
	view->connectRender([this, view](const std::shared_ptr<Gdk::GLContext> &pContext) {
		pContext->make_current();
		if (!modelSceneEditor->hasScene()) return true;
		modelSceneEditor->render();
		return true;
	});

	view->connectCursorMovedSignal(
		[this](const double pX, const double pY) { modelSceneEditor->onCursorPosChanged(pX, pY); });
	view->connectMouseButtonPressedSignal([this](unsigned int pButton, double pX, double pY) {
		const auto btn = static_cast<sdk::utils::MouseButton>(pButton);
		modelSceneEditor->onMouseButtonStateChanged(btn, true, pX, pY);
	});
	view->connectMouseButtonReleasedSignal([this](unsigned int pButton, const double pX, const double pY) {
		const auto btn = static_cast<sdk::utils::MouseButton>(pButton);
		modelSceneEditor->onMouseButtonStateChanged(btn, false, pX, pY);
	});

	view->connectKeyPressedSignal([this](guint /*pKeyVal*/, guint pKeyCode, const Gdk::ModifierType pState) {
		const auto key = static_cast<sdk::utils::KeyboardKey>(pKeyCode);
		const sdk::utils::ModifierKeys mods = convertToModifierKeys(pState);
		if (const auto scene = modelSceneEditor->getScene()) scene->onKeyChanged(key, true, mods);
		return true;
	});

	view->connectKeyReleasedSignal([this](guint /*pKeyVal*/, guint pKeyCode, const Gdk::ModifierType pState) {
		const auto key = static_cast<sdk::utils::KeyboardKey>(pKeyCode);
		const sdk::utils::ModifierKeys mods = convertToModifierKeys(pState);
		if (const auto scene = modelSceneEditor->getScene()) scene->onKeyChanged(key, false, mods);
	});

	view->openView();
	views.emplace(view, conns);
}

void PresenterSceneEditor::onGridToggled(bool pState) { modelSceneEditor->toggleGrid(pState); }

sdk::utils::ModifierKeys PresenterSceneEditor::convertToModifierKeys(const Gdk::ModifierType &pState) {
	using namespace sdk::utils;
	uint8_t mods = 0;
	if ((pState & Gdk::ModifierType::SHIFT_MASK) == Gdk::ModifierType::SHIFT_MASK) { mods |= ModifierKeys::MOD_SHIFT; }
	if ((pState & Gdk::ModifierType::CONTROL_MASK) == Gdk::ModifierType::CONTROL_MASK) {
		mods |= ModifierKeys::MOD_CONTROL;
	}
	if ((pState & Gdk::ModifierType::ALT_MASK) == Gdk::ModifierType::ALT_MASK) { mods |= ModifierKeys::MOD_ALT; }
	if ((pState & Gdk::ModifierType::SUPER_MASK) == Gdk::ModifierType::SUPER_MASK) { mods |= ModifierKeys::MOD_SUPER; }
	if ((pState & Gdk::ModifierType::LOCK_MASK) == Gdk::ModifierType::LOCK_MASK) {
		mods |= ModifierKeys::MOD_CAPS_LOCK;
	}
	return ModifierKeys(mods);
}

bool PresenterSceneEditor::onMouseScroll(double pDx, double pDy) {
	if (const auto scene = modelSceneEditor->getScene()) return scene->notifyOnMouseScroll(pDx, pDy);
	return false;
}

void PresenterSceneEditor::run() {}

void PresenterSceneEditor::stop() {
	for (auto view: views) view.first->closeView();
}
} // namespace mer::editor::mvp
