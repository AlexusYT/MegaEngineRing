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
	sdk::ReportMessagePtr msg;
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

		logMessage("Готово");

		/*TODO view->executeInMainThread([this, project](const std::promise<void> & ) {
			auto appControllerSelf = getAppController();
			sdk::ReportMessagePtr msg;
			auto viewMain = MainWindow::create(appControllerSelf->getApp(), msg);
			if (msg) return logError(std::move(msg));
			auto modelMain = std::make_shared<ModelMain>();
			modelMain->setProject(project);
			appControllerSelf->run(std::make_shared<PresenterMain>(viewMain, modelMain));
			appControllerSelf->stop(this);
		});*/

	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to run project tasks");
		msg->setMessage("Exception occurred");

		logError(std::move(msg));
	}
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

sdk::ReportMessagePtr PresenterCreatingProject::parseLibrariesScript(
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

			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to parse libraries scripts");
			msg->setMessage("Scripts not found");
			return msg;
		}
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
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

void PresenterCreatingProject::logError(sdk::ReportMessagePtr /*pError*/) const {
	/*TODO view->executeInMainThread(
		[&pError, this](const std::promise<void> & ) {
		view->reportError(std::move(pError));
	});*/
}

void PresenterCreatingProject::run() { view->openView(); }

void PresenterCreatingProject::stop() { view->closeView(); }

} // namespace mer::editor::mvp