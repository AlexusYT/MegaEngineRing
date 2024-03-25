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
// Created by alexus on 02.02.24.
//

#ifndef SCENE_H
#define SCENE_H
#include <EngineSDK/main/buffers/ProgramWideShaderBuffer.h>
#include <EngineSDK/main/resources/LazyResource.h>

#include <sigc++/signal.h>

#include "EngineSDK/main/resources/IResources.h"
#include "EngineSDK/main/resources/ResourceRequest.h"
#include "IScene.h"

namespace mer::sdk::main {
class ICamera;
class ISceneObject;

class Scene : public IScene {
	IResources* resources;
	std::shared_ptr<ProgramWideShaderBuffer> programBuffer;
	std::vector<std::shared_ptr<ISceneObject>> objects;
	sigc::signal<void(int pWidth, int pHeight)> onWindowSizeChangedSignal;
	sigc::signal<void(ISceneObject* pObject)> onObjectAddedSignal;
	ICamera* currentCamera;

public:
	Scene();

	void setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const override;

	[[nodiscard]] IResources* getResources() const final { return resources; }

	void onResourceLoadingError(const std::shared_ptr<ResourceRequest> &pRequest,
								const sdk::utils::ReportMessagePtr &pError) override;

	void enqueueResourceLoading(const std::shared_ptr<ResourceRequest> &pRequest,
								const IResources::ResourceSlot &pSlot) const override {
		resources->enqueueResourceLoading(pRequest, pSlot);
	}

	void switchCamera(ICamera* pNewCamera) override;

	[[nodiscard]] const std::shared_ptr<ProgramWideShaderBuffer> &getProgramBuffer() const override {
		return programBuffer;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<ISceneObject>> &getObjects() const override { return objects; }

	[[nodiscard]] sigc::signal<void(ISceneObject* pObject)> &getOnObjectAddedSignal() override {
		return onObjectAddedSignal;
	}


protected:
	virtual void beforeRender();

	virtual void afterRender() {}

	sdk::utils::ReportMessagePtr initScene() override;

	void addObject(const std::shared_ptr<ISceneObject> &pObject) override;

private:
	void setResources(IResources* pResources) final;

	void render() final;

	void resize(int pWidth, int pHeight) override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) override;
};
} // namespace mer::sdk::main


#endif //SCENE_H