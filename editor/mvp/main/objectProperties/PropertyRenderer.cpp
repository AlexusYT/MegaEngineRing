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
// Created by alexus on 09.11.24.
//

#include "PropertyRenderer.h"

#include "EngineSDK/main/resources/materials/IMaterialResource.h"
#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineUtils/utils/Property.h"
#include "PropertyRendererType.h"
#include "ui/customWidgets/resourceSelector/ResourceSelectorWidget.h"
#include "ui/customWidgets/resourceSelector/SourceSelectionNone.h"
#include "ui/customWidgets/resourceSelector/resources/SourceSelectionMaterial.h"
#include "ui/customWidgets/resourceSelector/resources/SourceSelectionModel3D.h"

namespace mer::editor::mvp {
PropertyRenderer::PropertyRenderer(sdk::utils::PropertyBase* pProperty, sdk::main::IResourceLoadExecutor* pLoader,
								   const PropertyRendererType pType)
	: property(pProperty), type(pType), loader(pLoader) {}

std::shared_ptr<PropertyRenderer> PropertyRenderer::create(sdk::utils::PropertyBase* pProperty,
														   sdk::main::IResourceLoadExecutor* pLoader) {
	return create(pProperty, pLoader, PropertyRendererType::DEFAULT);
}

std::shared_ptr<PropertyRenderer> PropertyRenderer::create(
	sdk::utils::PropertyBase* pProperty, sdk::main::IResourceLoadExecutor* pLoader, const PropertyRendererType pType) {
	return std::shared_ptr<PropertyRenderer>(new PropertyRenderer(pProperty, pLoader, pType));
}

Gtk::SpinButton* create_spin_button(const double pValue, const double pMinValue = std::numeric_limits<double>::lowest(),
									const double pMaxValue = std::numeric_limits<double>::max()) {

	auto* spin = Gtk::make_managed<Gtk::SpinButton>();
	spin->set_adjustment(Gtk::Adjustment::create(pValue, pMinValue, pMaxValue));
	spin->set_digits(3);
	spin->set_size_request();
	spin->set_width_chars(6);
	spin->set_update_policy(Gtk::SpinButton::UpdatePolicy::IF_VALID);

	return spin;
}

template<int L, typename T, glm::qualifier Q = glm::defaultp>
Gtk::Widget* create_widget_for_vec(sdk::utils::Property<glm::vec<L, T, Q>>* pProperty) {
	auto box = Gtk::make_managed<Gtk::Box>();
	auto vec = pProperty->getValue();
	if constexpr (L > 0) {
		auto* spin = create_spin_button(static_cast<double>(vec.x));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.x = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		box->append(*spin);
	}
	if constexpr (L > 1) {
		auto* spin = create_spin_button(static_cast<double>(vec.y));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.y = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		box->append(*spin);
	}
	if constexpr (L > 2) {
		auto* spin = create_spin_button(static_cast<double>(vec.z));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.z = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		box->append(*spin);
	}
	if constexpr (L > 3) {
		auto* spin = create_spin_button(static_cast<double>(vec.w));
		spin->signal_value_changed().connect([spin, pProperty] {
			auto vecValue = pProperty->getValue();
			vecValue.w = static_cast<T>(spin->get_value());
			pProperty->setValue(vecValue);
		});
		box->append(*spin);
	}
	return box;
}

Gtk::Widget* get_widget_missing(sdk::utils::PropertyBase* pProperty) {

	auto* widget = Gtk::make_managed<Gtk::Label>();
	widget->set_label("Missing widgets for " + Utils::getTypeName(pProperty));
	widget->set_ellipsize(Pango::EllipsizeMode::END);
	widget->set_wrap();
	widget->set_wrap_mode(Pango::WrapMode::CHAR);
	return widget;
}

Gtk::Widget* PropertyRenderer::getWidget() const {

	if (auto prop = dynamic_cast<sdk::utils::Property<std::string>*>(property)) {

		auto* entry = Gtk::make_managed<Gtk::Entry>();
		entry->set_size_request();
		entry->set_text(prop->getValue());
		entry->signal_changed().connect([entry, prop] { prop->setValue(entry->get_text()); });
		return entry;
	}
	if (auto prop = dynamic_cast<sdk::utils::Property<float>*>(property)) {

		auto* spin = create_spin_button(static_cast<double>(prop->getValue()));
		spin->signal_value_changed().connect([spin, prop] { prop->setValue(static_cast<float>(spin->get_value())); });
		return spin;
	}

	if (auto prop = dynamic_cast<sdk::utils::Property<glm::vec3>*>(property)) {
		using VecT = glm::vec3;
		return create_widget_for_vec<VecT::length(), VecT::value_type>(prop);
	}
	if (auto prop = dynamic_cast<sdk::utils::Property<glm::vec2>*>(property)) {
		using VecT = glm::vec2;
		return create_widget_for_vec<VecT::length(), VecT::value_type>(prop);
	}
	if (auto prop = dynamic_cast<sdk::utils::Property<std::shared_ptr<sdk::main::IModel3DObject>>*>(property)) {
		auto box = Gtk::make_managed<Gtk::Box>();

		auto* selector = Gtk::make_managed<ui::ResourceSelectorWidget>();
		selector->set_hexpand(true);
		selector->addSource(std::make_shared<ui::SourceSelectionNone>());
		selector->addSource(ui::SourceSelectionModel3D::create(loader));
		selector->getSelectionChangedSignal().connect(
			[prop](const std::shared_ptr<ui::ISourceSelectionResult> &pResult) {
				if (auto objectResult = std::dynamic_pointer_cast<ui::SourceSelectionModel3D::Result>(pResult)) {
					auto object = objectResult->getSelectionResult();
					prop->setValue(object);
				} else if (!objectResult) {
					prop->setValue(nullptr);
				}
			});
		if (auto value = prop->getValue()) {
			selector->setSelection(ui::SourceSelectionModel3D::Result::create(value));
		}
		box->append(*selector);
		return box;
	}
	if (auto prop = dynamic_cast<sdk::utils::Property<std::shared_ptr<sdk::main::IMaterialResource>>*>(property)) {
		auto box = Gtk::make_managed<Gtk::Box>();

		auto* selector = Gtk::make_managed<ui::ResourceSelectorWidget>();
		selector->set_hexpand(true);
		selector->addSource(std::make_shared<ui::SourceSelectionNone>());
		selector->addSource(ui::SourceSelectionMaterial::create(loader));
		selector->getSelectionChangedSignal().connect(
			[prop](const std::shared_ptr<ui::ISourceSelectionResult> &pResult) {
				if (auto objectResult = std::dynamic_pointer_cast<ui::SourceSelectionMaterial::Result>(pResult)) {
					auto object = objectResult->getSelectionResult();
					prop->setValue(object);
				} else if (!objectResult) {
					prop->setValue(nullptr);
				}
			});
		if (auto value = prop->getValue()) {
			selector->setSelection(ui::SourceSelectionMaterial::Result::create(value));
		}
		box->append(*selector);
		return box;
	}
	return get_widget_missing(property);
}
} // namespace mer::editor::mvp