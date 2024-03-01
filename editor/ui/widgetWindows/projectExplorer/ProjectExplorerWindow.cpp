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

#include "ProjectExplorerWindow.h"

#include <project/Project.h>

#include "ProjectExplorerEntry.h"

namespace mer::editor::ui {
ProjectExplorerWindow::ProjectExplorerWindow(project::Project* pProject) : project(pProject) {
	mainScrolledWindow.set_child(tree);
	const auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		// Each ListItem contains a TreeExpander, which contains a Label.
		// The Label shows the ModelColumns::m_col_id. That's done in on_bind_id().
		auto* expander = Gtk::make_managed<Gtk::TreeExpander>();
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		expander->set_child(*label);
		pListItem->set_child(*expander);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		// When TreeListModel::property_passthrough() is false, ListItem::get_item()
		// is a TreeListRow. TreeExpander needs the TreeListRow.
		// The ModelColumns item is returned by TreeListRow::get_item().
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;

		const auto col = std::dynamic_pointer_cast<ProjectExplorerEntry>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		expander->set_hide_expander(false);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getName());
	});

	tree.setContextMenuRequested([](Glib::ObjectBase* pObjectBase) -> Glib::RefPtr<Gio::Menu> {
		auto entry = dynamic_cast<ProjectExplorerEntry*>(pObjectBase);
		if (!entry) return nullptr;
		Glib::RefPtr<Gio::Menu> menu = entry->getContextMenu();
		if (!menu) return nullptr;
		return menu;
	});
	const auto column = Gtk::ColumnViewColumn::create("", factory);
	column->set_resizable(true);
	tree.append_column(column);
	tree.setSelectOnDoubleClick();
	tree.setSlotCreateModel([this](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
		auto col = std::dynamic_pointer_cast<ProjectExplorerEntry>(pItem);
		Glib::RefPtr<Gio::ListStore<ProjectExplorerEntry>> result;
		if (col) {
			result = col->getEntryChildren();
		} else {
			result = project->getProjectExplorerEntries();
		}
		// If result is empty, it's a leaf in the tree, i.e. an item without children.
		// Returning an empty RefPtr (not a RefPtr with an empty Gio::ListModel)
		// signals that the item is not expandable.
		return result;
		//	return (result->get_n_items() > 0) ? result : Glib::RefPtr<Gio::ListModel>();
	});
	tree.setSlotSelectionChanged([this](Glib::ObjectBase* pObjectBase) {
		if (entrySelectionChanged) entrySelectionChanged(dynamic_cast<ProjectExplorerEntry*>(pObjectBase));
	});
}
} // namespace mer::editor::ui