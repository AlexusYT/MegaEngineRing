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

#ifndef MODELRESOURCESELECTION_H
#define MODELRESOURCESELECTION_H
#include <memory>

#include "IModelResourceSelection.h"
#include "mvp/main/projectExplorer/entries/ProjectExplorerElement.h"
#include "project/LoadedScene.h"

namespace mer::sdk::utils {
class PropertyBase;
}
namespace mer::editor::mvp {
class FileResourceReader;
class IPresenterResourceSelection;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {

class ModelResourceSelection : public IModelResourceSelection {
	IPresenterResourceSelection* presenter{};
	sdk::utils::PropertyBase* propertyBase{};
	std::shared_ptr<project::LoadedScene> loadedScene;
	std::shared_ptr<sdk::main::IResource> selectedResource;
	ExplorerElementType elementType{ExplorerElementType::NONE};

public:
	void setPresenter(IPresenterResourceSelection* pPresenter) override { presenter = pPresenter; }

	[[nodiscard]] sdk::utils::PropertyBase* getPropertyBase() const override { return propertyBase; }

	void setPropertyBase(sdk::utils::PropertyBase* pPropertyBase) override { propertyBase = pPropertyBase; }

	void setLoadedScene(const std::shared_ptr<project::LoadedScene> &pLoadedScene) override {
		loadedScene = pLoadedScene;
	}

	[[nodiscard]] const std::shared_ptr<project::LoadedScene> &getLoadedScene() const override { return loadedScene; }

	void setSelectedResource(const std::shared_ptr<sdk::main::IResource> &pSelectedResource) override {
		selectedResource = pSelectedResource;
	}

	[[nodiscard]] const std::shared_ptr<sdk::main::IResource> &getSelectedResource() const override {
		return selectedResource;
	}

	[[nodiscard]] ExplorerElementType getElementType() const override { return elementType; }

	void setElementType(ExplorerElementType pElementType) override { elementType = pElementType; }
};

} // namespace mer::editor::mvp

#endif //MODELRESOURCESELECTION_H
