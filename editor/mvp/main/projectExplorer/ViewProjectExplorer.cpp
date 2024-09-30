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
// Created by alexus on 19.01.24.
//

#include "ViewProjectExplorer.h"

#include <project/Project.h>

#include "entries/ProjectExplorerElement.h"
#include "mvp/contexts/IWidgetContext.h"
#include "ui/customWidgets/CustomSignalListItemFactory.h"
#include "ui/customWidgets/CustomTreeView.h"

namespace mer::editor::mvp {
ViewProjectExplorer::ViewProjectExplorer(const std::shared_ptr<IWidgetContext> &pContext) : context(pContext) {

	treeView = Gtk::make_managed<ui::CustomTreeView>();
	mainScrolledWindow.set_child(*treeView);
	mainScrolledWindow.set_has_frame(true);
	treeView->setSelectOnDoubleClick();
	addNameColumn();
	addTypeColumn();
	treeView->setSlotSelectionChanged([this](Glib::ObjectBase* pObjectBase) {
		const auto element = dynamic_cast<ProjectExplorerElement*>(pObjectBase);
		if (!element) return;
		auto str = element->getPath().string();
		treeView->activate_action("file.open", Glib::Variant<Glib::ustring>::create(str));
	});
}

void ViewProjectExplorer::setSlotCreateModel(
	const sigc::slot<std::shared_ptr<Gio::ListModel>(const std::shared_ptr<Glib::ObjectBase> &)> &pSlot) {
	treeView->setSlotCreateModel(pSlot);
}

void ViewProjectExplorer::openView() { context->addWidget(&mainScrolledWindow); }

void ViewProjectExplorer::closeView() { context->removeWidget(); }

void ViewProjectExplorer::addNameColumn() const {

	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* expander = Gtk::make_managed<Gtk::TreeExpander>();
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		expander->set_child(*label);
		pListItem->set_child(*expander);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;

		const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getName());
	});

	const auto column = Gtk::ColumnViewColumn::create("Name", factory);
	column->set_resizable(true);
	treeView->append_column(column);
}

void ViewProjectExplorer::addTypeColumn() const {

	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		pListItem->set_child(*label);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;
		const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(row->get_item());
		if (!col) return;
		auto* label = dynamic_cast<Gtk::Label*>(pListItem->get_child());
		if (!label) return;
		if (col->isDirectory()) label->set_text("Folder");
		else if (col->isScene())
			label->set_text("Scene");
		else
			label->set_text("");
	});

	const auto column = Gtk::ColumnViewColumn::create("Type", factory);
	column->set_resizable(true);
	treeView->append_column(column);
}

void ViewProjectExplorer::onPathChanged(const std::filesystem::path &pPath) const {
	auto menu = Gio::Menu::create();
	const auto variant = Glib::Variant<Glib::ustring>::create(pPath.string());
	const auto menuNew = Gio::Menu::create();
	menuNew->append_item(createItem("Resource", "file.manage.new.resource", variant));
	menuNew->append_item(createItem("Scene", "file.manage.new.scene", variant));
	menuNew->append_item(createItem("Script", "file.manage.new.script", variant));
	const auto menuManage = Gio::Menu::create();
	menuManage->append_item(createItem("New folder", "file.manage.new.folder", variant));
	menuManage->append_submenu("New", menuNew);
	menu->append_section(menuManage);
	treeView->setMenu(menu);
}

std::shared_ptr<Gio::MenuItem> ViewProjectExplorer::createItem(const std::string &pName, const std::string &pAction,
															   const Glib::Variant<Glib::ustring> &pVariant) {

	std::shared_ptr<Gio::MenuItem> item = Gio::MenuItem::create(pName, "");
	item->set_action_and_target(pAction, pVariant);
	return item;
}
} // namespace mer::editor::mvp