//
// Created by alexus on 23.01.24.
//

#include "LogView.h"

#include <thread>

namespace UI_CORE {
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

} // namespace UI_CORE