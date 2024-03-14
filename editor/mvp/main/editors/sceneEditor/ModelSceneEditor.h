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

namespace mer::editor::mvp {
class ModelSceneEditor : public IModelSceneEditor {
	std::shared_ptr<project::Project> project;
	project::SceneInfo* sceneInfo{};

	std::shared_ptr<sdk::main::IScene> scene;
	bool interactive{};

	std::unordered_map<UUID*, ui::EditorSceneObject*> objects;
	std::shared_ptr<Gio::ListStore<ui::EditorSceneObject>> toplevelObjects;

public:
	ModelSceneEditor() : toplevelObjects(Gio::ListStore<ui::EditorSceneObject>::create()) {}

	[[nodiscard]] const std::shared_ptr<project::Project> &getProject() const override { return project; }

	void setProject(const std::shared_ptr<project::Project> &pProject) override { project = pProject; }

	[[nodiscard]] project::SceneInfo* getSceneInfo() const override { return sceneInfo; }

	void setSceneInfo(project::SceneInfo* const pSceneInfo) override { sceneInfo = pSceneInfo; }

	[[nodiscard]] const std::shared_ptr<sdk::main::IScene> &getScene() const override { return scene; }

	void setScene(const std::shared_ptr<sdk::main::IScene> &pScene) override { scene = pScene; }

	[[nodiscard]] bool isInteractive() const override { return interactive; }

	void setInteractive(const bool pInteractive) override { interactive = pInteractive; }

	void addToplevelSceneObject(UUID* pUuid, const std::shared_ptr<ui::EditorSceneObject> &pObject) override {
		toplevelObjects->append(pObject);
		objects.emplace(pUuid, pObject.get());
	}

	[[nodiscard]] const std::shared_ptr<Gio::ListStore<ui::EditorSceneObject>> &getToplevelObjects() const override {
		return toplevelObjects;
	}
};
} // namespace mer::editor::mvp
#endif //MODELSCENEEDITOR_H
