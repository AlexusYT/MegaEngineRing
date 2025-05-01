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
// Created by alexus on 25.09.24.
//

#ifndef LOADEDSCENE_H
#define LOADEDSCENE_H

namespace mer::sdk {
enum class MouseButton;
}

namespace mer::editor::mvp {
class ResourcesContext;
} // namespace mer::editor::mvp

namespace mer::sdk {
class IResourceLoadExecutor;
class Application;
class Extension;
class ISceneObject;
class IScene;
} // namespace mer::sdk

namespace SQLite {
class Database;
}

namespace mer::editor::project {
class LoadedScene {
	std::string name;
	std::shared_ptr<SQLite::Database> database;
	sigc::signal<void(const std::string &pName)> onNameChanged;
	std::shared_ptr<sdk::IScene> scene;
	sigc::signal<void(const sdk::ReportMessagePtr &pError)> onErrorOccurred;
	/**=
	 * @brief Locks the database from performing the insert operations. Typically, this is used to prevent duplication of
	 * data when reading it.
	 */
	bool databaseLocked{};
	sigc::signal<void(sdk::Extension* pObject)> onExtensionAdded;
	std::shared_ptr<sdk::Application> app;
	std::shared_ptr<sdk::ISceneObject> cameraObject;

	sigc::signal<void()> onLoadingSignal;
	sigc::signal<void()> onLoadedSignal;
	std::vector<sigc::connection> connections;
	std::shared_ptr<mvp::ResourcesContext> context;

public:
	LoadedScene();

	void setRunDirectory(const std::filesystem::path &pPath) const;

	bool hasScene() const;

	bool hasResourcesContext() const;

	sdk::IResourceLoadExecutor* getResourceLoadExecutor() const;

	void setupResourcesContext(const std::shared_ptr<mvp::ResourcesContext> &pResourcesContext);

	void connectErrorOccurred(const sigc::slot<void(const sdk::ReportMessagePtr &pError)> &pSlot) {
		onErrorOccurred.connect(pSlot);
	}

	void initScene();

	void uninitScene() const;

	void render() const;

	sdk::ReportMessagePtr load(const std::filesystem::path &pPath);

	sigc::connection connectOnLoadingSignal(const sigc::slot<void()> &pSlot) { return onLoadingSignal.connect(pSlot); }

	sigc::connection connectOnLoadedSignal(const sigc::slot<void()> &pSlot) { return onLoadedSignal.connect(pSlot); }

	void unload();

	sdk::ReportMessagePtr readObjects();

	void setName(const std::string &pName) {
		if (name == pName) return;
		name = pName;
		onNameChanged(name);
	}

	const std::string &getName() const { return name; }

	sigc::connection connectNameChanged(const sigc::slot<void(const std::string &pName)> &pSlot) {
		pSlot(name);
		return onNameChanged.connect(pSlot);
	}

	void addObject();

	void removeObject(sdk::ISceneObject* pObjectToRemove);

	void removeExtension(const sdk::Extension* pExtensionToRemove) const;

	void renameObject(sdk::ISceneObject* pObject, const std::string &pNewName) const;

	std::shared_ptr<sdk::Extension> addExtension(sdk::ISceneObject* pObject, const std::string &pType,
												 const std::string &pName) const;

	void saveObject(sdk::ISceneObject* pObject) const;

	void addObjectToDatabase(const std::shared_ptr<sdk::ISceneObject> &pObject) const;

	void removeObjectFromDatabase(sdk::ISceneObject* pObject) const;

	[[nodiscard]] const std::shared_ptr<sdk::IScene> &getScene() const { return scene; }

	void onCursorPosChanged(double pX, double pY) const;

	void onMouseButtonStateChanged(sdk::MouseButton pButton, bool pPressed, double pX, double pY) const;

	[[nodiscard]] const std::shared_ptr<sdk::Application> &getApp() const { return app; }

private:
	std::shared_ptr<sdk::ISceneObject> createObject() const;

	sdk::ReportMessagePtr createObjectsTable() const;
};
} // namespace mer::editor::project


#endif //LOADEDSCENE_H
