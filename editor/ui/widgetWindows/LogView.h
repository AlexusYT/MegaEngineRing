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

#ifndef LOGVIEW_H
#define LOGVIEW_H

namespace mer::editor::ui {
class LogView {
	Gtk::Box mainBox;
	Gtk::TextView logText;
	Gtk::Button wrapBtn;
	Glib::Dispatcher dispatcher;
	std::mutex newMessagesMutex;
	std::list<Glib::ustring> newMessages;
	//bool scrollToEnd{true};

public:
	LogView();

	Glib::RefPtr<Gtk::TextBuffer> getBuffer() { return logText.get_buffer(); }

	void setBuffer(const Glib::RefPtr<Gtk::TextBuffer> &pBuffer) { logText.set_buffer(pBuffer); }

	operator Gtk::Widget&() { return mainBox; }

	void scrollToEnd() {
		auto iter = logText.get_buffer()->end();
		logText.scroll_to(iter);
	}

	void notifyTextChanged() const { dispatcher.emit(); }

	void addMessage(const Glib::ustring &pMessage) {
		std::lock_guard lock(newMessagesMutex);
		newMessages.emplace_back(pMessage);
	}
};
} // namespace mer::editor::ui

#endif //LOGVIEW_H
