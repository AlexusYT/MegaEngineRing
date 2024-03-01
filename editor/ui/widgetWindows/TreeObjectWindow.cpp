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
// Created by alexus on 10.01.24.
//

#include "TreeObjectWindow.h"

#include <project/sceneObjects/World.h>
#include <ui/utils/ErrorDialog.h>

namespace mer::editor::ui {
TreeObjectWindow::TreeObjectWindow() {

	mainBox.set_expand(true);
	mainBox.set_orientation(Gtk::Orientation::VERTICAL);
	{
		{
			addBtn.set_tooltip_text("Добавить элемент");
			addBtn.set_icon_name("list-add");
			//addBtn.set_action_name("treeObjectWindow.add");
			addBtn.signal_clicked().connect([this] {
				Gtk::PopoverMenu menu;
				menu.set_menu_model(createAddContextMenu());
				menu.set_parent(addBtn);
				menu.popup();
			});
			buttonBox.append(addBtn);
		}
		{
			removeBtn.set_tooltip_text("Удалить элемент");
			removeBtn.set_icon_name("list-remove");
			buttonBox.append(removeBtn);
		}
		mainBox.append(buttonBox);
	}

	{
		Gtk::ScrolledWindow scrolledWindow;
		{

			auto factory = Gtk::SignalListItemFactory::create();
			factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
				// Each ListItem contains a TreeExpander, which contains a Label.
				// The Label shows the ModelColumns::m_col_id. That's done in on_bind_id().
				auto expander = Gtk::make_managed<Gtk::TreeExpander>();
				auto label = Gtk::make_managed<Gtk::Label>();
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

				const auto col = std::dynamic_pointer_cast<SceneObject>(row->get_item());
				if (!col) return;
				auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
				if (!expander) return;
				expander->set_data(Glib::Quark("treeObject"), col.get());
				expander->set_list_row(row);
				auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
				if (!label) return;
				label->set_text(col->getName());
			});
			auto column = Gtk::ColumnViewColumn::create("Имя", factory);
			column->set_resizable(true);
			tree.append_column(column);

			// Name column
			factory = Gtk::SignalListItemFactory::create();
			factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
				auto* label = Gtk::make_managed<Gtk::Label>("", Gtk::Align::FILL);
				label->set_margin(3);
				pListItem->set_child(*label);
			});
			factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
				auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
				if (!row) return;
				auto col = std::dynamic_pointer_cast<SceneObject>(row->get_item());
				if (!col) return;
				auto label = dynamic_cast<Gtk::Label*>(pListItem->get_child());
				if (!label) return;
				label->set_text(col->getType());
				label->set_data(Glib::Quark("treeObject"), col.get());
			});

			column = Gtk::ColumnViewColumn::create("Тип", factory);
			column->set_resizable(true);

			tree.append_column(column);

			tree.setContextMenuRequested([](Glib::ObjectBase* pObjectBase) {
				SceneObject* object = dynamic_cast<SceneObject*>(pObjectBase);
				auto menu = Gio::Menu::create();
				const auto section = Gio::Menu::create();
				menu->append_section(section);
				section->append_submenu("Добавить", createAddContextMenu());
				if (dynamic_cast<World*>(object)) {
					auto item = Gio::MenuItem::create("Удалить", "<disabled>");
					section->append_item(item);
				} else {
					auto item = Gio::MenuItem::create("Удалить", "treeObjectWindow.context.remove");
					section->append_item(item);
				}

				return menu;
			});

			tree.setSlotSelectionChanged([this](Glib::ObjectBase* pObjectBase) {
				removeBtn.set_action_name("treeObjectWindow.remove");
				auto* object = dynamic_cast<SceneObject*>(pObjectBase);

				removeBtn.set_sensitive(!dynamic_cast<World*>(object));
				sdk::utils::Logger::out("Object {} selected", object->getName());
			});

			const Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup = Gio::SimpleActionGroup::create();
			auto onSelect = [this]() {
				/*auto* widget = tree.getPopoverMenu().get_parent();
				auto* data = widget->get_data("treeObject");
				if (data) {
					const SceneObject* object = static_cast<SceneObject*>(data);
					std::string s = object->getName();
					engine::utils::Logger::out("Object selected: {}", s);
				}*/
			};


			refActionGroup->add_action("add", [this] {

			});
			refActionGroup->add_action("remove", [this] { removeObject(getSelectedObject()); });
			refActionGroup->add_action("context.add", onSelect);
			refActionGroup->add_action("context.remove", [this]() {
				//
				removeObject(getPopoverObject());
			});
			//refActionGroup->add_action("context.remove", [this] { removeObject(getPopoverObject()); });


			mainBox.insert_action_group("treeObjectWindow", refActionGroup);
			scrolledWindow.set_child(tree);
		}
		scrolledWindow.set_expand(true);
		mainBox.append(scrolledWindow);
		removeBtn.set_sensitive(false);
	}
}

void TreeObjectWindow::setSlotGetObjectChildren(const SlotGetObjectChildren &pSlotGetObjectChildren) {
	slotGetObjectChildren = pSlotGetObjectChildren;
	tree.setSlotCreateModel([this](const Glib::RefPtr<Glib::ObjectBase> &pItem) {
		auto col = std::dynamic_pointer_cast<SceneObject>(pItem);
		auto result = Gio::ListStore<SceneObject>::create();
		if (!col) {
			for (auto &child: slotGetObjectChildren(nullptr)) { result->append(child); }

		} else {
			if (col) { result = col->getChildrenUi(); }
		}

		// If result is empty, it's a leaf in the tree, i.e. an item without children.
		// Returning an empty RefPtr (not a RefPtr with an empty Gio::ListModel)
		// signals that the item is not expandable.
		return (result->get_n_items() > 0) ? result : Glib::RefPtr<Gio::ListModel>();
	});
}

void TreeObjectWindow::removeObject(SceneObject* pObject) {
	if (!pObject) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to remove an object");
		msg->setMessage("Object is nullptr. Most likely there is an error in the TreeWidget code");
		ErrorDialog::showErrorDialog(tree.get_root(), msg);
		return;
	}
	if (dynamic_cast<World*>(pObject)) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to remove an object");
		msg->setMessage("Removing the top-level object is not permitted by architecture");
		msg->addInfoLine("Object name: {}", pObject->getName());
		msg->addInfoLine("Object uuid: {}", pObject->getUuid());
		ErrorDialog::showErrorDialog(tree.get_root(), msg);
		return;
	}
	auto* parent = pObject->getParent();
	if (!parent) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to remove an object");
		msg->setMessage("Object is orphan. Most likely there is an error in the SceneObject code");
		ErrorDialog::showErrorDialog(tree.get_root(), msg);
		return;
	}

	if (!parent->removeChild(pObject->getUuid())) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to remove an object");
		msg->setMessage(
			"Parent object has no the removing object. Most likely there is an error in the SceneObject code");
		ErrorDialog::showErrorDialog(tree.get_root(), msg);
		return;
	}

	sdk::utils::Logger::out("Removing {}", pObject->getName());
}

SceneObject* TreeObjectWindow::getSelectedObject() {
	auto* objColumn = std::dynamic_pointer_cast<SceneObject>(tree.getSelectedItem()).get();
	if (!objColumn) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to get selected object");
		msg->setMessage("TreeView selected object is not the TreeObjectColumns*");

		ErrorDialog::showErrorDialog(tree.get_root(), msg);
		return nullptr;
	}

	return objColumn;
}

SceneObject* TreeObjectWindow::getPopoverObject() {

	auto columns = dynamic_cast<SceneObject*>(tree.getContextSelectedObject());

	return columns;
}

Glib::RefPtr<Gio::Menu> TreeObjectWindow::createContextMenu() {
	auto menu = Gio::Menu::create();
	const auto section = Gio::Menu::create();
	menu->append_section(section);
	section->append_submenu("Добавить", createAddContextMenu());
	section->append_item(Gio::MenuItem::create("Удалить", "treeObjectWindow.context.remove"));
	return menu;
}

Glib::RefPtr<Gio::Menu> TreeObjectWindow::createAddContextMenu() {
	auto menu = Gio::Menu::create();
	const auto section = Gio::Menu::create();
	menu->append_section(section);
	section->append_item(Gio::MenuItem::create("Простой меш", "treeObjectWindow.context.add"));
	return menu;
}

}