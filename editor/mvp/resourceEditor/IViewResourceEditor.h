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

#ifndef IVIEWRESOURCEEDITOR_H
#define IVIEWRESOURCEEDITOR_H
#include "EngineSDK/main/resources/IResource.h"
#include "mvp/IView.h"

namespace mer::sdk::main {
class IModel3DObject;
class IMaterialResource;
} // namespace mer::sdk::main

namespace mer::editor::mvp {
class IPresenterResourceEditor;
}

namespace mer::editor::mvp {

class IViewResourceEditor : public IView {
public:
	virtual void setPresenter(IPresenterResourceEditor* pPresenter) = 0;

	virtual sigc::connection connectChooseFile(const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const = 0;


	virtual void showFileDialog(const std::shared_ptr<Gtk::FileDialog> &pDialog,
								const sigc::slot<void(std::shared_ptr<Gio::AsyncResult> &pResult)> &pSlot) = 0;

	virtual void displayError(const sdk::utils::ReportMessagePtr &pError) = 0;

	virtual void displayChosenPath(const std::string &pPath) = 0;

	virtual void displayResourceName(const std::string &pName) = 0;

	virtual void switchTo(const std::string &pTabName) = 0;

	virtual void displayMessage(const std::string &pMessage) = 0;

	virtual void displayObjects(const std::vector<std::shared_ptr<sdk::main::IModel3DObject>> &pObjects,
								bool pFileObjects) = 0;

	virtual void setStackVisibility(bool pState) = 0;

	virtual void setSaveButtonSensitivity(bool pState) const = 0;

	virtual std::vector<std::string> getSelectedObjects() = 0;

	virtual void appendResource(const std::shared_ptr<sdk::main::IResource> &pNewResource) = 0;

	virtual void removeResource(const std::shared_ptr<sdk::main::IResource> &pResource) = 0;

	virtual void selectResource(const std::shared_ptr<sdk::main::IResource> &pResource) = 0;

	virtual void displayMaterial(const std::shared_ptr<sdk::main::IMaterialResource> &pMaterialResource) = 0;
};

} // namespace mer::editor::mvp

#endif //IVIEWRESOURCEEDITOR_H
