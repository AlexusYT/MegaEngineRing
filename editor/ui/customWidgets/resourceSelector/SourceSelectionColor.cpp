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
// Created by alexus on 22.11.24.
//

#include "SourceSelectionColor.h"

#include "EngineSDK/main/resources/materials/ColorComponent.h"

namespace mer::editor::ui {
bool SourceSelectionColor::Result::isSame(const std::shared_ptr<ISourceSelectionResult> &pOther) {

	auto other = std::dynamic_pointer_cast<TypedSourceSelectionResult>(pOther);
	if (!other) return false;
	return getSelectionResult()->color == other->getSelectionResult()->color;
}

std::shared_ptr<SourceSelectionColor> SourceSelectionColor::create() {
	return std::shared_ptr<SourceSelectionColor>(new SourceSelectionColor());
}

Gtk::Widget* SourceSelectionColor::getWidget() {
	auto box = Gtk::make_managed<Gtk::Box>();

	auto dialog = Gtk::ColorDialog::create();
	dialog->set_title("Select color");
	picker = Gtk::make_managed<Gtk::ColorDialogButton>();
	picker->set_dialog(dialog);
	box->append(*picker);
	auto rgbaProp = picker->property_rgba();
	rgbaProp.signal_changed().connect([rgbaProp, this] {
		auto rgba = rgbaProp.get_value();
		auto component = sdk::main::ColorComponent::create();
		component->color = glm::vec4(rgba.get_red(), rgba.get_green(), rgba.get_blue(), rgba.get_alpha());
		resultCallback(Result::create(component));
	});
	return box;
}

void SourceSelectionColor::onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	if (auto result = std::dynamic_pointer_cast<Result>(pResult)) {
		auto color = result->getSelectionResult()->color;
		picker->set_rgba(Gdk::RGBA(color->r, color->g, color->b, color->a));
	}
}

bool SourceSelectionColor::isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	return std::dynamic_pointer_cast<Result>(pResult) != nullptr;
}

std::string SourceSelectionColor::getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	if (auto model = std::dynamic_pointer_cast<Result>(pResult)) {
		auto color = glm::ivec4(glm::round(*model->getSelectionResult()->color * 255.0f));
		return std::format("#{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a);
	}
	return "";
}
} // namespace mer::editor::ui