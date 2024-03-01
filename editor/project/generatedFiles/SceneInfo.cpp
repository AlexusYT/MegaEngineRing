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

#include "mvp/main/editors/sceneEditor/ModelSceneEditor.h"
#include "mvp/main/editors/sceneEditor/PresenterSceneEditor.h"
#include "mvp/main/editors/sceneEditor/ViewSceneEditor.h"

namespace mer::editor::project {
SceneInfo::SceneInfo(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) {
	setName("Scene1");
	uuid = UUID::create();
	setDatabaseSaveRequired();
	if (const auto msg = pProject->saveFiles()) pProject->errorOccurred(msg);
}

SceneInfo::SceneInfo(const std::shared_ptr<Project> &pProject, const std::string &pName,
					 const std::shared_ptr<UUID> &pUuid)
	: GeneratedFileEntry(pProject), uuid(pUuid) {
	setName(pName);
}

std::shared_ptr<mvp::IEditorPresenter> SceneInfo::openEditor() {

	auto modelSceneEditor = std::make_shared<mvp::ModelSceneEditor>();
	modelSceneEditor->setProject(getProject());
	modelSceneEditor->setSceneInfo(this);
	auto viewSceneEditor = std::make_shared<mvp::ViewSceneEditor>();


	return std::make_shared<mvp::PresenterSceneEditor>(viewSceneEditor, modelSceneEditor);
}

sdk::utils::ReportMessagePtr SceneInfo::onLoadDatabase() { return nullptr; }

sdk::utils::ReportMessagePtr SceneInfo::onSaveDatabase() const { return nullptr; }

sdk::utils::ReportMessagePtr SceneInfo::onSaveFile() const { return writeFile(); }

sdk::utils::ReportMessagePtr SceneInfo::createTable() { return nullptr; }

sdk::utils::ReportMessagePtr SceneInfo::writeFile() const {
	using namespace std::string_literals;
	CppSourceFile source;
	source.addInclude(getHeaderPath().filename(), false);
	source.addInclude("EngineSDK/renderer/GL.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/BasicRenderExtension.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/CameraExtension.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/CameraKeyboardExtension.h", true);
	source.addInclude("EngineSDK/main/scene/objects/extensions/CameraMouseExtension.h", true);
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
	return source.writeFile(path / getSourcePath());
}

std::shared_ptr<CppExternC> SceneInfo::createExternCBlock(const std::string &pSceneName) {

	auto loadMethod = CppMethod::create();
	loadMethod->setReturnType<sdk::main::IScene*>();
	loadMethod->setName("load" + pSceneName);
	loadMethod->addStatement(CppCustomStatement::create("return new " + pSceneName + "()"));
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
	auto render = std::make_shared<BasicRenderExtension>();
	object->addExtension("render", render);
	addObject(object);

	auto player = std::make_shared<SceneObject>();
	auto camera = std::make_shared<CameraExtension>();
	auto cameraKeyboard = std::make_shared<CameraKeyboardExtension>();
	auto cameraMouse = std::make_shared<CameraMouseExtension>();
	cameraMouse->setMethodAddAngle(sigc::mem_fun(*camera, &CameraExtension::addAngle));
	cameraKeyboard->setMethodGetAngle(sigc::mem_fun(*camera, &CameraExtension::getAngle));
	camera->getOnMatrixChanged().connect(sigc::mem_fun(*this, &Scene::setViewProjMatrix));
	player->addExtension("cameraKeyboard", cameraKeyboard);
	player->addExtension("cameraMouse", cameraMouse);
	player->addExtension("camera", camera);
	addObject(player);

	GL::setClearColor(0.0f, 0.0f, 0.9f, 1.0f);
	return Scene::initScene())"));

	return method;
}
} // namespace mer::editor::project