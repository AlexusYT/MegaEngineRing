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

#ifndef VIEWRESOURCECREATION_H
#define VIEWRESOURCECREATION_H
#include "IViewResourceCreation.h"

namespace mer::editor::mvp {

class ViewResourceCreation : public IViewResourceCreation, Gtk::Window {
	IPresenterResourceCreation* presenter{};
	std::shared_ptr<IWidgetContext> context;
	std::shared_ptr<Gtk::Builder> builder;


	explicit ViewResourceCreation(const std::shared_ptr<IWidgetContext> &pContext,
								  const std::shared_ptr<Gtk::Builder> &pBuilder);

public:
	static std::shared_ptr<ViewResourceCreation> create(const std::shared_ptr<IWidgetContext> &pContext,
														sdk::utils::ReportMessagePtr &pMsg);

	void openView() override;

	void closeView() override;

	void setPresenter(IPresenterResourceCreation* pPresenter) override { presenter = pPresenter; }

	sigc::connection connectChooseFile(const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const override {
		return builder->get_widget<Gtk::Entry>("filePathEntry")->signal_icon_release().connect(pSlot);
	}

	void showFileDialog(const std::shared_ptr<Gtk::FileDialog> &pDialog,
						const sigc::slot<void(std::shared_ptr<Gio::AsyncResult> &pResult)> &pSlot) override {

		pDialog->open(*this, pSlot);
	}

	void displayError(const sdk::utils::ReportMessagePtr &pError) override;

	void displayChosenPath(const std::string &pPath) override;

	void displayResourceName(const std::string &pName) override;

	void switchTo(const std::string &pTabName) override;

	void displayMessage(const std::string &pMessage) override;
};

} // namespace mer::editor::mvp

#endif //VIEWRESOURCECREATION_H
