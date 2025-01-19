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
// Created by alexus on 24.01.24.
//

#ifndef PRESENTERCREATINGPROJECT_H
#define PRESENTERCREATINGPROJECT_H

#include "mvp/PresenterBase.h"

namespace mer::editor::mvp {
struct ProjectCreatingTask;
class IModelCreatingProject;
class IViewCreatingProject;

class PresenterCreatingProject : public PresenterBase {
	std::shared_ptr<IViewCreatingProject> view;
	std::shared_ptr<IModelCreatingProject> model;


public:
	PresenterCreatingProject(const PresenterCreatingProject &pOther) = delete;

	PresenterCreatingProject(PresenterCreatingProject &&pOther) noexcept
		: view{std::move(pOther.view)}, model{std::move(pOther.model)} {}

	PresenterCreatingProject &operator=(const PresenterCreatingProject &pOther) = delete;

	PresenterCreatingProject &operator=(PresenterCreatingProject &&pOther) noexcept {
		if (this == &pOther) return *this;
		view = std::move(pOther.view);
		model = std::move(pOther.model);
		return *this;
	}

	PresenterCreatingProject(std::shared_ptr<IViewCreatingProject> pView,
							 std::shared_ptr<IModelCreatingProject> pModel);

	void runTasks();

private:
	void runTasksImpl();

	sdk::utils::ReportMessagePtr generateFiles(const std::filesystem::path &pPath) const;

	sdk::utils::ReportMessagePtr installLibraries(const std::filesystem::path &pProjectPath) const;

	static std::string editTargetLinkLibraries(const std::string &pStr, const std::string &pFunc);

	sdk::utils::ReportMessagePtr reloadCmake() const;

	struct Library {
		std::string name;
		std::string script;
	};

	static sdk::utils::ReportMessagePtr parseLibrariesScript(std::stringstream &pLog,
	std::unordered_map<std::string, Library> &pLibraries);

	static std::string getLine(std::stringstream &pLog);

	void logMessage(const std::string &pMessage) const;

	void logError(sdk::utils::ReportMessagePtr pError) const;

	std::string getTypeName() override {
		return "PresenterCreatingProject";
	}

public:
	void run() override;

	void stop() override;
};
} // namespace mer::editor::mvp

#endif //PRESENTERCREATINGPROJECT_H
