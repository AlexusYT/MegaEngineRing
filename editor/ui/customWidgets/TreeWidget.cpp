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
// Created by alexus on 09.01.24.
//

#include "TreeWidget.h"

#include <ui/utils/ErrorDialog.h>

namespace mer::editor::ui {
TreeWidget::TreeWidget() {

	/*Glib::RefPtr<Gtk::GestureClick> refGesture = Gtk::GestureClick::create();
	refGesture->set_button(GDK_BUTTON_SECONDARY);
	refGesture->signal_pressed().connect([this](int, double x, double y) {
		const Gdk::Rectangle rect(x, y, 1, 1);
		popoverMenu.set_pointing_to(rect);
		popoverMenu.popup();
	});
	add_controller(refGesture);*/
}

void TreeWidget::setSlotCreateModel(const SlotCreateModel &pSlotCreateModel) {
	slotCreateModel = pSlotCreateModel;
	const auto root = slotCreateModel({});

	// Set list model and selection model.
	// passthrough must be false when Gtk::TreeExpander is used in the view.
	const auto treeListModel = Gtk::TreeListModel::create(root, slotCreateModel, false, false);

	const auto singleSelection = Gtk::SingleSelection::create(treeListModel);
	singleSelection->signal_selection_changed().connect([this](guint, guint) {
		if (!selectOnDoubleClick) selectionChanged(getSelectedItem().get());
	});
	singleSelection->set_autoselect(false);
	singleSelection->set_can_unselect(true);
	set_model(singleSelection);
}

void TreeWidget::onRightClick(int /*pNPress*/, double x, double y,
							  const Glib::RefPtr<Gtk::GestureClick> &pGestureClick) {
	auto* widget = pGestureClick->get_widget();
	auto* data = widget->get_data("contextSelectedObject");
	if (!data) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to get context selected object");
		msg->setMessage("Widget has no data named contextSelectedObject");
		msg->addInfoLine("This is an error in the TreeWidget logic");

		ErrorDialog::showErrorDialog(get_root(), msg);
		return;
	}

	const auto menu = contextMenuRequested(static_cast<Glib::ObjectBase*>(data));
	if (!menu) return;

	popoverMenu.set_menu_model(menu);
	const Gdk::Rectangle rect(static_cast<int>(x), static_cast<int>(y), 1, 1);
	if (popoverMenu.get_parent() != widget) {
		//popoverMenu.unparent();
		popoverMenu.set_parent(*widget);
	}
	popoverMenu.set_pointing_to(rect);
	popoverMenu.popup();
}

void TreeWidget::onLeftClick(int pNPress, double /*x*/, double /*y*/,
							 const Glib::RefPtr<Gtk::GestureClick> & /*pGestureClick*/) {
	if (pNPress == 2) {

		const auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(get_model());
		auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(selection->get_selected_item());
		if (row->is_expandable()) { row->set_expanded(!row->get_expanded()); }
		if (selectOnDoubleClick) { selectionChanged(getSelectedItem().get()); }
	}
}

} // namespace mer::editor::ui