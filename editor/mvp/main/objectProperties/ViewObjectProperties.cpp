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

#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineSDK/main/resources/models/IModel3DResource.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionProperty.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h"
#include "EngineSDK/main/scene/objects/extensions/MainObjectExtension.h"
#include "EngineSDK/utils/KeyboardKey.h"
#include "ObjectPropertyEntry.h"
#include "mvp/contexts/IWidgetContext.h"
#include "mvp/main/editors/sceneEditor/explorerObjects/ExplorerObject.h"
#include "mvp/resourceSelection/ModelResourceSelection.h"
#include "mvp/resourceSelection/ViewResourceSelection.h"
#include "ui/customWidgets/CustomSignalListItemFactory.h"
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
	nameColumn->set_fixed_width(100);
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
	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* expander = Gtk::make_managed<Gtk::TreeExpander>();
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		label->set_ellipsize(Pango::EllipsizeMode::END);
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

Gtk::SpinButton* createSpinButton(const double pValue, const double pMinValue = std::numeric_limits<double>::lowest(),
								  const double pMaxValue = std::numeric_limits<double>::max()) {

	auto* spin = Gtk::make_managed<Gtk::SpinButton>();
	spin->set_adjustment(Gtk::Adjustment::create(pValue, pMinValue, pMaxValue));
	spin->set_digits(3);
	spin->set_size_request();
	spin->set_width_chars(6);
	spin->set_update_policy(Gtk::SpinButton::UpdatePolicy::IF_VALID);

	return spin;
}

template<typename ClassT>
	requires std::is_same_v<ClassT, float>
void createWidgets(sdk::main::ExtensionProperty<ClassT>* pProperty, std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* spin = createSpinButton(static_cast<double>(pProperty->getValue()));
	spin->signal_value_changed().connect(
		[spin, pProperty] { pProperty->setValue(static_cast<ClassT>(spin->get_value())); });
	pWidgetsOut.emplace_back(spin);
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
		auto* spin = createSpinButton(static_cast<double>(vec.x));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.x = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		pWidgetsOut.emplace_back(spin);
	}
	if constexpr (L > 1) {
		auto* spin = createSpinButton(static_cast<double>(vec.y));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.y = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		pWidgetsOut.emplace_back(spin);
	}
	if constexpr (L > 2) {
		auto* spin = createSpinButton(static_cast<double>(vec.z));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.z = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		pWidgetsOut.emplace_back(spin);
	}
	if constexpr (L > 3) {
		auto* spin = createSpinButton(static_cast<double>(vec.w));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.w = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		pWidgetsOut.emplace_back(spin);
	}
}

template<typename ClassT>
	requires std::is_same_v<ClassT, sdk::utils::KeyboardKey>
void createWidgets(sdk::main::ExtensionProperty<ClassT>* /*pProperty*/, std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto* widget = Gtk::make_managed<Gtk::Label>();
	widget->set_label("Work in progress");
	pWidgetsOut.emplace_back(widget);
}

template<typename ClassT>
	requires std::is_same_v<ClassT, std::shared_ptr<sdk::main::IModel3DObject>>
void createWidgets(sdk::main::ExtensionProperty<ClassT>* pProperty, std::vector<Gtk::Widget*> &pWidgetsOut) {
	auto obj = pProperty->getValue();
	auto* label = Gtk::make_managed<Gtk::Label>();
	if (obj) {
		auto resource = obj->getIModelResource();
		label->set_label((resource->asResource()->getResourceUri() / obj->getName()).string());
	} else {
		label->set_label("None");
	}
	pWidgetsOut.emplace_back(label);

	auto* observeBtn = Gtk::make_managed<Gtk::Button>("...");
	pWidgetsOut.emplace_back(observeBtn);
	observeBtn->set_action_name("resource.select-for-property");
	const auto variant = Glib::Variant<uintptr_t>::create(reinterpret_cast<uintptr_t>(pProperty));
	observeBtn->set_action_target_value(variant);
}

std::shared_ptr<Gtk::ColumnViewColumn> ViewObjectProperties::createValueColumn() {
	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
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
		if (auto prop =
				dynamic_cast<sdk::main::ExtensionProperty<std::shared_ptr<sdk::main::IModel3DObject>>*>(property)) {
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