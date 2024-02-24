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