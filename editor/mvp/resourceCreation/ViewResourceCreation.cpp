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

#include "ViewResourceCreation.h"

#include "Globals.h"
#include "IPresenterResourceCreation.h"
#include "mvp/contexts/IWidgetContext.h"
#include "ui/utils/ErrorDialog.h"

namespace mer::editor::mvp {
ViewResourceCreation::ViewResourceCreation(const std::shared_ptr<IWidgetContext> &pContext,
										   const std::shared_ptr<Gtk::Builder> &pBuilder)
	: context(pContext), builder(pBuilder) {


	builder->get_widget<Gtk::Entry>("filePathEntry")
		->signal_icon_release()
		.connect([this](Gtk::Entry::IconPosition /*pPosition*/) {
			if (presenter) presenter->chooseFileClicked();
		});
	set_child(*builder->get_widget<Gtk::Widget>("root"));
}

std::shared_ptr<ViewResourceCreation> ViewResourceCreation::create(const std::shared_ptr<IWidgetContext> &pContext,
																   sdk::utils::ReportMessagePtr &pMsg) {
	std::shared_ptr<Gtk::Builder> builder;
	try {
		builder = Gtk::Builder::create_from_file(Globals::getResourcesPath() / "ResourceCreationWindow.ui", "root");
	} catch (...) {
		pMsg = sdk::utils::ReportMessage::create();
		pMsg->setTitle("Failed to load ui file");
		pMsg->setMessage("Error while loading resource creation window");
		return nullptr;
	}
	return std::shared_ptr<ViewResourceCreation>(new ViewResourceCreation(pContext, builder));
}

void ViewResourceCreation::openView() { context->addWidget(this); }

void ViewResourceCreation::closeView() { context->removeWidget(); }

void ViewResourceCreation::displayError(const sdk::utils::ReportMessagePtr &pError) {
	ErrorDialog::showErrorDialog(this, pError);
}

void ViewResourceCreation::displayChosenPath(const std::string &pPath) {
	builder->get_widget<Gtk::Entry>("filePathEntry")->set_text(pPath);
}

void ViewResourceCreation::displayResourceName(const std::string &pName) {
	builder->get_widget<Gtk::Entry>("nameEntry")->set_text(pName);
}

void ViewResourceCreation::switchTo(const std::string &pTabName) {
	builder->get_widget<Gtk::Stack>("preferencesStack")->set_visible_child(pTabName);
}

void ViewResourceCreation::displayMessage(const std::string &pMessage) {
	builder->get_widget<Gtk::Label>("messageLabel")->set_text(pMessage);
}
} // namespace mer::editor::mvp