//
// Created by alexus on 26.01.24.
//

#ifndef GENERATEDFILES_H
#define GENERATEDFILES_H
#include <unordered_set>

#include "GeneratedFileEntry.h"

namespace mer::editor::project {
class Project;

class GeneratedFiles : public GeneratedFileEntry {
	std::vector<std::string> prevFiles;

protected:
	explicit GeneratedFiles(const std::shared_ptr<Project> &pProject) : GeneratedFileEntry(pProject) {
		setName("Файлы проекта");
	}

public:
	static std::shared_ptr<GeneratedFiles> create(const std::shared_ptr<Project> &pProject) {
		return Glib::make_refptr_for_instance(new GeneratedFiles(pProject));
	}

	sdk::utils::ReportMessagePtr saveToCmake() const;

	static void getFilesToSave(const ProjectExplorerEntry* pEntry, std::vector<std::string> &pFiles);

private:
	bool isSaveRequired(const std::vector<std::string> &pFiles) const;
};
} // namespace mer::editor::project

#endif //GENERATEDFILES_H
