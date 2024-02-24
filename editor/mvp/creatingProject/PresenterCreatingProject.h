//
// Created by alexus on 24.01.24.
//

#ifndef PRESENTERCREATINGPROJECT_H
#define PRESENTERCREATINGPROJECT_H

#include <mvp/IPresenter.h>

namespace mer::editor::mvp {
struct ProjectCreatingTask;
class IModelCreatingProject;
class IViewCreatingProject;

class PresenterCreatingProject : public IPresenter {
	std::shared_ptr<IViewCreatingProject> view;
	std::shared_ptr<IModelCreatingProject> model;


public:
	PresenterCreatingProject(const PresenterCreatingProject &pOther) = delete;

	PresenterCreatingProject(PresenterCreatingProject &&pOther) noexcept
		: view{std::move(pOther.view)}, model{std::move(pOther.model)} {}

	PresenterCreatingProject &operator=(const PresenterCreatingProject &pOther) = delete;

	PresenterCreatingProject &operator=(PresenterCreatingProject &&pOther) noexcept {
		if (this == &pOther) return *this;
		view = std::move(pOther.view);
		model = std::move(pOther.model);
		return *this;
	}

	PresenterCreatingProject(std::shared_ptr<IViewCreatingProject> pView,
							 std::shared_ptr<IModelCreatingProject> pModel);

	void runTasks();

private:
	void runTasksImpl() const;

	sdk::utils::ReportMessagePtr generateFiles(const std::filesystem::path &pPath) const;

	sdk::utils::ReportMessagePtr installLibraries(const std::filesystem::path &pProjectPath) const;

	static std::string editTargetLinkLibraries(const std::string &pStr, const std::string &pFunc);

	sdk::utils::ReportMessagePtr reloadCmake() const;

	struct Library {
		std::string name;
		std::string script;
	};

	static sdk::utils::ReportMessagePtr parseLibrariesScript(std::stringstream &pLog,
																 std::unordered_map<std::string, Library> &pLibraries);

	static std::string getLine(std::stringstream &pLog);

	void logMessage(const std::string &pMessage) const;

	void logError(sdk::utils::ReportMessagePtr pError) const;
};
} // namespace mer::editor::mvp

#endif //PRESENTERCREATINGPROJECT_H
