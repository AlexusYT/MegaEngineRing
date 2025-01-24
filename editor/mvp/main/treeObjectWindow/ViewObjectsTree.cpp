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
// Created by alexus on 19.01.24.
//

#include "ViewObjectsTree.h"

#include <project/Project.h>

#include "mvp/contexts/IWidgetContext.h"
#include "mvp/main/editors/sceneEditor/explorerObjects/ExplorerObject.h"
#include "ui/customWidgets/CustomSignalListItemFactory.h"
#include "ui/customWidgets/CustomTreeView.h"

namespace mer::editor::mvp {

ViewObjectsTree::ViewObjectsTree(const std::shared_ptr<IWidgetContext> &pContext) : context(pContext) {
	tree = Gtk::make_managed<ui::CustomTreeView>();
	mainScrolledWindow.set_child(*tree);
	mainScrolledWindow.set_has_frame(false);
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

		const auto col = std::dynamic_pointer_cast<ExplorerObject>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		expander->set_hide_expander(false);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getName());
	});

	const auto column = Gtk::ColumnViewColumn::create("Name", factory);
	column->set_resizable(true);
	tree->append_column(column);
	tree->setSlotSelectionChanged([this](Glib::ObjectBase* pObjectBase) {
		auto element = dynamic_cast<ExplorerObject*>(pObjectBase);
		const auto variant = Glib::Variant<uintptr_t>::create(reinterpret_cast<uintptr_t>(element));
		tree->activate_action("object.open", variant);
	});
	auto mainMenu = Gio::Menu::create();

	const auto variant = Glib::Variant<uintptr_t>::create(reinterpret_cast<uintptr_t>(nullptr));
	mainMenu->append_item(createItem("New Object", "object.manage.new.object", variant));
	tree->setMenu(mainMenu);
}

void ViewObjectsTree::setTopLevelObjects(const std::shared_ptr<Gio::ListModel> &pTopLevelObjects) {
	tree->setSlotCreateModel(
		[this, pTopLevelObjects](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
			if (const auto col = std::dynamic_pointer_cast<ExplorerObject>(pItem)) return col->getChildren();
			return pTopLevelObjects;
		});
}

void ViewObjectsTree::openView() { context->addWidget(&mainScrolledWindow); }

void ViewObjectsTree::closeView() { context->removeWidget(); }

std::shared_ptr<Gio::MenuItem> ViewObjectsTree::createItem(const std::string &pName, const std::string &pAction,
														   const Glib::VariantBase &pVariant) {
	std::shared_ptr<Gio::MenuItem> item = Gio::MenuItem::create(pName, "");
	item->set_action_and_target(pAction, pVariant);
	return item;
}
} // namespace mer::editor::mvp