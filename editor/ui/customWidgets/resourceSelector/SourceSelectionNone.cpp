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
// Created by alexus on 09.11.24.
//

#include "SourceSelectionNone.h"

#include "ISourceSelectionResult.h"
#include "ResourceSelectorWidget.h"

namespace mer::editor::ui {
std::string SourceSelectionNone::getName() { return "none"; }

std::string SourceSelectionNone::getTitle() { return "None"; }

Gtk::Widget* SourceSelectionNone::getWidget() {
	auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 6);
	auto label = Gtk::make_managed<Gtk::Label>("Nothing will be selected");
	box->append(*label);
	auto btn = Gtk::make_managed<Gtk::Button>("Confirm resetting");
	btn->signal_clicked().connect([this] { resultCallback(nullptr); });
	box->append(*btn);
	return box;
}

void SourceSelectionNone::onResultSet(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	if (pResult == nullptr || pResult->isEmpty()) { getParent()->switchTo(getName()); }
}

bool SourceSelectionNone::isResultCompatible(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	return pResult == nullptr || pResult->isEmpty();
}

std::string SourceSelectionNone::getLabelFromResult(const std::shared_ptr<ISourceSelectionResult> &pResult) {
	if (isResultCompatible(pResult)) { return "None"; }
	return "";
}
} // namespace mer::editor::ui