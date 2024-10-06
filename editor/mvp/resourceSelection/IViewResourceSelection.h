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

#ifndef IVIEWRESOURCESELECTION_H
#define IVIEWRESOURCESELECTION_H
#include "mvp/IView.h"

namespace mer::editor::mvp {
class IViewProjectExplorer;
class IPresenterResourceSelection;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {

class IViewResourceSelection : public IView {
public:
	[[nodiscard]] virtual const std::shared_ptr<IViewProjectExplorer> &getExplorer() const = 0;

	virtual void setPresenter(IPresenterResourceSelection* pPresenter) = 0;

	virtual sigc::connection connectChooseFile(const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const = 0;


	virtual void showFileDialog(const std::shared_ptr<Gtk::FileDialog> &pDialog,
								const sigc::slot<void(std::shared_ptr<Gio::AsyncResult> &pResult)> &pSlot) = 0;

	virtual void displayError(const sdk::utils::ReportMessagePtr &pError) = 0;

	virtual void displayMessage(const std::string &pMessage) = 0;

	virtual void displayObjects(const std::vector<std::string> &pObjects) = 0;

	virtual void setSelectButtonSensitivity(bool pState) const = 0;

	virtual void selectResourceByUri(const std::filesystem::path &pPath) = 0;

	virtual void setVisibleNoObjectsMessage(bool pState) = 0;

	virtual std::string getSelectedObject() const = 0;

	virtual void selectObject(const std::string &pObjectToSelect) = 0;
};

} // namespace mer::editor::mvp

#endif //IVIEWRESOURCESELECTION_H
