//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 12.08.24.
//

#include "ModelProjectExplorer.h"

#include "entries/ProjectExplorerElement.h"

namespace mer::editor::mvp {
ModelProjectExplorer::ModelProjectExplorer(const std::filesystem::path &pPath) {
	ModelProjectExplorer::setRootPath(pPath);
}

void ModelProjectExplorer::setRootPath(const std::filesystem::path &pPath) {
	path = pPath;
	if (!exists(pPath)) create_directories(pPath);
	elements = getDirectoryEntry(pPath, pPath);
}

std::shared_ptr<ProjectExplorerElement> ModelProjectExplorer::getDirectoryEntry(
	const std::filesystem::path &pPath, const std::filesystem::path &pRootPath) {
	auto explorerEntry = ProjectExplorerElement::create(pPath, pRootPath, true);
	for (const auto &entry: std::filesystem::directory_iterator(pPath)) {
		auto entryPath = entry.path();
		if (entryPath.filename().string().starts_with('.')) continue;
		if (entry.is_directory()) {
			explorerEntry->addChild(getDirectoryEntry(entry.path(), pRootPath));
		} else {
			explorerEntry->addChild(ProjectExplorerElement::create(entryPath, pRootPath, false));
		}
	}
	explorerEntry->sort();
	return explorerEntry;
}

} // namespace mer::editor::mvp