//
// Created by alexus on 30.10.2021.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <mvp/main/IViewMain.h>
#include <ui/widgetWindows/CenterWindow.h>
#include <ui/widgetWindows/LogView.h>
#include <ui/widgetWindows/TreeObjectWindow.h>
#include <ui/widgetWindows/projectExplorer/ProjectExplorerWindow.h>

#include "graphic/viewport/OpenGLFwd.h"
#include "project/ProjectFwd.h"

namespace MVP_CORE {
class MainWindow : public IViewMain, public Gtk::Window {
	std::vector<ui::LogView> logs{3};

	std::shared_ptr<project::Project> project;

	ui::ProjectExplorerWindow projectExplorerWindow;
	ui::CenterWindow centerWindow;
	Glib::RefPtr<Gtk::Builder> builder;
	Glib::RefPtr<Gtk::EventControllerKey> keyController;


public:
	static std::shared_ptr<MainWindow> create(const std::shared_ptr<project::Project> &pProject,
											  engine::utils::ReportMessagePtr &pReportMessage) {
		const auto builder = Gtk::Builder::create();
		try {
			builder->add_from_file("Resources/base.ui");
		} catch (...) {
			pReportMessage = engine::utils::ReportMessage::create();
			pReportMessage->setTitle("Failed to init main window");
			pReportMessage->setMessage("Error while loading UI from file");
		}

		auto viewMain =
			std::shared_ptr<MainWindow>(Gtk::Builder::get_widget_derived<MainWindow>(builder, "MainWindow", pProject));
		if (!viewMain) {
			pReportMessage = engine::utils::ReportMessage::create();
			pReportMessage->setTitle("Failed to init main window");
			pReportMessage->setMessage("Could not get the dialog");
		}
		pReportMessage = nullptr;
		return viewMain;
	}

	bool reloadUi = false;
	OpenGLUPtr render;

	Gtk::Notebook* notebook;
	explicit MainWindow(BaseObjectType* pCobject, const Glib::RefPtr<Gtk::Builder> &pBuilder,
						std::shared_ptr<project::Project> pProject);

	~MainWindow() override;

	sigc::connection connectReloadCmakeClickedSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Button>("btn_reloadCMake")->signal_clicked().connect(pSlot);
	}

	sigc::connection connectRunClickedSignal(const sigc::slot<void()> &pSlot) const override {
		return builder->get_widget<Gtk::Button>("btn_run")->signal_clicked().connect(pSlot);
	}

	sigc::connection connectKeyPressedSignal(
		const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override {
		return keyController->signal_key_pressed().connect(pSlot, false);
	}

	void switchLogPage(const int pId) const override { notebook->set_current_page(pId); }

	void addLogMessage(const int pId, const Glib::ustring &pMessage) override {
		auto &log = logs[static_cast<size_t>(pId)];
		const auto buffer = log.getBuffer();
		buffer->insert_markup(buffer->end(), pMessage);
		log.scrollToEnd();
	}

	void clearLogMessage(const int pId) override {
		auto &log = logs[static_cast<size_t>(pId)];
		const auto buffer = log.getBuffer();
		buffer->set_text("");
	}

	void setWindowTitle(const std::string &pTitle) override {
		const auto bar = dynamic_cast<Gtk::HeaderBar*>(get_titlebar());
		dynamic_cast<Gtk::Label*>(bar->get_title_widget())->set_label(pTitle);
		set_title(pTitle);
	}

	void addActionGroup(const std::string &pName, const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup) override {
		insert_action_group(pName, pActionGroup);
	}

	void reportError(const engine::utils::ReportMessagePtr &pError) override {
		if (pError) engine::utils::Logger::error(pError);
	}

	void addWindow(const std::shared_ptr<Gtk::Window> & /*pWindow*/) override {}

	void closeWindow() override {}

protected:
};
} // namespace MVP_CORE

#endif //GAMEENGINE_H
