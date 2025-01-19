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
class Extension;
class IScene;
class ICamera;
class ISceneObject;
} // namespace mer::sdk::main

namespace mer::editor::project {
class Project;
} // namespace mer::editor::project

namespace mer::sdk::utils {
enum class MouseButton;
}

namespace SQLite {
class Database;
}

namespace mer::editor::mvp {
class ResourcesContext;

class IModelSceneEditor {
public:
	virtual ~IModelSceneEditor() = default;

	virtual sigc::connection connectOnLoadingSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual sigc::connection connectOnLoadedSignal(const sigc::slot<void()> &pSlot) const = 0;

	virtual bool hasScene() const = 0;

	virtual bool hasResourcesContext() const = 0;

	virtual void setupResourcesContext(const std::shared_ptr<ResourcesContext> &pResourcesContext) const = 0;

	virtual void initScene() = 0;

	virtual void uninitScene() = 0;

	virtual void render() = 0;

	virtual void setName(const std::string &pName) = 0;

	[[nodiscard]] virtual const std::string &getName() const = 0;

	virtual sigc::connection connectNameChanged(const sigc::slot<void(const std::string &pName)> &pSlot) = 0;

	virtual void createObject() = 0;

	virtual void removeObject(sdk::main::ISceneObject* pObjectToRemove) = 0;

	virtual void renameObject(sdk::main::ISceneObject* pObject, const std::string &pNewName) const = 0;

	virtual void saveObject(sdk::main::ISceneObject* pObject) = 0;

	[[nodiscard]] virtual const std::shared_ptr<sdk::main::IScene> &getScene() const = 0;

	virtual void onCursorPosChanged(double pX, double pY) = 0;

	virtual void onMouseButtonStateChanged(sdk::utils::MouseButton pButton, bool pPressed, double pX, double pY) = 0;

	virtual void toggleGrid(bool pState) = 0;
};
} // namespace mer::editor::mvp


#endif //IMODELSCENEEDITOR_H
