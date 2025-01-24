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
// Created by alexus on 09.01.24.
//

#ifndef OBJECTTREE_H
#define OBJECTTREE_H
#include <giomm/menu.h>
#include <gtkmm/columnview.h>
#include <gtkmm/gesture.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/singleselection.h>
#include <gtkmm/treelistmodel.h>
#include <ui/utils/ErrorDialog.h>

namespace mer::editor::ui {

class TreeWidget : public Gtk::ColumnView {
public:
	using SlotContextMenuRequested = sigc::slot<Glib::RefPtr<Gio::Menu>(Glib::ObjectBase* pObjectBase)>;
	using SlotSelectionChanged = sigc::slot<void(Glib::ObjectBase* pObjectBase)>;
	using SlotCreateModel = Gtk::TreeListModel::SlotCreateModel;

private:
	explicit TreeWidget(ColumnView &&) = delete;

	explicit TreeWidget(const ColumnView &) = delete;

	explicit TreeWidget(const Glib::ConstructParams &construct_params) : ColumnView(construct_params) {}

	explicit TreeWidget(GtkColumnView* const castitem) : ColumnView(castitem) {}

	Gtk::PopoverMenu popoverMenu;
	SlotCreateModel slotCreateModel;
	SlotContextMenuRequested contextMenuRequested;
	SlotSelectionChanged selectionChanged;
	bool selectOnDoubleClick{};

public:
	TreeWidget();

	[[nodiscard]] const SlotCreateModel &getSlotCreateModel() const { return slotCreateModel; }

	void setSlotCreateModel(const SlotCreateModel &pSlotCreateModel);

	void append_column(const Glib::RefPtr<Gtk::ColumnViewColumn> &column) {
		auto factory = std::dynamic_pointer_cast<Gtk::SignalListItemFactory>(column->get_factory());

		factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
			const auto leftClickGesture = Gtk::GestureClick::create();
			leftClickGesture->set_button(GDK_BUTTON_PRIMARY);
			pListItem->get_child()->add_controller(leftClickGesture);
			leftClickGesture->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
			leftClickGesture->signal_pressed().connect(
				sigc::bind(sigc::mem_fun(*this, &TreeWidget::onLeftClick), leftClickGesture));

			const auto rightClickGesture = Gtk::GestureClick::create();
			rightClickGesture->set_button(GDK_BUTTON_SECONDARY);
			rightClickGesture->signal_pressed().connect(
				sigc::bind(sigc::mem_fun(*this, &TreeWidget::onRightClick), rightClickGesture));
			pListItem->get_child()->add_controller(rightClickGesture);
		});
		factory->signal_bind().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
			auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
			pListItem->get_child()->set_data("contextSelectedObject", row->get_item().get());
		});
		factory->signal_unbind().connect(
			[this](const Glib::RefPtr<Gtk::ListItem> & /*pListItem*/) { popoverMenu.unparent(); });
		ColumnView::append_column(column);
	}

	Glib::RefPtr<Glib::ObjectBase> getSelectedItem() {

		const auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(get_model());
		auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(selection->get_selected_item());
		return row->get_item();
	}

	Glib::ObjectBase* getContextSelectedObject() {
		auto* widget = popoverMenu.get_parent();
		auto* data = widget->get_data("contextSelectedObject");
		if (!data) {
			auto msg = sdk::ReportMessage::create();
			msg->setTitle("Failed to get context selected object");
			msg->setMessage("Widget has no data named contextSelectedObject");
			msg->addInfoLine("This is an error in the TreeWidget logic");

			ErrorDialog::showErrorDialog(get_root(), msg);
			return nullptr;
		}

		return static_cast<Glib::ObjectBase*>(data);
	}

	[[nodiscard]] const SlotContextMenuRequested &getContextMenuRequested() const { return contextMenuRequested; }

	void setContextMenuRequested(const SlotContextMenuRequested &pContextMenuRequested) {
		contextMenuRequested = pContextMenuRequested;
	}

	[[nodiscard]] const SlotSelectionChanged &getSlotSelectionChanged() const { return selectionChanged; }

	void setSlotSelectionChanged(const SlotSelectionChanged &pSelectionChanged) {
		selectionChanged = pSelectionChanged;
	}

	[[nodiscard]] bool isSelectOnDoubleClick() const { return selectOnDoubleClick; }

	void setSelectOnDoubleClick(const bool pSelectOnDoubleClick = true) { selectOnDoubleClick = pSelectOnDoubleClick; }


protected:
	void on_realize() override { ColumnView::on_realize(); }

	void size_allocate_vfunc(int pWidth, int pHeight, int pBaseline) override {
		popoverMenu.present();
		ColumnView::size_allocate_vfunc(pWidth, pHeight, pBaseline);
	}

	void onRightClick(int pNPress, double x, double y, const Glib::RefPtr<Gtk::GestureClick> &);

	void onLeftClick(int pNPress, double x, double y, const Glib::RefPtr<Gtk::GestureClick> &);
};

} // namespace mer::editor::ui

#endif //OBJECTTREE_H
