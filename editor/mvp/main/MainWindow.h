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
#include <mvp/main/centerWindow/ViewCenterWindow.h>

#include "ui/customWidgets/multipaned/MultiPaned.h"

namespace mer::editor::mvp {
class PanedLayoutTab;

class MainWindow : public IViewMain, public Gtk::Window, ThreadDispatcher {
	std::shared_ptr<IWidgetContext> context;

	Glib::RefPtr<Gtk::EventControllerKey> keyController;
	IPresenterMain* presenter{};
	Gtk::Notebook panedTabs;

public:
	static std::shared_ptr<MainWindow> create(const std::shared_ptr<IWidgetContext> &pContext,
											  sdk::utils::ReportMessagePtr &pReportMessage);

	bool reloadUi = false;

	explicit MainWindow(const Glib::RefPtr<Gtk::Builder> &pBuilder, const std::shared_ptr<IWidgetContext> &pContext);

	~MainWindow() override;

	sigc::connection connectKeyPressedSignal(
		const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override {
		return keyController->signal_key_pressed().connect(pSlot, false);
	}

	sigc::connection connectKeyReleasedSignal(
		const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override {
		return keyController->signal_key_released().connect(pSlot, false);
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

	void openView() override;

	void closeView() override;

	void setPresenter(IPresenterMain* pPresenter) override { presenter = pPresenter; }

	Gtk::Window* getWindow() override { return this; }

	void setTabs(const std::vector<PanedLayoutTab> &pPanedLayoutTabs) override;

	void openTab(int32_t pTabIndex) override;

	ui::MultiPaned* getMultiPanedByIndex(int32_t pIndex) override;

protected:
};
} // namespace mer::editor::mvp

#endif //MAINWINDOW_H
