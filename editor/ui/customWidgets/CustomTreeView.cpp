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
// Created by alexus on 05.09.24.
//

#include "CustomTreeView.h"

#include "TreeElementBase.h"

namespace mer::editor::ui {

CustomTreeView::CustomTreeView() {
	signal_destroy().connect([this] { popoverMenu.unparent(); });
	popoverMenu.set_parent(*this);
	auto rightClick = Gtk::GestureClick::create();
	rightClick->set_button(GDK_BUTTON_SECONDARY);
	rightClick->signal_pressed().connect([this](int, const double pX, const double pY) {
		if (const auto elementMenu = getMenuAt(pX, pY)) {
			popoverMenu.set_menu_model(elementMenu);
		} else if (menu) {
			popoverMenu.set_menu_model(menu);
		} else
			return;
		const Gdk::Rectangle rect(static_cast<int>(pX), static_cast<int>(pY), 1, 1);
		popoverMenu.set_pointing_to(rect);
		popoverMenu.popup();
	});
	add_controller(rightClick);
	auto leftClick = Gtk::GestureClick::create();
	leftClick->set_button(GDK_BUTTON_PRIMARY);
	leftClick->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
	leftClick->signal_pressed().connect(sigc::mem_fun(*this, &CustomTreeView::onLeftClick));
	add_controller(leftClick);
}

CustomTreeView::~CustomTreeView() {
	//
	popoverMenu.unparent();
}

void CustomTreeView::setSlotCreateModel(const SlotCreateModel &pSlotCreateModel) {
	slotCreateModel = pSlotCreateModel;
	const auto root = slotCreateModel({});

	const auto treeListModel = Gtk::TreeListModel::create(root, slotCreateModel, false, false);

	const auto singleSelection = Gtk::SingleSelection::create(treeListModel);
	auto prop = singleSelection->property_selected_item();
	prop.signal_changed().connect([this, prop] {
		if (!selectOnDoubleClick) {
			auto selectedItem = prop.get_value();
			if (!selectedItem) return;
			auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(selectedItem);
			if (!row) return;
			auto element = std::dynamic_pointer_cast<TreeElementBase>(row->get_item());
			if (!element) return;
			selectionChanged(element.get());
		}
	});
	singleSelection->signal_selection_changed().connect([this](guint, guint) {
		//if (!selectOnDoubleClick) selectionChanged(getSelectedItem().get());
	});
	singleSelection->set_autoselect(false);
	singleSelection->set_can_unselect(true);
	singleSelection->set_selected(GTK_INVALID_LIST_POSITION);
	set_model(singleSelection);
}

void CustomTreeView::unselect() { get_model()->unselect_all(); }

void CustomTreeView::onLeftClick(int pNPress, double pX, double pY) {
	if (pNPress == 2) {

		const auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(get_model());
		if (!selection) return;
		auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(selection->get_selected_item());
		if (!row) return;
		auto selectedElement = std::dynamic_pointer_cast<TreeElementBase>(row->get_item());
		if (!selectedElement) return;
		auto elementAtPos = getElementAt(pX, pY);
		if (elementAtPos != selectedElement.get()) return;
		if (row->is_expandable()) { row->set_expanded(!row->get_expanded()); }

		if (selectOnDoubleClick) { selectionChanged(elementAtPos); }
	}
}

TreeElementBase* CustomTreeView::getElementAt(double pX, double pY) {

	const auto child = this->pick(pX, pY, Gtk::PickFlags::INSENSITIVE | Gtk::PickFlags::NON_TARGETABLE);
	if (!child) return nullptr;
	const auto cell = child->get_ancestor(g_type_from_name("GtkColumnViewCellWidget"));
	if (!cell) return nullptr;
	auto widget = cell->get_first_child();
	if (!widget) return nullptr;
	void* data{};
	/*if (auto expander = dynamic_cast<Gtk::TreeExpander*>(widget)) {
		auto expChild = expander->get_child();
		if (!expChild) return nullptr;
		data = expChild->get_data("contextSelectedElement");
	} else*/
	data = widget->get_data("contextSelectedElement");
	if (!data) return nullptr;
	const auto element = static_cast<TreeElementBase*>(data);
	return element;
}

std::shared_ptr<Gio::MenuModel> CustomTreeView::getMenuAt(const double pX, const double pY) {

	const auto data = getElementAt(pX, pY);
	if (!data) return nullptr;
	return data->getMenu();
}

} // namespace mer::editor::ui