//
// Created by alexus on 13.12.23.
//

#ifndef CREATEPROJECTWINDOW_H
#define CREATEPROJECTWINDOW_H

namespace UI_CORE {
class MainWindow;

class CreateProjectWindow {
	Glib::Dispatcher logDispatcher;
	std::map<Gtk::TextView*, std::string> logs;

	std::vector<sigc::slot<void()>> mainThreadQueue;
	std::mutex queueMutex;
	bool queueRunning{};

public:
	bool reloadUi = false;
	Gtk::Window* window = nullptr;
	MainWindow* mainWindow;
	Glib::RefPtr<Gtk::Builder> builder;

	Gtk::Notebook* notebook;
	explicit CreateProjectWindow();
	~CreateProjectWindow();

	[[nodiscard]] const Glib::Dispatcher &getLogDispatcher() const { return logDispatcher; }

	bool init();
	void on_folder_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result,
								 const Glib::RefPtr<Gtk::FileDialog> &dialog, Gtk::Entry* pathEntry,
								 Gtk::Label* errorLabel) const;

	void notifyLogChanged(Gtk::TextView* logWindow, const std::string &text);

	void addToMainThread(const sigc::slot<void()> &slot) {
		std::lock_guard lock(queueMutex);
		mainThreadQueue.emplace_back(slot);
		if (!queueRunning) {
			logDispatcher.emit();
			queueRunning = true;
		}
	}
};
} // namespace UI_CORE

#endif //CREATEPROJECTWINDOW_H
