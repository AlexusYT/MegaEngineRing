//
// Created by alexus on 29.01.24.
//

#ifndef IMODELAPPINFO_H
#define IMODELAPPINFO_H

namespace MVP_CORE {
class IModelAppInfo {
public:
	virtual ~IModelAppInfo() = default;

	[[nodiscard]] virtual project::ApplicationInfo* getInfo() const = 0;

	virtual void setInfo(project::ApplicationInfo* pInfo) = 0;

	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;

};
} // namespace MVP_CORE

#endif //IMODELAPPINFO_H
