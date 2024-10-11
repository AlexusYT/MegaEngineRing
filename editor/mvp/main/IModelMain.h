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

#ifndef IMODELMAIN_H
#define IMODELMAIN_H

namespace mer::editor::project {
class Project;
}

namespace mer::editor::mvp {
class PanedLayoutTab;

class IModelMain {
public:
	virtual ~IModelMain() = default;

	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;

	[[nodiscard]] virtual const std::vector<PanedLayoutTab> &getPanedLayoutTabs() const = 0;

	virtual void setPanedLayoutTabs(const std::vector<PanedLayoutTab> &pPanedLayoutTabs) = 0;

	[[nodiscard]] virtual int32_t getCurrentTab() const = 0;

	virtual void setCurrentTab(int32_t pCurrentTab) = 0;
};

} // namespace mer::editor::mvp

#endif //IMODELMAIN_H
