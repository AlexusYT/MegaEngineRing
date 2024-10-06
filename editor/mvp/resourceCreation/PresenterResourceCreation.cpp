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

#include "EngineSDK/main/resources/IResource.h"
#include "EngineSDK/main/resources/models/IModel3DResource.h"
#include "IModelResourceCreation.h"
#include "IViewResourceCreation.h"
#include "mvp/ApplicationController.h"
#include "readers/ObjFileResourceReader.h"
#include "savers/Model3DResourceSaver.h"

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

void PresenterResourceCreation::onResourceNameChanged(const std::string &pNewName) {
	if (model->getResourceName() == pNewName) return;
	model->setResourceName(pNewName);
	auto path = model->getPathToResource() / (pNewName + ".enmodel");
	if (exists(path)) {
		view->displayMessage("Resource with the such name is already exists");
		view->setSaveButtonSensitivity(false);
		return;
	}
	view->displayMessage("");
	view->setSaveButtonSensitivity(true);
}

void PresenterResourceCreation::onPathToFileChanged() {
	auto path = model->getPathToFile();
	auto reader = std::make_shared<ObjFileResourceReader>(path);
	reader->setSdk(model->getSdk());
	std::string fileExt = path.extension().string();
	if (auto msg = reader->checkType()) {
		view->displayMessage(msg->getReport(false));
		view->setStackVisibility(false);
	} else {
		view->displayMessage("");
		view->setStackVisibility(true);
		view->switchTo("objModel");
		auto objects = reader->getObjects() | std::views::keys;
		view->displayObjects(std::vector(objects.begin(), objects.end()));
		resourceReader = reader;
	}
	view->displayChosenPath(path.string());
	view->displayResourceName(path.stem().string());
}

void PresenterResourceCreation::saveClicked() {
	if (auto objReader = std::dynamic_pointer_cast<ObjFileResourceReader>(resourceReader)) {
		auto objects = view->getSelectedObjects();
		auto resource = objReader->generateResource(objects);
		auto resourceName = model->getResourceName() + ".enmodel";
		resource->asResource()->setResourceUri(model->getRelativePathToResource() / resourceName);
		if (auto msg = Model3DResourceSaver::saveToFile(model->getPathToResource() / resourceName, resource)) {
			view->displayError(msg);
		} else {
			getAppController()->stop(this);
		}
	}
}

void PresenterResourceCreation::onCancelClicked() { getAppController()->stop(this); }

void PresenterResourceCreation::run() { view->openView(); }

void PresenterResourceCreation::stop() { view->closeView(); }
} // namespace mer::editor::mvp