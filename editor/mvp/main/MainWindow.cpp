//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 30.10.2021.
//
#include "MainWindow.h"

#include "EngineSDK/main/resources/ResourceType.h"
#include "Globals.h"
#include "IPresenterMain.h"
#include "PanedLayoutTab.h"
#include "editors/sceneEditor/ResourcesContext.h"
#include "mvp/contexts/IWidgetContext.h"
#include "project/Project.h"

namespace mer::editor::mvp {
std::shared_ptr<MainWindow> MainWindow::create(const std::shared_ptr<IWidgetContext> &pContext,
											   sdk::utils::ReportMessagePtr &pReportMessage) {
	const auto builder = Gtk::Builder::create();
	try {
		builder->add_from_resource("/ui/base.ui");
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
	set_child(panedTabs);
	panedTabs.signal_switch_page().connect([this](Widget* /*pWidget*/, int32_t pIndex) {
		const auto overlay = dynamic_cast<Gtk::Overlay*>(panedTabs.get_nth_page(pIndex));
		if (!overlay) return;
		const auto overlayBox = dynamic_cast<Gtk::Box*>(overlay->get_last_child());
		if (!overlayBox) return;
		const auto paned = dynamic_cast<ui::MultiPaned*>(overlay->get_child());
		if (!paned) return;
		if (paned->get_first_child()) {
			overlayBox->set_visible(false);
		} else {
			overlayBox->set_visible(true);
			if (presenter)
				presenter->readJsonForTab(pIndex, [this, pIndex](const sdk::utils::ReportMessagePtr &pError) {
					const auto pageOverlay = dynamic_cast<Gtk::Overlay*>(panedTabs.get_nth_page(pIndex));
					if (!pageOverlay) return;
					const auto pageOverlayBox = dynamic_cast<Gtk::Box*>(pageOverlay->get_last_child());
					if (!pageOverlayBox) return;
					if (pError) {
						if (auto spinner = dynamic_cast<Gtk::Spinner*>(pageOverlayBox->get_first_child()))
							spinner->set_spinning(false);
						if (auto label = dynamic_cast<Gtk::Label*>(pageOverlayBox->get_last_child()))
							label->set_text(pError->getReport(false));
					} else {
						pageOverlayBox->set_visible(false);
					}
				});
		}
	});
	signal_realize().connect([this] {
		const auto surface = this->get_native()->get_surface();
		auto sharedContext = surface->create_gl_context();
		sharedContext->realize();
		resourcesContext = std::make_shared<ResourcesContext>(sharedContext);
	});
	/*auto motionController = Gtk::EventControllerMotion::create();
	motionController->signal_motion().connect([this](double x, double y) {
		auto widget = multiPaned.pick(x, y, Gtk::PickFlags::INSENSITIVE | Gtk::PickFlags::NON_TARGETABLE);
		if (!widget) return;
		hoveredWidget = widget->get_ancestor(Gtk::Frame::get_base_type());
	});
	add_controller(motionController);*/
	auto actionGroupResource = Gio::SimpleActionGroup::create();
	actionGroupResource->add_action_with_parameter(
		"select-for-property", Glib::VARIANT_TYPE_UINT64, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<uintptr_t>>(pBase);
			auto ext = reinterpret_cast<sdk::utils::PropertyBase*>(var.get());
			if (presenter) presenter->selectResourceForProperty(ext);
		});

	insert_action_group("resource", actionGroupResource);
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
	auto actionGroupNew = Gio::SimpleActionGroup::create();
	actionGroupNew->add_action_with_parameter(
		"resource.model", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createResource(var.get().raw(), sdk::main::ResourceType::MODEL);
		});
	actionGroupNew->add_action_with_parameter(
		"resource.texture", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createResource(var.get().raw(), sdk::main::ResourceType::TEXTURE);
		});
	actionGroupNew->add_action_with_parameter(
		"resource.material", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createResource(var.get().raw(), sdk::main::ResourceType::MATERIAL);
		});
	actionGroupNew->add_action_with_parameter(
		"scene", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createScene(var.get().raw());
		});
	actionGroupNew->add_action_with_parameter(
		"script", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createScript(var.get().raw());
		});

	actionGroupNew->add_action_with_parameter(
		"folder", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->createDirectory(var.get().raw());
		});
	insert_action_group("new", actionGroupNew);
	auto actionGroupFile = Gio::SimpleActionGroup::create();
	actionGroupFile->add_action_with_parameter(
		"open", Glib::VARIANT_TYPE_STRING, [this](const Glib::VariantBase &pBase) {
			const auto var = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(pBase);
			if (presenter) presenter->openFile(var.get().raw());
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
	signal_close_request().connect(
		[this]() {
			if (presenter) presenter->onViewClosed();
			return false;
		},
		false);
}

MainWindow::~MainWindow() { presenter = nullptr; }

void MainWindow::openView() { context->addWidget(this); }

void MainWindow::closeView() { context->removeWidget(); }

void MainWindow::setTabs(const std::vector<PanedLayoutTab> &pPanedLayoutTabs) {
	for (int32_t i = 0, maxI = panedTabs.get_n_pages(); i < maxI; i++) { panedTabs.remove_page(i); }
	for (auto panedLayoutTab: pPanedLayoutTabs) {
		const auto multiPanedTab = Gtk::make_managed<ui::MultiPaned>();
		multiPanedTab->connectCreateWidgetSignal(sigc::mem_fun(*presenter, &IPresenterMain::createView));
		Gtk::Overlay overlay;
		overlay.set_child(*multiPanedTab);
		Gtk::Box box(Gtk::Orientation::VERTICAL);
		box.set_valign(Gtk::Align::CENTER);
		box.set_halign(Gtk::Align::CENTER);
		Gtk::Spinner spinner;
		spinner.set_spinning();
		box.append(spinner);
		Gtk::Label label("Preparing the layout");
		label.set_margin(6);
		box.append(label);
		overlay.add_overlay(box);
		panedTabs.append_page(overlay, panedLayoutTab.getName());
	}
}

void MainWindow::openTab(const int32_t pTabIndex) { panedTabs.set_current_page(pTabIndex); }

ui::MultiPaned* MainWindow::getMultiPanedByIndex(const int32_t pIndex) {
	auto overlay = dynamic_cast<Gtk::Overlay*>(panedTabs.get_nth_page(pIndex));
	if (!overlay) return nullptr;
	auto paned = dynamic_cast<ui::MultiPaned*>(overlay->get_child());
	return paned;
}

}