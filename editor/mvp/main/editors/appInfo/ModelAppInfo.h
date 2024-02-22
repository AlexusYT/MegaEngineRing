//
// Created by alexus on 29.01.24.
//

#ifndef MODELAPPINFO_H
#define MODELAPPINFO_H
#include <project/generatedFiles/ApplicationInfo.h>

#include "IModelAppInfo.h"

namespace MVP_CORE {

class ModelAppInfo : public IModelAppInfo {
	project::ApplicationInfo* info{};
	std::shared_ptr<project::Project> project;

public:
	[[nodiscard]] project::ApplicationInfo* getInfo() const override { return info; }

	void setInfo(project::ApplicationInfo* const pInfo) override { info = pInfo; }

	[[nodiscard]] const std::shared_ptr<project::Project> &getProject() const override { return project; }

	void setProject(const std::shared_ptr<project::Project> &pProject) override { project = pProject; }
};

} // namespace MVP_CORE

#endif //MODELAPPINFO_H
