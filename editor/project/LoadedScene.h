//  KwasarEngine is an SDK that can help you speed up game development.
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

namespace ke {
enum class MouseButton;
}

namespace ked {
class ResourcesContext;
} // namespace ked

namespace ke {
class IResourceLoadExecutor;
class Application;
class Extension;
class ISceneObject;
class IScene;
} // namespace ke

namespace ked {
class LoadedScene {
	std::string name;
	sigc::signal<void(const std::string &pName)> onNameChanged;
	std::shared_ptr<ke::IScene> scene;
	sigc::signal<void(const ke::ReportMessagePtr &pError)> onErrorOccurred;
	sigc::signal<void(ke::Extension* pObject)> onExtensionAdded;
	std::shared_ptr<ke::Application> app;
	std::shared_ptr<ke::ISceneObject> cameraObject;

	sigc::signal<void()> onLoadingSignal;
	sigc::signal<void()> onLoadedSignal;
	std::vector<sigc::connection> connections;
	std::shared_ptr<ResourcesContext> context;

public:
	LoadedScene();

	void setRunDirectory(const std::filesystem::path &pPath) const;

	bool hasScene() const;

	bool hasResourcesContext() const;

	ke::IResourceLoadExecutor* getResourceLoadExecutor() const;

	void setupResourcesContext(const std::shared_ptr<ResourcesContext> &pResourcesContext);

	void connectErrorOccurred(const sigc::slot<void(const ke::ReportMessagePtr &pError)> &pSlot) {
		onErrorOccurred.connect(pSlot);
	}

	void initScene();

	void uninitScene() const;

	void render() const;

	ke::ReportMessagePtr load(const std::filesystem::path &pPath);

	sigc::connection connectOnLoadingSignal(const sigc::slot<void()> &pSlot) { return onLoadingSignal.connect(pSlot); }

	sigc::connection connectOnLoadedSignal(const sigc::slot<void()> &pSlot) { return onLoadedSignal.connect(pSlot); }

	void unload();

	ke::ReportMessagePtr readObjects();

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

	void removeObject(ke::ISceneObject* pObjectToRemove);

	void removeExtension(const ke::Extension* pExtensionToRemove) const;

	void renameObject(ke::ISceneObject* pObject, const std::string &pNewName) const;

	std::shared_ptr<ke::Extension> addExtension(ke::ISceneObject* pObject, const std::string &pType,
												 const std::string &pName) const;

	void saveObject(ke::ISceneObject* pObject) const;

	void addObjectToDatabase(const std::shared_ptr<ke::ISceneObject> &pObject) const;

	[[nodiscard]] const std::shared_ptr<ke::IScene> &getScene() const { return scene; }

	void onCursorPosChanged(double pX, double pY) const;

	void onMouseButtonStateChanged(ke::MouseButton pButton, bool pPressed, double pX, double pY) const;

	[[nodiscard]] const std::shared_ptr<ke::Application> &getApp() const { return app; }

private:
	std::shared_ptr<ke::ISceneObject> createObject() const;
};
} // namespace ked


#endif //LOADEDSCENE_H
