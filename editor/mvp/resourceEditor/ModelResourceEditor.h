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

#ifndef MODELRESOURCEEDITOR_H
#define MODELRESOURCEEDITOR_H
#include "IModelResourceEditor.h"

namespace mer::editor::project {
class Sdk;
}

namespace mer::editor::mvp {
class FileResourceReader;
class IPresenterResourceEditor;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {

class ModelResourceEditor : public IModelResourceEditor {
	IPresenterResourceEditor* presenter{};
	std::filesystem::path pathToDataDir;
	std::filesystem::path pathToResource;
	std::filesystem::path relativePathToResource;
	std::string resourceName;
	std::shared_ptr<project::Sdk> sdk;
	std::shared_ptr<EditingResourceList> editingResources;

public:
	void setPresenter(IPresenterResourceEditor* pPresenter) override { presenter = pPresenter; }

	void setPathToDataDir(const std::filesystem::path &pPathToDataDir) override;

	[[nodiscard]] const std::filesystem::path &getPathToDataDir() const override { return pathToDataDir; }

	[[nodiscard]] const std::filesystem::path &getPathToResource() const override { return pathToResource; }

	void setPathToResource(const std::filesystem::path &pPathToResource) override { pathToResource = pPathToResource; }

	[[nodiscard]] const std::string &getResourceName() const override { return resourceName; }

	void setResourceName(const std::string &pResourceName) override { resourceName = pResourceName; }

	void setSdk(const std::shared_ptr<project::Sdk> &pSdk) override { sdk = pSdk; }

	[[nodiscard]] const std::shared_ptr<project::Sdk> &getSdk() const override { return sdk; }

	void setRelativePathToResource(const std::filesystem::path &pPath) override { relativePathToResource = pPath; }

	[[nodiscard]] const std::filesystem::path &getRelativePathToResource() const override {
		return relativePathToResource;
	}

	[[nodiscard]] const std::shared_ptr<EditingResourceList> &getEditingResources() const override {
		return editingResources;
	}

	void setEditingResources(const std::shared_ptr<EditingResourceList> &pEditingResources) override {
		editingResources = pEditingResources;
	}
};

} // namespace mer::editor::mvp

#endif //MODELRESOURCEEDITOR_H
