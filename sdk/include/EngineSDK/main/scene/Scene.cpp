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

#include "EngineSDK/main/resources/shaders/BuiltInProgramRequest.h"
#include "EngineSDK/renderer/GL.h"
#include "EngineUtils/utils/Logger.h"
#include "actor/IActor.h"
#include "objects/ISceneObject.h"
#include "objects/extensions/cameras/ICamera.h"

namespace mer::sdk::main {
Scene::Scene() : programBuffer(std::make_shared<ProgramWideShaderBuffer>()) {}

void Scene::setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const {
	programBuffer->setViewProjMatrix(pViewProjMatrix);
}

void Scene::onResourceLoadingError(const std::shared_ptr<ResourceRequest> & /*pRequest*/,
								   const sdk::utils::ReportMessagePtr &pError) {

	sdk::utils::Logger::error(pError);
}

void Scene::switchCamera(ICamera* pNewCamera) {
	currentCamera = pNewCamera;
	static sigc::connection connection;
	if (!connection.empty()) connection.disconnect();
	connection = pNewCamera->getOnMatrixChanged().connect(sigc::mem_fun(*this, &Scene::setViewProjMatrix));
	pNewCamera->updateMatrix();
}

void Scene::beforeRender() { renderer::GL::clear(renderer::ClearBits::COLOR_BUFFER_BIT); }

sdk::utils::ReportMessagePtr Scene::initScene() {
	for (const auto &object: objects) {
		if (auto msg = object->init()) return msg;
	}

	return nullptr;
}

void Scene::addObject(const std::shared_ptr<ISceneObject> &pObject) {
	pObject->setScene(this);
	objects.emplace_back(pObject);
	onObjectAddedSignal(pObject.get());
}

void Scene::setResources(IResources* pResources) { resources = pResources; }

void Scene::render() {
	beforeRender();
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
} // namespace mer::sdk::main