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
// Created by alexus on 29.01.24.
//

#ifndef APPINFOEDITOR_H
#define APPINFOEDITOR_H

#include "IViewAppInfo.h"

namespace mer::editor::project {
class ApplicationInfo;
class AppInfoProperty;
} // namespace mer::editor::project

namespace mer::editor::mvp {
class TabPlaceholder;

class AppInfoEditor : public IViewAppInfo {
	std::shared_ptr<TabPlaceholder> placeholder;
	ValueChangedSlot valueChangedSlot;
	std::unordered_map<Glib::RefPtr<Gtk::ListItem>, sigc::connection> connections;
	Gtk::Box main;
	ui::TreeWidget tree;


public:
	AppInfoEditor(const std::shared_ptr<TabPlaceholder> &pPlaceholder);

	void closeView() override;

	void openView() override;

	void setSlotCreateModel(const ui::TreeWidget::SlotCreateModel &pSlotCreateModel) override;

	Gtk::Widget &getMainWidget() override { return main; }

	[[nodiscard]] const ValueChangedSlot &getValueChangedSlot() const override { return valueChangedSlot; }

	void setValueChangedSlot(const ValueChangedSlot &pValueChangedSlot) override {
		valueChangedSlot = pValueChangedSlot;
	}
};
} // namespace mer::editor::mvp


#endif //APPINFOEDITOR_H
