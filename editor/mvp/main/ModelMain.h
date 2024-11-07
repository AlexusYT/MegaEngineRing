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

#ifndef MODELMAIN_H
#define MODELMAIN_H
#include "IModelMain.h"
#include "PanedLayoutTab.h"

namespace mer::editor::mvp {
class ModelMain : public IModelMain {
	IPresenterMain* presenter{};
	std::shared_ptr<project::Project> project;
	std::vector<PanedLayoutTab> panedLayoutTabs;

	int32_t currentTab{};

public:
	void loadLayoutsFile() override;

	[[nodiscard]] const std::shared_ptr<project::Project> &getProject() const override { return project; }

	void setProject(const std::shared_ptr<project::Project> &pProject) override { project = pProject; }

	[[nodiscard]] const std::vector<PanedLayoutTab> &getPanedLayoutTabs() const override;

	void setPanedLayoutTabs(const std::vector<PanedLayoutTab> &pPanedLayoutTabs) override;

	[[nodiscard]] int32_t getCurrentTab() const override { return currentTab; }

	void setCurrentTab(const int32_t pCurrentTab) override;

	[[nodiscard]] IPresenterMain* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterMain* pPresenter) override { presenter = pPresenter; }

private:
	void tryCreateLayoutFile(const std::shared_ptr<Gio::AsyncResult> &pResult, const std::shared_ptr<Gio::File> &pFile);

	void userLayoutsReadFinished(const std::string &pStrResult, const std::exception_ptr &pException,
								 const std::shared_ptr<Gio::File> &pFile);

	void readInternalLayouts(const std::shared_ptr<Gio::OutputStream> &pSaveTo = nullptr,
							 const std::shared_ptr<Gio::File> &pUserFile = nullptr);

	void internalLayoutsReadFinished(const std::string &pStrResult, const std::exception_ptr &pException,
									 const std::shared_ptr<Gio::OutputStream> &pSaveTo,
									 const std::shared_ptr<Gio::File> &pUserFile);

	void parseLayoutsContent(const std::string &pContent, bool pUser);
};
} // namespace mer::editor::mvp


#endif //MODELMAIN_H
