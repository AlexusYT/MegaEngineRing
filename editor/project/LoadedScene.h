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
// Created by alexus on 25.09.24.
//

#ifndef LOADEDSCENE_H
#define LOADEDSCENE_H

namespace mer::sdk::utils {
enum class MouseButton;
}

namespace mer::editor::mvp {
class ResourcesContext;
class ExplorerObject;
} // namespace mer::editor::mvp

namespace mer::sdk::main {
class IResourceLoadExecutor;
class Application;
class Extension;
class ISceneObject;
class IScene;
} // namespace mer::sdk::main

namespace mer::editor::project {
class Sdk;
}

namespace SQLite {
class Database;
}

namespace mer::editor::project {
class LoadedScene {
	std::shared_ptr<Sdk> sdk;
	std::string name;
	std::shared_ptr<SQLite::Database> database;
	sigc::signal<void(const std::string &pName)> onNameChanged;
	std::shared_ptr<sdk::main::IScene> scene;
	std::shared_ptr<mvp::ExplorerObject> mainObject;
	sigc::signal<void(const sdk::utils::ReportMessagePtr &pError)> onErrorOccurred;
	std::unordered_map<mvp::ExplorerObject*, sdk::main::ISceneObject*> sceneObjByExplorer;
	std::unordered_map<sdk::main::ISceneObject*, std::shared_ptr<mvp::ExplorerObject>> explorerBySceneObj;
	/**=
	 * @brief Locks the database from performing the insert operations. Typically, this is used to prevent duplication of
	 * data when reading it.
	 */
	bool databaseLocked{};
	sigc::signal<void(sdk::main::Extension* pObject)> onExtensionAdded;
	std::shared_ptr<sdk::main::Application> app;
	std::shared_ptr<sdk::main::ISceneObject> cameraObject;

	sigc::signal<void()> onLoadingSignal;
	sigc::signal<void()> onLoadedSignal;
	mvp::ExplorerObject* selectedObject{};
	sigc::signal<void(mvp::ExplorerObject* pSelectedObject)> onSelectionChanged;
	std::vector<sigc::connection> connections;

public:
	explicit LoadedScene(const std::shared_ptr<Sdk> &pSdk);

	void setRunDirectory(const std::filesystem::path &pPath) const;

	bool hasScene() const;

	bool hasResourcesContext() const;

	sdk::main::IResourceLoadExecutor* getResourceLoadExecutor() const;
	void setupResourcesContext(const std::shared_ptr<mvp::ResourcesContext> &pResourcesContext) const;

	void connectErrorOccurred(const sigc::slot<void(const sdk::utils::ReportMessagePtr &pError)> &pSlot) {
		onErrorOccurred.connect(pSlot);
	}

	void initScene() const;

	void render() const;

	sdk::utils::ReportMessagePtr load(const std::filesystem::path &pPath);

	sigc::connection connectOnLoadingSignal(const sigc::slot<void()> &pSlot) { return onLoadingSignal.connect(pSlot); }

	sigc::connection connectOnLoadedSignal(const sigc::slot<void()> &pSlot) { return onLoadedSignal.connect(pSlot); }

	void unload();

	sdk::utils::ReportMessagePtr readSceneSettings();

	sdk::utils::ReportMessagePtr readObjects();

	void setName(const std::string &pName) {
		if (name == pName) return;
		name = pName;
		onNameChanged(name);
	}

	const std::string &getName() const { return name; }

	[[nodiscard]] const std::shared_ptr<mvp::ExplorerObject> &getMainObject() const { return mainObject; }

	sigc::connection connectNameChanged(const sigc::slot<void(const std::string &pName)> &pSlot) {
		pSlot(name);
		return onNameChanged.connect(pSlot);
	}

	void addObject();

	void removeObject(sdk::main::ISceneObject* pObjectToRemove);

	void removeExtension(const sdk::main::Extension* pExtensionToRemove) const;

	void renameObject(sdk::main::ISceneObject* pObject, const std::string &pNewName) const;

	std::shared_ptr<sdk::main::Extension> addExtension(sdk::main::ISceneObject* pObject, const std::string &pType,
													   const std::string &pName) const;

	sigc::connection connectExtensionAdded(const sigc::slot<void(sdk::main::Extension* pExtension)> &pSlot) {
		return onExtensionAdded.connect(pSlot);
	}

	sigc::connection connectSelectionChanged(const sigc::slot<void(mvp::ExplorerObject* pSelectedObject)> &pSlot) {
		pSlot(selectedObject);
		return onSelectionChanged.connect(pSlot);
	}

	void saveObject(sdk::main::ISceneObject* pObject) const;

	void addObjectToDatabase(const std::shared_ptr<sdk::main::ISceneObject> &pObject) const;

	void removeObjectFromDatabase(sdk::main::ISceneObject* pObject) const;

	[[nodiscard]] const std::shared_ptr<Sdk> &getSdk() const { return sdk; }

	[[nodiscard]] const std::shared_ptr<sdk::main::IScene> &getScene() const { return scene; }

	void onCursorPosChanged(double pX, double pY) const;

	void onMouseButtonStateChanged(sdk::utils::MouseButton pButton, bool pPressed, double pX, double pY) const;

	void selectObject(mvp::ExplorerObject* pObjectToSelect);

	[[nodiscard]] mvp::ExplorerObject* getSelectedObject() const { return selectedObject; }

	[[nodiscard]] const std::shared_ptr<sdk::main::Application> &getApp() const { return app; }

private:
	std::shared_ptr<sdk::main::ISceneObject> createObject() const;

	sdk::utils::ReportMessagePtr createObjectsTable() const;
};
} // namespace mer::editor::project


#endif //LOADEDSCENE_H
