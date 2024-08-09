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
// Created by alexus on 24.06.24.
//

#ifndef SCRIPTPARSER_H
#define SCRIPTPARSER_H

namespace mer::editor::project {
class CppSourceFile;
class Project;
class GraphicsScript;
} // namespace mer::editor::project

namespace mer::editor::project {
class ScriptInfo;

class ScriptParser {
	std::shared_ptr<Project> project;

public:
	static void generateSource(const GraphicsScript* pScript, CppSourceFile &pSourceFile);

	[[nodiscard]] const std::shared_ptr<Project> &getProject() const { return project; }

	void setProject(const std::shared_ptr<Project> &pProject) { project = pProject; }
};
} // namespace mer::editor::project


#endif //SCRIPTPARSER_H
