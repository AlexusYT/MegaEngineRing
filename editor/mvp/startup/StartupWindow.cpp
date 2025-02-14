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
// Created by alexus on 28.12.23.
//

#include "StartupWindow.h"

#include "Globals.h"
#include "mvp/contexts/IWidgetContext.h"

namespace mer::editor::mvp {

StartupWindow::StartupWindow(const std::shared_ptr<IWidgetContext> &pContext) : context(pContext) {

	try {
		builder = Gtk::Builder::create_from_resource("/ui/CreateOpenProject.ui");
	} catch (...) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to init window");
		msg->setMessage("Error while loading UI from file");
		sdk::Logger::error(msg);
		return;
	}
	window = builder->get_widget<Gtk::ApplicationWindow>("window_createOpenProj");
	if (!window) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Failed to init window");
		msg->setMessage("Window not found");
		msg->addInfoLine("Window name: window_createOpenProj");
		sdk::Logger::error(msg);
		return;
	}
	window->set_visible();
}

void StartupWindow::updateInterface() {}

std::shared_ptr<Gtk::FileDialog> StartupWindow::showFolderChooserDialog(
	const std::string &pInitialFolder,
	const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
		&pSlot) {

	const std::shared_ptr<Gtk::FileDialog> dialog = Gtk::FileDialog::create();
	dialog->set_modal(true);
	dialog->set_initial_folder(Gio::File::create_for_path(pInitialFolder));

	// Show the dialog and wait for a user response:
	dialog->select_folder(*window, sigc::bind(pSlot, dialog));
	return dialog;
}

void StartupWindow::showFileChooserDialog(
	const std::string &pInitialFolder,
	const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
		&pSlot) {

	const std::shared_ptr<Gtk::FileDialog> dialog = Gtk::FileDialog::create();
	dialog->set_modal(true);
	auto result = Gio::ListStore<Gtk::FileFilter>::create();
	auto filter = Gtk::FileFilter::create();
	filter->add_suffix("enproj");
	result->append(filter);
	dialog->set_filters(result);
	dialog->set_initial_folder(Gio::File::create_for_path(pInitialFolder));

	dialog->open(*window, sigc::bind(pSlot, dialog));
}

void StartupWindow::closeView() { context->removeWidget(); }

void StartupWindow::openView() { context->addWidget(this->window); }

} // namespace mer::editor::mvp