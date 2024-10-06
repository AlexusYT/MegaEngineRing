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

#ifndef VIEWRESOURCESELECTION_H
#define VIEWRESOURCESELECTION_H
#include "IViewResourceSelection.h"

namespace mer::editor::ui {
class CustomTreeView;
}

namespace mer::editor::mvp {
class IViewProjectExplorer;

class ViewResourceSelection : public IViewResourceSelection, Gtk::Window {
	IPresenterResourceSelection* presenter{};
	std::shared_ptr<IWidgetContext> context;
	std::shared_ptr<Gtk::Builder> builder;
	std::shared_ptr<IViewProjectExplorer> explorer;
	Gtk::ListView* listView;


	explicit ViewResourceSelection(const std::shared_ptr<IWidgetContext> &pContext,
								   const std::shared_ptr<Gtk::Builder> &pBuilder);

public:
	static std::shared_ptr<ViewResourceSelection> create(const std::shared_ptr<IWidgetContext> &pContext,
														 sdk::utils::ReportMessagePtr &pMsg);

	void openView() override;

	void closeView() override;

	[[nodiscard]] const std::shared_ptr<IViewProjectExplorer> &getExplorer() const override { return explorer; }

	void setPresenter(IPresenterResourceSelection* pPresenter) override { presenter = pPresenter; }

	sigc::connection connectChooseFile(const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const override {
		return builder->get_widget<Gtk::Entry>("filePathEntry")->signal_icon_release().connect(pSlot);
	}

	void showFileDialog(const std::shared_ptr<Gtk::FileDialog> &pDialog,
						const sigc::slot<void(std::shared_ptr<Gio::AsyncResult> &pResult)> &pSlot) override {

		pDialog->open(*this, pSlot);
	}

	void displayError(const sdk::utils::ReportMessagePtr &pError) override;

	void displayMessage(const std::string &pMessage) override;

	void displayObjects(const std::vector<std::string> &pObjects) override;

	void setSelectButtonSensitivity(bool pState) const override;

	void selectResourceByUri(const std::filesystem::path &pPath) override;

	void selectObject(const std::string &pObjectToSelect) override;

	void setVisibleNoObjectsMessage(bool pState) override;

	std::string getSelectedObject() const override;
};


} // namespace mer::editor::mvp

#endif //VIEWRESOURCESELECTION_H
