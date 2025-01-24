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

#include "LogView.h"

#include <thread>

namespace mer::editor::ui {
LogView::LogView() {
	Gtk::Box buttonBox(Gtk::Orientation::VERTICAL);
	mainBox.append(buttonBox);
	buttonBox.set_margin_start(5);
	buttonBox.set_margin_end(5);
	buttonBox.append(wrapBtn);
	wrapBtn.set_margin(5);

	Gtk::ScrolledWindow scrolledWindow;
	scrolledWindow.set_child(logText);
	logText.place_cursor_onscreen();
	logText.set_margin(5);
	logText.set_expand();
	mainBox.append(scrolledWindow);

	dispatcher.connect([this] {
		const auto buffer = logText.get_buffer();
		std::lock_guard lock(newMessagesMutex);
		for (const auto &newMessage: newMessages) { buffer->insert_markup(buffer->end(), newMessage); }
		/*if (scrollToEnd) {
			auto iter = buffer->end();
			logText.scroll_to(iter);
		}*/
		newMessages.clear();
	});
}

} // namespace mer::editor::ui