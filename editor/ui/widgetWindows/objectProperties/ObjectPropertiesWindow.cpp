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
// Created by alexus on 17.01.24.
//

#include "ObjectPropertiesWindow.h"

#include "ObjectPropertyEntry.h"

namespace mer::editor::ui {
ObjectPropertiesWindow::ObjectPropertiesWindow() {
	Gtk::ScrolledWindow propertiesScrolledWindow;
	propertiesScrolledWindow.set_child(propertiesTree);
	mainPaned.set_start_child(propertiesScrolledWindow);
	mainPaned.set_resize_start_child(true);
	mainPaned.set_shrink_start_child(false);
	Gtk::ScrolledWindow descScrolledWindow;

	descScrolledWindow.set_size_request(150, 100);
	descScrolledWindow.set_child(descriptionLabel);
	mainPaned.set_end_child(descScrolledWindow);
	mainPaned.set_resize_end_child(false);
	mainPaned.set_shrink_end_child(false);
	mainPaned.set_orientation(Gtk::Orientation::VERTICAL);
	mainPaned.set_wide_handle();

	descriptionLabel.set_text("Описание свойства");
	descriptionLabel.set_wrap();
	descriptionLabel.set_wrap_mode(Pango::WrapMode::WORD);
	descriptionLabel.set_valign(Gtk::Align::START);
	descriptionLabel.set_halign(Gtk::Align::START);

	const auto nameColumn = createNameColumn();
	nameColumn->set_resizable(true);
	propertiesTree.append_column(nameColumn);
	const auto valueColumn = createValueColumn();
	valueColumn->set_resizable(true);
	propertiesTree.append_column(valueColumn);

	propertiesTree.setContextMenuRequested([](Glib::ObjectBase* /*pObjectBase*/) -> Glib::RefPtr<Gio::Menu> {
		/*auto entry = dynamic_cast<ObjectPropertyEntryBase*>(pObjectBase);
		if (!entry) return nullptr;
		Glib::RefPtr<Gio::Menu> menu = entry->getContextMenu();
		if (!menu) return nullptr;
		return menu;*/
		return nullptr;
	});

	propertiesTree.setSlotSelectionChanged([this](Glib::ObjectBase* pObjectBase) {
		if (auto entry = dynamic_cast<ObjectPropertyEntry*>(pObjectBase)) {
			auto property = entry->getNativeProperty();
			descriptionLabel.set_label(property->getDescription());
		}
		//if (entrySelectionChanged) entrySelectionChanged(dynamic_cast<ObjectPropertyEntryBase*>(pObjectBase));
	});
}

void ObjectPropertiesWindow::setEntries(const std::shared_ptr<Gio::ListStore<ObjectPropertyEntry>> &pEntries) {
	propertiesTree.setSlotCreateModel([this, pEntries](
										  const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
		auto col = std::dynamic_pointer_cast<ObjectPropertyEntry>(pItem);
		std::shared_ptr<Gio::ListStore<ObjectPropertyEntry>> result;
		if (col) {
			if (auto group = std::dynamic_pointer_cast<sdk::main::ExtensionPropertyGroup>(col->getNativeProperty())) {
				result = Gio::ListStore<ObjectPropertyEntry>::create();
				for (const auto &extensionPropertyBase: group->getChildren())
					result->append(ObjectPropertyEntry::create(extensionPropertyBase));
			}
		} else {
			result = pEntries;
		}

		return result;
	});
}

std::shared_ptr<Gtk::ColumnViewColumn> ObjectPropertiesWindow::createNameColumn() {
	const auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
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

		const auto col = std::dynamic_pointer_cast<ObjectPropertyEntry>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		expander->set_hide_expander(false);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getNativeProperty()->getName());
	});

	return Gtk::ColumnViewColumn::create("Name", factory);
}

Gtk::Entry* createEntry() {
	auto* entry = Gtk::make_managed<Gtk::Entry>();
	entry->set_size_request();

	return entry;
}

template<typename ClassT>
	requires std::is_same_v<ClassT, float>
void createWigets(const std::shared_ptr<sdk::main::ExtensionProperty<ClassT>> &pProperty,
				  std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* entry = createEntry();
	entry->set_text(std::to_string(pProperty->getGetterFunc()()));
	entry->signal_changed().connect([entry, pProperty] { pProperty->getSetterFunc()(std::stof(entry->get_text())); });
	pWidgetsOut.emplace_back(entry);
}

template<typename ClassT>
	requires std::is_same_v<ClassT, std::string>
void createWigets(const std::shared_ptr<sdk::main::ExtensionProperty<ClassT>> &pProperty,
				  std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* entry = createEntry();
	entry->set_text(pProperty->getGetterFunc()());
	entry->signal_changed().connect([entry, pProperty] { pProperty->getSetterFunc()(entry->get_text()); });
	pWidgetsOut.emplace_back(entry);
}

template<typename ClassT, int L, typename T, glm::qualifier Q = glm::defaultp>
	requires std::is_same_v<ClassT, glm::vec<L, T, Q>>
void createWigets(const std::shared_ptr<sdk::main::ExtensionProperty<ClassT>> &pProperty,
				  std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto getter = pProperty->getGetterFunc();
	auto vec = getter();
	if constexpr (L > 0) {
		auto* entryX = createEntry();
		entryX->set_text(std::to_string(vec.x));
		entryX->signal_changed().connect([entryX, pProperty, getter] {
			auto vec3 = getter();
			vec3.x = std::stof(entryX->get_text(), nullptr);
			pProperty->getSetterFunc()(vec3);
		});
		pWidgetsOut.emplace_back(entryX);
	}
	if constexpr (L > 1) {
		auto* entryY = createEntry();
		entryY->set_text(std::to_string(vec.y));
		entryY->signal_changed().connect([entryY, pProperty, getter] {
			auto vec3 = getter();
			vec3.y = std::stof(entryY->get_text(), nullptr);
			pProperty->getSetterFunc()(vec3);
		});
		pWidgetsOut.emplace_back(entryY);
	}
	if constexpr (L > 2) {
		auto* entryZ = createEntry();
		entryZ->set_text(std::to_string(vec.z));
		entryZ->signal_changed().connect([entryZ, pProperty, getter] {
			auto vec3 = getter();
			vec3.z = std::stof(entryZ->get_text(), nullptr);
			pProperty->getSetterFunc()(vec3);
		});
		pWidgetsOut.emplace_back(entryZ);
	}
	if constexpr (L > 3) {
		auto* entryW = createEntry();
		entryW->set_text(std::to_string(vec.w));
		entryW->signal_changed().connect([entryW, pProperty, getter] {
			auto vec3 = getter();
			vec3.w = std::stof(entryW->get_text(), nullptr);
			pProperty->getSetterFunc()(vec3);
		});
		pWidgetsOut.emplace_back(entryW);
	}
}

template<typename ClassT>
	requires std::is_same_v<ClassT, sdk::utils::KeyboardKey>
void createWigets(const std::shared_ptr<sdk::main::ExtensionProperty<ClassT>> & /*pProperty*/,
				  std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* widget = Gtk::make_managed<Gtk::Label>();
	widget->set_label("Work in progress");
	pWidgetsOut.emplace_back(widget);
}

std::shared_ptr<Gtk::ColumnViewColumn> ObjectPropertiesWindow::createValueColumn() {
	const auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
		box->set_margin(5);
		pListItem->set_child(*box);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;

		const auto col = std::dynamic_pointer_cast<ObjectPropertyEntry>(row->get_item());
		if (!col) return;
		auto property = col->getNativeProperty();
		auto* const box = dynamic_cast<Gtk::Box*>(pListItem->get_child());
		auto children = box->observe_children();
		for (uint32_t i = 0, maxI = children->get_n_items(); i < maxI; i++) {
			auto child = children->get_typed_object<Gtk::Widget>(0);
			child->unparent();
		}
		std::vector<Gtk::Widget*> widgets;
		if (std::dynamic_pointer_cast<sdk::main::ExtensionPropertyGroup>(property)) { return; }

		if (auto prop = std::dynamic_pointer_cast<sdk::main::ExtensionProperty<std::string>>(property)) {
			createWigets(prop, widgets);
		}
		if (auto prop = std::dynamic_pointer_cast<sdk::main::ExtensionProperty<float>>(property)) {
			createWigets(prop, widgets);
		}
		if (auto prop = std::dynamic_pointer_cast<sdk::main::ExtensionProperty<glm::vec3>>(property)) {
			using vecT = decltype(prop)::element_type::PropertyT;
			createWigets<vecT, vecT::length(), vecT::value_type>(prop, widgets);
		}
		if (auto prop = std::dynamic_pointer_cast<sdk::main::ExtensionProperty<glm::vec2>>(property)) {
			using vecT = decltype(prop)::element_type::PropertyT;
			createWigets<vecT, vecT::length(), vecT::value_type>(prop, widgets);
		}
		if (auto prop = std::dynamic_pointer_cast<sdk::main::ExtensionProperty<sdk::utils::KeyboardKey>>(property)) {
			createWigets(prop, widgets);
		}
		if (widgets.empty()) {
			auto* widget = Gtk::make_managed<Gtk::Label>();
			widget->set_label("Missing widgets for " + Utils::getTypeName(property.get()));
			widget->set_wrap();
			widget->set_wrap_mode(Pango::WrapMode::WORD);
			widgets.emplace_back(widget);
		}
		for (auto widget: widgets) { box->append(*widget); }
	});

	return Gtk::ColumnViewColumn::create("Value", factory);

}
}