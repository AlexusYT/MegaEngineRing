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

namespace PROJECT_CORE {
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

engine::utils::ReportMessagePtr SceneInfo::onLoadDatabase() { return nullptr; }

engine::utils::ReportMessagePtr SceneInfo::onSaveDatabase() const { return nullptr; }

engine::utils::ReportMessagePtr SceneInfo::onSaveFile() const { return writeFile(); }

engine::utils::ReportMessagePtr SceneInfo::createTable() { return nullptr; }

engine::utils::ReportMessagePtr SceneInfo::writeFile() const {
	using namespace std::string_literals;
	CppSourceFile source;
	source.addInclude(getHeaderPath().filename(), false);
	source.addInclude("EngineSDK/main/resources/ResourceRequests.h", true);
	source.addInclude("EngineSDK/main/resources/shaders/BuiltInProgramRequest.h", true);
	source.addInclude("EngineSDK/main/resources/LazyResource.h", true);
	source.addInclude("EngineSDK/renderer/shaders/ShaderProgram.h", true);
	source.addInclude("EngineUtils/utils/Logger.h", true);
	source.addDefinition(createExternCBlock(getName())->getDefinition());

	const std::shared_ptr<CppClass> class_ = CppClass::create(getName());
	class_->addImplement("public n::sdk::main::Scene");

	const std::shared_ptr<CppMethod> method = createPreloadSceneMethod(class_);
	source.addDefinition(method->getDefinition());
	class_->addDeclaration(method->getDeclaration(), AccessModifier::PRIVATE);

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

std::shared_ptr<CppMethod> SceneInfo::createPreloadSceneMethod(const std::shared_ptr<CppClass> &pClass) {
	auto method = CppMethod::create();
	method->setKlass(pClass.get());
	method->setName("preloadScene");
	method->setReturnType<engine::utils::ReportMessagePtr>();
	method->setParamsList<const std::shared_ptr<sdk::main::ResourceRequests> &>("pRequests");
	method->setIsOverride(true);

	method->addStatement(CppCustomStatement::create("using namespace n::sdk::main"));
	method->addStatement(CppCustomStatement::create("using namespace n::engine::utils"));
	method->addStatement(
		CppCustomStatement::create("pRequests->addRequest(BuiltInProgramRequest::getDefaultProgram())"));
	method->addStatement(CppCustomStatement::create("return nullptr"));
	return method;
}

std::shared_ptr<CppMethod> SceneInfo::createInitMethod(const std::shared_ptr<CppClass> &pClass) {
	auto method = CppMethod::create();
	method->setKlass(pClass.get());
	method->setName("initScene");
	method->setReturnType<engine::utils::ReportMessagePtr>();
	method->setIsOverride(true);
	method->addStatement(CppCustomStatement::create("using namespace n::sdk::main"));
	method->addStatement(CppCustomStatement::create("using namespace n::engine::utils"));
	//language=c++
	method->addStatement(
		CppCustomStatement::create(R"(auto shader = getResourceByRequest(BuiltInProgramRequest::getDefaultProgram());

	Logger::out("{}", shader->getSync<n::sdk::renderer::ShaderProgram>()->getLinkStatus()))"));
	method->addStatement(CppCustomStatement::create("return nullptr"));
	return method;
}
} // namespace PROJECT_CORE