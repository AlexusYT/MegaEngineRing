//
// Created by alexus on 28.12.23.
//

#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <mvp/startup/IViewStartup.h>

namespace mer::editor::mvp {
class StartupWindow final : public Gtk::Window, public IViewStartup {

	Glib::RefPtr<Gtk::Builder> builder{};

public:
	StartupWindow(BaseObjectType* pCobject, const Glib::RefPtr<Gtk::Builder> &pBuilder)
		: Window(pCobject), builder(pBuilder) {
		set_visible();
	}

	sigc::connection connectNewProjectClickSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Button>("btn_newProject")->signal_clicked().connect(pSlot);
	}

	sigc::connection connectCancelClickSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Button>("btn_cancel")->signal_clicked().connect(pSlot);
	}

	sigc::connection connectCreateClickSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Button>("btn_create")->signal_clicked().connect(pSlot);
	}

	sigc::connection connectOpenProjectClickSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Button>("btn_openProject")->signal_clicked().connect(pSlot);
	}

	sigc::connection connectPathChangedSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Entry>("entry_path")->signal_changed().connect(pSlot);
	}

	sigc::connection connectIconReleasedSignal(const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const override {
		return builder->get_widget<Gtk::Entry>("entry_path")->signal_icon_release().connect(pSlot);
	}

	sigc::connection connectNameChangedSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Entry>("entry_name")->signal_changed().connect(pSlot);
	}

	void setPath(const std::string &pPath) const override {
		builder->get_widget<Gtk::Entry>("entry_path")->set_text(pPath);
	}

	std::string getPath() const override { return builder->get_widget<Gtk::Entry>("entry_path")->get_text(); }

	void setName(const std::string &pPath) const override {
		builder->get_widget<Gtk::Entry>("entry_name")->set_text(pPath);
	}

	std::string getName() const override { return builder->get_widget<Gtk::Entry>("entry_name")->get_text(); }

	void switchMode(const int pMode) override {
		builder->get_widget<Gtk::Notebook>("notebook_mode")->set_current_page(pMode);
	}

	void showError(const std::string &pText) const override {
		auto* errorLabel = builder->get_widget<Gtk::Label>("label_error");
		errorLabel->set_visible();
		errorLabel->set_text(pText);
	}

	void hideError() const override {
		auto* errorLabel = builder->get_widget<Gtk::Label>("label_error");
		errorLabel->set_visible(false);
	}

	std::shared_ptr<Gtk::FileDialog> showFolderChooserDialog(
		const std::string &pInitialFolder,
		const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
			&pSlot) override;

	void showFileChooserDialog(
		const std::string &pInitialFolder,
		const sigc::slot<void(const std::shared_ptr<Gio::AsyncResult> &, const std::shared_ptr<Gtk::FileDialog> &)>
			&pSlot) override;

	void addWindow(const std::shared_ptr<Gtk::Window> &pWindow) override {
		pWindow->set_application(get_application());
	}

	void closeWindow() override { close(); }

	/*void openMainWindow(project::ProjectUPtr pProject) {

		//pProject->init();
		mainWindow = std::make_unique<ui::MainWindow>(std::move(pProject));
		if (auto msg = mainWindow->init()) {
			engine::utils::Logger::error("Failed to open main window");
			engine::utils::Logger::error(msg);
		}
		this->get_application()->add_window(*mainWindow->mainWindow);
		this->close();
	}*/
};
} // namespace mer::editor::mvp


#endif //STARTUPWINDOW_H
