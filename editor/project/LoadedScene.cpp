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

#include "LoadedScene.h"

#include <nlohmann/json.hpp>

#include "KwasarEngine/context/Application.h"
#include "KwasarEngine/context/IApplicationSettings.h"
#include "KwasarEngine/extensions/Extension.h"
#include "KwasarEngine/extensions/ExtensionRegistry.h"
#include "KwasarEngine/extensions/MainObjectExtension.h"
#include "KwasarEngine/extensions/MouseButtonExtension.h"
#include "KwasarEngine/extensions/cameras/CameraMouseExtension.h"
#include "KwasarEngine/extensions/cameras/OrbitCameraExtension.h"
#include "KwasarEngine/resources/LoadedResources.h"
#include "KwasarEngine/scene/IScene.h"
#include "KwasarEngine/scene/Scene.h"
#include "KwasarEngine/scene/objects/ISceneObject.h"
#include "KwasarEngine/scene/objects/SceneObject.h"
#include "KwasarEngine/utils/UUID.h"
#include "mvp/sceneEditor/ResourcesContext.h"

namespace ke {
class PropertyBase;
}

namespace ked {
LoadedScene::LoadedScene() {
	app = ke::Application::create();
	app->initEngine();
}

void LoadedScene::setRunDirectory(const std::filesystem::path &pPath) const {
	app->getApplicationSettings()->setRunDirectory(pPath.string());
}

bool LoadedScene::hasScene() const { return scene != nullptr; }

bool LoadedScene::hasResourcesContext() const { return getResourceLoadExecutor() != nullptr; }

ke::IResourceLoadExecutor* LoadedScene::getResourceLoadExecutor() const { return scene->getResourceExecutor(); }

void LoadedScene::setupResourcesContext(const std::shared_ptr<ResourcesContext> &pResourcesContext) {
	context = pResourcesContext;
	const auto resources = ke::LoadedResources::create();
	pResourcesContext->setResources(resources);
	pResourcesContext->setApplication(app.get());
	pResourcesContext->preloadResources();
}

void LoadedScene::initScene() { if (scene) scene->initScene(); }

void LoadedScene::uninitScene() const { if (scene) scene->deinitScene(); }

void LoadedScene::render() const {
	scene->resize(100, 100);
	scene->render();
}

ke::ReportMessagePtr LoadedScene::load(const std::filesystem::path & /*pPath*/) {
	auto resourcesContext = std::make_shared<ResourcesContext>();
	setupResourcesContext(resourcesContext);
	unload();
	scene = ke::Scene::create();
	scene->setResourceExecutor(context.get());
	onLoadingSignal();
	setName("Untitled Scene");

	if (auto msg = readObjects()) { return msg; }
	onLoadedSignal();

	std::shared_ptr<ke::ICamera> editorCamera;
	auto camera = ke::OrbitCameraExtension::create();
	auto cameraMouse = ke::CameraMouseExtension::create();
	cameraMouse->setEnabled(false);
	auto mouseButton = ke::MouseButtonExtension::create();
	mouseButton->connectButtonSignal(ke::MouseButton::BUTTON_MIDDLE,
									 [cameraMouse](ke::MouseButton /*pButton*/, bool pPressed, double /*pX*/,
												   double /*pY*/) {
										 cameraMouse->setEnabled(pPressed);
									 });

	cameraMouse->propertyAngle.getEvent().connect(camera->propertyAngle.getSetter());
	cameraObject = ke::SceneObject::create();
	cameraObject->addExtension("cameraMouse", cameraMouse);
	cameraObject->addExtension("mouseButton", mouseButton);
	cameraObject->addExtension("camera", camera);
	editorCamera = camera;
	cameraObject->getMainExtension()->propertyName = "EditorCamera";
	//viewSceneEditor->makeCurrent();
	scene->switchCamera(editorCamera.get());
	scene->addObject(cameraObject);
	return nullptr;
}

void LoadedScene::unload() {
	for (auto connection: connections) { connection.disconnect(); }
	connections.clear();
	cameraObject.reset();
	if (scene) {
		scene->deinitScene();
		scene.reset();
	}
	name.clear();
}

ke::ReportMessagePtr LoadedScene::readObjects() { return nullptr; }

void LoadedScene::addObject() {
	auto obj = createObject();
	obj->init();
	addObjectToDatabase(obj);
}

void LoadedScene::removeObject(ke::ISceneObject* pObjectToRemove) {
	std::thread([this, pObjectToRemove] { if (scene) scene->removeObject(pObjectToRemove); }).detach();
}

void LoadedScene::removeExtension(const ke::Extension* pExtensionToRemove) const {
	auto obj = pExtensionToRemove->getObject();
	if (!obj) return;

	std::shared_ptr<ke::Extension> removedExtension;
	obj->removeExtension(pExtensionToRemove->getName(), removedExtension);

	std::thread([this, obj] { saveObject(obj); }).detach();
}

void LoadedScene::renameObject(ke::ISceneObject* pObject, const std::string &pNewName) const {
	pObject->getMainExtension()->propertyName = pNewName;
	std::thread([this, pObject] { saveObject(pObject); }).detach();
}

std::shared_ptr<ke::Extension> LoadedScene::addExtension(ke::ISceneObject* pObject, const std::string &pType,
														  const std::string &pName) const {
	const auto ext = ke::ExtensionRegistry::newInstance(pType);
	pObject->addExtension(pName, ext);
	//if (hasResourcesContext()) ext->onInit();
	onExtensionAdded(ext.get());
	std::thread([this, pObject] { saveObject(pObject); }).detach();
	return ext;
}

void LoadedScene::saveObject(ke::ISceneObject* /*pObject*/) const {}

void LoadedScene::addObjectToDatabase(const std::shared_ptr<ke::ISceneObject> &pObject) const {
	nlohmann::json json;
	for (auto [extName, ext]: pObject->getExtensions()) {
		nlohmann::json arrayElement;
		arrayElement["ExtensionName"] = ext->getName();
		arrayElement["ExtensionType"] = ext->getTypeName();
		ext->serialize(arrayElement);
		json.emplace_back(arrayElement);
	}
}

void LoadedScene::onCursorPosChanged(const double pX, const double pY) const {
	if (cameraObject) cameraObject->onCursorPosChanged(pX, pY);
}

void LoadedScene::onMouseButtonStateChanged(const ke::MouseButton pButton, const bool pPressed, const double pX,
											const double pY) const {
	if (cameraObject) cameraObject->onMouseButtonStateChanged(pButton, pPressed, pX, pY);
}

std::shared_ptr<ke::ISceneObject> LoadedScene::createObject() const {
	auto object = ke::SceneObject::create();
	object->connectOnExtensionPropertyChanged(
		[this, object](ke::Extension* /*pExtension*/, ke::PropertyBase* /*pProperty*/) {
			std::thread([this, object] { saveObject(object.get()); }).detach();
		});
	scene->addObject(object);
	return object;
}
} // namespace ked
