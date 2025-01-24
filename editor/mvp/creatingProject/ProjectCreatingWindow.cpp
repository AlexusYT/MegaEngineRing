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
// Created by alexus on 23.01.24.
//

#include "ProjectCreatingWindow.h"

#include "mvp/contexts/IWidgetContext.h"
#include "ui/widgetWindows/LogView.h"

namespace mer::editor::mvp {
ProjectCreatingWindow::ProjectCreatingWindow(const std::shared_ptr<IWidgetContext> &pContext)
	: cancelButton("Отмена"), context(pContext) {
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

void ProjectCreatingWindow::reportError(sdk::ReportMessagePtr pMessage) { sdk::Logger::error(pMessage); }

void ProjectCreatingWindow::closeView() { context->removeWidget(); }

void ProjectCreatingWindow::openView(){
context->addWidget(this);
}

}