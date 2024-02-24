//
// Created by alexus on 26.01.24.
//

#ifndef IMODELMAIN_H
#define IMODELMAIN_H

namespace mer::editor::project {
class Project;
}

namespace mer::editor::mvp {

class IModelMain {
public:
	virtual ~IModelMain() = default;

	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;
};

} // namespace mer::editor::mvp

#endif //IMODELMAIN_H
