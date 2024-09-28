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

#include "EngineSDK/main/Application.h"
#include "EngineSDK/main/scene/IScene.h"
#include "EngineSDK/main/scene/ISceneDataInjector.h"
#include "EngineSDK/main/scene/objects/ISceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "EngineUtils/utils/UUID.h"
#include "Sdk.h"
#include "mvp/main/editors/sceneEditor/ResourcesContext.h"
#include "mvp/main/editors/sceneEditor/explorerObjects/RootExplorerObject.h"
#include "mvp/main/editors/sceneEditor/explorerObjects/SceneExplorerObject.h"

namespace mer::editor::project {
LoadedScene::LoadedScene(const std::shared_ptr<Sdk> &pSdk) : sdk(pSdk), mainObject(mvp::RootExplorerObject::create()) {
	app = sdk->createApplication();
	app->initEngine();
}

void LoadedScene::setRunDirectory(const std::filesystem::path &pPath) const {
	app->getApplicationSettings()->setRunDirectory(pPath);
}

bool LoadedScene::hasScene() const { return scene != nullptr; }

bool LoadedScene::hasResourcesContext() const { return scene->getResources() != nullptr; }

void LoadedScene::setupResourcesContext(const std::shared_ptr<mvp::ResourcesContext> &pResourcesContext) const {
	scene->setResources(pResourcesContext.get());
	const auto resources = sdk->createLoadedResources(pResourcesContext.get());
	pResourcesContext->setResources(resources);
	pResourcesContext->setApplication(app.get());
}

void LoadedScene::initScene() const { scene->initScene(); }

void LoadedScene::render() const { scene->render(); }

sdk::utils::ReportMessagePtr LoadedScene::load(const std::filesystem::path &pPath) {

	unload();
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
	scene = sdk->createScene();
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
	auto injector = sdk->createSceneInjector(scene.get());
	std::shared_ptr<sdk::main::ICamera> editorCamera;
	injector->setupEditorCamera(cameraObject, editorCamera);
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
			std::string objectName = statement.getColumn(1);
			auto object = createObject(objectName);
			object->setUuid(UUID::parse(statement.getColumn(2)));
			nlohmann::json json = nlohmann::json::parse(statement.getColumn(3).getString());
			if (json.is_array()) {
				for (const auto &ext: json) {
					if (ext.is_object()) {
						auto extName = ext["name"].get<std::string>();
						auto extType = ext["type"].get<std::string>();
						addExtension(object.get(), extType, extName);
					}
				}
			}
		}
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to parse scene settings");
		msg->setMessage("Exception occurred while querying the data");
		return msg;
	}
	return nullptr;
}

void LoadedScene::addObject(const std::string &pName) {
	auto obj = createObject(pName);
	obj->init();
	//addExtension(obj.get(), "ModelRenderExtension", "render");
	//auto render = sdk->newExtensionInstance<sdk::main::ModelRenderExtension>();
	//render->setShaderRequest(sdk->getDefaultShaderProgram());
	//render->setModelRequest(sdk->createFileModelRequest("TestModel", "Resources/Cube.obj"), "Cube");
	//pObject->addExtension("render", render);
	addObjectToDatabase(obj);
}

void LoadedScene::removeObject(sdk::main::ISceneObject* pObjectToRemove) {

	std::thread([this, pObjectToRemove] {
		removeObjectFromDatabase(pObjectToRemove);
		if (scene) scene->removeObject(pObjectToRemove);
	}).detach();
}

void LoadedScene::renameObject(sdk::main::ISceneObject* pObject, const std::string &pNewName) const {
	pObject->setName(pNewName);
	std::thread([this, pObject] { saveObject(pObject); }).detach();
}

std::shared_ptr<sdk::main::Extension> LoadedScene::addExtension(
	sdk::main::ISceneObject* pObject, const std::string &pType, const std::string &pName) const {
	const auto ext = sdk->newExtensionInstance(pType);
	pObject->addExtension(pName, ext);
	if (hasResourcesContext()) ext->onInit();
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
		arrayElement["name"] = ext->getName();
		arrayElement["type"] = ext->getTypeName();
		ext->serialize(arrayElement);
		json.emplace_back(arrayElement);
	}
	try {
		//language=sql
		SQLite::Statement statement(*database, R"(
UPDATE Objects SET Name = ?, Extensions = ? WHERE UUID = ?
)");
		statement.bind(1, pObject->getName());
		statement.bind(2, json.dump(4));
		statement.bind(3, pObject->getUuid()->toString());
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

	try {
		//language=sql
		SQLite::Statement statement(*database, R"(
INSERT INTO Objects (Name, UUID, Extensions) VALUES (?, ?, ?)
)");
		statement.bind(1, pObject->getName());
		statement.bind(2, pObject->getUuid()->toString());
		statement.bind(3, "null");
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

void LoadedScene::renameObjectInDatabase(const std::string &pOldName, const std::string &pNewName) const {
	if (!database->tableExists("Objects")) return;
	try {
		//language=sql
		SQLite::Statement statement(*database, R"(UPDATE Objects SET Name = ? WHERE Name = ?)");
		statement.bind(1, pNewName);
		statement.bind(2, pOldName);
		statement.executeStep();
		statement.clearBindings();
		statement.reset();
	} catch (...) {
		const auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to rename the object in the table");
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
    Name  TEXT    NOT NULL,
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

std::shared_ptr<sdk::main::ISceneObject> LoadedScene::createObject(const std::string &pName) const {
	auto object = sdk->createSceneObject();
	object->setName(pName);

	object->connectOnNameChanged([this, object](const std::string & /*pOldName*/, const std::string & /*pNewName*/) {
		std::thread([this, object] { saveObject(object.get()); }).detach();
	});
	scene->addObject(object);
	return object;
}

} // namespace mer::editor::project