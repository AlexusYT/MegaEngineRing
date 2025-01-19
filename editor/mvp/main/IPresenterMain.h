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
// Created by alexus on 23.09.24.
//

#ifndef IPRESENTERMAIN_H
#define IPRESENTERMAIN_H

#include "EngineSDK/main/extensions/BasicRenderExtension.h"
#include "mvp/PresenterBase.h"
#include "ui/customWidgets/multipaned/MultiPaned.h"

namespace mer::sdk::utils {
class PropertyBase;
}

namespace mer::sdk::main {
class Extension;
} // namespace mer::sdk::main

namespace mer::editor::mvp {
class ExplorerObject;

class IPresenterMain : public PresenterBase {
public:
	virtual std::shared_ptr<IView> createView(const IPresenter* pPresenter,
											  const std::shared_ptr<MultiPanedContext> &pContext) = 0;

	virtual void displayError(const sdk::utils::ReportMessagePtr &pMsg) = 0;

	virtual void onLayoutLoadFatal() = 0;

	virtual void onPanedLayoutTabsChanged() = 0;

	virtual void onCurrentTabChanged() = 0;

	virtual void readJsonForTab(int32_t pIndex,
								const sigc::slot<void(const sdk::utils::ReportMessagePtr &pError)> &pCallback) = 0;

	virtual void selectResourceForProperty(sdk::utils::PropertyBase* pProperty) = 0;

	virtual void addExtension(const std::string &pExtensionName) = 0;

	virtual void selectObject(ExplorerObject* pObjectToSelect) = 0;

	virtual void createObject(ExplorerObject* pParentObject) = 0;

	virtual void removeObject(ExplorerObject* pObjectToRemove) = 0;

	virtual void removeExtension(sdk::main::Extension* pExtensionToRemove) = 0;

	virtual void openFile(const std::filesystem::path &pPathToFile) = 0;

	virtual void createResource(const std::filesystem::path &pPathToCreate, sdk::main::ResourceType pType) = 0;

	virtual void createScene(const std::filesystem::path &pPathToCreate) = 0;

	virtual void createScript(const std::filesystem::path &pPathToCreate) = 0;

	virtual void createDirectory(const std::filesystem::path &pPathToCreate) = 0;

	virtual void renameFile(const std::filesystem::path &pPathToRemove) = 0;

	virtual void deleteFile(const std::filesystem::path &pPathToDelete) = 0;

	virtual void showInFiles(const std::filesystem::path &pPathToShow) = 0;

	virtual void onViewClosed() = 0;
};
} // namespace mer::editor::mvp


#endif //IPRESENTERMAIN_H
