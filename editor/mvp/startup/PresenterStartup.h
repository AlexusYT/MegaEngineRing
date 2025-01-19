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
// Created by alexus on 25.01.24.
//

#ifndef PRESENTERSTARTUP_H
#define PRESENTERSTARTUP_H

#include "IModelStartup.h"
#include "IViewStartup.h"
#include "mvp/PresenterBase.h"

namespace mer::editor::project {
class Project;
}

namespace mer::editor::mvp {
class IModelStartup;
class IViewStartup;

class PresenterStartup : public PresenterBase{


	std::shared_ptr<IViewStartup> view;
	std::shared_ptr<IModelStartup> model;

public:
	PresenterStartup(const std::shared_ptr<IViewStartup> &pView, const std::shared_ptr<IModelStartup> &pModel);

	void run() override;

	void stop() override;

	std::string getTypeName() override {
		return "PresenterStartup";
	}

private:
	static sdk::utils::ReportMessagePtr initProject(const std::shared_ptr<project::Project> &pProject);

	void openProjectCreatingWindow(const std::shared_ptr<project::Project> &pProject);

	void openMainWindow(const std::shared_ptr<project::Project> &pProject);
};
} // namespace mer::editor::mvp

#endif //PRESENTERSTARTUP_H
