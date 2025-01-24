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
// Created by alexus on 30.09.24.
//

#ifndef IPRESENTERRESOURCEEDITOR_H
#define IPRESENTERRESOURCEEDITOR_H
#include <memory>

#include "EngineSDK/resources/models/IModel3DResource.h"
#include "mvp/PresenterBase.h"

namespace mer::editor::ui {
class ISourceSelectionResult;
}

namespace mer::sdk {
class IResourceLoadExecutor;
class IResource;
} // namespace mer::sdk

namespace mer::editor::mvp {
class IViewResourceEditor;

class IPresenterResourceEditor : public PresenterBase {
public:
	virtual void chooseFileClicked(IViewResourceEditor* pView) = 0;

	virtual void onResourceNameChanged(const std::string &pNewName) = 0;

	virtual void onPathToFileChanged(IViewResourceEditor* pView, const std::filesystem::path &pPath) = 0;

	virtual void savePathClicked(IViewResourceEditor* pView, const std::string &pNewPath,
								 const std::string &pNewName) = 0;

	virtual void onCancelClicked() = 0;

	virtual std::shared_ptr<sdk::IResource> getSelectedResource(IViewResourceEditor* pView) = 0;

	virtual std::filesystem::path getDataPath() = 0;

	virtual void onSelectionChanged(const std::shared_ptr<sdk::IResource> &pResource,
									IViewResourceEditor* pView) = 0;

	virtual void removeObject(const std::shared_ptr<sdk::IModel3DObject> &pObjectToRemove,
							  IViewResourceEditor* pView) = 0;

	virtual void addObject(const std::shared_ptr<sdk::IModel3DObject> &pObjectToRemove,
						   IViewResourceEditor* pView) = 0;

	virtual sdk::IResourceLoadExecutor* getResourceLoader() = 0;

	virtual void onMaterialBaseColorChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
											IViewResourceEditor* pView) = 0;
	virtual void onMaterialMetallicChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
										   IViewResourceEditor* pView) = 0;
	virtual void onMaterialNormalChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
										 IViewResourceEditor* pView) = 0;
	virtual void onMaterialRoughnessChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
											IViewResourceEditor* pView) = 0;
	virtual void onMaterialAOChanged(const std::shared_ptr<ui::ISourceSelectionResult> &pResult,
									 IViewResourceEditor* pView) = 0;
};

} // namespace mer::editor::mvp

#endif //IPRESENTERRESOURCEEDITOR_H
