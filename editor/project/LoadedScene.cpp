//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

#include <SQLiteCpp/Database.h>
#include <nlohmann/json.hpp>

#include "EditorFileSystemResourceBundle.h"
#include "EngineSDK/main/Application.h"
#include "EngineSDK/main/resources/LoadedResources.h"
#include "EngineSDK/main/scene/IScene.h"
#include "EngineSDK/main/scene/Scene.h"
#include "EngineSDK/main/scene/objects/ISceneObject.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h"
#include "EngineSDK/main/scene/objects/extensions/MainObjectExtension.h"
#include "EngineSDK/main/scene/objects/extensions/MouseButtonExtension.h"
#include "EngineSDK/main/scene/objects/extensions/cameras/CameraMouseExtension.h"
#include "EngineSDK/main/scene/objects/extensions/cameras/OrbitCameraExtension.h"
#include "EngineUtils/utils/UUID.h"
#include "mvp/main/editors/sceneEditor/EditorCameraScript.h"
#include "mvp/main/editors/sceneEditor/ResourcesContext.h"
#include "mvp/main/editors/sceneEditor/explorerObjects/RootExplorerObject.h"
#include "mvp/main/editors/sceneEditor/explorerObjects/SceneExplorerObject.h"

namespace mer::sdk::utils {
class PropertyBase;
}

namespace mer::editor::project {
LoadedScene::LoadedScene() : mainObject(mvp::RootExplorerObject::create()) {
	app = sdk::main::Application::create();
	app->initEngine();
}

void LoadedScene::setRunDirectory(const std::filesystem::path &pPath) const {
	app->getApplicationSettings()->setRunDirectory(pPath);
	app->setResourceBundle(EditorFileSystemResourceBundle::create(pPath / "data"));
}

bool LoadedScene::hasScene() const { return scene != nullptr; }

bool LoadedScene::hasResourcesContext() const { return getResourceLoadExecutor() != nullptr; }

sdk::main::IResourceLoadExecutor* LoadedScene::getResourceLoadExecutor() const { return scene->getResourceExecutor(); }

void LoadedScene::setupResourcesContext(const std::shared_ptr<mvp::ResourcesContext> &pResourcesContext) {
	context = pResourcesContext;
	const auto resources = sdk::main::LoadedResources::create();
	pResourcesContext->setResources(resources);
	pResourcesContext->setApplication(app.get());
	pResourcesContext->preloadResources();
}

void LoadedScene::initScene() const {
	if (scene) scene->initScene();
}

void LoadedScene::uninitScene() const {
	if (scene) scene->deinitScene();
}

void LoadedScene::render() const { scene->render(); }

sdk::utils::ReportMessagePtr LoadedScene::load(const std::filesystem::path &pPath) {

	unload();
	scene = sdk::main::Scene::create();
	scene->setResourceExecutor(context.get());
	onLoadingSignal();
	setName("Untitled Scene");
	databaseLocked = true;

	try {
		database = std::make_shared<SQLite::Database>(pPath, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to open scene file");
		msg->setMessage("Database connection failed");
		msg->addInfoLine("File path: {}", pPath.string());
		databaseLocked = false;
		return msg;
	}
	if (auto msg = readSceneSettings()) {
		databaseLocked = false;
		return msg;
	}
	connections.push_back(scene->getOnObjectAddedSignal().connect([this](sdk::main::ISceneObject* pObject) {
		if (pObject == cameraObject.get()) return;
		auto explorerObject = mvp::SceneExplorerObject::create(pObject);

		sceneObjByExplorer.emplace(explorerObject.get(), pObject);
		explorerBySceneObj.emplace(pObject, explorerObject);
		mainObject->addChild(explorerObject);
		//std::thread([this, explorerObject] { addObjectToDatabase(explorerObject); }).detach();
	}));

	connections.push_back(scene->getOnObjectRemovedSignal().connect([this](sdk::main::ISceneObject* pObject) {
		auto iter = explorerBySceneObj.find(pObject);
		if (iter == explorerBySceneObj.end()) return;
		auto sceneObj = iter->second;
		if (selectedObject == sceneObj.get()) { selectObject(nullptr); }
		mainObject->removeChild(sceneObj);
		sceneObjByExplorer.erase(sceneObj.get());
		explorerBySceneObj.erase(iter);
	}));

	if (auto msg = readObjects()) {
		databaseLocked = false;
		return msg;
	}
	onLoadedSignal();

	std::shared_ptr<sdk::main::ICamera> editorCamera;
	auto camera = sdk::main::OrbitCameraExtension::create();
	auto cameraMouse = sdk::main::CameraMouseExtension::create();
	cameraMouse->setEnabled(false);
	auto mouseButton = sdk::main::MouseButtonExtension::create();
	mouseButton->connectButtonSignal(sdk::utils::MouseButton::BUTTON_MIDDLE,
									 [cameraMouse](sdk::utils::MouseButton /*pButton*/, bool pPressed, double /*pX*/,
												   double /*pY*/) { cameraMouse->setEnabled(pPressed); });

	cameraMouse->propertyAngle.getEvent().connect(camera->propertyAngle.getSetter());
	cameraObject = sdk::main::SceneObject::create();
	cameraObject->addExtension("cameraMouse", cameraMouse);
	cameraObject->addExtension("mouseButton", mouseButton);
	cameraObject->addExtension("camera", camera);
	editorCamera = camera;
	cameraObject->getMainExtension()->propertyName = "EditorCamera";
	auto editorCameraScript = std::make_shared<mvp::EditorCameraScript>();
	cameraObject->setScript(editorCameraScript);
	//viewSceneEditor->makeCurrent();
	scene->switchCamera(editorCamera.get());
	scene->addObject(cameraObject);
	databaseLocked = false;
	return nullptr;
}

void LoadedScene::unload() {
	for (auto connection: connections) { connection.disconnect(); }
	connections.clear();
	selectObject(nullptr);
	cameraObject.reset();
	mainObject->clearChildren();
	sceneObjByExplorer.clear();
	explorerBySceneObj.clear();
	database.reset();
	if (scene) {
		scene->deinitScene();
		scene.reset();
	}
	name.clear();
}

sdk::utils::ReportMessagePtr LoadedScene::readSceneSettings() {
	if (!database->tableExists("Settings")) return nullptr;

	try {
		//language=sql
		SQLite::Statement statement(*database, "SELECT * FROM Settings");
		while (statement.executeStep()) {
			std::string propertyName = statement.getColumn(1);
			const auto &column = statement.getColumn(2);
			if (propertyName == "Name") { setName(column.getString()); }
		}
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to parse scene settings");
		msg->setMessage("Exception occurred while querying the data");
		return msg;
	}

	return nullptr;
}

sdk::utils::ReportMessagePtr LoadedScene::readObjects() {
	if (!database->tableExists("Objects")) return nullptr;

	try {
		//language=sql
		SQLite::Statement statement(*database, "SELECT * FROM Objects");
		while (statement.executeStep()) {
			auto object = sdk::main::SceneObject::create();
			object->setUuid(UUID::parse(statement.getColumn(1)));
			scene->addObject(object);
			nlohmann::json json = nlohmann::json::parse(statement.getColumn(2).getString());
			if (json.is_array()) {
				for (const auto &ext: json) {
					if (ext.is_object()) {
						sdk::main::Extension* extension;
						auto extType = ext.at("ExtensionType").get<std::string>();
						if (extType == sdk::main::MainObjectExtension::typeName()) {
							extension = object->getMainExtension();
						} else {
							auto extName = ext.at("ExtensionName").get<std::string>();
							extension = addExtension(object.get(), extType, extName).get();
						}
						extension->deserialize(ext);
					}
				}
			}
			object->connectOnExtensionPropertyChanged(
				[this, object](sdk::main::Extension* /*pExtension*/, sdk::utils::PropertyBase* /*pProperty*/) {
					std::thread([this, object] { saveObject(object.get()); }).detach();
				});
		}
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to parse scene objects");
		msg->setMessage("Exception occurred while querying the data");
		return msg;
	}
	return nullptr;
}

void LoadedScene::addObject() {
	auto obj = createObject();
	obj->init();
	addObjectToDatabase(obj);
}

void LoadedScene::removeObject(sdk::main::ISceneObject* pObjectToRemove) {

	std::thread([this, pObjectToRemove] {
		removeObjectFromDatabase(pObjectToRemove);
		if (scene) scene->removeObject(pObjectToRemove);
	}).detach();
}

void LoadedScene::removeExtension(const sdk::main::Extension* pExtensionToRemove) const {
	auto obj = pExtensionToRemove->getObject();
	if (!obj) return;

	std::shared_ptr<sdk::main::Extension> removedExtension;
	obj->removeExtension(pExtensionToRemove->getName(), removedExtension);

	std::thread([this, obj] { saveObject(obj); }).detach();
}

void LoadedScene::renameObject(sdk::main::ISceneObject* pObject, const std::string &pNewName) const {
	pObject->getMainExtension()->propertyName = pNewName;
	std::thread([this, pObject] { saveObject(pObject); }).detach();
}

std::shared_ptr<sdk::main::Extension> LoadedScene::addExtension(
	sdk::main::ISceneObject* pObject, const std::string &pType, const std::string &pName) const {
	const auto ext = sdk::main::ExtensionRegistry::newInstance(pType);
	pObject->addExtension(pName, ext);
	//if (hasResourcesContext()) ext->onInit();
	onExtensionAdded(ext.get());
	std::thread([this, pObject] { saveObject(pObject); }).detach();
	return ext;
}

void LoadedScene::saveObject(sdk::main::ISceneObject* pObject) const {
	if (databaseLocked) return;
	if (!database->tableExists("Objects")) return;
	nlohmann::json json;
	for (auto [extName, ext]: pObject->getExtensions()) {
		nlohmann::json arrayElement;
		arrayElement["ExtensionName"] = ext->getName();
		arrayElement["ExtensionType"] = ext->getTypeName();
		ext->serialize(arrayElement);
		json.emplace_back(arrayElement);
	}
	try {
		//language=sql
		SQLite::Statement statement(*database, R"(
UPDATE Objects SET Extensions = ? WHERE UUID = ?
)");
		statement.bind(1, json.dump(4));
		statement.bind(2, pObject->getUuid()->toString());
		statement.executeStep();
		statement.clearBindings();
		statement.reset();
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to save the object to the table");
		msg->setMessage("Exception occurred while executing query");
		onErrorOccurred(msg);
	}
}

void LoadedScene::addObjectToDatabase(const std::shared_ptr<sdk::main::ISceneObject> &pObject) const {
	if (databaseLocked) return;

	if (!database->tableExists("Objects")) {
		if (const auto msg = createObjectsTable()) {
			onErrorOccurred(msg);
			return;
		}
	}
	nlohmann::json json;
	for (auto [extName, ext]: pObject->getExtensions()) {
		nlohmann::json arrayElement;
		arrayElement["ExtensionName"] = ext->getName();
		arrayElement["ExtensionType"] = ext->getTypeName();
		ext->serialize(arrayElement);
		json.emplace_back(arrayElement);
	}

	try {
		//language=sql
		SQLite::Statement statement(*database, R"(
INSERT INTO Objects (UUID, Extensions) VALUES (?, ?)
)");
		statement.bind(1, pObject->getUuid()->toString());
		statement.bind(2, json.dump(4));
		statement.executeStep();
		statement.clearBindings();
		statement.reset();
	} catch (...) {
		const auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to insert the new object to the table");
		msg->setMessage("Exception occurred while executing query");
		onErrorOccurred(msg);
	}
}

void LoadedScene::removeObjectFromDatabase(sdk::main::ISceneObject* pObject) const {
	if (!database->tableExists("Objects")) return;

	try {
		//language=sql
		SQLite::Statement statement(*database, R"(DELETE FROM Objects WHERE (UUID = ?))");
		statement.bind(1, pObject->getUuid()->toString());
		statement.executeStep();
		statement.clearBindings();
		statement.reset();
	} catch (...) {
		const auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to delete the object from the table");
		msg->setMessage("Exception occurred while executing query");
		onErrorOccurred(msg);
	}
}

sdk::utils::ReportMessagePtr LoadedScene::createObjectsTable() const {
	try {
		//language=sql
		database->exec(R"(CREATE TABLE Objects
(
    Id    INTEGER
        CONSTRAINT Objects_pk
            PRIMARY KEY AUTOINCREMENT,
    UUID  TEXT    NOT NULL,
    Extensions  TEXT    NOT NULL
);
)");
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to create the objects table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}
	return nullptr;
}

void LoadedScene::onCursorPosChanged(const double pX, const double pY) const {
	if (cameraObject) cameraObject->onCursorPosChanged(pX, pY);
}

void LoadedScene::onMouseButtonStateChanged(const sdk::utils::MouseButton pButton, const bool pPressed, const double pX,
											const double pY) const {
	if (cameraObject) cameraObject->onMouseButtonStateChanged(pButton, pPressed, pX, pY);
}

void LoadedScene::selectObject(mvp::ExplorerObject* pObjectToSelect) {
	if (selectedObject == pObjectToSelect) return;
	selectedObject = pObjectToSelect;
	onSelectionChanged(selectedObject);
}

std::shared_ptr<sdk::main::ISceneObject> LoadedScene::createObject() const {
	auto object = sdk::main::SceneObject::create();
	object->connectOnExtensionPropertyChanged(
		[this, object](sdk::main::Extension* /*pExtension*/, sdk::utils::PropertyBase* /*pProperty*/) {
			std::thread([this, object] { saveObject(object.get()); }).detach();
		});
	scene->addObject(object);
	return object;
}

} // namespace mer::editor::project