//
// Created by alexus on 27.01.24.
//

#ifndef GENERATEDFILEENTRY_H
#define GENERATEDFILEENTRY_H
#include <ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h>

namespace PROJECT_CORE {
class Project;

class GeneratedFileEntry : public ui::ProjectExplorerEntry {
	std::shared_ptr<Project> project{};

protected:
	explicit GeneratedFileEntry(const std::shared_ptr<Project> &pProject) : project(pProject) {}

public:
	[[nodiscard]] const std::shared_ptr<Project> &getProject() const { return project; }

	void setProject(const std::shared_ptr<Project> &pProject) { project = pProject; }
};

} // namespace PROJECT_CORE

#endif //GENERATEDFILEENTRY_H
