//
// Created by alexus on 26.01.24.
//

#ifndef IMODELMAIN_H
#define IMODELMAIN_H

namespace n::core::project {
class Project;
}

namespace MVP_CORE {

class IModelMain {
public:
	virtual ~IModelMain() = default;

	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;
};

} // namespace MVP_CORE

#endif //IMODELMAIN_H
