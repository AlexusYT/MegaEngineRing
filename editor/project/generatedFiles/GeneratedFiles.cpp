// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.01.24.
//

#include "GeneratedFiles.h"

#include <project/Project.h>
#include <project/toolchain/ToolchainUtils.h>

namespace mer::editor::project {
mer::sdk::utils::ReportMessagePtr GeneratedFiles::saveToCmake() const {
	std::vector<std::string> files;
	getFilesToSave(this, files);
	if (!isSaveRequired(files)) return nullptr;
	std::stringstream ss;
	ss << "set(GENERATED_SOURCES\n";
	for (const auto &file: files) { ss << "\t" << file << "\n"; }
	//ss << "\tsource/scripts/TestScript.cpp\n";
	ss << "\tsource/scripts/EditorCameraScript.cpp\n";
	ss << "\tsource/scripts/SceneObject0Script.cpp\n";
	ss << "\tsource/scripts/SceneObject1Script.cpp\n";
	ss << ")";

	return ToolchainUtils::writeFile(getProject()->getProjectPath() / "cmake/generated-sources.cmake", ss.str());
}

void GeneratedFiles::getFilesToSave(const ProjectExplorerEntry* pEntry, std::vector<std::string> &pFiles) {
	if (auto entryPath = pEntry->getHeaderPath().string(); !entryPath.empty()) pFiles.emplace_back(entryPath);
	if (auto entryPath = pEntry->getSourcePath().string(); !entryPath.empty()) pFiles.emplace_back(entryPath);
	auto &entryChildren = pEntry->getEntryChildren();
	if (!entryChildren) return;
	for (uint32_t i = 0; i < entryChildren->get_n_items(); i++) {
		auto child = entryChildren->get_item(i);
		getFilesToSave(child.get(), pFiles);
	}
}

bool GeneratedFiles::isSaveRequired(const std::vector<std::string> &pFiles) const {
	if (pFiles.size() != prevFiles.size()) return true;
	for (size_t i = 0; i < pFiles.size(); i++) {
		if (pFiles.at(i) != prevFiles.at(i)) return true;
	}
	return false;
}

} // namespace mer::editor::project