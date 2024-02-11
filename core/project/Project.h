//
// Created by alexus on 12.12.23.
//

#ifndef PROJECT_H
#define PROJECT_H
#include <SQLiteCpp/SQLiteCpp.h>
#include <future>
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

	sigc::signal<void(const engine::utils::ReportMessagePtr &pError)> onErrorSignal;
	std::atomic<bool> editorLibLoading{};
	std::atomic<engine::utils::ReportMessagePtr> editorLibError{nullptr};
	sigc::signal<void(const engine::utils::ReportMessagePtr &pError)> onEditorLibLoadedSignal;
	sigc::signal<void()> onEditorLibLoadingSignal;

	void* editorLib{};

	void* editorSdkLib{};
	Project();

public:
	~Project();

	static std::shared_ptr<Project> create() { return std::shared_ptr<Project>(new (std::nothrow) Project()); }

	engine::utils::ReportMessagePtr openDatabase();

	void initProject();

	engine::utils::ReportMessagePtr loadProject();

	engine::utils::ReportMessagePtr saveProject() const;

	engine::utils::ReportMessagePtr saveFiles() const;

	engine::utils::ReportMessagePtr generateMainFile() const;

	Glib::RefPtr<Gio::SimpleActionGroup> getActionGroups() const;

	using CallbackSlot = sigc::slot<void(const std::string &pLogLine)>;

	void requestRebuildEditorLib();

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

	[[nodiscard]] void* getEditorLib() const { return editorLib; }

	sigc::connection connectOnErrorSignal(
		const sigc::slot<void(const engine::utils::ReportMessagePtr &pError)> &pSlot) {
		return onErrorSignal.connect(pSlot);
	}

	sigc::connection connectOnEditorLibLoadedSignal(
		const sigc::slot<void(const engine::utils::ReportMessagePtr &pError)> &pSlot) {
		return onEditorLibLoadedSignal.connect(pSlot);
	}

	sigc::connection connectOnEditorLibLoadingSignal(const sigc::slot<void()> &pSlot) {
		return onEditorLibLoadingSignal.connect(pSlot);
	}

	[[nodiscard]] void* getEditorSdkLib() const { return editorSdkLib; }

	void setEditorSdkLib(void* const pEditorSdkLib) { editorSdkLib = pEditorSdkLib; }

	[[nodiscard]] const std::atomic<bool> &getEditorLibLoading() const { return editorLibLoading; }

	[[nodiscard]] const std::atomic<engine::utils::ReportMessagePtr> &getEditorLibError() const {
		return editorLibError;
	}

	void errorOccurred(const engine::utils::ReportMessagePtr &pError) const { onErrorSignal(pError); }

private:
	void editorLibLoadStarted();

	void editorLibLoadFinished();

	void editorLibLoadErrored(const engine::utils::ReportMessagePtr &pError);

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
