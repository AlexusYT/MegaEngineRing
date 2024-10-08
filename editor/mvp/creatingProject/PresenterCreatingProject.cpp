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
// Created by alexus on 24.01.24.
//

#include "PresenterCreatingProject.h"

#include <GameEngine.h>
#include <mvp/creatingProject/IModelCreatingProject.h>
#include <mvp/creatingProject/IViewCreatingProject.h>
#include <mvp/main/MainWindow.h>
#include <mvp/main/ModelMain.h>
#include <mvp/main/PresenterMain.h>
#include <project/Project.h>
#include <project/toolchain/ToolchainSettings.h>
#include <project/toolchain/ToolchainUtils.h>
#include <regex>
#include <thread>
#include <utility>

#include "mvp/ApplicationController.h"
#include "mvp/contexts/ApplicationContext.h"

namespace mer::editor::mvp {
PresenterCreatingProject::PresenterCreatingProject(std::shared_ptr<IViewCreatingProject> pView,
												   std::shared_ptr<IModelCreatingProject> pModel)
	: view(std::move(pView)), model(std::move(pModel)) {}

void PresenterCreatingProject::runTasks() {
	//std::thread thread(&PresenterCreatingProject::runTasksImpl, this);
	const auto project = model->getProject();
	if (auto msg = project->saveProject()) return logError(std::move(msg));

	auto appControllerSelf = getAppController();
	sdk::utils::ReportMessagePtr msg;
	auto viewMain = MainWindow::create(ApplicationContext::create(appControllerSelf->getApp()), msg);
	if (msg) return logError(std::move(msg));
	auto modelMain = std::make_shared<ModelMain>();
	modelMain->setProject(project);
	appControllerSelf->run(std::make_shared<PresenterMain>(viewMain, modelMain));
	appControllerSelf->stop(this);
	//thread.detach();
}

void PresenterCreatingProject::runTasksImpl() {
	try {
		const auto project = model->getProject();

		logMessage("Сохранение проекта...");
		if (auto msg = project->saveProject()) return logError(std::move(msg));
		auto projectPath = project->getProjectPath();
		if (auto msg = generateFiles(projectPath)) return logError(std::move(msg));
		if (auto msg = installLibraries(projectPath)) return logError(std::move(msg));
		if (auto msg = reloadCmake()) return logError(std::move(msg));

		logMessage("Готово");

		/*TODO view->executeInMainThread([this, project](const std::promise<void> & ) {
			auto appControllerSelf = getAppController();
			sdk::utils::ReportMessagePtr msg;
			auto viewMain = MainWindow::create(appControllerSelf->getApp(), msg);
			if (msg) return logError(std::move(msg));
			auto modelMain = std::make_shared<ModelMain>();
			modelMain->setProject(project);
			appControllerSelf->run(std::make_shared<PresenterMain>(viewMain, modelMain));
			appControllerSelf->stop(this);
		});*/

	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to run project tasks");
		msg->setMessage("Exception occurred");

		logError(std::move(msg));
	}
}

sdk::utils::ReportMessagePtr PresenterCreatingProject::generateFiles(const std::filesystem::path &pPath) const {
	using namespace mer::editor::project;
	logMessage("Генерация vcpkg.json...");
	if (auto msg = ToolchainUtils::generateVcpkgManifestFile(pPath / "vcpkg.json")) return msg;
	logMessage("Генерация CMakePresets.json...");
	if (auto msg = ToolchainUtils::generateCMakePresetsFile(pPath / "CMakePresets.json")) return msg;
	logMessage("Генерация CMakeUserPresets.json...");
	if (auto msg = ToolchainUtils::generateCMakeUserPresetsFile(pPath / "CMakeUserPresets.json")) return msg;
	logMessage("Генерация CMakeLists.txt...");
	if (auto msg = ToolchainUtils::generateCMakeListsFile(pPath / "CMakeLists.txt")) return msg;
	logMessage("Генерация директории cmake...");
	create_directories(pPath / "cmake");
	if (auto msg = ToolchainUtils::generateCMakeDirectory(pPath / "cmake")) return msg;
	auto project = model->getProject();
	logMessage("Генерация main.cpp...");
	if (auto msg = project->generateMainFile()) return msg;
	logMessage("Генерация generated-files.cmake...");
	if (auto msg = project->saveFiles()) return msg;
	return nullptr;
}

sdk::utils::ReportMessagePtr PresenterCreatingProject::installLibraries(
	const std::filesystem::path &pProjectPath) const {
	using namespace mer::editor::project;
	logMessage("Установка необходимых библиотек...");
	const auto vcpkgPath = ToolchainSettings::getVcpkgPath();
	const auto installRoot = model->getProject()->getProjectBuildPath() / "vcpkg_installed";
	create_directories(installRoot);
	const std::string command =
		std::format("install --triplet x64-linux --vcpkg-root {} --x-wait-for-lock --x-manifest-root={} "
					"--x-install-root={} --x-feature=test",
					vcpkgPath.parent_path().string(), pProjectPath.string(), installRoot.string());
	std::stringstream out;
	const sigc::slot<void(const std::string &pLog)> &coutCallback = [&out](const std::string &pLine) { out << pLine; };
	const sigc::slot<void(const std::string &pLog)> &cerrCallback = [this](const std::string &pLine) {
		logMessage("<span foreground='red'>" + Glib::Markup::escape_text(pLine) + "</span>");
	};

	if (auto exitCode = ToolchainUtils::executeSync(pProjectPath, vcpkgPath, command, coutCallback, cerrCallback);
		exitCode != 0) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to install libraries");
		msg->setMessage("Vcpkg finished with error");
		msg->addInfoLine("Exit code: {}", exitCode);
		return msg;
	}

	logMessage("Получение скриптов для подключения библиотек...");
	std::unordered_map<std::string, Library> libraries;

	if (auto msg = parseLibrariesScript(out, libraries)) return msg;

	logMessage("Создание файлов скриптов...");
	auto cmakePath = pProjectPath / "cmake/libraries/required";
	try {
		create_directories(cmakePath);
		for (const auto &[name, script]: libraries | std::views::values) {

			std::string editedScript = editTargetLinkLibraries(script, "target_link_libraries");
			editedScript = editTargetLinkLibraries(editedScript, "target_include_directories");
			ToolchainUtils::writeFile(cmakePath / (name + ".cmake"), editedScript);
		}
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to install libraries");
		msg->setMessage("Exception occurred while writing cmake scripts");
		msg->addInfoLine("Scripts path: {}", cmakePath.string());
		return msg;
	}
	return nullptr;
}

std::string PresenterCreatingProject::editTargetLinkLibraries(const std::string &pStr, const std::string &pFunc) {

	std::smatch matches;
	//language=regexp
	std::regex regex(pFunc + R"(\((\S*) (PRIVATE )?(.*)\))");
	std::string scriptCopy = pStr;
	std::stringstream editedScript;
	while (std::regex_search(scriptCopy, matches, regex)) {
		editedScript << matches.prefix();

		editedScript << std::format("{}(${{target}} PRIVATE {})\n", pFunc, matches[3].str());
		scriptCopy = matches.suffix().str();
	}
	editedScript << scriptCopy;
	return editedScript.str();
}

sdk::utils::ReportMessagePtr PresenterCreatingProject::reloadCmake() const {
	logMessage("Создание сборочных файлов");
	if (auto exitCode = model->getProject()->reloadCMake([](const std::string & /*pLine*/) {},
														 [this](const std::string &pLine) {
															 logMessage("<span foreground='red'>" +
																		Glib::Markup::escape_text(pLine) + "</span>");
														 });
		exitCode != 0) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to generate cmake files");
		msg->setMessage("Cmake finished with error");
		msg->addInfoLine("Exit code: {}", exitCode);
		return msg;
	}
	return nullptr;
}

sdk::utils::ReportMessagePtr PresenterCreatingProject::parseLibrariesScript(
	std::stringstream &pLog, std::unordered_map<std::string, Library> &pLibraries) {
	try {
		bool foundScripts = false;
		std::optional<Library> lib;
		while (!pLog.eof()) {
			std::string line = getLine(pLog);
			if (line.empty()) continue;
			if (!foundScripts && line.starts_with("Total install time: ")) {
				foundScripts = true;
				continue;
			}
			if (!foundScripts) continue;

			if (lib && line.starts_with(" ")) {
				lib.value().script += line + "\n";
			} else {
				const std::string s = line.starts_with("The package") ? line.substr(12) : line;
				if (lib) {
					auto val = lib.value();
					if (auto iter = pLibraries.find(val.name); iter == pLibraries.end()) {
						pLibraries.emplace_hint(iter, val.name, val);
					} else {
						iter->second.script += val.script;
					}
					lib.reset();
				}
				if (s.contains(" provides pkg-config modules:")) continue;

				if (auto str = s.substr(0, s.find(' ')); !str.empty()) lib.emplace(str);
			}
		}
		if (!foundScripts) {

			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to parse libraries scripts");
			msg->setMessage("Scripts not found");
			return msg;
		}
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to parse libraries scripts");
		msg->setMessage("Exception occurred while parsing cmake scripts of installed libraries");
		return msg;
	}
	return nullptr;
}

std::string PresenterCreatingProject::getLine(std::stringstream &pLog) {

	std::array<char, 4096> buffer{};
	pLog.getline(buffer.data(), buffer.size());
	const auto count = pLog.gcount() - 1;
	if (count <= 0) return {};
	std::string line(buffer.data(), static_cast<std::string::size_type>(count));
	if (line.ends_with('\r')) line.pop_back();
	return line;
}

void PresenterCreatingProject::logMessage(const std::string & /*pMessage*/) const {
	/* TODO view->executeInMainThread(
		[pMessage, this](const std::promise<void> & ) { view->addMessageToLog(pMessage + "\n"); });*/
}

void PresenterCreatingProject::logError(sdk::utils::ReportMessagePtr /*pError*/) const {
	/*TODO view->executeInMainThread(
		[&pError, this](const std::promise<void> & ) {
		view->reportError(std::move(pError));
	});*/
}
void PresenterCreatingProject::run(){
	view->openView();
}
void PresenterCreatingProject::stop(){
view->closeView();
}

} // namespace mer::editor::mvp