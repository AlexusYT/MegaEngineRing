//
// Created by alexus on 02.02.24.
//

#include "Scene.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "EngineSDK/main/resources/shaders/BuiltInProgramRequest.h"
#include "EngineSDK/renderer/GL.h"
#include "EngineUtils/utils/Logger.h"
#include "actor/IActor.h"
#include "cameras/Camera.h"
#include "objects/ISceneObject.h"

namespace mer::sdk::main {
Scene::Scene() : programBuffer(std::make_shared<ProgramWideShaderBuffer>()) {}

void Scene::onResourceLoadingError(const std::shared_ptr<ResourceRequest> & /*pRequest*/,
								   const sdk::utils::ReportMessagePtr &pError) {

	sdk::utils::Logger::error(pError);
}

void Scene::beforeRender() { renderer::GL::clear(renderer::ClearBits::COLOR_BUFFER_BIT); }

sdk::utils::ReportMessagePtr Scene::initScene() {
	camera = std::make_shared<Camera>();
	addObject(camera);
	for (const auto &actor: objects) {
		if (const auto sceneObject = std::dynamic_pointer_cast<ISceneObject>(actor))
			if (auto msg = sceneObject->init()) return msg;
	}
	getResourceByRequest(BuiltInProgramRequest::getDefaultProgram())
		->attachSsbo(programBuffer, "ProgramWideSettings", 0);
	camera->connectOnMatrixChangedSignal(
		[this](const glm::mat4 &pMatrix) { programBuffer->setViewProjMatrix(pMatrix); });

	return nullptr;
}

sdk::utils::ReportMessagePtr Scene::preloadScene(const std::shared_ptr<ResourceRequests> &pRequests) {

	for (const auto &actor: objects) {
		if (const auto sceneObject = std::dynamic_pointer_cast<ISceneObject>(actor))
			sceneObject->fillResourceRequests(pRequests);
	}
	return nullptr;
}

void Scene::addObject(const std::shared_ptr<IActor> &pObject) { objects.emplace_back(pObject); }

void Scene::setResources(const std::shared_ptr<Resources> &pResources) {

	for (const auto &actor: objects) {
		if (const auto sceneObject = std::dynamic_pointer_cast<ISceneObject>(actor))
			sceneObject->setResources(pResources);
	}
	resources = pResources;
}

void Scene::render() {
	beforeRender();
	for (const auto &actor: objects) {
		if (const auto sceneObject = std::dynamic_pointer_cast<ISceneObject>(actor))
			//

			sceneObject->render();
		else
			actor->update();
	}
	afterRender();
}

void Scene::resize(const int pWidth, const int pHeight) {
	renderer::GL::viewport(0, 0, pWidth, pHeight);
	if (pHeight <= 0) return;
	camera->setViewportAspect(static_cast<float>(pWidth) / static_cast<float>(pHeight));
}

void Scene::onCursorPosChanged(const double pX, const double pY) {

	for (const auto &actor: objects) { actor->onCursorPosChanged(pX, pY); }
}

void Scene::onKeyChanged(const utils::KeyboardKey pKey, const bool pPressed, const utils::ModifierKeys pMods) {
	for (const auto &actor: objects) { actor->onKeyStateChanged(pKey, pPressed, pMods); }
}
} // namespace mer::sdk::main