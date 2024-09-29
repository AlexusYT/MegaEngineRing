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

#include "ViewObjectProperties.h"

#include "EngineSDK/main/scene/objects/extensions/ExtensionProperty.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionPropertyBase.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h"
#include "EngineSDK/main/scene/objects/extensions/MainObjectExtension.h"
#include "EngineSDK/utils/KeyboardKey.h"
#include "ObjectPropertyEntry.h"
#include "mvp/contexts/IWidgetContext.h"
#include "mvp/main/editors/sceneEditor/explorerObjects/ExplorerObject.h"
#include "ui/customWidgets/CustomTreeView.h"

namespace mer::editor::mvp {
ViewObjectProperties::ViewObjectProperties(const std::shared_ptr<IWidgetContext> &pContext) : context(pContext) {

	Gtk::Box buttonBox;
	addBtn.set_label("Add");
	auto menu = Gio::Menu::create();
	for (auto extension: sdk::main::ExtensionRegistry::getExtensions()) {
		if (extension.first == sdk::main::MainObjectExtension::typeName()) continue;
		auto item = Gio::MenuItem::create(extension.first, "");

		item->set_action_and_target("object.selected.extension.new",
									Glib::Variant<std::string>::create(extension.first));
		menu->append_item(item);
	}
	addBtn.set_menu_model(menu);
	addBtn.set_sensitive(false);

	removeBtn.set_label("Remove");
	removeBtn.set_sensitive(false);
	buttonBox.append(addBtn);
	buttonBox.append(removeBtn);
	removeBtn.signal_clicked().connect([this] {
		auto selectedItem = propertiesTree->getSelectedItem();
		sdk::main::Extension* extToRemove{};
		if (const auto property = std::dynamic_pointer_cast<ObjectPropertyEntry>(selectedItem)) {
			extToRemove = property->getExtension();
		} else if (const auto extension = std::dynamic_pointer_cast<ObjectExtensionEntry>(selectedItem)) {
			extToRemove = extension->getNativeExtension();
		}
		if (!extToRemove) return;
		const auto variant = Glib::Variant<uintptr_t>::create(reinterpret_cast<uintptr_t>(extToRemove));

		mainWidget.activate_action("object.extension.remove", variant);
		propertiesTree->unselect();
		removeBtn.set_sensitive(false);
	});
	mainWidget.set_orientation(Gtk::Orientation::VERTICAL);
	mainWidget.append(buttonBox);
	propertiesTree = Gtk::make_managed<ui::CustomTreeView>();
	Gtk::ScrolledWindow propertiesScrolledWindow;
	propertiesScrolledWindow.set_expand(true);
	propertiesScrolledWindow.set_child(*propertiesTree);
	mainWidget.append(propertiesScrolledWindow);


	const auto nameColumn = createNameColumn();
	nameColumn->set_resizable(true);
	propertiesTree->append_column(nameColumn);
	const auto valueColumn = createValueColumn();
	valueColumn->set_resizable(true);
	propertiesTree->append_column(valueColumn);

	propertiesTree->setSlotSelectionChanged([this](Glib::ObjectBase* pObjectBase) {
		auto mainExtTypeName = sdk::main::MainObjectExtension::typeName();
		if (auto entry = dynamic_cast<ObjectPropertyEntry*>(pObjectBase)) {
			removeBtn.set_sensitive(entry->getExtension()->getTypeName() != mainExtTypeName);
		} else if (auto ext = dynamic_cast<ObjectExtensionEntry*>(pObjectBase)) {
			removeBtn.set_sensitive(ext->getNativeExtension()->getTypeName() != mainExtTypeName);
		}
		//if (entrySelectionChanged) entrySelectionChanged(dynamic_cast<ObjectPropertyEntryBase*>(pObjectBase));
	});
}

void ViewObjectProperties::setObject(ExplorerObject* pObject) {
	addBtn.set_sensitive(pObject != nullptr);
	propertiesTree->setSlotCreateModel(
		[this, pObject](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
			if (const auto col = std::dynamic_pointer_cast<ObjectExtensionEntry>(pItem))
				//
				return col->getChildren();
			if (std::dynamic_pointer_cast<ObjectPropertyEntry>(pItem)) return nullptr;
			if (pObject) return pObject->getPropertyEntries();

			return Gio::ListStore<ObjectExtensionEntry>::create();
		});
	propertiesTree->unselect();
}

std::shared_ptr<Gtk::ColumnViewColumn> ViewObjectProperties::createNameColumn() {
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

		const auto col = std::dynamic_pointer_cast<ObjectPropertyEntryBase>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		expander->set_hide_expander(false);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getName());
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
void createWidgets(sdk::main::ExtensionProperty<ClassT>* pProperty, std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* entry = createEntry();
	entry->set_text(std::to_string(pProperty->getValue()));
	entry->signal_changed().connect([entry, pProperty] { pProperty->setValue(std::stof(entry->get_text())); });
	pWidgetsOut.emplace_back(entry);
}

template<typename ClassT>
	requires std::is_same_v<ClassT, std::string>
void createWidgets(sdk::main::ExtensionProperty<ClassT>* pProperty, std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* entry = createEntry();
	entry->set_text(pProperty->getValue());
	entry->signal_changed().connect([entry, pProperty] { pProperty->setValue(entry->get_text()); });
	pWidgetsOut.emplace_back(entry);
}

template<typename ClassT, int L, typename T, glm::qualifier Q = glm::defaultp>
	requires std::is_same_v<ClassT, glm::vec<L, T, Q>>
void createWidgets(sdk::main::ExtensionProperty<ClassT>* pProperty, std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto vec = pProperty->getValue();
	if constexpr (L > 0) {
		auto* entryX = createEntry();
		entryX->set_text(std::to_string(vec.x));
		entryX->signal_changed().connect([entryX, pProperty] {
			auto vec3 = pProperty->getValue();
			vec3.x = std::stof(entryX->get_text(), nullptr);
			pProperty->setValue(vec3);
		});
		pWidgetsOut.emplace_back(entryX);
	}
	if constexpr (L > 1) {
		auto* entryY = createEntry();
		entryY->set_text(std::to_string(vec.y));
		entryY->signal_changed().connect([entryY, pProperty] {
			auto vec3 = pProperty->getValue();
			vec3.y = std::stof(entryY->get_text(), nullptr);
			pProperty->setValue(vec3);
		});
		pWidgetsOut.emplace_back(entryY);
	}
	if constexpr (L > 2) {
		auto* entryZ = createEntry();
		entryZ->set_text(std::to_string(vec.z));
		entryZ->signal_changed().connect([entryZ, pProperty] {
			auto vec3 = pProperty->getValue();
			vec3.z = std::stof(entryZ->get_text(), nullptr);
			pProperty->setValue(vec3);
		});
		pWidgetsOut.emplace_back(entryZ);
	}
	if constexpr (L > 3) {
		auto* entryW = createEntry();
		entryW->set_text(std::to_string(vec.w));
		entryW->signal_changed().connect([entryW, pProperty] {
			auto vec3 = pProperty->getValue();
			vec3.w = std::stof(entryW->get_text(), nullptr);
			pProperty->setValue(vec3);
		});
		pWidgetsOut.emplace_back(entryW);
	}
}

template<typename ClassT>
	requires std::is_same_v<ClassT, sdk::utils::KeyboardKey>
void createWidgets(sdk::main::ExtensionProperty<ClassT>* /*pProperty*/, std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* widget = Gtk::make_managed<Gtk::Label>();
	widget->set_label("Work in progress");
	pWidgetsOut.emplace_back(widget);
}

std::shared_ptr<Gtk::ColumnViewColumn> ViewObjectProperties::createValueColumn() {
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

		if (auto prop = dynamic_cast<sdk::main::ExtensionProperty<std::string>*>(property)) {
			createWidgets(prop, widgets);
		}
		if (auto prop = dynamic_cast<sdk::main::ExtensionProperty<float>*>(property)) { createWidgets(prop, widgets); }
		if (auto prop = dynamic_cast<sdk::main::ExtensionProperty<glm::vec3>*>(property)) {
			using vecT = glm::vec3;
			createWidgets<vecT, vecT::length(), vecT::value_type>(prop, widgets);
		}
		if (auto prop = dynamic_cast<sdk::main::ExtensionProperty<glm::vec2>*>(property)) {
			using vecT = glm::vec2;
			createWidgets<vecT, vecT::length(), vecT::value_type>(prop, widgets);
		}
		if (auto prop = dynamic_cast<sdk::main::ExtensionProperty<sdk::utils::KeyboardKey>*>(property)) {
			createWidgets(prop, widgets);
		}
		if (widgets.empty()) {
			auto* widget = Gtk::make_managed<Gtk::Label>();
			widget->set_label("Missing widgets for " + Utils::getTypeName(property));
			widget->set_wrap();
			widget->set_wrap_mode(Pango::WrapMode::CHAR);
			widgets.emplace_back(widget);
		}
		for (auto widget: widgets) { box->append(*widget); }
	});

	return Gtk::ColumnViewColumn::create("Value", factory);
}

void ViewObjectProperties::openView() { context->addWidget(&mainWidget); }

void ViewObjectProperties::closeView() { context->removeWidget(); }
} // namespace mer::editor::mvp