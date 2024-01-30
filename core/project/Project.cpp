//
// Created by alexus on 12.12.23.
//

#include "Project.h"

#include <ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h>

#include "generatedFiles/ApplicationInfo.h"
#include "generatedFiles/ScenesInfo.h"
#include "generators/cpp/CppCustomStatement.h"
#include "generators/cpp/CppGenerator.h"
#include "generators/cpp/CppMethod.h"
#include "sceneObjects/SimpleMesh.h"
#include "sceneObjects/World.h"
#include "toolchain/ToolchainSettings.h"
#include "ui/widgetWindows/projectExplorer/DirectoryEntry.h"

namespace PROJECT_CORE {
Project::Project() : projectExplorerEntries(Gio::ListStore<ui::ProjectExplorerEntry>::create()) {

	auto obj = SceneObject::create<World>();
	obj->setName("World");
	for (int i = 0; i < 50; i++) {

		auto obj2 = SceneObject::create<SimpleMesh>();
		obj2->setName("Object" + std::to_string(i));
		obj->addChild(obj2);
	}
	addToplevelObject(obj);
}

engine::utils::ReportMessageUPtr Project::openDatabase() {

	try {
		database = std::make_shared<SQLite::Database>(projectPath / (projectName + ".enproj"),
													  SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
	} catch (...) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to connect to database");
		return std::move(msg);
	}
	return nullptr;
}

void Project::initProject() {
	filesystemEntries = getDirectoryEntry(projectPath);
	filesystemEntries->setName("Файлы сборки");
	projectExplorerEntries->append(filesystemEntries);
	applicationInfo = ApplicationInfo::create(shared_from_this());
	engineFileEntries = GeneratedFiles::create(shared_from_this());
	engineFileEntries->addChildEntry(applicationInfo);
	scenesInfo = ScenesInfo::create(shared_from_this());
	engineFileEntries->addChildEntry(scenesInfo);
	projectExplorerEntries->append(engineFileEntries);
}

engine::utils::ReportMessageUPtr Project::loadProject() const {

	if (auto msg = engineFileEntries->load()) return msg;

	return nullptr;
}

engine::utils::ReportMessageUPtr Project::saveProject() const {
	try {
		database->backup((database->getFilename() + ".bak").c_str(), SQLite::Database::Save);
	} catch (...) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Failed to save project");
		msg->setMessage("Exception occurred while backup");
		return msg;
	}
	if (auto msg = engineFileEntries->save()) return msg;
	return nullptr;
}

engine::utils::ReportMessageUPtr Project::saveGeneratedFilesCmake() const {

	if (auto msg = engineFileEntries->saveToCmake()) { return msg; }

	return nullptr;
}

n::engine::utils::ReportMessageUPtr Project::generateMainFile() const {

	const auto sourcesPath = projectPath / "source";
	CppGenerator gen;
	gen.addInclude("EngineSDK/main/Application.h");
	gen.addInclude("ApplicationSettings.h");
	CppMethod main;
	main.setReturnType("int");
	main.setName("main");
	main.setParamsList({"int argc", "char* argv[]"});
	main.addStatement(CppCustomStatement::create("using namespace n::sdk::main"));
	main.addStatement(CppCustomStatement::create("Application app"));
	main.addStatement(CppMethodCall::create("app.setApplicationSettings", {"std::make_shared<ApplicationSettings>()"}));
	main.addStatement(CppCustomStatement::create("return app.runMainLoop(argc, argv)"));
	gen.addElement(&main);
	return gen.saveFile(sourcesPath / "main.cpp");
}

Glib::RefPtr<Gio::SimpleActionGroup> Project::getActionGroups() const {
	const Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup = Gio::SimpleActionGroup::create();
	scenesInfo->getActionGroup(refActionGroup);
	return refActionGroup;
}

int Project::reloadCMake(const sigc::slot<void(const std::string &pLogLine)> &pCoutCallback,
						 const sigc::slot<void(const std::string &pLogLine)> &pCerrCallback) const {
	create_directories(projectPath);

	const std::string command = std::format("-DCMAKE_BUILD_TYPE=Debug --preset dev -S {} -B {}", projectPath.string(),
											projectBuildPath.string());
	return ToolchainUtils::executeSync(projectPath, ToolchainSettings::getCMakePath(), command, pCoutCallback,
									   pCerrCallback);
}

int Project::build(const sigc::slot<void(const std::string &pLogLine)> &pCoutCallback,
				   const sigc::slot<void(const std::string &pLogLine)> &pCerrCallback) const {

	const std::string command = "--build --target GameEngine_exe --preset dev";

	return ToolchainUtils::executeSync(projectPath, ToolchainSettings::getCMakePath(), command, pCoutCallback,
									   pCerrCallback);
}

int Project::run(const sigc::slot<void(const std::string &pLogLine)> &pCoutCallback,
				 const sigc::slot<void(const std::string &pLogLine)> &pCerrCallback) const {

	const std::string command = "";

	return ToolchainUtils::executeSync(projectPath, projectPath / "build/dev/GameEngine", command, pCoutCallback,
									   pCerrCallback);
}
} // namespace PROJECT_CORE