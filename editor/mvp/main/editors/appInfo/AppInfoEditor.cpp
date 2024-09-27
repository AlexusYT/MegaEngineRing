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
// Created by alexus on 29.01.24.
//

#include "AppInfoEditor.h"

#include <project/generatedFiles/ApplicationInfo.h>

#include "mvp/main/centerWindow/IPresenterCenterWindow.h"
#include "mvp/main/centerWindow/TabPlaceholder.h"

namespace mer::editor::mvp {
AppInfoEditor::AppInfoEditor(const std::shared_ptr<TabPlaceholder> &pPlaceholder) : placeholder(pPlaceholder) {
	Gtk::ScrolledWindow scrolledWindow;
	scrolledWindow.set_child(tree);
	scrolledWindow.set_expand();
	main.append(scrolledWindow);
	main.set_expand();
	auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
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

		const auto col = std::dynamic_pointer_cast<project::AppInfoProperty>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		expander->set_hide_expander(false);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getName());
	});
	auto column = Gtk::ColumnViewColumn::create("Название", factory);
	column->set_resizable(true);
	tree.append_column(column);

	factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* entry = Gtk::make_managed<Gtk::Entry>();
		entry->set_halign(Gtk::Align::END);
		entry->set_margin(3);
		pListItem->set_child(*entry);
	});
	factory->signal_unbind().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		if (const auto iter = connections.find(pListItem); iter != connections.end()) { iter->second.disconnect(); }
	});
	factory->signal_bind().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;

		const auto col = std::dynamic_pointer_cast<project::AppInfoProperty>(row->get_item());
		if (!col) return;
		auto* entry = dynamic_cast<Gtk::Entry*>(pListItem->get_child());
		if (!entry) return;
		entry->set_text(col->getValue());
		auto connection = entry->signal_changed().connect(
			sigc::bind([this](const std::shared_ptr<project::AppInfoProperty> &pInfoProperty,
							  Gtk::Entry* pEntry) { valueChangedSlot(pInfoProperty, pEntry->get_text()); },
					   col, entry));
		connections.emplace(pListItem, connection);
	});
	column = Gtk::ColumnViewColumn::create("Значение", factory);
	column->set_resizable(true);
	tree.append_column(column);
	tree.setSelectOnDoubleClick();

	/*tree.setContextMenuRequested([](Glib::ObjectBase* pObjectBase) -> Glib::RefPtr<Gio::Menu> {
		auto entry = dynamic_cast<project::AppInfoProperty*>(pObjectBase);
		const Glib::RefPtr<Gio::Menu> &menu = entry->getContextMenu();
		if (!menu) return nullptr;
		return menu;
	});*/

	/*std::map s(applicationInfo->properties.begin(), applicationInfo->properties.end());

	for (int row = 0; const auto &[fst, snd]: s) {
		Gtk::Label label;
		Gtk::Entry entry;
		label.set_text(fst);
		entry.set_text(snd);
		grid.insert_row(row);
		grid.attach(label, 1, row);
		grid.attach(entry, 2, row);
		row++;
	}*/
}

void AppInfoEditor::closeView() { placeholder->removeWidget(); }

void AppInfoEditor::openView() {
	placeholder->addWidget(&main);
	placeholder->setTitle("Настройки приложения");
}

void AppInfoEditor::setSlotCreateModel(const ui::TreeWidget::SlotCreateModel &pSlotCreateModel) {
	tree.setSlotCreateModel(pSlotCreateModel);
}
} // namespace mer::editor::mvp