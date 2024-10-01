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

#include "PresenterResourceCreation.h"

#include "IModelResourceCreation.h"
#include "IViewResourceCreation.h"
#include "readers/ObjFileResourceReader.h"

namespace mer::editor::mvp {
PresenterResourceCreation::PresenterResourceCreation(const std::shared_ptr<IModelResourceCreation> &pModel,
													 const std::shared_ptr<IViewResourceCreation> &pView)
	: model(pModel), view(pView) {

	model->setPresenter(this);
	view->setPresenter(this);
}

void PresenterResourceCreation::chooseFileClicked() {
	const std::shared_ptr<Gtk::FileDialog> dialog = Gtk::FileDialog::create();
	dialog->set_modal(false);
	auto result = Gio::ListStore<Gtk::FileFilter>::create();
	auto filter = Gtk::FileFilter::create();
	filter->add_suffix("obj");
	result->append(filter);
	dialog->set_filters(result);
	//dialog->set_default_filter(filter);

	view->showFileDialog(dialog, [dialog, this](std::shared_ptr<Gio::AsyncResult> &pResult) {
		try {
			const auto file = dialog->open_finish(pResult);
			model->setPathToFile(file->get_path());
		} catch (const Gtk::DialogError &err) {
			if (err.code() == Gtk::DialogError::FAILED) {
				const auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to show file dialog");
				msg->setMessage(err.what());
				view->displayError(msg);
			}
		} catch (const Glib::Error &err) {
			const auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to show file dialog");
			msg->setMessage(err.what());
			view->displayError(msg);
		}
	});
}

void PresenterResourceCreation::onPathToFileChanged() {
	auto path = model->getPathToFile();
	view->displayChosenPath(path.string());
	view->displayResourceName(path.stem().string());
	ObjFileResourceReader reader(path);

	std::string fileExt = path.extension().string();
	if (auto msg = reader.checkType()) {
		view->switchTo("message");
		view->displayMessage(msg->getReport(false));
	} else {
		view->switchTo("objModel");
	}
}

void PresenterResourceCreation::run() { view->openView(); }

void PresenterResourceCreation::stop() { view->closeView(); }
} // namespace mer::editor::mvp