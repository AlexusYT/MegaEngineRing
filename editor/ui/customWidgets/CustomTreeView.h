//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

#ifndef CUSTOMTREEVIEW_H
#define CUSTOMTREEVIEW_H

namespace mer::editor::ui {
class TreeElementBase;

class CustomTreeView : public Gtk::ColumnView {

public:
	using SlotContextMenuRequested = sigc::slot<std::shared_ptr<Gio::Menu>(TreeElementBase* pObjectBase)>;
	using SlotSelectionChanged = sigc::slot<void(Glib::ObjectBase* pObjectBase)>;
	using SlotCreateModel = Gtk::TreeListModel::SlotCreateModel;

private:
	explicit CustomTreeView(ColumnView &&) = delete;

	explicit CustomTreeView(const ColumnView &) = delete;

	explicit CustomTreeView(const Glib::ConstructParams &construct_params) : ColumnView(construct_params) {}

	explicit CustomTreeView(GtkColumnView* const castitem) : ColumnView(castitem) {}

	Gtk::PopoverMenu popoverMenu;
	SlotCreateModel slotCreateModel;
	SlotSelectionChanged selectionChanged;
	bool selectOnDoubleClick{};
	std::shared_ptr<Gio::MenuModel> menu;

public:
	CustomTreeView();

	~CustomTreeView() override;

	[[nodiscard]] const SlotCreateModel &getSlotCreateModel() const { return slotCreateModel; }

	void setSlotCreateModel(const SlotCreateModel &pSlotCreateModel);

	Glib::RefPtr<Glib::ObjectBase> getSelectedItem() {

		const auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(get_model());
		auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(selection->get_selected_item());
		return row->get_item();
	}

	[[nodiscard]] const SlotSelectionChanged &getSlotSelectionChanged() const { return selectionChanged; }

	void setSlotSelectionChanged(const SlotSelectionChanged &pSelectionChanged) {
		selectionChanged = pSelectionChanged;
	}

	[[nodiscard]] bool isSelectOnDoubleClick() const { return selectOnDoubleClick; }

	void setSelectOnDoubleClick(const bool pSelectOnDoubleClick = true) { selectOnDoubleClick = pSelectOnDoubleClick; }

	void setMenu(const std::shared_ptr<Gio::MenuModel> &pMenu) { menu = pMenu; }

protected:
	void on_realize() override { ColumnView::on_realize(); }

	void size_allocate_vfunc(int pWidth, int pHeight, int pBaseline) override {
		popoverMenu.present();
		ColumnView::size_allocate_vfunc(pWidth, pHeight, pBaseline);
	}

	void onLeftClick(int pNPress, double pX, double pY);

	std::shared_ptr<Gio::MenuModel> getMenuAt(double pX, double pY);
};
} // namespace mer::editor::ui


#endif //CUSTOMTREEVIEW_H
