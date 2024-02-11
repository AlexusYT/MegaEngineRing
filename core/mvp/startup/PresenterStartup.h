//
// Created by alexus on 25.01.24.
//

#ifndef PRESENTERSTARTUP_H
#define PRESENTERSTARTUP_H
#include <mvp/IPresenter.h>

#include "IModelStartup.h"
#include "IViewStartup.h"

namespace n::core::project {
class Project;
}

namespace MVP_CORE {
class IModelStartup;
class IViewStartup;

class PresenterStartup : public IPresenter {


	std::shared_ptr<IViewStartup> view;
	std::shared_ptr<IModelStartup> model;

public:
	PresenterStartup(const std::shared_ptr<IViewStartup> &pView, const std::shared_ptr<IModelStartup> &pModel);

private:
	static engine::utils::ReportMessagePtr initProject(const std::shared_ptr<project::Project> &pProject);

	void openProjectCreatingWindow(const std::shared_ptr<project::Project> &pProject) const;

	void openMainWindow(const std::shared_ptr<project::Project> &pProject) const;
};
} // namespace MVP_CORE

#endif //PRESENTERSTARTUP_H
