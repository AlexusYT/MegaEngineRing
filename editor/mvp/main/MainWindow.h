// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 30.10.2021.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <mvp/main/IViewMain.h>
#include <ui/widgetWindows/CenterWindow.h>
#include <ui/widgetWindows/LogView.h>
#include <ui/widgetWindows/projectExplorer/ProjectExplorerWindow.h>

#include "graphic/viewport/OpenGLFwd.h"
#include "project/ProjectFwd.h"

namespace mer::editor::mvp {
class MainWindow : public IViewMain, public Gtk::Window {
	std::vector<ui::LogView> logs{3};

	std::shared_ptr<project::Project> project;

	ui::ProjectExplorerWindow projectExplorerWindow;
	ui::CenterWindow centerWindow;
	Glib::RefPtr<Gtk::Builder> builder;
	Glib::RefPtr<Gtk::EventControllerKey> keyController;


public:
	static std::shared_ptr<MainWindow> create(const std::shared_ptr<project::Project> &pProject,
											  sdk::utils::ReportMessagePtr &pReportMessage);

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

	sigc::connection connectKeyReleasedSignal(
		const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override {
		return keyController->signal_key_released().connect(pSlot, false);
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

	void reportError(const sdk::utils::ReportMessagePtr &pError) override {
		if (pError) sdk::utils::Logger::error(pError);
	}

	void addWindow(const std::shared_ptr<Gtk::Window> & /*pWindow*/) override {}

	void closeWindow() override {}

protected:
};
} // namespace mer::editor::mvp

#endif //MAINWINDOW_H
