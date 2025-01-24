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
// Created by alexus on 16.09.24.
//

#ifndef TREEELEMENTBASE_H
#define TREEELEMENTBASE_H

namespace mer::editor::ui {
class TreeElementBase : public Glib::Object {
	sigc::slot<void(bool)> onShowExpanderSlot;
	std::shared_ptr<Gio::ListModel> children;

public:
	explicit TreeElementBase(const std::shared_ptr<Gio::ListModel> &pChildren) : children(pChildren) {}

	void notifyChanged() const {
		if (!children) return;
		onShowExpanderSlot(children->get_n_items() != 0);
	}

	void setOnShowExpanderSlot(const sigc::slot<void(bool)> &pOnShowExpanderSlot) {
		onShowExpanderSlot = pOnShowExpanderSlot;
		notifyChanged();
	}

	virtual std::shared_ptr<Gio::MenuModel> getMenu() = 0;

	[[nodiscard]] const std::shared_ptr<Gio::ListModel> &getChildren() const { return children; }
};
} // namespace mer::editor::ui


#endif //TREEELEMENTBASE_H
