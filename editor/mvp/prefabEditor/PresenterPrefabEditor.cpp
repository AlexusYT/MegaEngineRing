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

#include "PresenterPrefabEditor.h"

#include "KwasarEngine/prefabs/Prefab.h"
#include "KwasarEngine/prefabs/elements/MeshPrefabElement.h"
#include "IModelPrefabEditor.h"
#include "IViewPrefabEditor.h"
#include "mvp/scenePreview/ViewScenePreview.h"

namespace mer::editor::mvp {
PresenterPrefabEditor::PresenterPrefabEditor(const std::shared_ptr<IModelPrefabEditor> &pModel,
											 const std::shared_ptr<IViewPrefabEditor> &pView)
	: model(pModel), view(pView) { view->setPresenter(this); }

std::shared_ptr<PresenterPrefabEditor> PresenterPrefabEditor::create(const std::shared_ptr<IModelPrefabEditor> &pModel,
																	 const std::shared_ptr<IViewPrefabEditor> &pView) {
	return std::shared_ptr < PresenterPrefabEditor > (new PresenterPrefabEditor(pModel, pView));
}

void PresenterPrefabEditor::addMeshesFrom(const std::filesystem::path & /*pPath*/) {}

void PresenterPrefabEditor::setSelectedPrefab(const std::shared_ptr<ke::Prefab> &pPrefab) {
	model->setSelectedPrefab(pPrefab);
}

void PresenterPrefabEditor::run() {
	view->openView();
	/*auto scenePreview = ViewScenePreview::create("PrefabScenePreview1", "Prefab preview",
												 UiWinChildContext::create(view->getSubWindows()));
	auto modelPreview = std::make_shared<ModelPrefabPreview>();
	auto presenter = PresenterPrefabPreview::create(scenePreview, modelPreview);
	getAppController()->run(presenter);*/
}

void PresenterPrefabEditor::stop() { view->closeView(); }
} // namespace mer::editor::mvp