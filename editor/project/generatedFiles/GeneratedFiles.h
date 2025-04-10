//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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

	sdk::ReportMessagePtr saveToCmake() const;

	static void getFilesToSave(const ProjectExplorerEntry* pEntry, std::vector<std::string> &pFiles);

private:
	bool isSaveRequired(const std::vector<std::string> &pFiles) const;
};
} // namespace mer::editor::project

#endif //GENERATEDFILES_H
