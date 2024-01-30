//
// Created by alexus on 23.01.24.
//

#ifndef LOGVIEW_H
#define LOGVIEW_H

namespace UI_CORE {
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
} // namespace UI_CORE

#endif //LOGVIEW_H
