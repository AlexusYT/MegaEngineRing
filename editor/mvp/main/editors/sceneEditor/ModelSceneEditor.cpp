//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 30.08.24.
//

#include "ModelSceneEditor.h"

#include "project/LoadedScene.h"

namespace mer::editor::mvp {

ModelSceneEditor::ModelSceneEditor() {}

sigc::connection ModelSceneEditor::connectOnLoadingSignal(const sigc::slot<void()> &pSlot) const {
	return loadedScene->connectOnLoadingSignal(pSlot);
}

sigc::connection ModelSceneEditor::connectOnLoadedSignal(const sigc::slot<void()> &pSlot) const {
	return loadedScene->connectOnLoadedSignal(pSlot);
}

bool ModelSceneEditor::hasScene() const { return loadedScene->hasScene(); }

bool ModelSceneEditor::hasResourcesContext() const { return loadedScene->hasResourcesContext(); }

void ModelSceneEditor::initScene() { loadedScene->initScene(); }

void ModelSceneEditor::uninitScene() { loadedScene->uninitScene(); }

void ModelSceneEditor::render() { loadedScene->render(); }

void ModelSceneEditor::setupResourcesContext(const std::shared_ptr<ResourcesContext> &pResourcesContext) const {
	loadedScene->setupResourcesContext(pResourcesContext);
}

void ModelSceneEditor::setName(const std::string &pName) { loadedScene->setName(pName); }

const std::string &ModelSceneEditor::getName() const { return loadedScene->getName(); }

sigc::connection ModelSceneEditor::connectNameChanged(const sigc::slot<void(const std::string &pName)> &pSlot) {
	return loadedScene->connectNameChanged(pSlot);
}

void ModelSceneEditor::createObject() { return loadedScene->addObject(); }

void ModelSceneEditor::removeObject(sdk::main::ISceneObject* pObjectToRemove) {
	loadedScene->removeObject(pObjectToRemove);
}

void ModelSceneEditor::renameObject(sdk::main::ISceneObject* pObject, const std::string &pNewName) const {
	loadedScene->renameObject(pObject, pNewName);
}

void ModelSceneEditor::saveObject(sdk::main::ISceneObject* pObject) { loadedScene->saveObject(pObject); }

const std::shared_ptr<sdk::main::IScene> &ModelSceneEditor::getScene() const { return loadedScene->getScene(); }

void ModelSceneEditor::onCursorPosChanged(const double pX, const double pY) { loadedScene->onCursorPosChanged(pX, pY); }

void ModelSceneEditor::onMouseButtonStateChanged(const sdk::utils::MouseButton pButton, const bool pPressed,
												 const double pX, const double pY) {
	loadedScene->onMouseButtonStateChanged(pButton, pPressed, pX, pY);
}


} // namespace mer::editor::mvp
