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
// Created by alexus on 12.12.23.
//

#ifndef PROJECT_H
#define PROJECT_H
#include <SQLiteCpp/SQLiteCpp.h>
#include <future>
#include <ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h>

#include "generatedFiles/ApplicationInfo.h"
#include "generatedFiles/GeneratedFiles.h"
#include "sceneObjects/EditorSceneObject.h"

namespace mer::editor::project {
class Sdk;
}

namespace mer::sdk::main {
class Application;
}

namespace mer::editor::ui {
class ProjectExplorerEntry;
} // namespace mer::editor::ui

namespace mer::editor::project {
class ScriptParser;

class Project : public std::enable_shared_from_this<Project> {
	std::filesystem::path projectPath;
	std::filesystem::path projectBuildPath;
	std::filesystem::path projectSourcesPath;
	std::filesystem::path projectDataPath;
	std::string projectName;

	Glib::RefPtr<Gio::ListStore<ui::ProjectExplorerEntry>> projectExplorerEntries;
	std::shared_ptr<SQLite::Database> database;
	std::shared_ptr<GeneratedFiles> engineFileEntries;
	std::shared_ptr<ApplicationInfo> applicationInfo;
	std::shared_ptr<ScriptParser> scriptParser;

	sigc::signal<void(const sdk::utils::ReportMessagePtr &pError)> onErrorSignal;
	std::atomic<bool> editorLibLoading{};
	std::atomic<sdk::utils::ReportMessagePtr> editorLibError{nullptr};

	std::shared_ptr<Sdk> editorSdkLib{};

	std::shared_ptr<sdk::main::Application> application;
	Project();

public:
	~Project();

	static std::shared_ptr<Project> create() { return std::shared_ptr<Project>(new (std::nothrow) Project()); }

	sdk::utils::ReportMessagePtr openDatabase();

	void initProject();

	sdk::utils::ReportMessagePtr loadProject();

	sdk::utils::ReportMessagePtr saveProject();

	sdk::utils::ReportMessagePtr saveFiles() const;

	sdk::utils::ReportMessagePtr generateMainFile() const;

	Glib::RefPtr<Gio::SimpleActionGroup> getActionGroups() const;

	using CallbackSlot = sigc::slot<void(const std::string &pLogLine)>;

	int reloadCMake(const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const;

	int build(const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const {
		return build("GameEngine_exe", pCoutCallback, pCerrCallback);
	}

	int build(const std::string &pTarget, const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const {
		return build(pTarget, "dev", pCoutCallback, pCerrCallback);
	}

	int build(const std::string &pTarget, const std::string &pPreset, const CallbackSlot &pCoutCallback,
			  const CallbackSlot &pCerrCallback) const;

	int run(const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const;

	int runDebug(const CallbackSlot &pCoutCallback, const CallbackSlot &pCerrCallback) const;

	[[nodiscard]] const std::filesystem::path &getProjectPath() { return projectPath; }

	void setProjectPath(const std::filesystem::path &pProjectPath) {
		projectPath = pProjectPath;
		projectBuildPath = pProjectPath / "build/dev";
		projectSourcesPath = pProjectPath / "source";
		projectDataPath = pProjectPath / "data";
	}

	[[nodiscard]] const std::filesystem::path &getProjectSourcesPath() const { return projectSourcesPath; }

	[[nodiscard]] const std::string &getProjectName() const { return projectName; }

	void setProjectName(const std::string &pProjectName) { projectName = pProjectName; }

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<ui::ProjectExplorerEntry>> &getProjectExplorerEntries() const {
		return projectExplorerEntries;
	}

	[[nodiscard]] const std::filesystem::path &getProjectBuildPath() const { return projectBuildPath; }

	void setProjectBuildPath(const std::filesystem::path &pProjectBuildPath) { projectBuildPath = pProjectBuildPath; }

	[[nodiscard]] const std::filesystem::path &getProjectDataPath() const { return projectDataPath; }

	void setProjectDataPath(const std::filesystem::path &pProjectDataPath) { projectDataPath = pProjectDataPath; }

	[[nodiscard]] const std::shared_ptr<SQLite::Database> &getDatabase() const { return database; }

	sigc::connection connectOnErrorSignal(const sigc::slot<void(const sdk::utils::ReportMessagePtr &pError)> &pSlot) {
		return onErrorSignal.connect(pSlot);
	}

	[[nodiscard]] std::shared_ptr<Sdk> getEditorSdkLib() const { return editorSdkLib; }

	void setEditorSdkLib(const std::shared_ptr<Sdk> &pEditorSdkLib);

	[[nodiscard]] const std::atomic<bool> &getEditorLibLoading() const { return editorLibLoading; }

	[[nodiscard]] const std::atomic<sdk::utils::ReportMessagePtr> &getEditorLibError() const { return editorLibError; }

	void errorOccurred(const sdk::utils::ReportMessagePtr &pError) const { onErrorSignal(pError); }

	[[nodiscard]] const std::shared_ptr<ScriptParser> &getScriptParser() const { return scriptParser; }

	void setScriptParser(const std::shared_ptr<ScriptParser> &pScriptParser) { scriptParser = pScriptParser; }
};
} // namespace mer::editor::project

#endif //PROJECT_H
