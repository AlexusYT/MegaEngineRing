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

#ifndef MODELPROJECTEXPLORER_H
#define MODELPROJECTEXPLORER_H
#include "IModelProjectExplorer.h"

namespace mer::editor::mvp {

class ModelProjectExplorer : public IModelProjectExplorer {
	std::filesystem::path path;
	std::shared_ptr<ProjectExplorerElement> elements;

public:
	explicit ModelProjectExplorer(const std::filesystem::path &pPath);

	void setRootPath(const std::filesystem::path &pPath) override;

	[[nodiscard]] const std::filesystem::path &getRootPath() const override { return path; }

	[[nodiscard]] const std::shared_ptr<ProjectExplorerElement> &getElements() const override { return elements; }

private:
	static std::shared_ptr<ProjectExplorerElement> getDirectoryEntry(const std::filesystem::path &pPath);
};
} // namespace mer::editor::mvp


#endif //MODELPROJECTEXPLORER_H
