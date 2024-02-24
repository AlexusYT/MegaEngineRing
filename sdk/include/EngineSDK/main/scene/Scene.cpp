//
// Created by alexus on 02.02.24.
//

#include "Scene.h"

#include "EngineSDK/renderer/GL.h"
#include "EngineUtils/utils/Logger.h"
#include "objects/BasicRenderObject.h"
#include "objects/ISceneObject.h"

namespace mer::sdk::main {
Scene::Scene() {}

void Scene::onResourceLoadingError(const std::shared_ptr<ResourceRequest> & /*pRequest*/,
								   const sdk::utils::ReportMessagePtr &pError) {

	sdk::utils::Logger::error(pError);
}

void Scene::beforeRender() { renderer::GL::clear(renderer::ClearBits::COLOR_BUFFER_BIT); }

sdk::utils::ReportMessagePtr Scene::initScene() {
	for (const auto &sceneObject: objects) {
		if (auto msg = sceneObject->init()) return msg;
	}
	return nullptr;
}

sdk::utils::ReportMessagePtr Scene::preloadScene(const std::shared_ptr<ResourceRequests> &pRequests) {
	for (const auto &sceneObject: objects) { sceneObject->fillResourceRequests(pRequests); }
	return nullptr;
}

void Scene::addObject(const std::shared_ptr<ISceneObject> &pObject) { objects.emplace_back(pObject); }

void Scene::setResources(const std::shared_ptr<Resources> &pResources) {
	for (const auto &object: objects) { object->setResources(pResources); }
	resources = pResources;
}

void Scene::render() {
	beforeRender();
	for (const auto &object: objects)
		//

		object->render();
	afterRender();
}

void Scene::resize(const int pWidth, const int pHeight) { renderer::GL::viewport(0, 0, pWidth, pHeight); }
} // namespace mer::sdk::main