//
// Created by alexus on 26.01.24.
//

#ifndef GENERATEDFILES_H
#define GENERATEDFILES_H
#include <unordered_set>

#include "GeneratedFileEntry.h"

namespace PROJECT_CORE {
class Project;

class GeneratedFiles : public GeneratedFileEntry {

protected:
	explicit GeneratedFiles(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) {
		setName("Файлы проекта");
	}

public:
	static std::shared_ptr<GeneratedFiles> create(const std::shared_ptr<Project> &pProject) {
		return Glib::make_refptr_for_instance(new GeneratedFiles(pProject));
	}

	engine::utils::ReportMessageUPtr saveToCmake() const;

	static void getFilesToSave(const ProjectExplorerEntry* pEntry, std::vector<std::string> &pFiles);
};
} // namespace PROJECT_CORE

#endif //GENERATEDFILES_H
