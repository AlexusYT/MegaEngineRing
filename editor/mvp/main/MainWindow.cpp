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
#include "MainWindow.h"

#include <ui/widgetWindows/CenterWindow.h>
#include <ui/widgetWindows/projectExplorer/ProjectExplorerWindow.h>

#include "Globals.h"
#include "graphic/viewport/OpenGL.h"
#include "project/Project.h"

namespace mer::editor::mvp {
std::shared_ptr<MainWindow> MainWindow::create(const std::shared_ptr<project::Project> &pProject,
											   sdk::utils::ReportMessagePtr &pReportMessage) {
	const auto builder = Gtk::Builder::create();
	try {
		builder->add_from_file(Globals::getResourcesPath() / "base.ui");
	} catch (...) {
		pReportMessage = sdk::utils::ReportMessage::create();
		pReportMessage->setTitle("Failed to init main window");
		pReportMessage->setMessage("Error while loading UI from file");

		return nullptr;
	}

	auto viewMain =
		std::shared_ptr<MainWindow>(Gtk::Builder::get_widget_derived<MainWindow>(builder, "MainWindow", pProject));
	if (!viewMain) {
		pReportMessage = sdk::utils::ReportMessage::create();
		pReportMessage->setTitle("Failed to init main window");
		pReportMessage->setMessage("Could not get the dialog");
		return nullptr;
	}
	pReportMessage = nullptr;
	return viewMain;
}

MainWindow::MainWindow(BaseObjectType* pCobject, const Glib::RefPtr<Gtk::Builder> &pBuilder,
					   std::shared_ptr<project::Project> pProject)
	: Window(pCobject), project(std::move(pProject)), projectExplorerWindow{(project.get())}, builder(pBuilder),
	  notebook(nullptr) {
	set_visible();

	/*auto* area = builder->get_widget<Gtk::GLArea>("OpenGLRenderer");
	render = OpenGL::create(area);
	render->init();*/

	notebook = builder->get_widget<Gtk::Notebook>("noteboook_logs");
	notebook->append_page(logs[0], "CMake");
	notebook->append_page(logs[1], "Сборка");
	notebook->append_page(logs[2], "Запуск");

	auto* popoverMenu = builder->get_widget<Gtk::PopoverMenu>("popoverMenu");

	const auto menu = Gio::Menu::create();
	const auto fileSubmenu = Gio::Menu::create();
	const auto createSubmenu = Gio::Menu::create();

	const auto projectItem = Gio::MenuItem::create("dddddd", "app.CreateProject");

	/*mainWindow->get_application()
		->add_action("CreateProject")
		->signal_activate()
		.connect([this](const Glib::VariantBase &variantBase) {
			auto* window = createProjectWindow.get();
			window->mainWindow = this;
			window->window->set_transient_for(*this->mainWindow);
			window->window->show();
		});*/

	createSubmenu->append_item(projectItem);
	fileSubmenu->append_submenu("Создать", createSubmenu);
	fileSubmenu->append("Открыть...");
	menu->append_submenu("Файл", fileSubmenu);
	popoverMenu->set_menu_model(menu);

	auto* topPaned = builder->get_widget<Gtk::Paned>("paned_top");
	topPaned->set_end_child(centerWindow);

	auto* leftTopNotebook = builder->get_widget<Gtk::Notebook>("notebook_leftTop");
	leftTopNotebook->append_page(projectExplorerWindow, "Структура проекта");
	projectExplorerWindow.setEntrySelectionChanged(
		[this](ui::ProjectExplorerEntry* pEntry) { centerWindow.openEntry(pEntry); });


	keyController = Gtk::EventControllerKey::create();
	add_controller(keyController);
}

MainWindow::~MainWindow() {}

}