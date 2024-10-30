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
// Created by alexus on 06.02.24.
//

#ifndef MODELSCENEEDITOR_H
#define MODELSCENEEDITOR_H

#include "IModelSceneEditor.h"

namespace mer::sdk::utils {
enum class MouseButton;
}

namespace mer::editor::project {
class LoadedScene;
}

namespace mer::editor::mvp {
class ResourcesContext;

class ModelSceneEditor : public IModelSceneEditor {
	std::shared_ptr<project::LoadedScene> loadedScene;

public:
	ModelSceneEditor();

	void setLoadedScene(const std::shared_ptr<project::LoadedScene> &pLoadedScene) { loadedScene = pLoadedScene; }

	sigc::connection connectOnLoadingSignal(const sigc::slot<void()> &pSlot) const override;

	sigc::connection connectOnLoadedSignal(const sigc::slot<void()> &pSlot) const override;

	bool hasScene() const override;

	bool hasResourcesContext() const override;

	void initScene() override;

	void render() override;

	void setupResourcesContext(const std::shared_ptr<ResourcesContext> &pResourcesContext) const override;

	void setName(const std::string &pName) override;

	const std::string &getName() const override;

	sigc::connection connectNameChanged(const sigc::slot<void(const std::string &pName)> &pSlot) override;

	void createObject() override;

	void removeObject(sdk::main::ISceneObject* pObjectToRemove) override;

	void renameObject(sdk::main::ISceneObject* pObject, const std::string &pNewName) const override;

	void saveObject(sdk::main::ISceneObject* pObject) override;

	[[nodiscard]] const std::shared_ptr<sdk::main::IScene> &getScene() const override;

	void onCursorPosChanged(double pX, double pY) override;

	void onMouseButtonStateChanged(sdk::utils::MouseButton pButton, bool pPressed, double pX, double pY) override;
};
} // namespace mer::editor::mvp
#endif //MODELSCENEEDITOR_H
