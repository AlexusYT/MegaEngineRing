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

#ifndef IMODELRESOURCEEDITOR_H
#define IMODELRESOURCEEDITOR_H

namespace mer::editor::mvp {
class EditingResourceList;
class IPresenterResourceEditor;

class IModelResourceEditor {

public:
	virtual ~IModelResourceEditor() = default;

	virtual void setPresenter(IPresenterResourceEditor* pPresenter) = 0;

	virtual void setPathToDataDir(const std::filesystem::path &pPathToFile) = 0;

	[[nodiscard]] virtual const std::filesystem::path &getPathToDataDir() const = 0;

	[[nodiscard]] virtual const std::filesystem::path &getPathToResource() const = 0;

	virtual void setPathToResource(const std::filesystem::path &pPathToResource) = 0;

	[[nodiscard]] virtual const std::string &getResourceName() const = 0;

	virtual void setResourceName(const std::string &pResourceName) = 0;

	virtual void setRelativePathToResource(const std::filesystem::path &pPath) = 0;

	[[nodiscard]] virtual const std::filesystem::path &getRelativePathToResource() const = 0;

	[[nodiscard]] virtual const std::shared_ptr<EditingResourceList> &getEditingResources() const = 0;

	virtual void setEditingResources(const std::shared_ptr<EditingResourceList> &pEditingResources) = 0;
};

} // namespace mer::editor::mvp

#endif //IMODELRESOURCEEDITOR_H
