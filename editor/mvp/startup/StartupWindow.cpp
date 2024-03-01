// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 28.12.23.
//

#include "StartupWindow.h"

namespace mer::editor::mvp {
std::shared_ptr<Gtk::FileDialog> StartupWindow::showFolderChooserDialog(
	const std::string &pInitialFolder,
	const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
		&pSlot) {

	const std::shared_ptr<Gtk::FileDialog> dialog = Gtk::FileDialog::create();
	dialog->set_modal(true);
	dialog->set_initial_folder(Gio::File::create_for_path(pInitialFolder));

	// Show the dialog and wait for a user response:
	dialog->select_folder(*this, sigc::bind(pSlot, dialog));
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

	dialog->open(*this, sigc::bind(pSlot, dialog));
}
} // namespace mer::editor::mvp