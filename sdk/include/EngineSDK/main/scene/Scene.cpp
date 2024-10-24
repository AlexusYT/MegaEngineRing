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

#include "Scene.h"

#include "EngineSDK/main/light/ILightInstance.h"
#include "EngineSDK/main/light/LightSources.h"
#include "EngineSDK/main/resources/LoadedResources.h"
#include "EngineSDK/main/resources/shaders/BuiltInProgramRequest.h"
#include "EngineSDK/renderer/GL.h"
#include "EngineUtils/utils/Logger.h"
#include "actor/IActor.h"
#include "objects/ISceneObject.h"
#include "objects/extensions/Extension.h"
#include "objects/extensions/cameras/ICamera.h"

namespace mer::sdk::main {
Scene::Scene() : programBuffer(std::make_shared<ProgramWideShaderBuffer>()), lightSources(LightSources::create()) {}

Scene::~Scene() {
	Scene::deinitScene();
	objects.clear();
}

std::shared_ptr<Scene> Scene::create() { return std::shared_ptr<Scene>(new Scene()); }

void Scene::setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const {
	programBuffer->setViewProjMatrix(pViewProjMatrix);
}

void Scene::onResourceLoadingError(const std::string & /*pRequest*/, const sdk::utils::ReportMessagePtr &pError) {

	sdk::utils::Logger::error(pError);
}

void Scene::switchCamera(ICamera* pNewCamera) {
	currentCamera = pNewCamera;
	static sigc::connection connection;
	if (!connection.empty()) connection.disconnect();
	connection = pNewCamera->getOnMatrixChanged().connect(sigc::mem_fun(*this, &Scene::setViewProjMatrix));
	pNewCamera->updateMatrix();
}

void Scene::beforeRender() {
	renderer::GL::setClearColor(0.2f, 0.2f, 0.2f, 0);
	renderer::GL::clear(renderer::ClearBits::COLOR_BUFFER_BIT);
}

sdk::utils::ReportMessagePtr Scene::initScene() {
	if (inited) return nullptr;
	for (const auto &object: objects) {
		if (auto msg = object->init()) return msg;
	}
	inited = true;
	return nullptr;
}

void Scene::deinitScene() {
	for (const auto &object: objects) { object->deinit(); }
}

void Scene::addObject(const std::shared_ptr<ISceneObject> &pObject) {
	pObject->setScene(this);

	if (inited) {
		if (auto msg = pObject->init()) {
			utils::Logger::warn("Object shouldn't report about errors. Ignoring");
			utils::Logger::error(msg);
		}
	}
	pObject->connectOnExtensionAdded([this](const std::shared_ptr<Extension> &pExtension) {
		if (auto lightInstance = std::dynamic_pointer_cast<ILightInstance>(pExtension)) {
			lightSources->addLightInstance(lightInstance);
		}
	});
	pObject->connectOnExtensionRemoved([this](const std::shared_ptr<Extension> &pExtension) {
		if (auto lightInstance = std::dynamic_pointer_cast<ILightInstance>(pExtension)) {
			lightSources->removeLightInstance(lightInstance);
		}
	});
	objects.emplace_back(pObject);
	onObjectAddedSignal(pObject.get());
}

void Scene::removeObject(ISceneObject* pObjectToRemove) {
	erase_if(objects, [&pObjectToRemove](const std::shared_ptr<ISceneObject> &pObject) {
		return pObject.get() == pObjectToRemove;
	});
	onObjectRemovedSignal(pObjectToRemove);
}

void Scene::render() {
	beforeRender();
	programBuffer->bindBufferBase(0);
	resourceExecutor->getResources()->render();
	for (const auto &object: objects) { object->render(); }
	afterRender();
}

void Scene::resize(const int pWidth, const int pHeight) {
	renderer::GL::viewport(0, 0, pWidth, pHeight);
	onWindowSizeChangedSignal(pWidth, pHeight);
	for (const auto &object: objects) { object->onWindowSizeChanged(pWidth, pHeight); }
}

void Scene::onCursorPosChanged(const double pX, const double pY) {
	for (const auto &object: objects) { object->onCursorPosChanged(pX, pY); }
}

void Scene::onKeyChanged(const utils::KeyboardKey pKey, const bool pPressed, const utils::ModifierKeys &pMods) {
	for (const auto &object: objects) { object->onKeyStateChanged(pKey, pPressed, pMods); }
}

void Scene::onMouseButtonStateChanged(const utils::MouseButton pButton, const bool pPressed, const double pX,
									  const double pY) {
	for (const auto &object: objects) { object->onMouseButtonStateChanged(pButton, pPressed, pX, pY); }
}

bool Scene::notifyOnMouseScroll(double pDx, double pDy) {
	bool handled{};
	for (const auto &object: objects) handled = object->notifyOnMouseScroll(pDx, pDy) || handled;
	return handled;
}
} // namespace mer::sdk::main