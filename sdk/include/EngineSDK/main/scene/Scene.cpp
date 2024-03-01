//
// Created by alexus on 02.02.24.
//

#include "Scene.h"

#include "EngineSDK/main/resources/shaders/BuiltInProgramRequest.h"
#include "EngineSDK/renderer/GL.h"
#include "EngineUtils/utils/Logger.h"
#include "actor/IActor.h"
#include "cameras/Camera.h"
#include "objects/ISceneObject.h"

namespace mer::sdk::main {
Scene::Scene() : programBuffer(std::make_shared<ProgramWideShaderBuffer>()) {}

void Scene::setViewProjMatrix(const glm::mat4 &pViewProjMatrix) const {
	programBuffer->setViewProjMatrix(pViewProjMatrix);
}

void Scene::onResourceLoadingError(const std::shared_ptr<ResourceRequest> & /*pRequest*/,
								   const sdk::utils::ReportMessagePtr &pError) {

	sdk::utils::Logger::error(pError);
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
} // namespace mer::sdk::main