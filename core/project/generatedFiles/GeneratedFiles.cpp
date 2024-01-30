//
// Created by alexus on 26.01.24.
//

#include "GeneratedFiles.h"

#include <project/Project.h>
#include <project/toolchain/ToolchainUtils.h>

namespace PROJECT_CORE {
n::engine::utils::ReportMessageUPtr GeneratedFiles::saveToCmake() const {
	std::vector<std::string> files;
	getFilesToSave(this, files);
	std::stringstream ss;
	ss << "set(GENERATED_SOURCES\n";
	for (const auto &file: files) { ss << "\t" << file << "\n"; }
	ss << ")";

	return ToolchainUtils::writeFile(getProject()->getProjectPath() / "cmake/generated-sources.cmake", ss.str());
}

void GeneratedFiles::getFilesToSave(const ProjectExplorerEntry* pEntry, std::vector<std::string> &pFiles) {
	if (auto entryPath = pEntry->getPath().string(); !entryPath.empty()) pFiles.emplace_back(entryPath);
	auto &entryChildren = pEntry->getEntryChildren();
	if (!entryChildren) return;
	for (uint32_t i = 0; i < entryChildren->get_n_items(); i++) {
		auto child = entryChildren->get_item(i);
		getFilesToSave(child.get(), pFiles);
	}
}

} // namespace PROJECT_CORE