//
// Created by alexus on 24.01.24.
//

#ifndef PRESENTERCREATINGPROJECT_H
#define PRESENTERCREATINGPROJECT_H

#include <mvp/IPresenter.h>

namespace MVP_CORE {
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

	engine::utils::ReportMessageUPtr generateFiles(const std::filesystem::path &pPath) const;

	engine::utils::ReportMessageUPtr installLibraries(const std::filesystem::path &pProjectPath) const;

	static std::string editTargetLinkLibraries(const std::string &pStr, const std::string &pFunc);

	engine::utils::ReportMessageUPtr reloadCmake() const;

	struct Library {
		std::string name;
		std::string script;
	};

	static engine::utils::ReportMessageUPtr parseLibrariesScript(std::stringstream &pLog,
																 std::unordered_map<std::string, Library> &pLibraries);

	static std::string getLine(std::stringstream &pLog);

	void logMessage(const std::string &pMessage) const;

	void logError(engine::utils::ReportMessageUPtr pError) const;
};
} // namespace MVP_CORE

#endif //PRESENTERCREATINGPROJECT_H
