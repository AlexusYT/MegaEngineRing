//
// Created by alexus on 23.01.24.
//

#include "ProjectCreatingWindow.h"

#include <thread>

#include "ui/widgetWindows/LogView.h"

namespace MVP_CORE {
ProjectCreatingWindow::ProjectCreatingWindow() : cancelButton("Отмена") {
	Gtk::Box box(Gtk::Orientation::VERTICAL);
	set_child(box);
	Gtk::Label label("Подготовка проекта к работе. Наберитесь терпения. Это займет несколько минут...");
	label.set_margin(5);
	label.set_wrap();
	label.set_wrap_mode(Pango::WrapMode::WORD);
	box.append(label);
	box.append(logView);
	box.append(cancelButton);
	cancelButton.set_margin(5);
	cancelButton.set_halign(Gtk::Align::END);

	set_size_request(600, 300);
}

void ProjectCreatingWindow::addMessageToLog(const std::string &pMessage) {
	const auto buffer = logView.getBuffer();
	buffer->insert_markup(buffer->end(), pMessage);
	logView.scrollToEnd();
}

void ProjectCreatingWindow::reportError(engine::utils::ReportMessagePtr pMessage) {
	engine::utils::Logger::error(pMessage);
}

}