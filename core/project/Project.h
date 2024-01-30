//
// Created by alexus on 12.12.23.
//

#ifndef PROJECT_H
#define PROJECT_H
#include <SQLiteCpp/SQLiteCpp.h>
#include <mvp/creatingProject/ProjectCreatingTask.h>
#include <ui/widgetWindows/projectExplorer/DirectoryEntry.h>
#include <ui/widgetWindows/projectExplorer/FileEntry.h>
#include <ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h>

#include "generatedFiles/ApplicationInfo.h"
#include "generatedFiles/GeneratedFiles.h"
#include "generators/cpp/CppGenerator.h"
#include "sceneObjects/SceneObject.h"
#include "toolchain/ToolchainUtils.h"

namespace UI_CORE {
class ProjectExplorerEntry;
} // namespace UI_CORE

namespace PROJECT_CORE {
class ScenesInfo;

class Project : public std::enable_shared_from_this<Project> {
	std::filesystem::path projectPath;
	std::filesystem::path projectBuildPath;
	std::string projectName;

	std::vector<std::shared_ptr<SceneObject>> toplevelObjects;
	Glib::RefPtr<Gio::ListStore<ui::ProjectExplorerEntry>> projectExplorerEntries;
	std::shared_ptr<ui::ProjectExplorerEntry> filesystemEntries;
	std::shared_ptr<SQLite::Database> database;
	std::shared_ptr<GeneratedFiles> engineFileEntries;
	std::shared_ptr<ApplicationInfo> applicationInfo;
	std::shared_ptr<ScenesInfo> scenesInfo;


	Project();

public:
	static std::shared_ptr<Project> create() { return std::shared_ptr<Project>(new (std::nothrow) Project()); }

	engine::utils::ReportMessageUPtr openDatabase();

	void initProject();

	engine::utils::ReportMessageUPtr loadProject() const;

	engine::utils::ReportMessageUPtr saveProject() const;

	engine::utils::ReportMessageUPtr saveGeneratedFilesCmake() const;

	engine::utils::ReportMessageUPtr generateMainFile() const;

	Glib::RefPtr<Gio::SimpleActionGroup> getActionGroups() const;

	void addScene(const std::string &sceneName) {}

	int reloadCMake(const sigc::slot<void(const std::string &pLogLine)> &pCoutCallback,
					const sigc::slot<void(const std::string &pLogLine)> &pCerrCallback) const;

	int build(const sigc::slot<void(const std::string &pLogLine)> &pCoutCallback,
			  const sigc::slot<void(const std::string &pLogLine)> &pCerrCallback) const;

	int run(const sigc::slot<void(const std::string &pLogLine)> &pCoutCallback,
			const sigc::slot<void(const std::string &pLogLine)> &pCerrCallback) const;

	[[nodiscard]] const std::filesystem::path &getProjectPath() { return projectPath; }

	void setProjectPath(const std::filesystem::path &pProjectPath) {
		projectPath = pProjectPath;
		projectBuildPath = pProjectPath / "build/dev";
	}

	[[nodiscard]] const std::string &getProjectName() const { return projectName; }

	void setProjectName(const std::string &pProjectName) { projectName = pProjectName; }

	[[nodiscard]] const std::vector<std::shared_ptr<SceneObject>> &getToplevelObjects() const {

		return toplevelObjects;
	}

	void addToplevelObject(std::shared_ptr<SceneObject> pObject) { toplevelObjects.emplace_back(pObject); }

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<ui::ProjectExplorerEntry>> &getProjectExplorerEntries() const {
		return projectExplorerEntries;
	}

	[[nodiscard]] const std::filesystem::path &getProjectBuildPath() const { return projectBuildPath; }

	void setProjectBuildPath(const std::filesystem::path &pProjectBuildPath) { projectBuildPath = pProjectBuildPath; }

	[[nodiscard]] const std::shared_ptr<SQLite::Database> &getDatabase() const { return database; }

private:
	static std::shared_ptr<ui::ProjectExplorerEntry> getDirectoryEntry(const std::filesystem::path &pPath) {
		auto explorerEntry = ui::DirectoryEntry::create(pPath.filename());
		for (const auto &entry: std::filesystem::directory_iterator(pPath)) {
			auto entryPath = entry.path();
			if (entry.is_directory()) {
				explorerEntry->addChildEntry(getDirectoryEntry(entry.path()));
			} else {
				explorerEntry->addChildEntry(ui::FileEntry::create(entryPath.filename()));
			}
		}
		explorerEntry->sortChildren();
		return explorerEntry;
	}
};
} // namespace PROJECT_CORE

#endif //PROJECT_H
