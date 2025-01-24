//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 11.09.24.
//

#include "CustomSignalListItemFactory.h"

#include <gtkmm/treeexpander.h>
#include <gtkmm/treelistrow.h>

#include "TreeElementBase.h"

namespace mer::editor::ui {
CustomSignalListItemFactory::CustomSignalListItemFactory() {
	signal_bind().connect([this](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pItem->get_item());
		if (!row) return;
		const auto data = std::dynamic_pointer_cast<TreeElementBase>(row->get_item());
		if (!data) return;

		const auto widget = pItem->get_child();
		widget->set_data("contextSelectedElement", data.get());
		auto expander = dynamic_cast<Gtk::TreeExpander*>(widget);
		if (!expander) return;
		data->setOnShowExpanderSlot([expander](const bool pShow) { expander->set_hide_expander(!pShow); });
	});
	signal_unbind().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto widget = pItem->get_child();
		widget->remove_data("contextSelectedElement");
	});
}

std::shared_ptr<CustomSignalListItemFactory> CustomSignalListItemFactory::create() {
	return make_refptr_for_instance(new CustomSignalListItemFactory());
}
} // namespace mer::editor::ui
