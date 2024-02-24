//
// Created by alexus on 26.01.24.
//

#ifndef MODELMAIN_H
#define MODELMAIN_H
#include "IModelMain.h"

namespace mer::editor::mvp {
class ModelMain : public IModelMain {
	std::shared_ptr<project::Project> project;

public:
	[[nodiscard]] const std::shared_ptr<project::Project> &getProject() const override { return project; }

	void setProject(const std::shared_ptr<project::Project> &pProject) override { project = pProject; }
};
} // namespace mer::editor::mvp


#endif //MODELMAIN_H
