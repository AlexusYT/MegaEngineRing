//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 12.12.23.
//

#include "Project.h"

#include <ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h>

#include "EngineSDK/extensions/ExtensionRegistry.h"
#include "generatedFiles/ApplicationInfo.h"
#include "generators/cpp/CppCustomStatement.h"
#include "generators/cpp/CppHeaderFile.h"
#include "generators/cpp/CppMethod.h"
#include "scripting/ScriptParser.h"
#include "toolchain/ToolchainSettings.h"
#include "toolchain/ToolchainUtils.h"

namespace mer::editor::project {
Project::Project() : projectExplorerEntries(Gio::ListStore<ui::ProjectExplorerEntry>::create()) {
	scriptParser = std::make_shared<ScriptParser>();
}

Project::~Project() {}

sdk::utils::ReportMessagePtr Project::openDatabase() {

	try {
		database = std::make_shared<SQLite::Database>(projectPath / (projectName + ".enproj"),
													  SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to connect to database");
		return msg;
	}
	return nullptr;
}

void Project::initProject() {

	scriptParser->setProject(shared_from_this());

	applicationInfo = ApplicationInfo::create(shared_from_this());
	engineFileEntries = GeneratedFiles::create(shared_from_this());
	//engineFileEntries->addChildEntry(applicationInfo);
	projectExplorerEntries->append(engineFileEntries);
}

sdk::utils::ReportMessagePtr Project::loadProject() {

	if (auto msg = engineFileEntries->loadDatabase()) return msg;
	if (auto msg = saveFiles()) return msg;
	return nullptr;
}

sdk::utils::ReportMessagePtr Project::saveProject() {
	sdk::utils::Logger::info("Saving project...");
	try {
		database->backup((database->getFilename() + ".bak").c_str(), SQLite::Database::Save);
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to save project");
		msg->setMessage("Exception occurred while backup");
		return msg;
	}
	if (auto msg = engineFileEntries->saveDatabase()) return msg;
	sdk::utils::Logger::info("Project saved");

	return nullptr;
}

sdk::utils::ReportMessagePtr Project::saveFiles() const {
	if (auto msg = engineFileEntries->saveToCmake()) return msg;
	if (auto msg = engineFileEntries->saveFile()) return msg;
	return nullptr;
}

mer::sdk::utils::ReportMessagePtr Project::generateMainFile() const {

	const auto sourcesPath = projectPath / "source/main";
	CppSourceFile sourceFile;
	sourceFile.addInclude("EngineSDK/Application.h");
	sourceFile.addInclude("ApplicationSettings.h");
	auto main = CppMethod::create();
	main->setReturnType("int");
	main->setName("main");
	main->setParamsList({"int argc", "char* argv[]"});
	main->addStatement(CppCustomStatement::create("using namespace mer::sdk::main"));
	main->addStatement(CppCustomStatement::create("using namespace mer::sdk::utils"));
	main->addStatement(CppCustomStatement::create("auto app = Application::create()"));
	main->addStatement(
		CppMethodCall::create("app->setApplicationSettings", {"std::make_shared<ApplicationSettings>()"}));
	//language=c++
	main->addStatement(CppCustomStatement::create(R"(
	if(auto msg = app->initEngine()) {
		Logger::error(msg);
		return -1;
	}
	int ret = app->runMainLoop(argc, argv);
	app->deinitEngine();
	return ret;
)"));
	sourceFile.addDefinition(main->getDefinition());
	return sourceFile.writeFile(sourcesPath);
}

Glib::RefPtr<Gio::SimpleActionGroup> Project::getActionGroups() const {
	const Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup = Gio::SimpleActionGroup::create();
	return refActionGroup;
}

int Project::reloadCMake(const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const {
	create_directories(projectPath);

	const std::string command = std::format("-DCMAKE_BUILD_TYPE=Debug --preset dev -S {} -B {}", projectPath.string(),
											projectBuildPath.string());
	return ToolchainUtils::executeSync(projectPath, ToolchainSettings::getCMakePath(), command, pCoutCallback,
									   pCerrCallback);
}

int Project::build(const std::string &pTarget, const std::string &pPreset, const CallbackSlot &pCoutCallback,
				   const CallbackSlot &pCerrCallback) const {
	std::string args = std::format("--build --target {} --preset {}", pTarget, pPreset);
	return ToolchainUtils::executeSync(projectPath, ToolchainSettings::getCMakePath(), args, pCoutCallback,
									   pCerrCallback);
}

int Project::run(const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const {

	const std::string command = "";

	return ToolchainUtils::executeSync(projectPath, projectPath / "build/dev/GameEngine", command, pCoutCallback,
									   pCerrCallback);
}

int Project::runDebug(const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const {

	const std::string command = projectPath / "build/dev/GameEngine";

	return ToolchainUtils::executeSync(projectPath, "gdb", command, pCoutCallback, pCerrCallback);
}

} // namespace mer::editor::project