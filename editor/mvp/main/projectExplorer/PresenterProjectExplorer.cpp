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
// Created by alexus on 12.08.24.
//

#include "PresenterProjectExplorer.h"

#include "IModelProjectExplorer.h"
#include "IViewProjectExplorer.h"
#include "entries/ProjectExplorerElement.h"
#include "mvp/ApplicationController.h"
#include "mvp/dialogs/confirmationDialog/ConfirmationDialog.h"
#include "project/Project.h"

namespace mer::editor::mvp {
PresenterProjectExplorer::PresenterProjectExplorer(const std::shared_ptr<IModelProjectExplorer> &pModel)
	: model(pModel) {
	for (auto view: views) {
		view->onPathChanged(model->getRootPath());
		view->setPresenter(this);
	}
}

void PresenterProjectExplorer::setEntrySelectionChanged(
	const sigc::slot<void(ui::ProjectExplorerEntry*)> & /*pEntrySelectionChanged*/) {
	//viewProjectExplorer->setEntrySelectionChanged(pEntrySelectionChanged);
}

void PresenterProjectExplorer::openInFilesystem(const std::filesystem::path &pPathToFile) {
	sdk::Logger::info("Show {} in Files", pPathToFile.string());
}

void PresenterProjectExplorer::deleteFile(const std::filesystem::path &pPathToDelete) {
	using namespace std::filesystem;

	sdk::Logger::info("Starting {} deletion", pPathToDelete.string());
	/*std::error_code error;
	auto type = status(pPathToDelete, error).type();
	if (error) {

		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to delete file");
		msg->setMessage(error.message());
		msg->addInfoLine("File path: {}", pPathToDelete.string());

		sdk::Logger::error(msg);
		return;
	}

	std::string name;
	if (type == file_type::directory) {
		name = "directory";
	} else if (type == file_type::regular) {
		name = "file";
	} else if (type == file_type::symlink) {
		name = "symlink";
	} else {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to delete the file");
		msg->setMessage("Trying to delete the file with the unknown type");
		msg->addInfoLine("Type must be directory or file");
		msg->addInfoLine("File path: {}", pPathToDelete.string());

		sdk::Logger::error(msg);
		return;
	}*/

	views.at(0)->startConfirmation(
		"Confirm the file deletion",
		std::format("Confirm deletion of {}. This operation cannot be undone!", pPathToDelete.filename().string()),
		[pPathToDelete](const int pId) {
			if (pId == 0) {
				if (std::error_code removeError; !remove_all(pPathToDelete, removeError)) {
					const auto msg = sdk::ReportMessage::create();
					msg->setTitle("Failed to delete file");
					msg->setMessage(removeError.message());
					msg->addInfoLine("File path: {}", pPathToDelete.string());

					sdk::Logger::error(msg);
					return;
				}
				sdk::Logger::info("File deleted");
				/*path uri = "/";
				auto root = modelMain->getProject()->getProjectDataPath();
				if (pPathToDelete != "/" && pPathToDelete != root) { uri = relative(pPathToDelete, root); }
				auto ext = pPathToDelete.extension().string();
				if (ext == ".enscene") { loadedScene->unload(); }
				if (ext == ".enmodel" || ext == ".entex" || ext == ".enmat") { editingResources->deleteResource(uri); }*/
			} else if (pId == 1) {
				sdk::Logger::info("Deletion canceled");
			}
		});


	//getAppController()->addPopup(dialog);

	/*dialog->set_message(std::format("Confirm deleting {} {}", name, pPathToDelete.filename().string()));
	dialog->set_detail("This operation cannot be undone");
	dialog->set_modal();
	dialog->set_buttons({"Delete", "Cancel"});
	dialog->set_default_button(1);
	dialog->set_cancel_button(1);
	auto lambda = [dialog, pPathToDelete, this](const std::shared_ptr<Gio::AsyncResult> &pResult) {
		if (const int btn = dialog->choose_finish(pResult); btn == 0) {
		}
	};
	dialog->choose(*viewMain->getWindow(), lambda);*/
}

void PresenterProjectExplorer::addView(const std::shared_ptr<IView> &pNewView) {
	const auto view = std::dynamic_pointer_cast<IViewProjectExplorer>(pNewView);
	if (!view) return;
	view->setElements(model->getElements()->getChildren());
	view->onPathChanged(model->getRootPath());
	view->setPresenter(this);
	views.push_back(view);
	view->openView();
}

void PresenterProjectExplorer::removeView(const std::shared_ptr<IView> &pOldView) { erase(views, pOldView); }

void PresenterProjectExplorer::run() {
	for (const auto &viewProjectExplorer: views) viewProjectExplorer->openView();
}

void PresenterProjectExplorer::stop() {
#ifdef USE_OLD_UI
	while (!views.empty()) { views.front()->closeView(); }
#else
	for (auto viewProjectExplorer: views) { viewProjectExplorer->closeView(); }
	views.clear();
#endif
}
} // namespace mer::editor::mvp