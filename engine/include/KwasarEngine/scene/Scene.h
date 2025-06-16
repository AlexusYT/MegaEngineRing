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
// Created by alexus on 02.02.24.
//

#ifndef SCENE_H
#define SCENE_H
#include <KwasarEngine/buffers/ProgramWideShaderBuffer.h>

#include <sigc++/signal.h>

#include "KwasarEngine/resources/IResourceLoadExecutor.h"
#include "KwasarEngine/resources/ResourceLoadResult.h"
#include "KwasarEngine/utils/MouseButton.h"
#include "IScene.h"

namespace mer::sdk {
class PrefabInstance;
class Prefab;
class ILightSources;
} // namespace mer::sdk

namespace mer::sdk {
class ICamera;
class ISceneObject;

class Scene : public IScene {
	IResourceLoadExecutor* resourceExecutor;
	std::shared_ptr<ProgramWideShaderBuffer> programBuffer;
	std::vector<std::shared_ptr<ISceneObject>> objects;
	sigc::signal<void(int pWidth, int pHeight)> onWindowSizeChangedSignal;
	sigc::signal<void(ISceneObject* pObject)> onObjectAddedSignal;
	sigc::signal<void(ISceneObject* pObject)> onObjectRemovedSignal;
	ICamera* currentCamera;
	IApplication* application;
	bool inited{};
	std::shared_ptr<ILightSources> lightSources;
	std::shared_ptr<Prefab> prefab;
	std::shared_ptr<PrefabInstance> instance;
	std::shared_ptr<PrefabInstance> instance2;

public:
	Scene();

	~Scene() override;

	static std::shared_ptr<Scene> create();

	void setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const override;

	[[nodiscard]] IResourceLoadExecutor* getResourceExecutor() const final { return resourceExecutor; }

private:
	void setResourceExecutor(IResourceLoadExecutor* pResources) final { resourceExecutor = pResources; }

public:
	std::shared_ptr<IResource> loadResourceSync(const std::string &pName) final {
		auto result = resourceExecutor->loadResourceSync(pName);
		if (result->isErrored()) { onResourceLoadingError(pName, result->getError()); }
		return result->getResource();
	}

	void loadResourceAsync(
		const std::string &pResourceUri,
		const sigc::slot<void(const std::shared_ptr<ResourceLoadResult> &pResult)> &pSlot) const override {
		resourceExecutor->loadResourceAsync(pResourceUri, pSlot);
	}

	void onResourceLoadingError(const std::string &pResourceUri, const ReportMessagePtr &pError) override;


	void switchCamera(ICamera* pNewCamera) override;

	[[nodiscard]] const std::shared_ptr<ProgramWideShaderBuffer> &getProgramBuffer() const override {
		return programBuffer;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<ISceneObject>> &getObjects() const override { return objects; }

	[[nodiscard]] sigc::signal<void(ISceneObject* pObject)> &getOnObjectAddedSignal() override {
		return onObjectAddedSignal;
	}

	[[nodiscard]] sigc::signal<void(ISceneObject* pObject)> &getOnObjectRemovedSignal() override {
		return onObjectRemovedSignal;
	}

	[[nodiscard]] ICamera* getCurrentCamera() const override { return currentCamera; }

	[[nodiscard]] IApplication* getApplication() const override { return application; }

	[[nodiscard]] const std::shared_ptr<ILightSources> &getLightSources() const { return lightSources; }

protected:
	void setApplication(IApplication* pApplication) override { application = pApplication; }

	virtual void beforeRender();

	virtual void afterRender() {}

	sdk::ReportMessagePtr initScene() override;

	void deinitScene() override;

	void addObject(const std::shared_ptr<ISceneObject> &pObject) override;

	void removeObject(ISceneObject* pObjectToRemove) override;

private:
	void render() final;

	void resize(int pWidth, int pHeight) override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(KeyboardKey pKey, bool pPressed, const ModifierKeys &pMods) override;

	void onMouseButtonStateChanged(MouseButton pButton, bool pPressed, double pX, double pY) override;

	bool notifyOnMouseScroll(double pDx, double pDy) final;
};
} // namespace mer::sdk


#endif //SCENE_H
