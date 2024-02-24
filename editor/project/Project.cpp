//
// Created by alexus on 12.12.23.
//

#include "Project.h"

#include <dlfcn.h>
#include <ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h>

#include "generatedFiles/ApplicationInfo.h"
#include "generatedFiles/ScenesInfo.h"
#include "generators/cpp/CppCustomStatement.h"
#include "generators/cpp/CppGenerator.h"
#include "generators/cpp/CppHeaderFile.h"
#include "generators/cpp/CppMethod.h"
#include "sceneObjects/SimpleMesh.h"
#include "sceneObjects/World.h"
#include "toolchain/ToolchainSettings.h"
#include "ui/widgetWindows/projectExplorer/DirectoryEntry.h"

namespace mer::editor::project {
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

Project::~Project() {
	if (editorLib) dlclose(editorLib);
}

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

sdk::utils::ReportMessagePtr Project::loadProject() {

	if (auto msg = engineFileEntries->loadDatabase()) return msg;
	if (auto msg = saveFiles()) return msg;

	requestRebuildEditorLib();
	return nullptr;
}

sdk::utils::ReportMessagePtr Project::saveProject() const {
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
	sourceFile.addInclude("EngineSDK/main/Application.h");
	sourceFile.addInclude("ApplicationSettings.h");
	auto main = CppMethod::create();
	main->setReturnType("int");
	main->setName("main");
	main->setParamsList({"int argc", "char* argv[]"});
	main->addStatement(CppCustomStatement::create("using namespace mer::sdk::main"));
	main->addStatement(CppCustomStatement::create("Application app"));
	main->addStatement(
		CppMethodCall::create("app.setApplicationSettings", {"std::make_shared<ApplicationSettings>()"}));
	main->addStatement(CppCustomStatement::create("return app.runMainLoop(argc, argv)"));
	sourceFile.addDefinition(main->getDefinition());
	return sourceFile.writeFile(sourcesPath);
}

Glib::RefPtr<Gio::SimpleActionGroup> Project::getActionGroups() const {
	const Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup = Gio::SimpleActionGroup::create();
	scenesInfo->getActionGroup(refActionGroup);
	return refActionGroup;
}

void Project::requestRebuildEditorLib() {
	std::thread([this] {
		editorLibLoadStarted();
		int result = reloadCMake([](const std::string &pLine) { sdk::utils::Logger::info(pLine); },
								 [](const std::string &pLine) { sdk::utils::Logger::error(pLine); });
		if (result != 0) {

			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to open editor library");
			msg->setMessage("Cmake failed");
			editorLibLoadErrored(msg);
			return;
		}
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1s);
		result = build(
			"_EDITOR_TMP_", [](const std::string &pLine) { sdk::utils::Logger::info(pLine); },
			[](const std::string &pLine) { sdk::utils::Logger::error(pLine); });
		if (result != 0) {

			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to open editor library");
			msg->setMessage("Library compilation failed");
			editorLibLoadErrored(msg);
			return;
		}
		auto path = (getProjectBuildPath() / "lib_EDITOR_TMP_.so").string();
		editorLib = dlopen((path).c_str(), RTLD_LAZY | RTLD_GLOBAL);
		if (!editorLib) {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to open editor library");
			msg->setMessage("The function dlopen() returned error");
			msg->addInfoLine("Path: {}", path);
			msg->addInfoLine("Error: {}", Utils::parseDlError(dlerror()));
			editorLibLoadErrored(msg);
			return;
		}
		editorLibLoadFinished();
	}).detach();
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

void Project::editorLibLoadStarted() {

	editorLibLoading = true;
	editorLibError = nullptr;
	onEditorLibLoadingSignal();
}

void Project::editorLibLoadFinished() {
	editorLibLoading = false;
	editorLibError = nullptr;
	onEditorLibLoadedSignal(nullptr);
}

void Project::editorLibLoadErrored(const sdk::utils::ReportMessagePtr &pError) {
	editorLibLoading = false;
	editorLibError = pError;
	onEditorLibLoadedSignal(pError);
}
} // namespace mer::editor::project