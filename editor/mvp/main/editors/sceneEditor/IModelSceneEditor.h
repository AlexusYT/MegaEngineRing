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

#ifndef IMODELSCENEEDITOR_H
#define IMODELSCENEEDITOR_H

namespace mer::editor::ui {
class EditorSceneObject;
}
class UUID;

namespace mer::sdk::main {
class IScene;
}

namespace mer::editor::project {
class Project;
class SceneInfo;
} // namespace mer::editor::project

namespace mer::editor::mvp {
class IModelSceneEditor {
public:
	virtual ~IModelSceneEditor() = default;
	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;

	[[nodiscard]] virtual project::SceneInfo* getSceneInfo() const = 0;

	virtual void setSceneInfo(project::SceneInfo* const pSceneInfo) = 0;

	[[nodiscard]] virtual const std::shared_ptr<sdk::main::IScene> &getScene() const = 0;

	virtual void setScene(const std::shared_ptr<sdk::main::IScene> &pScene) = 0;

	[[nodiscard]] virtual bool isSimMode() const = 0;

	virtual void setSimMode(bool pSimMode) = 0;

	[[nodiscard]] virtual sdk::main::ICamera* getPrimaryCamera() const = 0;

	void virtual setPrimaryCamera(sdk::main::ICamera* pPrimaryCamera) = 0;

	[[nodiscard]] virtual sdk::main::ICamera* getEditorCamera() const = 0;

	virtual void setEditorCamera(sdk::main::ICamera* pEditorCamera) = 0;

	[[nodiscard]] virtual sdk::main::ISceneObject* getEditorCameraObject() const = 0;

	virtual void setEditorCameraObject(sdk::main::ISceneObject* const pEditorCameraObject) = 0;
};
} // namespace mer::editor::mvp


#endif //IMODELSCENEEDITOR_H
