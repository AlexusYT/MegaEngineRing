//
// Created by alexus on 24.01.24.
//

#ifndef MODELCREATINGPROJECT_H
#define MODELCREATINGPROJECT_H

#include "IModelCreatingProject.h"

namespace mer::editor::mvp {

class ModelCreatingProject : public IModelCreatingProject {
	std::shared_ptr<project::Project> project;

public:
	[[nodiscard]] const std::shared_ptr<project::Project> &getProject() const override { return project; }

	void setProject(const std::shared_ptr<project::Project> &pProject) override { project = pProject; }
};
} // namespace mer::editor::mvp

#endif //MODELCREATINGPROJECT_H
