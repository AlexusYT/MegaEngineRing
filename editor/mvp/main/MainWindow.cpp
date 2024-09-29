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

#include "Globals.h"
#include "IPresenterMain.h"
#include "mvp/contexts/IWidgetContext.h"
#include "project/Project.h"
#include "ui/customWidgets/multipaned/MultiPanedPanel.h"

namespace mer::editor::mvp {
std::shared_ptr<MainWindow> MainWindow::create(const std::shared_ptr<IWidgetContext> &pContext,
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

	auto viewMain = std::make_shared<MainWindow>(builder, pContext);
	if (!viewMain) {
		pReportMessage = sdk::utils::ReportMessage::create();
		pReportMessage->setTitle("Failed to init main window");
		pReportMessage->setMessage("Could not get the dialog");
		return nullptr;
	}
	pReportMessage = nullptr;
	return viewMain;
}

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Builder> &pBuilder, const std::shared_ptr<IWidgetContext> &pContext)
	: context(pContext) {


	set_size_request(800, 600);
	maximize();

	Gtk::HeaderBar* bar = pBuilder->get_widget<Gtk::HeaderBar>("header");
	bar->unparent();
	set_titlebar(*bar);
	/*auto* popoverMenu = builder->get_widget<Gtk::PopoverMenu>("popoverMenu");

	const auto menu = Gio::Menu::create();
	const auto fileSubmenu = Gio::Menu::create();
	const auto createSubmenu = Gio::Menu::create();

	const auto projectItem = Gio::MenuItem::create("dddddd", "app.CreateProject");

	createSubmenu->append_item(projectItem);
	fileSubmenu->append_submenu("Создать", createSubmenu);
	fileSubmenu->append("Открыть...");
	menu->append_submenu("Файл", fileSubmenu);
	popoverMenu->set_menu_model(menu);*/


	keyController = Gtk::EventControllerKey::create();
	add_controller(keyController);
	set_child(multiPaned);
	/*auto motionController = Gtk::EventControllerMotion::create();
	motionController->signal_motion().connect([this](double x, double y) {
		auto widget = multiPaned.pick(x, y, Gtk::PickFlags::INSENSITIVE | Gtk::PickFlags::NON_TARGETABLE);
		if (!widget) return;
		hoveredWidget = widget->get_ancestor(Gtk::Frame::get_base_type());
	});
	add_controller(motionController);*/

	auto actionGroupObject = Gio::SimpleActionGroup::create();
	actionGroupObject->add_action_with_parameter(
		"extension.remove", Glib::VARIANT_TYPE_UINT64, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<uintptr_t>>(pBase);
			auto ext = reinterpret_cast<sdk::main::Extension*>(var.get());
			if (presenter) presenter->removeExtension(ext);
		});
	actionGroupObject->add_action_with_parameter(
		"selected.extension.new", Glib::Variant<std::string>::variant_type(), [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<std::string>>(pBase);
			if (presenter) presenter->addExtension(var.get());
		});
	actionGroupObject->add_action_with_parameter(
		"manage.new.object", Glib::VARIANT_TYPE_UINT64, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<uintptr_t>>(pBase);
			ExplorerObject* object = reinterpret_cast<ExplorerObject*>(var.get());
			if (presenter) presenter->createObject(object);
		});
	actionGroupObject->add_action_with_parameter(
		"manage.remove.object", Glib::VARIANT_TYPE_UINT64, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<uintptr_t>>(pBase);
			ExplorerObject* object = reinterpret_cast<ExplorerObject*>(var.get());
			if (presenter) presenter->removeObject(object);
		});
	actionGroupObject->add_action_with_parameter(
		"open", Glib::VARIANT_TYPE_UINT64, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<uintptr_t>>(pBase);
			ExplorerObject* object = reinterpret_cast<ExplorerObject*>(var.get());
			if (presenter) presenter->selectObject(object);
		});
	insert_action_group("object", actionGroupObject);
	auto actionGroupFile = Gio::SimpleActionGroup::create();
	actionGroupFile->add_action_with_parameter(
		"open", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->openFile(var.get().raw());
		});
	actionGroupFile->add_action_with_parameter(
		"manage.new.scene", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createScene(var.get().raw());
		});
	actionGroupFile->add_action_with_parameter(
		"manage.new.script", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createScript(var.get().raw());
		});

	actionGroupFile->add_action_with_parameter(
		"manage.new.folder", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createDirectory(var.get().raw());
		});
	actionGroupFile->add_action_with_parameter(
		"manage.rename", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->renameFile(var.get().raw());
		});
	actionGroupFile->add_action_with_parameter(
		"manage.delete", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->deleteFile(var.get().raw());
		});
	actionGroupFile->add_action_with_parameter(
		"manage.showInFiles", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->showInFiles(var.get().raw());
		});
	insert_action_group("file", actionGroupFile);
	set_visible();
}

MainWindow::~MainWindow() {}

void MainWindow::openView() { context->addWidget(this); }

void MainWindow::closeView() { context->removeWidget(); }

ui::MultiPaned* MainWindow::getMultiPaned() { return &multiPaned;}

}