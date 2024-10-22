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
// Created by alexus on 30.09.24.
//

#ifndef PRESENTERRESOURCEEDITOR_H
#define PRESENTERRESOURCEEDITOR_H
#include <memory>

#include "EngineSDK/main/resources/models/IModel3DResource.h"
#include "IPresenterResourceEditor.h"

namespace mer::sdk::main {
class IModel3DObject;
}

namespace mer::sdk::main {
class IResource;
}

namespace mer::editor::mvp {
class FileResourceReader;
class IViewResourceEditor;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {
class IModelResourceEditor;
}

namespace mer::editor::mvp {

class PresenterResourceEditor : public IPresenterResourceEditor {
	class ViewInfo {
	public:
		std::shared_ptr<sdk::main::IResource> selectedResource;
	};

	class ResourceInfo {
	public:
		std::shared_ptr<FileResourceReader> resourceReader;
		std::vector<std::shared_ptr<sdk::main::IModel3DObject>> resourceObjects;
		std::vector<std::shared_ptr<sdk::main::IModel3DObject>> fileObjects;
		std::filesystem::path srcFilePath;
	};

	std::shared_ptr<IModelResourceEditor> model;
	std::vector<std::shared_ptr<IViewResourceEditor>> views;
	std::map<IViewResourceEditor*, ViewInfo> viewsInfo;
	std::map<sdk::main::IResource*, ResourceInfo> resourcesInfo;

public:
	PresenterResourceEditor(const std::shared_ptr<IModelResourceEditor> &pModel);

	void chooseFileClicked(IViewResourceEditor* pView) override;

	void onResourceNameChanged(const std::string &pNewName) override;

	void onPathToFileChanged(IViewResourceEditor* pView, const std::filesystem::path &pPath) override;

	void savePathClicked(IViewResourceEditor* pView, const std::string &pNewPath, const std::string &pNewName) override;

	void onCancelClicked() override;

	void run() override;

	void stop() override;

	inline void addView(const std::shared_ptr<IView> &pNewView) override;

	inline void removeView(const std::shared_ptr<IView> &pOldView) override;

	std::shared_ptr<sdk::main::IResource> getSelectedResource(IViewResourceEditor* pView) override;

	std::string getTypeName() override { return "PresenterResourceEditor"; }

	std::filesystem::path getDataPath() override;

	void onSelectionChanged(const std::shared_ptr<sdk::main::IResource> &pResource,
							IViewResourceEditor* pView) override;
	void removeObject(const std::shared_ptr<sdk::main::IModel3DObject> &pObjectToRemove,
					  IViewResourceEditor* pView) override;
	void addObject(const std::shared_ptr<sdk::main::IModel3DObject> &pObjectToAdd, IViewResourceEditor* pView) override;

private:
	ViewInfo* getViewInfo(IViewResourceEditor* pView);

	ResourceInfo* getResourceInfo(sdk::main::IResource* pResource);

	sdk::utils::ReportMessagePtr saveResource(const std::shared_ptr<sdk::main::IResource> &pResource);
};

} // namespace mer::editor::mvp

#endif //PRESENTERRESOURCEEDITOR_H
