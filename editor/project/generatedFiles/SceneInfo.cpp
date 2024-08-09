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
// Created by alexus on 28.01.24.
//

#include "SceneInfo.h"

#include <EngineSDK/main/scene/Scene.h>

#include <project/Project.h>
#include <project/generators/cpp/AccessModifier.h>
#include <project/generators/cpp/CppClass.h>
#include <project/generators/cpp/CppCustomStatement.h>
#include <project/generators/cpp/CppExternC.h>
#include <project/generators/cpp/CppHeaderFile.h>
#include <project/generators/cpp/CppMethod.h>

#include "EngineSDK/main/scene/ISceneDataInjector.h"
#include "GraphicsScript.h"
#include "mvp/main/editors/sceneEditor/ModelSceneEditor.h"
#include "mvp/main/editors/sceneEditor/PresenterSceneEditor.h"
#include "mvp/main/editors/sceneEditor/ViewSceneEditor.h"

namespace mer::editor::project {
SceneInfo::SceneInfo(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) {
	resetSceneInfo();
	setName("Scene1");
	uuid = UUID::create();
	setDatabaseSaveRequired();
	if (const auto msg = pProject->saveFiles()) pProject->errorOccurred(msg);
}

SceneInfo::SceneInfo(const std::shared_ptr<Project> &pProject, const std::string &pName,
					 const std::shared_ptr<UUID> &pUuid)
	: GeneratedFileEntry(pProject), uuid(pUuid) {
	resetSceneInfo();
	setName(pName);
}

std::shared_ptr<mvp::IEditorPresenter> SceneInfo::openEditor() {

	auto modelSceneEditor = std::make_shared<mvp::ModelSceneEditor>();
	modelSceneEditor->setProject(getProject());
	modelSceneEditor->setSceneInfo(this);
	auto viewSceneEditor = std::make_shared<mvp::ViewSceneEditor>();


	return std::make_shared<mvp::PresenterSceneEditor>(viewSceneEditor, modelSceneEditor);
}

void SceneInfo::onGetActionGroup(const Glib::RefPtr<Gio::SimpleActionGroup> &pSimpleActionGroup) {

	if (!toplevelObjects) return;
	for (uint32_t i = 0; i < toplevelObjects->get_n_items(); i++) {
		auto entry = toplevelObjects->get_item(i);
		entry->getActionGroup(pSimpleActionGroup);
	}
}

void SceneInfo::resetSceneInfo() {
	objects.clear();
	toplevelObjects = Gio::ListStore<ui::EditorSceneObject>::create();
}

sdk::utils::ReportMessagePtr SceneInfo::onLoadDatabase() { return nullptr; }

sdk::utils::ReportMessagePtr SceneInfo::onSaveDatabase() const {
	auto database = getProject()->getDatabase();

	if (auto msg = createTable()) return msg;
	if (auto msg = GraphicsScript::createTable(database)) return msg;

	std::unique_ptr<SQLite::Transaction> transaction;
	try {
		transaction = std::make_unique<SQLite::Transaction>(*database);
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to save Scripts into database");
		msg->setMessage("Exception occurred while starting transaction");
		return msg;
	}
	try {
		//language=sql
		SQLite::Statement statement(*database, R"(
INSERT INTO Scripts (ObjectUUID) VALUES (?)
)");
		for (auto [objectUuid, object]: objects) {
			auto script = object->getGraphicsScript();
			if (!script) continue;
			statement.bind(1, objectUuid->toString());
			statement.executeStep();
			statement.clearBindings();
			statement.reset();
			script->setScriptId(database->getLastInsertRowid());
		}
	} catch (...) {
		transaction->rollback();
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to create Scripts table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	transaction->commit();
	for (auto [objUuid, object]: objects) {
		if (auto msg = object->saveDatabase()) return msg;
	}
	return nullptr;
}

sdk::utils::ReportMessagePtr SceneInfo::onSaveFile() const { return writeFile(); }

sdk::utils::ReportMessagePtr SceneInfo::createTable() const {
	try {
		//language=sql
		getProject()->getDatabase()->exec(R"(DROP TABLE IF EXISTS Scripts;
CREATE TABLE Scripts
(
    ScriptID   INTEGER
        CONSTRAINT Scripts_pk
            PRIMARY KEY AUTOINCREMENT,
    ObjectUUID TEXT    NOT NULL
);
)");
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to create Scripts table");
		msg->setMessage("Exception occurred while executing query");
		return msg;
	}

	return nullptr;
}

sdk::utils::ReportMessagePtr SceneInfo::writeFile() const {
	using namespace std::string_literals;
	CppSourceFile source;
	source.addInclude(getHeaderPath().filename(), false);
	source.addInclude("EngineSDK/renderer/GL.h", true);
	source.addInclude("EngineSDK/main/scene/SceneDataInjector.h", true);
	source.addInclude("EngineSDK/main/resources/shaders/BuiltInProgramRequest.h", true);
	source.addInclude("EngineSDK/main/resources/models/ObjModelRequest.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/ModelRenderExtension.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/cameras/CameraExtension.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/cameras/CameraKeyboardExtension.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/cameras/CameraMouseExtension.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h", true);
	source.addInclude("EngineSDK/main/scene/objects/SceneObject.h", true);
	if (primaryScene) {
		source.addInclude("EngineSDK/main/scene/IScene.h");
		auto getPrimarySceneMethod = CppMethod::create();
		getPrimarySceneMethod->setReturnType<std::shared_ptr<mer::sdk::main::IScene>>();
		getPrimarySceneMethod->setName("getPrimaryScene");
		getPrimarySceneMethod->addStatement(
			CppCustomStatement::create(std::format("return std::make_shared<{}>()", getName())));
		source.addDefinition(getPrimarySceneMethod->getDefinition());
	}
	source.addDefinition(createExternCBlock(getName())->getDefinition());

	const std::shared_ptr<CppClass> class_ = CppClass::create(getName());
	class_->addImplement("public mer::sdk::main::Scene");

	const auto initMethod = createInitMethod(class_);
	source.addDefinition(initMethod->getDefinition());
	class_->addDeclaration(initMethod->getDeclaration(), AccessModifier::PRIVATE);

	CppHeaderFile header;
	header.addDefinition(class_->getDefinition());
	header.addInclude("EngineSDK/main/scene/Scene.h");
	header.addInclude("memory");
	auto path = getProject()->getProjectPath();
	if (auto msg = header.writeFile(path / getHeaderPath())) return msg;
	if (auto msg = source.writeFile(path / getSourcePath())) return msg;
	for (auto [objUuid, object]: objects) {
		if (auto msg = object->saveFile()) return msg;
	}
	return nullptr;
}

std::shared_ptr<CppExternC> SceneInfo::createExternCBlock(const std::string &pSceneName) {

	auto loadMethod = CppMethod::create();
	loadMethod->setReturnType<sdk::main::ISceneDataInjector*>();
	loadMethod->setName("load" + pSceneName);
	loadMethod->addStatement(
		CppCustomStatement::create("return new mer::sdk::main::SceneDataInjector(new " + pSceneName + "())"));
	auto externC = CppExternC::create();
	externC->addDefinition(loadMethod->getDefinition());
	return externC;
}

std::shared_ptr<CppMethod> SceneInfo::createInitMethod(const std::shared_ptr<CppClass> &pClass) {
	auto method = CppMethod::create();
	method->setKlass(pClass.get());
	method->setName("initScene");
	method->setReturnType<sdk::utils::ReportMessagePtr>();
	method->setIsOverride(true);
	//language=c++
	method->addStatement(CppCustomStatement::create(R"(
	using namespace mer::sdk::renderer;
	using namespace mer::sdk::main;
	using namespace mer::sdk::utils;

	auto object = std::make_shared<SceneObject>();
	object->setScriptName("SceneObject0Script");
	auto render = ModelRenderExtension::create();
	render->setShaderRequest(BuiltInProgramRequest::getDefaultProgram());
	render->setModelRequest(std::make_shared<FileModelRequest>("TestModel",  "Resources/Cube.obj"), "Cube");
	object->addExtension("render", render);
	addObject(object);

	auto player = std::make_shared<SceneObject>();
	player->setScriptName("SceneObject1Script");
	auto camera = CameraExtension::create();
	auto cameraKeyboard = CameraKeyboardExtension::create();
	auto cameraMouse = CameraMouseExtension::create();
	//cameraMouse->setMethodAddAngle(sigc::mem_fun(*camera, &CameraExtension::addAngle));

	//camera->getOnAngleChanged().connect(sigc::mem_fun(*cameraKeyboard, &CameraKeyboardExtension::setAngle));
	switchCamera(camera.get());
	player->addExtension("cameraKeyboard", cameraKeyboard);
	player->addExtension("cameraMouse", cameraMouse);
	player->addExtension("camera", camera);
	addObject(player);

	GL::setClearColor(0.0f, 0.0f, 0.9f, 1.0f);
	return Scene::initScene())"));

	return method;
}
} // namespace mer::editor::project