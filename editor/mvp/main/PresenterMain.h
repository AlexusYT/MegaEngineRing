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

#ifndef PRESENTERMAIN_H
#define PRESENTERMAIN_H
#include "EngineSDK/main/scene/objects/extensions/BasicRenderExtension.h"
#include "IPresenterMain.h"
#include "ui/customWidgets/multipaned/MultiPaned.h"

namespace mer::editor::mvp {
class EditingResourceList;
}

namespace mer::editor::project {
class LoadedScene;
}

namespace mer::editor::mvp {
class PresenterCenterWindow;
class PresenterProjectExplorer;
class IViewMain;
class IModelMain;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {

class PresenterMain : public IPresenterMain {
	std::shared_ptr<IViewMain> viewMain;
	std::shared_ptr<IModelMain> modelMain;
	std::shared_ptr<PresenterProjectExplorer> presenterProjectExplorer;
	std::shared_ptr<project::LoadedScene> loadedScene;
	std::shared_ptr<EditingResourceList> editingResources;
	std::vector<std::shared_ptr<IPresenter>> presenters;

public:
	PresenterMain(const std::shared_ptr<IViewMain> &pViewMain, const std::shared_ptr<IModelMain> &pModelMain);

	~PresenterMain() override;

private:
	void generateCmakeFiles(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void build(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void runProject(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void logMessage(int pId, const std::string &pMessage) const;

	void logError(int pId, const std::string &pMessage) const;

	void displayError(const sdk::utils::ReportMessagePtr &pMsg) override;

	void onLayoutLoadFatal() override;

	void onPanedLayoutTabsChanged() override;

	void onCurrentTabChanged() override;

private:
	std::string getTypeName() override { return "PresenterMain"; }

public:
	void run() override;

	void stop() override;

	std::shared_ptr<IView> createView(const IPresenter* pPresenter,
									  const std::shared_ptr<MultiPanedContext> &pContext) override;
	void readJsonForTab(int32_t pIndex,
						const sigc::slot<void(const sdk::utils::ReportMessagePtr &pError)> &pCallback) override;

	void selectResourceForProperty(sdk::utils::PropertyBase* pProperty) override;

	void addExtension(const std::string &pExtensionName) override;

	void selectObject(ExplorerObject* pObjectToSelect) override;

	void createObject(ExplorerObject* pParentObject) override;

	void removeObject(ExplorerObject* pObjectToRemove) override;

	void removeExtension(sdk::main::Extension* pExtensionToRemove) override;

	void openFile(const std::filesystem::path &pPathToFile) override;

	void createResource(const std::filesystem::path &pPathToCreate, sdk::main::ResourceType pType) override;

	void createScene(const std::filesystem::path &pPathToCreate) override;

	void createScript(const std::filesystem::path &pPathToCreate) override;

	void createDirectory(const std::filesystem::path &pPathToCreate) override;

	void renameFile(const std::filesystem::path &pPathToRemove) override;

	void deleteFile(const std::filesystem::path &pPathToDelete) override;

	void showInFiles(const std::filesystem::path &pPathToShow) override;

	void onViewClosed() override;
};


} // namespace mer::editor::mvp

#endif //PRESENTERMAIN_H
