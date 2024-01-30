//
// Created by alexus on 23.01.24.
//

#ifndef PROJECTCREATINGWINDOW_H
#define PROJECTCREATINGWINDOW_H

#include <mvp/creatingProject/IViewCreatingProject.h>
#include <ui/widgetWindows/LogView.h>

namespace MVP_CORE {


class ProjectCreatingWindow final : public Gtk::Window, public IViewCreatingProject {

	ui::LogView logView;
	Gtk::ToggleButton cancelButton;
	std::vector<std::shared_ptr<Gtk::TextBuffer>> buffers;


public:
	explicit ProjectCreatingWindow();

private:
	void addMessageToLog(const std::string &pMessage) override;

	void reportError(engine::utils::ReportMessageUPtr pMessage) override;

	void addWindow(const std::shared_ptr<Gtk::Window> &pWindow) override {
		pWindow->set_application(get_application());
	}

	void closeWindow() override { close(); }
};
} // namespace MVP_CORE

#endif //PROJECTCREATINGWINDOW_H
