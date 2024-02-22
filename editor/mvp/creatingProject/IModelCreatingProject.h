//
// Created by alexus on 24.01.24.
//

#ifndef IMODELCREATINGPROJECT_H
#define IMODELCREATINGPROJECT_H

namespace n::core::project {
class Project;
}

namespace MVP_CORE {

class IModelCreatingProject {
public:
	virtual ~IModelCreatingProject() = default;

	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;
};
} // namespace MVP_CORE


#endif //IMODELCREATINGPROJECT_H
