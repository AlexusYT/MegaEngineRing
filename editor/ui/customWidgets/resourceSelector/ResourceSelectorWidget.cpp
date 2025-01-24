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
// Created by alexus on 07.11.24.
//

#include "ResourceSelectorWidget.h"

#include <glibmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <ranges>

#include "ISourceSelection.h"
#include "ISourceSelectionResult.h"

namespace mer::editor::ui {
ResourceSelectorWidget::ResourceSelectorWidget() {

	stack = std::make_shared<Gtk::Stack>();
	stack->set_interpolate_size(true);
	stack->set_vhomogeneous(false);
	stack->set_hhomogeneous(false);
	stack->set_transition_type(Gtk::StackTransitionType::CROSSFADE);
	auto visibleChild = stack->property_visible_child_name();
	visibleChild.signal_changed().connect([this, visibleChild]() {
		const auto iter = sources.find(visibleChild.get_value());
		if (iter == sources.end()) return;
		iter->second->onResultSet(selection);
	});
	stackSwitcher = std::make_shared<Gtk::StackSwitcher>();
	stackSwitcher->set_stack(*stack);
	gtk_orientable_set_orientation(GTK_ORIENTABLE(stackSwitcher->gobj()), GTK_ORIENTATION_VERTICAL);
	Gtk::Box box(Gtk::Orientation::VERTICAL, 6);
	Gtk::Label hintLabel("Select a data source");
	box.append(hintLabel);
	box.append(*stackSwitcher);
	box.append(*stack);

	Gtk::Popover popover;
	popover.set_child(box);
	this->set_popover(popover);
	popover.set_autohide(false);
	auto visible = popover.property_visible();
	visible.signal_changed().connect([this] {
		if (!get_popover()->get_visible()) return;
		for (const auto &sourceSelection: sources | std::views::values) { sourceSelection->onShow(); }
		Glib::signal_timeout().connect(
			[this] {
				for (auto [name, source]: sources) {
					if (source->isResultCompatible(selection)) {
						switchTo(name);
						break;
					}
				}
				return false;
			},
			16);
	});
}

void ResourceSelectorWidget::addSource(const std::shared_ptr<ISourceSelection> &pSource) {

	pSource->setParent(this);
	auto name = pSource->getName();
	sources.emplace(name, pSource);
	stack->add(*pSource->getWidget(), name, pSource->getTitle());
}

void ResourceSelectorWidget::setSelection(const std::shared_ptr<ISourceSelectionResult> &pSelection) {
	if (selection == pSelection || (selection && selection->isSame(pSelection))) return;
	selection = pSelection;
	selectionChangedSignal.emit(pSelection);
	updateLabel();
	if (get_active()) {
		popdown();
		for (auto [name, source]: sources) {
			if (source->isResultCompatible(selection)) {
				switchTo(name);
				break;
			}
		}
	}
}

void ResourceSelectorWidget::switchTo(const std::string &pName) const { stack->set_visible_child(pName); }

void ResourceSelectorWidget::on_realize() {
	MenuButton::on_realize();
	updateLabel();
}

void ResourceSelectorWidget::size_allocate_vfunc(const int pWidth, const int pHeight, const int pBaseline) {

	MenuButton::size_allocate_vfunc(pWidth, pHeight, pBaseline);
	get_popover()->present();
}

void ResourceSelectorWidget::updateLabel() {
	for (const auto &source: sources | std::views::values) {
		auto label = source->getLabelFromResult(selection);
		if (label.empty()) continue;
		set_label(label);
		break;
	}
}
} // namespace mer::editor::ui