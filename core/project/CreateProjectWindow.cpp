//
// Created by alexus on 13.12.23.
//

#include "CreateProjectWindow.h"

//#include "MainWindow.h"
#include "Project.h"

namespace UI_CORE {


CreateProjectWindow::CreateProjectWindow() {}

CreateProjectWindow::~CreateProjectWindow() {}

bool CreateProjectWindow::init() {
	// Load the GtkBuilder file and instantiate its widgets:
	builder = Gtk::Builder::create();
	try {
		builder->add_from_file("Resources/createProject.ui");
	} catch (const Glib::FileError &ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return false;
	} catch (const Glib::MarkupError &ex) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return false;
	} catch (const Gtk::BuilderError &ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return false;
	}

	// Get the GtkBuilder-instantiated dialog:
	window = builder->get_widget<Gtk::Window>("dialog_createProject");
	if (!window) {
		std::cerr << "Could not get the dialog" << std::endl;
		return false;
	}

	/*window->signal_show().connect([this] { mainWindow->mainWindow->set_sensitive(false); });
	window->signal_hide().connect([this] { mainWindow->mainWindow->set_sensitive(true); });*/

	auto* pathEntry = builder->get_widget<Gtk::Entry>("entry_path");
	pathEntry->set_text("/home/alexus/GameEngineProjects");
	auto* errorLabel = builder->get_widget<Gtk::Label>("label_error");
	errorLabel->set_visible(false);
	pathEntry->signal_icon_release().connect([this, pathEntry, errorLabel](Gtk::Entry::IconPosition) {
		window->set_sensitive(false);
		const auto dialog = Gtk::FileDialog::create();
		dialog->set_modal(true);
		dialog->set_initial_folder(Gio::File::create_for_path(pathEntry->get_text()));
		// Show the dialog and wait for a user response:
		dialog->select_folder(*window, sigc::bind(sigc::mem_fun(*this, &CreateProjectWindow::on_folder_dialog_finish),
												  dialog, pathEntry, errorLabel));
	});


	auto* cancelBtn = builder->get_widget<Gtk::Button>("btn_cancel");
	cancelBtn->signal_clicked().connect([this] { window->hide(); });

	return true;
}

void CreateProjectWindow::on_folder_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result,
												  const Glib::RefPtr<Gtk::FileDialog> &dialog, Gtk::Entry* pathEntry,
												  Gtk::Label* errorLabel) const {
	// Handle the response:
	try {
		const auto folder = dialog->select_folder_finish(result);
		pathEntry->set_text(folder->get_path());
		window->set_sensitive(true);
	} catch (const Gtk::DialogError &err) {
		if (err.code() != Gtk::DialogError::DISMISSED) {
			using namespace std::string_literals;
			errorLabel->set_text("No folder selected. "s + err.what());
			errorLabel->set_visible(true);
		}
	} catch (const Glib::Error &err) {
		using namespace std::string_literals;
		errorLabel->set_text("Unexpected exception. "s + err.what());
		errorLabel->set_visible(true);
	}
}

void CreateProjectWindow::notifyLogChanged(Gtk::TextView* logWindow, const std::string &text) {
	addToMainThread([logWindow, text] {
		const auto buffer = logWindow->get_buffer();
		buffer->insert_markup(buffer->end(), text);
		auto scrollTo = buffer->end();
		logWindow->scroll_to(scrollTo, 0.0, 0.5, 0.5);
	});
}

} // namespace UI_CORE